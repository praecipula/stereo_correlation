#include "strcon.h"

#include <sstream>
#include <QPoint>
#include <QRect>

namespace Stereo
{

    StrCon::StrCon(const QPoint& point):
      m_str()
    {
      std::stringstream buf;
      buf << point.x() << 'x' << point.y();
      m_str = buf.str();
    }

    StrCon::StrCon(const QRect &rect):
        m_str()
    {
      std::stringstream buf;
      buf << rect.left() << ',' << rect.top() << '@' << rect.width() << "x" << rect.height();
      m_str = buf.str();
    }

    StrCon::StrCon(const QString& string):
        m_str()
    {
      m_str = string.toUtf8().constData();
    }

    StrCon::StrCon(const QStringList& stringList):
        m_str()
    {
        std::stringstream buf;
        buf << "[";
        for (QString str : stringList)
        {
            buf << StrCon(str).to_s() << std::endl;
        }
        m_str = buf.str();
        // Replace last endl char with end bracket
        m_str.replace(m_str.length() - 1, 1, "]");
    }

    StrCon::StrCon(const std::string& string):
      m_str(string)
    {}
}
