#ifndef DISTANCEFIELDGLFONT_H
#define DISTANCEFIELDGLFONT_H

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


    };
}
#endif // DISTANCEFIELDGLFONT_H
