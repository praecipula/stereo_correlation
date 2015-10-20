#ifndef STRCON_H
#define STRCON_H

#include "boost/shared_ptr.hpp"

// forward declarations
class QPoint;

// Some string conversion routines for convenience
class StrCon
{
public:

    // From point.
    StrCon(const QPoint& point);

    operator std::string() const{return m_str;}

private:
    std::string m_str;
};

#endif // STRCON_H
