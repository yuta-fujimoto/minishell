
NAME := minishell
CC 	 := gcc
CFLAG := -Wall -Werror -Wextra
SRCS :=	srcs/main.c \
		srcs/lexar.c \
		srcs/parser.c \
		srcs/parser2.c \
		srcs/tree.c \
		srcs/syntax_error.c
OBJS := $(SRCS:.c=.o)
INCS := ./incs
LIB := libft
LIBNAME := libft.a

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

all :$(NAME)

$(NAME): $(OBJS)
	@$(MAKE) bonus -C $(LIB) 
	$(CC) $(CFLAG) -I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME)  -L .brew/opt/readline/lib -I .bre

debug: $(OBJS)
	@$(MAKE) bonus -C $(LIB) 
	$(CC) $(CFLAG) -I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME)  -lreadline -fsanitize=address

clean:
	rm -f $(OBJS)
##TODO need to remove libft/*.o too
fclean: clean
	rm -f $(NAME)
##TODO need to remove libft/libft.a too

re: fclean all
