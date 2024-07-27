#include "./PasswordNotAcceptedException.hpp"

const char *PasswordNotAcceptedException::what() const throw()
{
	return "password not accepted , the password has to be more than 4 characters in length, no white spaces allowed";
}
