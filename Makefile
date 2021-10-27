
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
		srcs/handle_heredocs.c \
		srcs/execution/execute_input.c \
		srcs/execution/execute_simple_cmd.c \
		srcs/execution/create_cmd_path.c \
		srcs/execution/run_builtin_cmd.c \
		srcs/execution/execution_utils.c \
		srcs/builtin/ft_exit.c \
		srcs/builtin/ft_export.c \
		srcs/builtin/ft_cd.c \
		srcs/builtin/ft_echo.c \
		srcs/builtin/ft_env.c \
		srcs/builtin/ft_unset.c \
		srcs/builtin/ft_pwd.c \
		srcs/builtin/ft_cd_utils.c \
		srcs/builtin/canonical_path.c \
		srcs/builtin/ft_export_utils.c \
		srcs/builtin/env_utils.c \
		srcs/expansion/expansion.c \
		srcs/expansion/expansion_node.c \
		srcs/expansion/add_to_word.c \
		srcs/expansion/add_to_word_utils.c \
		srcs/expansion/eliminate_null_node.c \
		srcs/expansion/split_argv_by_blank.c \
		srcs/pipeline/execute_pipeline.c \
		srcs/pipeline/pipeline_utils.c \
		srcs/pipeline/run_pipe_cmd.c \
		srcs/pipeline/run_child.c \
		srcs/redirection/ms_redirection.c \
		srcs/redirection/set_redirection.c \
		srcs/redirection/redirection_handling.c \
		srcs/redirection/redirection_utils.c \
		srcs/redirection/open_heredoc.c \
		srcs/redirection/create_new_cmd.c
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
