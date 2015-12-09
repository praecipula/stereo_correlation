#include "qtdistancefieldfonttexturegenerator.h"
#include <QFontMetrics>
#include <QPainter>
#include <strcon.h>
#include <iostream>
#include <locale>
#include <codecvt>

namespace Stereo
{
    QtDistanceFieldFontTextureGenerator::CharacterRange QtDistanceFieldFontTextureGenerator::defaultCharacterRange()
    {
        static QtDistanceFieldFontTextureGenerator::CharacterRange defaultRange(0x0001, 0x05F4);
        return defaultRange;
    }

    QtDistanceFieldFontTextureGenerator::QtDistanceFieldFontTextureGenerator(const QFont& font, int spreadFactor):
        m_font(font), //QFonts use shared-ptr like reference counting.
        m_spreadFactor(spreadFactor)
    {
        m_font.setStyleStrategy(QFont::NoAntialias);
    }

    QtDistanceFieldFontTextureGenerator::~QtDistanceFieldFontTextureGenerator()
    {

    }


    QString QtDistanceFieldFontTextureGenerator::generateFontTexture(CharacterRange characterRange)
    {

        // For each glyph

        short intsize = sizeof(int);
        // Starting from 1 (ommitting the null character) and going through a basic set.
        for (int c = characterRange.begin; c <= characterRange.end; ++c)
        {
            if (isprint(c))
            {
                std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv1;
                std::string u8str = conv1.to_bytes(c);
                // Paint into QImage
                QImage characterImage;
                DistanceFieldGLFont::FontMetrics metrics;
                tie(characterImage, metrics) = generateCharacterImage(QString(StrCon(u8str)));
                // Process QImage to generate distance field
                QImage distanceField = generateDistanceField(characterImage);
                // Pack the distance field image into the font buffer image
                QRect cropRect = calculateAutocrop(distanceField);
                // Do the autocrop
                QImage cropped = crop(cropRect, metrics, distanceField);
                // Set the parameters in QDistanceFieldFontTexture
            }
        }
        // Pack the image


        // Save the image
    }


    tuple<QImage, DistanceFieldGLFont::FontMetrics> QtDistanceFieldFontTextureGenerator::generateCharacterImage(QString character)
    {
        /*
         * Find the size of the drawn character.
         * This is actually complicated a bit by the occurrence in, say, cursive or italicized
         * characters (though it happens in normal characters two) where the character intrudes
         * to the left or the right of its logical width. In an italicized character, the next
         * character might have its origin beneath an overhang of the previous character.
         *
         * Therefore, we build a metric based on the information Qt gives us, then we later
         * (since we're modifying the image for the displacement map anyway) recalculate an
         * effective version of these metrics to be serialized for use in our application.
         */
        DistanceFieldGLFont::FontMetrics metrics = getCharacterFontMetrics(character);

        /*
         * OK. At this point, if we need to guess the spread factor due to applying our displacement
         * map, we can do it with the information in character 'l' as assumed to be
         * a near-vertical line of a font-representative stroke width.
        */
        if (m_spreadFactor == -1)
        {
           DistanceFieldGLFont::FontMetrics l_metrics = getCharacterFontMetrics(QString("l"));
            // Go in a factor of the width of l's stride. Heuristic.
            m_spreadFactor = l_metrics.stride / 4;
            std::cout << m_spreadFactor << std::endl;
        }

        /*
         * I think there may be a rounding error somewhere down the stack. Qt seems to have
         * trouble with pixel alignment, at least with Arial. It's close enough to work
         * here, since we have to do a manual autocrop later.
         */
        int padding = m_spreadFactor * 2;
        QImage buffer(metrics.span.x() + padding * 2, metrics.span.y() + padding * 2, QImage::Format_RGB32);
        buffer.fill(Qt::white);
        QPainter painter(&buffer);
        painter.setPen(Qt::black);
        painter.setFont(m_font);
        /* QImages start at 0, 0, so we need to draw based on the
         * effective offset from drawData
         *
         */
        QPoint origin(padding + metrics.logical_origin.x(), padding + (metrics.span.y() - metrics.logical_origin.y()));
        painter.drawText(origin.x(), origin.y(), character);
        painter.setPen(Qt::red);
        painter.drawPoint(origin);

        return make_tuple(buffer, metrics);
    }

