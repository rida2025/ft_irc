#ifndef NONBLOCKSERVERSOCKETEXCEPTION_HPP
# define NONBLOCKSERVERSOCKETEXCEPTION_HPP
# include <exception>
# include <iostream>

class NonBlockServerSocketException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
