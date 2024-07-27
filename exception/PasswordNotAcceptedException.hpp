#ifndef PASSWORDNOTACCEPTEDEXCEPTION_HPP
# define PASSWORDNOTACCEPTEDEXCEPTION_HPP
# include <exception>

class PasswordNotAcceptedException : public std::exception
{
	private:
		/* data */
	public:
		virtual const char *what() const throw();
};
#endif
