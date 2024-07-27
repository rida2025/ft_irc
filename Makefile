NAME=ircserv
CPP= c++ 
CPPFLAGS=-Wall -Wextra -Werror -std=c++98
RM=rm -f
COMMANDSPATH=./Commands/
SERVERPATH= ./server/
EXCEPTIONPATH= ./exception/
SRC= $(SERVERPATH)Server.cpp $(SERVERPATH)Authenticator.cpp $(SERVERPATH)Client.cpp \
	 $(EXCEPTIONPATH)PasswordNotAcceptedException.cpp $(EXCEPTIONPATH)CouldNotBindServerSocketException.cpp\
	 $(EXCEPTIONPATH)CouldNotListenServerSocketException.cpp $(EXCEPTIONPATH)NewClientNotAcceptedException.cpp \
	 $(EXCEPTIONPATH)NonBlockServerSocketException.cpp $(EXCEPTIONPATH)OpenServerSocketException.cpp \
	 $(EXCEPTIONPATH)PollCheckFdsEventsException.cpp $(EXCEPTIONPATH)SocketCouldNotReuseAddrException.cpp \
	 $(COMMANDSPATH)Bot.cpp $(COMMANDSPATH)Commands.cpp $(COMMANDSPATH)Invite.cpp $(COMMANDSPATH)Join.cpp \
	 $(COMMANDSPATH)Kick.cpp $(COMMANDSPATH)Mode.cpp $(COMMANDSPATH)Privmsg.cpp $(COMMANDSPATH)Topic.cpp \
	 $(COMMANDSPATH)Quit.cpp

HEADERS= $(SERVERPATH)Server.hpp $(SERVERPATH)Authenticator.hpp $(SERVERPATH)Client.hpp $(EXCEPTIONPATH)Exceptions.hpp \
		 $(EXCEPTIONPATH)PasswordNotAcceptedException.hpp $(EXCEPTIONPATH)CouldNotBindServerSocketException.hpp \
		 $(EXCEPTIONPATH)CouldNotListenServerSocketException.hpp $(EXCEPTIONPATH)NewClientNotAcceptedException.hpp \
		 $(EXCEPTIONPATH)NonBlockServerSocketException.hpp $(EXCEPTIONPATH)OpenServerSocketException.hpp \
		 $(EXCEPTIONPATH)PollCheckFdsEventsException.hpp $(EXCEPTIONPATH)SocketCouldNotReuseAddrException.hpp \
		 $(COMMANDSPATH)Commands.hpp

SRC_MAIN = main.cpp
	
OBJ = $(SRC:.cpp=.o)
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ_MAIN) $(OBJ)
		@$(CPP) $(CPPFLAGS) -o $@ $(OBJ_MAIN) $(OBJ)
		@echo "the files has ben archived successfully"

%.o: %.cpp $(HEADERS)
		@$(CPP) $(CPPFLAGS) -o $@ -c $<
		@echo "the file $@ has been created from $<"


clean:
		@$(RM) $(OBJ) $(OBJ_MAIN)
		@echo "all the .o has been deleted successfully"
fclean: clean
		@$(RM) $(NAME)
		@echo "the $(NAME) has been deleted"
	
re: fclean all

.PHONY : clean all fclean re