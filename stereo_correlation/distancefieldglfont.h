#ifndef DISTANCEFIELDGLFONT_H
#define DISTANCEFIELDGLFONT_H

#include <sstream>
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>
#include <unordered_set>
#include "model/coordinate.h"
#include "common.h"

namespace Stereo
{
    /*
     * Distance field fonts enable crisp, rotatable billboard fonts
     * that alias well in 3d space, instead of being expanded with
     * a less-correct method.
     * Valve discovered this technique:
     * http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
     *
     * This class assumes a generated distance field texture has been created with a given
     * name under our naming scheme.
     *
     */
    class DistanceFieldGLFont
    {
    public:
        DistanceFieldGLFont();
        ~DistanceFieldGLFont();

        struct FontMetrics {
            string character; // Which character this actually is
            Coordinate location; // Information on where in the type atlas
            Coordinate span;     // represents this character.
            Coordinate logical_origin; // Where the "cursor point" is relative to bottom,left.
            int stride; // How far to move the logical origin to the next character.

            std::string to_s()
            {
                std::stringstream buf;
                buf << character << ": " << span.to_s() << "@" << location.to_s() << "[" << logical_origin.to_s() << "->" << stride << "]";
                return buf.str();
            }
        };

        struct FontMetricsHasher : public unary_function<FontMetrics, size_t> {
            size_t operator()(const FontMetrics& metrics) const {
                // Use std::hash for std::string
                std::hash<std::string> hasher;
                return hasher(metrics.character);
            }
        };

        struct FontMetricsComparator : public binary_function<FontMetrics, FontMetrics, bool>{
            bool operator()(const FontMetrics& first, const FontMetrics& second) const {
                // use std::equal_to for std::string
                std::equal_to<std::string> comparator;
                return comparator(first.character, second.character);
            }
        };

        typedef unordered_set<FontMetrics, FontMetricsHasher, FontMetricsComparator> metrics_set;

    };

}
namespace Boost
{
    namespace Serialization
    {
        template <class Archive>
        void serialize(Archive &ar, const Stereo::DistanceFieldGLFont::FontMetrics& metrics, const unsigned int file_version)
        {

        }
    }
}
BOOST_CLASS_VERSION(Stereo::DistanceFieldGLFont::FontMetrics, 0)
#endif // DISTANCEFIELDGLFONT_H
