
NAME := minishell
CC 	 := gcc
CFLAG := -Wall -Werror -Wextra -I$(shell brew --prefix readline)/include
CDFLAGS := -lreadline -lhistory -L$(shell brew --prefix readline)/lib
SRCS :=	srcs/main.c \
		srcs/lexar.c \
		srcs/parser.c \
		srcs/parser2.c \
		srcs/tree.c \
		srcs/syntax_error.c \
		srcs/execute_input.c
OBJS := $(SRCS:.c=.o)
INCS := ./incs
LIB := libft
LIBNAME := libft.a

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

all :$(NAME)

$(NAME): $(OBJS)
	$(MAKE) bonus -C $(LIB) 
	$(CC) $(CFLAG) $(CDFLAGS) -I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME)

debug: $(OBJS)
	$(MAKE) bonus -C $(LIB) 
	$(CC) $(CFLAG) $(CDFLAGS) -I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME) -fsanitize=address

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C $(LIB)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIB)

re: fclean all
