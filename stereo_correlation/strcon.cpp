#include "strcon.h"

#include <sstream>
#include <QPoint>

StrCon::StrCon(const QPoint& point):
    m_str()
{
    std::stringstream buf;
    buf << point.x() << 'x' << point.y();
    m_str = buf.str();
}

