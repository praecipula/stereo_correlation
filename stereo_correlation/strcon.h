#ifndef STRCON_H
#define STRCON_H

#include <QString>

// forward declarations
class QPoint;
class QRect;

namespace Stereo
{

/* StrCon is essentially some string conversion routines for convenience.
 * I may migrate these over to macros, but I'm not performance limited at
 * this point, and it's kind of nice to have some of the built-in
 * class functionality to work with.
 */
class StrCon
{
public:

    // From point.
    StrCon(const QPoint& point);
    // From rect.
    StrCon(const QRect& rect);
    // From QString
    StrCon(const QString& string);
    // From std string
    StrCon(const std::string& string);

    // To std string
    std::string to_s() const{return m_str;}
    operator std::string() const{return to_s();}
    // To qstring
    operator QString() const {return QString::fromStdString(m_str);}

private:
    std::string m_str;
};

}
#endif // STRCON_H
