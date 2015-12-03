#ifndef QTDISTANCEFIELDFONTTEXTUREGENERATOR_H
#define QTDISTANCEFIELDFONTTEXTUREGENERATOR_H

#include <memory>
#include <boost/serialization/list.hpp>
#include <QString>
#include <QFont>
#include <QPixmap>
#include <QChar>
#include <sstream>
#include <tuple>

#include "common.h"
#include "distancefieldglfont.h"

namespace Stereo
{
    class QtDistanceFieldFontTextureGenerator
    {
    public:

        QtDistanceFieldFontTextureGenerator(const QFont& font, int spreadFactor = -1);
        ~QtDistanceFieldFontTextureGenerator();

        struct CharacterRange{
            CharacterRange(int beginning, int ending) :
                begin(beginning), end(ending) {}
            int begin;
            int end;
        };

        static CharacterRange defaultCharacterRange();

        /* Generate the font texture and save it in the application's
         * QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
         * app data location.
         */
        QString generateFontTexture(CharacterRange characterRange);
        // Overload to use default range
        QString generateFontTexture(){return generateFontTexture(defaultCharacterRange());}


    protected:
        /*
         * Generate a single character glyph.
         * Note: QImages are copied shallowly, so can simply be returned.
         */
        tuple<QImage, DistanceFieldGLFont::FontMetrics> generateCharacterImage(QString character);


        /*
         * Characters have a 0, 0 origin in our representation, but can be drawn
         * with descenders (lowering the bottom into negative territory)
         * or be italicized (encroaching to the left). This data is crucial
         * for proper drawing of characters.
         *
         * In a simplified form, this is a translator between QFontMetrics, which is
         * a bit overblown for our purposes, into our own DistanceFieldGLFont::FontMetrics,
         * which has enough information for us to be able to redraw the font
         * into character strings based on its data.
         */
        DistanceFieldGLFont::FontMetrics getCharacterFontMetrics(QString character);

        /*
         * Generate a distance field based on the glyph.
         * The returned pixmap will be cropped to the nearest white pixels.
         */
        QImage generateDistanceField(const QImage& character);

        int distanceToPixel(const QImage& characterImage, int x, int y, QColor colorToFind);

        QRect calculateAutocrop(const QImage& image);

        // Crop the image. Note that both cropExtents and metrics can be modified here.
        QImage crop(QRect& cropExtents, DistanceFieldGLFont::FontMetrics metrics, const QImage& image);

        /* When doing the pack operation, we need to have a type that will be used to pass a bitmap into the function
         * to record where we've painted. This is that type. Hopefully, if we're lucky, whichever standard library
         * implementation we're using will optimize vector<bool>.
         */
        typedef std::vector<std::vector<bool> > packBitmap;

        /* Take characterImage and place it into fontAtlas, returning a modified FontMetrics.
         */
        DistanceFieldGLFont::FontMetrics pack(QImage characterImage, const DistanceFieldGLFont::FontMetrics& sourceMetrics, QImage& fontAtlas, packBitmap& marked);


    private:
        QFont m_font;
        int m_spreadFactor;



    };
}
#endif // QTDISTANCEFIELDFONTTEXTUREGENERATOR_H
