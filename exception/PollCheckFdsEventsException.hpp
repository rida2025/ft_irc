#ifndef POOLCHECKFDSEVENTSEXCEPTION_HPP
# define POOLCHECKFDSEVENTSEXCEPTION_HPP
# include <exception>
# include <iostream>

class PollCheckFdsEventsException : public std::exception
{
    private:
        /* data */
    public:
        virtual const char *what(void) const throw();
};

#endif
