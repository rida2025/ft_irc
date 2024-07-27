#ifndef OPENSERVERSOCKETEXCEPTION_HPP
# define OPENSERVERSOCKETEXCEPTION_HPP
# include <exception>
# include <iostream>

class OpenServerSocketException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
