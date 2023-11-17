GCC = g++
FLAGS = -std=c++98 -g #-Wall -Wextra -Werror 

NAME = ircserv

GREEN       = \033[32m
RED         = \033[31m
DEFAULT     = \033[37m

PATHOBJ = obj/

PATHSRC = src/

HEAD =   src/classes/Server.hpp src/classes/User.hpp src/classes/Executor.hpp src/classes/Channel.hpp

FILES = main.cpp pr.cpp classes/Server.cpp classes/User.cpp classes/Executor.cpp classes/Channel.cpp

SRCS = $(addprefix $(PATHSRC)/,$(FILES))

OBJ = $(patsubst %.cpp,$(PATHOBJ)/%.o,$(FILES))

DEP = Makefile $(HEAD)

ifeq ($(shell uname),Darwin)
    ECHO    = echo
else
	ECHO    = echo
endif

all: $(NAME)

$(PATHOBJ):
	@mkdir -p obj
	@mkdir -p obj/classes

$(NAME): $(DEP) $(SRCS) $(PATHOBJ) $(OBJ)
	@$(ECHO) "\r$(GREEN) The .o from $(NAME) are compiled.$(DEFAULT)"
	@$(GCC) $(FLAGS) $(OBJ) -o $(NAME)
	@$(ECHO) "$(GREEN)$(NAME)$(DEFAULT) created."

$(PATHOBJ)/%.o: $(PATHSRC)/%.cpp $(DEP)
	@$(ECHO) "\r$(GREEN) Compiling $@                      \c\033[K"
	@$(GCC) $(FLAGS) -c $< -o $@

clean:
	@$(ECHO) "All $(RED).o$(DEFAULT) are now deleted for $(NAME)."
	@rm -rf $(PATHOBJ)

fclean: clean
	@$(ECHO) "$(RED)$(NAME)$(DEFAULT) is now deleted."
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re