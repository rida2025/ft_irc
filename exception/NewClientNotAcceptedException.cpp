#include "NewClientNotAcceptedException.hpp"

const char *NewClientNotAcceptedException::what(void) const throw()
{
    return "NewClientNotAcceptedException";
}
