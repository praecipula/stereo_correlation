#ifndef COORDINATE_H
#define COORDINATE_H

#include "common.h"
#include <string>
#include <limits>
#include "boost/archive/xml_wiarchive.hpp"
#include "boost/archive/xml_woarchive.hpp"
#include "boost/serialization/nvp.hpp"

namespace Stereo
{
   /*
    * The Coordinate class is a simple decarator over a 2-int tuple
    * Any 2d int coordinates should use this class.
    * Why not use QPoint? Because I want to decouple as much as possible
    * for Qt in order to potentially build a command-line image processor.
    */
    class Coordinate
    {
        // Just for convenience
        typedef std::numeric_limits<int> int_l;
    public:


        Coordinate();
        Coordinate(int x, int y);
        Coordinate(const Coordinate& other);

        // getters
        int x() const {return m_x;}
        int y() const {return m_y;}
        /*
        * Some explanation: I'm reserving [INT_MIN, INT_MIN] to be a sentinel value
        * for "unset". Therefore, we can't naturally use this number accidentally.
        */
        void setX(int x){m_x = x == int_l::min() ? int_l::min() + 1 : x;}
        void setY(int y){m_y = y == int_l::min() ? int_l::min() + 1 : y;}
        void set(int x, int y){setX(x); setY(y);}

        // NO_POINT handlers
        bool is_unset() const {return *this==NO_POINT;}
        void unset(){*this = NO_POINT;}

        // coords-based tuple assignment
        Coordinate& operator= (const Coordinate& coords);
        bool operator==(const Coordinate& other) const;

        // Math
        Coordinate operator+ (const Coordinate& other) const;
        Coordinate operator- (const Coordinate& other) const;
        Coordinate operator* (int factor) const;
        Coordinate operator/ (int factor) const;

        // String print
        std::string to_s() const {std::ostringstream buf;
                                  buf << "x:" << m_x << ", y:" << m_y;
                                  return buf.str();}
        // With implicit conversion
        operator string() {return to_s();}

        // "unset" sentinel
        static const Coordinate NO_POINT;
    private:
        int m_x;
        int m_y;

    };
}

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        void serialize(Archive& ar, const Stereo::Coordinate& coordinate, const unsigned int version)
        {
            ar << coordinate.x();
            ar << coordinate.y();
        }
    }
}
BOOST_CLASS_VERSION(Stereo::Coordinate, 0)
#endif // COORDINATE_H
