#include "Environment/StringUtilities.hpp"
#include "UnitTest++/UnitTest++.h"

using namespace Sysmel::Environment;

SUITE(StringUtilities)
{
    TEST(FormatSymbol)
    {
        CHECK_EQUAL("#\"\"", formatSymbolLiteral(""));
        CHECK_EQUAL("#+", formatSymbolLiteral("+"));
        CHECK_EQUAL("#a", formatSymbolLiteral("a"));
        CHECK_EQUAL("#_hello1234", formatSymbolLiteral("_hello1234"));
        CHECK_EQUAL("#\"1 _hello1234\"", formatSymbolLiteral("1 _hello1234"));
        CHECK_EQUAL("#\"1a\"", formatSymbolLiteral("1a"));
        CHECK_EQUAL("#\"1\"", formatSymbolLiteral("1"));
    }

    TEST(Format)
    {
        CHECK_EQUAL("test format", formatString("test format", {}));
        CHECK_EQUAL("test format 1234", formatString("test format {0}", {"1234"}));
        CHECK_EQUAL("test format {0}", formatString("test format {{0}}", {"1234"}));
        CHECK_EQUAL("test format Hello World 1234", formatString("test format {1} {0}", {"1234", "Hello World"}));
    }
}
