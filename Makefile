
NAME		= webserv

OBJS_DIR	= objects
SRC_DIR		= .

CCF_STRICT	= -Wall -Wextra -Werror -std=c++98
CCF_DEBUG	= -g3 -D VERBOSE=1
FLAGS		= -I ./$(SRC_DIR)/includes

SOURCES 	:= $(shell find . -name "*.cpp")
OBJECTS 	:= $(addprefix $(OBJS_DIR)/,$(patsubst %.cpp,%.o,$(SOURCES)))


ifdef VERBOSE
	CCF_STRICT += $(CCF_DEBUG)
endif

CCF_STRICT += $(FLAGS)


.PHONY: all
all: $(NAME);

$(NAME): $(OBJECTS)
	c++ $(OBJECTS) -o $(NAME)

$(addprefix $(OBJS_DIR)/,%.o): %.cpp
	mkdir -p $(dir $@)
	c++ $(CCF_STRICT) -c $< -o $@

.PHONY: clean
clean:
	rm -f .gitignore
	rm -rf $(OBJS_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all;

################################################################################
# DEBUG
################################################################################

.PHONY: debug/run
debug/run: re
	./$(NAME)

.PHONY: debug/valgrind
debug/valgrind: re
	valgrind ./$(NAME)


