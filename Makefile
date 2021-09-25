
NAME := minishell
CC 	 := gcc
CFLAG := -Wall -Werror -Wextra -I$(shell brew --prefix readline)/include
CDFLAGS := -lreadline -lhistory -L$(shell brew --prefix readline)/lib
SRCS :=	srcs/main.c \
		srcs/common_utils.c \
		srcs/lexar.c \
		srcs/parser.c \
		srcs/parser2.c \
		srcs/tree.c \
		srcs/syntax_error.c \
		srcs/execution/execute_input.c \
		srcs/execution/create_cmd_path.c \
		srcs/execution/run_builtin_cmd.c \
		srcs/execution/execution_utils.c \
		srcs/builtin/ft_exit.c \
		srcs/builtin/ft_export.c \
		srcs/builtin/ft_cd.c \
		srcs/builtin/cd_utils.c \
		srcs/builtin/ft_env.c \
		srcs/builtin/ft_unset.c \
		srcs/builtin/ft_pwd.c \
		srcs/builtin/ft_export_utils.c \
		srcs/builtin/env_utils.c \
		srcs/piping/execute_pipe.c \
		srcs/piping/pipe_nodes_utils.c \
		srcs/piping/run_pipe_cmd.c
OBJS := $(SRCS:.c=.o)
INCS := ./incs
LIB := libft
LIBNAME := libft.a

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

all :$(NAME)

ifeq ($(shell uname),Linux)
$(NAME): $(OBJS)
	$(MAKE) bonus -C $(LIB)
	$(CC) $(CFLAG)-I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME) $(CDFLAGS) 
else
$(NAME): $(OBJS)
	$(MAKE) bonus -C $(LIB)
	$(CC) $(CFLAG) $(CDFLAGS) -I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME)
endif

ifeq ($(shell uname),Linux)
debug: $(OBJS)
	$(MAKE) bonus -C $(LIB)
	$(CC) $(CFLAG)-I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME) $(CDFLAGS) -fsanitize=address
else
debug: $(OBJS)
	$(MAKE) bonus -C $(LIB)
	$(CC) $(CFLAG) $(CDFLAGS) -I$(INCS) -o $(NAME) $(OBJS) $(LIB)/$(LIBNAME) -fsanitize=address
endif

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C $(LIB)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIB)

re: fclean all
