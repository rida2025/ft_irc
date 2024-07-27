#ifndef COULDNOTLISTENSERVERSOCKETEXCEPTION_HPP
# define COULDNOTLISTENSERVERSOCKETEXCEPTION_HPP
# include <exception>
# include <iostream>

class CouldNotListenServerSocketException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
