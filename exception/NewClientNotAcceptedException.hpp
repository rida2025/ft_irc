#ifndef NEWCLIENTNOTACCEPTEDEXCEPTION_HPP
# define NEWCLIENTNOTACCEPTEDEXCEPTION_HPP
# include <exception>
# include <iostream>

class NewClientNotAcceptedException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
