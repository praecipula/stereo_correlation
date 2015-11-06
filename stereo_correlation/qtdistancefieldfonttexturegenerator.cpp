#include "qtdistancefieldfonttexturegenerator.h"

namespace Stereo
{
    QtDistanceFieldFontTextureGenerator::QtDistanceFieldFontTextureGenerator(const QFont& font):
        m_font(new QFont(font))
    {

    }


    QString QtDistanceFieldFontTextureGenerator::generateFontTexture()
    {
        // For each glyph
        // Paint into QPixmap
        generateCharacterImage('A');
        // Process QPixmap to generate distance field
        // Pack the distance field image into the font buffer image
        // Set the parameters in QDistanceFieldFontTexture
        // Save the image
    }

    QPixmap QtDistanceFieldFontTextureGenerator::generateCharacterImage(QChar character)
    {

    }
}
