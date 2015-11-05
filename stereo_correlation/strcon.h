#ifndef STRCON_H
#define STRCON_H

#include <QString>

// forward declarations
class QPoint;


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
    // From QString
    StrCon(const QString& string);
    // From std string
    StrCon(const std::string& string);

    // To std string
    operator std::string() const{return m_str;}
    // To qstring
    operator QString() const {return QString::fromStdString(m_str);}

private:
    std::string m_str;
};

}
#endif // STRCON_H
