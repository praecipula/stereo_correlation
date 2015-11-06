#ifndef ROUNDEDEDGEORTHOGRAPHICTEXTABLEBOX_H
#define ROUNDEDEDGEORTHOGRAPHICTEXTABLEBOX_H

#include "model/coordinate.h"

namespace Stereo
{
    /*
     * That's a mouthful.
     * When we need to display information in a 2d orthographic view,
     * this is an option: a box that has rounded corners and can render
     * a text texture inside itself to display information.
     */
    class RoundedEdgeOrthographicTextableBox
    {
    public:
        RoundedEdgeOrthographicTextableBox();
        ~RoundedEdgeOrthographicTextableBox();

        void moveTo(const Coordinate& coords);
        void resizeTo(const Coordinate& dimensions);
        void writeText(std::string text, const Coordinate& leftBaseline);
        void paintGL();

        Coordinate m_location;
        Coordinate m_dimensions;

    };
}
#endif // ROUNDEDEDGEORTHOGRAPHICTEXTABLEBOX_H