    QImage QtDistanceFieldFontTextureGenerator::generateDistanceField(const QImage& characterImage)
    {
        int padding = m_spreadFactor * 2;
        QImage buffer(characterImage.width(), characterImage.height(), QImage::Format_RGB32);
        QPainter painter(&buffer);
        QColor distanceColor;
        for (int j = 0; j < characterImage.height(); ++j)
        {
            for (int i = 0; i < characterImage.width(); ++i)
            {
                QColor myColor = characterImage.pixel(i, j);
                int distance = 0;
                int component = 0;
                if (myColor == QColor(Qt::black)){
                    distance = distanceToPixel(characterImage, i, j, QColor(Qt::white));
                    /* Large distances should be white. Small ones should be gray */
                    component = distance >= m_spreadFactor ? 255 : static_cast<int>(124 + ((124.0 / m_spreadFactor) * (distance)));
                    distanceColor.setRgb(component, component, component);
                QColor distanceColor(component, component, component);
                } else if (myColor == QColor(Qt::white)) {
                    distance = distanceToPixel(characterImage, i, j, QColor(Qt::black));
                    /* Large distances should be black. Small ones should be gray. */
                    component = distance >= m_spreadFactor ? 0 : static_cast<int>(124 - ((124.0 / m_spreadFactor) * (distance - 1)));
                    distanceColor.setRgb(component, component, component);
                } else {
                    distanceColor = myColor;
                }
                painter.setPen(distanceColor);
                painter.drawPoint(i, j);

            }
        }
        return buffer;
    }

    int QtDistanceFieldFontTextureGenerator::distanceToPixel(const QImage& characterImage, int x, int y, QColor colorToFind)
    {
        // iterate over Neighborhood until Spread is exceeded.
        for (int neighborhood = 1; neighborhood <= m_spreadFactor; ++neighborhood)
        {
            std::vector<QPoint> pixelsToSearch;
            for (int cursor = -neighborhood; cursor <= neighborhood; ++cursor)
            {
                auto add_if_in_image = [&] (int x_p, int y_p) {
                    if (x_p >= 0 && x_p < characterImage.width() &&
                            y_p >= 0 && y_p < characterImage.height())
                    {
                        pixelsToSearch.push_back(QPoint(x_p, y_p));
                    }
                };
                //top
                add_if_in_image(x + cursor, y + neighborhood);
                //bottom
                add_if_in_image(x + cursor, y - neighborhood);
                //left
                add_if_in_image(x - neighborhood, y + cursor);
                //right
                add_if_in_image(x + neighborhood, y + cursor);
            }
            if (std::any_of(pixelsToSearch.begin(), pixelsToSearch.end(), [&](QPoint point){
                            bool found = (characterImage.pixel(point) == colorToFind.rgb());
                            return found;}))
            {
                return neighborhood;
            }
        }
        return m_spreadFactor + 1;
    }

    QImage QtDistanceFieldFontTextureGenerator::crop(QRect &cropExtents, DistanceFieldGLFont::FontMetrics metrics, const QImage &image)
    {
        // Check to see that we won't crop out the logical origin
        int deltaX = cropExtents.x() - metrics.logical_origin.x();
        if (deltaX > 0)
        {
            cropExtents.setX(cropExtents.x() - deltaX);
            cropExtents.setWidth(cropExtents.width() + deltaX);
        }
        int deltaY = cropExtents.y() - metrics.logical_origin.y();
        if (deltaY > 0)
        {
            cropExtents.setY(cropExtents.y() - deltaY);
            cropExtents.setHeight(cropExtents.height() + deltaY);
        }
        LOGD << "Cropping";
        // Adjust the logical origin of metrics
        metrics.logical_origin.set(metrics.logical_origin.x() - cropExtents.x(), metrics.logical_origin.y() - cropExtents.y());
        // Adjust the width and height of metrics
        metrics.span.set(cropExtents.width(), cropExtents.height());
        // crop the image
        QImage croppedImage = image.copy(cropExtents);
        return croppedImage;
    }



