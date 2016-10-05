#ifndef TO_STRINGABLE_H
#define TO_STRINGABLE_H

#include <string>

namespace Stereo
{
    namespace Util
    {
        /**
         * Abstract base class for classes which will implement a to_s interface.
         */
        class ToStringable
        {
        public:

            enum Verbosity {low, medium, high, extreme};

            ToStringable(Verbosity defaultVerbosity = low) :
                m_defaultVerbosity(low) {}

        virtual std::string to_s(Verbosity verbosity) const = 0;
        std::string to_s(){return to_s(m_defaultVerbosity);}

        private:
            Verbosity m_defaultVerbosity;
        };
    }
}

#endif // WORKSPACE_H
