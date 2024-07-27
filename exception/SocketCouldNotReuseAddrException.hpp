#ifndef SOCKETCOULDNOTREUSEADDREXCEPTION_HPP
# define SOCKETCOULDNOTREUSEADDREXCEPTION_HPP
# include <exception>
# include <iostream>


class SocketCouldNotReuseAddrException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
