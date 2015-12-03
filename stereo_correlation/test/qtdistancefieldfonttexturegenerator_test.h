#ifndef QTDISTANCEFIELDFONTTEXTUREGENERATOR_TEST_H
#define QTDISTANCEFIELDFONTTEXTUREGENERATOR_TEST_H

#include "qtdistancefieldfonttexturegenerator.h"
#include "gtest/gtest.h"
#include "strcon.h"
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <QChar>

#ifdef STEREO_TEST
    #pragma message "TESTING"
#endif


using namespace Stereo;
using namespace std;


class FriendlyQtDistanceFieldFontTextureGenerator : public QtDistanceFieldFontTextureGenerator {
public:
    FriendlyQtDistanceFieldFontTextureGenerator(const QFont& font) :
        QtDistanceFieldFontTextureGenerator(font) {
    }
    ~FriendlyQtDistanceFieldFontTextureGenerator(){}

    using QtDistanceFieldFontTextureGenerator::generateCharacterImage;
    using QtDistanceFieldFontTextureGenerator::generateDistanceField;
    using QtDistanceFieldFontTextureGenerator::calculateAutocrop;
    using QtDistanceFieldFontTextureGenerator::crop;
    using QtDistanceFieldFontTextureGenerator::pack;
};

// The fixture for testing class Foo.
class QtDistanceFieldFontGeneratorTest : public ::testing::Test {
 protected:
    FriendlyQtDistanceFieldFontTextureGenerator m_generator;
    QtDistanceFieldFontGeneratorTest() :
        m_generator(QFont("Arial", 128))
    {
    }

    // Get an image from a standard location to choose as a known correct image
    QImage openExpectedImage(QImage image, QString filename) {

    }

    // Save an image as a potential correct image to be viewed and approved
    void saveImageAsPotentialExpectedImage(QImage image, QString filename) {
        image.save(filename);
    }

};


// Tests that the Foo::Bar() method does Abc.
TEST_F(QtDistanceFieldFontGeneratorTest, GeneratesImage) {

    short intsize = sizeof(int);
    // Starting from 1 (ommitting the null character) and going through a basic set.
    for (int c = QtDistanceFieldFontTextureGenerator::defaultCharacterRange().begin;
         c <= QtDistanceFieldFontTextureGenerator::defaultCharacterRange().end; ++c)
    {
        if (isprint(c))
        {
            std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv1;
            std::string u8str = conv1.to_bytes(c);
            cout << u8str << endl;
            QImage characterImage = std::get<0>(m_generator.generateCharacterImage(StrCon(u8str)));
            stringstream filename;
            filename << "/tmp/charimage_" << std::hex << int(c) << ".png";
            saveImageAsPotentialExpectedImage(characterImage, StrCon(filename.str()));
        }
    }
}

TEST_F(QtDistanceFieldFontGeneratorTest, GeneratesDistanceField) {

    for (char c = 'a'; c <= 'z'; ++c)
    {
        QImage characterImage = std::get<0>(m_generator.generateCharacterImage(QString(QChar(c))));
        QImage distanceField = m_generator.generateDistanceField(characterImage);
        stringstream filename;
        filename << "/tmp/distimage_" << c << ".png";
        saveImageAsPotentialExpectedImage(distanceField, StrCon(filename.str()));
    }
}

TEST_F(QtDistanceFieldFontGeneratorTest, CropsImage) {
    for (char c = 'a'; c <= 'z'; ++c)
    {
        QImage characterImage;
        DistanceFieldGLFont::FontMetrics metrics;
        tie(characterImage, metrics) = m_generator.generateCharacterImage(QString(QChar(c)));
        QImage distanceField = m_generator.generateDistanceField(characterImage);
        // Pack the distance field image into the font buffer image
        QRect cropRect = m_generator.calculateAutocrop(distanceField);
        // Do the autocrop
        QImage cropped = m_generator.crop(cropRect, metrics, distanceField);

        stringstream filename;
        filename << "/tmp/cropimage_" << c << ".png";
        saveImageAsPotentialExpectedImage(cropped, StrCon(filename.str()));
    }
}

TEST_F(QtDistanceFieldFontGeneratorTest, PacksImage) {
    for (char c = 'a'; c <= 'z'; ++c)
    {
        QImage characterImage;
        DistanceFieldGLFont::FontMetrics metrics;
        tie(characterImage, metrics) = m_generator.generateCharacterImage(QString(QChar(c)));
        QImage distanceField = m_generator.generateDistanceField(characterImage);
        // Pack the distance field image into the font buffer image
        QRect cropRect = m_generator.calculateAutocrop(distanceField);
        // Do the autocrop
        QImage cropped = m_generator.crop(cropRect, metrics, distanceField);

        stringstream filename;
        filename << "/tmp/cropimage_" << c << ".png";
        saveImageAsPotentialExpectedImage(cropped, StrCon(filename.str()));
    }
}


#endif // QTDISTANCEFIELDFONTTEXTUREGENERATOR_TEST_H

