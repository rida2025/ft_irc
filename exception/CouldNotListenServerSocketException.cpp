#include "CouldNotListenServerSocketException.hpp"

const char *CouldNotListenServerSocketException::what(void) const throw()
{
    return "CouldNotListenServerSocketException";
}
