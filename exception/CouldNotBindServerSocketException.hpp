#ifndef COULDNOTBINDSERVERSOCKETEXCEPTION_HPP
# define COULDNOTBINDSERVERSOCKETEXCEPTION_HPP
# include <exception>
# include <iostream>

class CouldNotBindServerSocketException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
