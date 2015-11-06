#ifndef QTDISTANCEFIELDFONTTEXTUREGENERATOR_H
#define QTDISTANCEFIELDFONTTEXTUREGENERATOR_H

#include <memory>
#include <boost/serialization/list.hpp>
#include <QString>
#include <QFont>
#include <QPixmap>
#include <QChar>


namespace Stereo
{
    class QtDistanceFieldFontTextureGenerator
    {
    public:

        typedef std::shared_ptr<QFont> font_ptr;

        QtDistanceFieldFontTextureGenerator(const QFont& font);
        ~QtDistanceFieldFontTextureGenerator();

        /* Generate the font texture and save it in the application's
         *QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
         * app data location.
         */
        QString generateFontTexture();

    private:
        // Generate a single character glyph
        QPixmap generateCharacterImage(QChar character);
        /*
         * Generate a distance field based on the glyph.
         * The returned pixmap will be cropped to the nearest white pixels.
         */
        //QPixmap generateDistanceField(const QPixmap& character);

        font_ptr m_font;

    };
}
#endif // QTDISTANCEFIELDFONTTEXTUREGENERATOR_H
