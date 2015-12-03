#include "coordinate.h"

#include <sstream>

namespace Stereo
{

    const Coordinate Coordinate::NO_POINT(Coordinate::int_l::min(), Coordinate::int_l::min());

    Coordinate::Coordinate():
      m_x(0), m_y(0)
    {}

    Coordinate::Coordinate(int x, int y):
        m_x(x), m_y(y)
    {}

    Coordinate::Coordinate(const Coordinate &other):
        m_x(other.m_x), m_y(other.m_y)
    {}

    /*
     * Yes, we have the setX and setY functions. They differ from
     * implementation (we don't use them here) because we want to
     * be able to assign the sentinel value to this if we pass
     * NO_POINT into the assignment operator.
     */
    Coordinate& Coordinate::operator =(const Coordinate& other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        return *this;
    }

    bool Coordinate::operator==(const Coordinate& other) const
    {
        return m_x == other.m_x && m_y == other.m_y;
    }

    Coordinate Coordinate::operator +(const Coordinate& other) const
    {
        if (this->is_unset() || other.is_unset()) return NO_POINT;
        return Coordinate(m_x + other.m_x, m_y + other.m_y);
    }

    Coordinate Coordinate::operator -(const Coordinate& other) const
    {
        if (this->is_unset() || other.is_unset()) return NO_POINT;
        return Coordinate(m_x - other.m_x, m_y - other.m_y);
    }

    Coordinate Coordinate::operator *(int factor) const
    {
        if (this->is_unset()) return NO_POINT;
        return Coordinate(m_x * factor, m_y * factor);
    }

    Coordinate Coordinate::operator /(int factor) const
    {
        if (this->is_unset() || factor == 0) return NO_POINT;
        return Coordinate(m_x / factor, m_y / factor);

    }
}