    QRect QtDistanceFieldFontTextureGenerator::calculateAutocrop(const QImage& image)
    {
        typedef vector<QPoint> line;
        /*
         *Binary search for all-black scanlines.
         */
        auto blackScanline = [&](line scanline) {
            return !(any_of(scanline.begin(), scanline.end(), [&](QPoint point){return image.pixel(point) != QColor(Qt::black).rgb();}));
        };

        auto buildHorizontalScanline = [&](int lineNumber){
            line scanline;
            for (int i = 0; i < image.width(); ++i)
            {
                scanline.push_back(QPoint(i, lineNumber));
            }
            return scanline;
        };

        auto buildVerticalScanline = [&](int lineNumber){
            line scanline;
            for (int j = 0; j < image.height(); ++j)
            {
                scanline.push_back(QPoint(lineNumber, j));
            }
            return scanline;
        };

        // Scan between blackLimit and middleLimit, either horizontally or vertically, and return the result.
        auto binarySearch = [&](int blackLimit, int middleLimit, bool horizontal_scan, bool increasingLine) {

            int testLineNumber = middleLimit;
            int allBlackScanline = blackLimit;
            int hasWhiteScanline = middleLimit;
            int delta;
            while (abs(allBlackScanline - hasWhiteScanline) > 1)
            {
                line scanline = horizontal_scan ? buildHorizontalScanline(testLineNumber) : buildVerticalScanline(testLineNumber);
                bool isBlack = blackScanline(scanline);
                if (isBlack){
                    allBlackScanline = testLineNumber;
                    delta = abs((allBlackScanline - hasWhiteScanline) / 2);
                    //increasingLine means black scanline moving positive.
                    testLineNumber = increasingLine ? allBlackScanline + delta : allBlackScanline - delta;
                } else {
                    hasWhiteScanline = testLineNumber;
                    delta = abs((allBlackScanline - hasWhiteScanline) / 2);
                    // It's met by a negative scanline.
                    testLineNumber = increasingLine ? hasWhiteScanline - delta : hasWhiteScanline + delta;
                }
            }
            return allBlackScanline;
        };

        // Recall in Qt land, 0 = top, height = bottom.
        int topCrop = binarySearch(0, image.height() / 2, true, true);
        int leftCrop = binarySearch(0, image.width() / 2, false, true);
        int bottomCrop = binarySearch(image.height(), image.height() / 2, true, false);
        int rightCrop = binarySearch(image.width(), image.width() / 2, false, false);

        QRect rect(leftCrop, topCrop, rightCrop - leftCrop, bottomCrop - topCrop);
        cout << StrCon(rect).to_s();
        return rect;

    }

    DistanceFieldGLFont::FontMetrics QtDistanceFieldFontTextureGenerator::pack(QImage characterImage,
                                                                               const DistanceFieldGLFont::FontMetrics& sourceMetrics,
                                                                               QImage& fontAtlas,
                                                                               packBitmap& marked)
    {
        // Scanline search from left to right, top to bottom to find an open area for the image.
        for (auto row = marked.begin(); row < marked.end(); ++row)
        {

        }


    }

    DistanceFieldGLFont::FontMetrics QtDistanceFieldFontTextureGenerator::getCharacterFontMetrics(QString character)
    {

        QFontMetrics fontMetrics(m_font);
        DistanceFieldGLFont::FontMetrics data;
        /* Here we go again.
         * Qt uses a top-left coordinate frame, which gets me every time. I guess this makes sense
         * from a "this is where text starts" perspective, but it's always a gotcha when drawing anything.
         * The origin of this font data is at 0, 0. It draws in the negative direction, "above" the origin,
         * because of this. That is, if it drew into +Y, since +Y is down, the font would be upside down.
         * Therefore, the value of "Q" for this rectangle is
         * left: 0, right: 8, bottom: 1, top: -11. It's drawing from 1 pixel below the origin
         * to 11 pixels above the origin. We use a sane coordinate frame for drawing, so we'll invert
         * to just fix that issue right away.
         *
         */
        data.character = static_cast<std::string>(StrCon(character))[0];
        /* leftBearing is the right-ward distance of the left-most pixel from the logical origin.
         * This means we have to shift the image -leftBearing to put the image origin at the logical origin.*/
        data.logical_origin.setX(-fontMetrics.leftBearing(character[0]));
        data.logical_origin.setY(fontMetrics.descent());
        /*... except that the image width must be the logical origin, _plus_ the left margin,
         * _plus_ the width, _plus_ the right margin to set the edge correctly. */
        data.stride = fontMetrics.width(character[0]);
        data.span.setX(data.stride - fontMetrics.rightBearing(character[0]));
        data.span.setY(fontMetrics.height());

        /*
         * OK, all our work has gotten us this:
         * The logical origin is where we want to put the character. It is vertically displaced at least
         * as high as the descender height of the font, and it is horizontally displaced if there is ink
         * in the character (like 'j') that extend left of the logical origin (this is when logical origin's
         * x() is greater than 0). The span is at least as high (from the origin) as the font gets, at least
         * as low (from the origin) as the descenders get, and as wide as required as to cover the distance
         * to the next character's logical origin _plus_ any overhang due to ink in the character spilling
         * into the next character's boundary (as happens with cursive / italicized fonts).
         */
        return data;
    }
}
