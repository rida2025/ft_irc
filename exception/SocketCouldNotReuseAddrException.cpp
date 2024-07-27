#include "SocketCouldNotReuseAddrException.hpp"

const char *SocketCouldNotReuseAddrException::what(void) const throw()
{
    return "SocketCouldNotReuseAddrException";
}
