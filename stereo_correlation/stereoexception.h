#ifndef STEREOEXCEPTION_H
#define STEREOEXCEPTION_H

#include <exception>
#include <sstream>
#include "common.h"
#include "boost/config.hpp"

namespace Stereo
{

    /**
     * @brief The AssertionFailure struct
     * An object that immediately logs to our error logger and can be thrown, constructed via
     * a macro for convenience.
     */
    struct AssertionFailure : public exception
    {
    public:
        AssertionFailure(char const* expression, char const* function, char const* file, long line, char const* message = NULL) :
            m_expression(expression), m_function(function), m_file(file), m_line(line), m_message(message)
        {
            std::stringstream sstr;
            if (!m_message)
            {
                sstr << "*ASSERT FAILED: " << m_expression;
                m_message = sstr.str().c_str();
            }
            LOGE << this->what();
        }

        virtual const char* what() const _NOEXCEPT
        {
            std::stringstream sstr;
            sstr << m_file << ":" << m_line << "(" << m_function << ")" << m_message;
            return sstr.str().c_str();
        }

        const char* m_expression;
        const char* m_function;
        const char* m_file;
        const long m_line;
        const char* m_message;
    };
}

#define STEREO_ASSERT(expr) (BOOST_LIKELY(!!(expr))? ((void)0): throw Stereo::AssertionFailure(#expr, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))
#define STEREO_LOG_ASSERT(expr, message) (BOOST_LIKELY(!!(expr))? ((void)0): throw Stereo::AssertionFailure(#expr, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#endif // STEREOEXCEPTION_H
