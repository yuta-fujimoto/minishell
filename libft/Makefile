# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fyuta <fyuta@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/04/08 23:10:27 by fyuta             #+#    #+#              #
#    Updated: 2021/10/27 20:31:42 by jthompso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAG  = -Wall -Wextra -Werror
SRCS = ft_memset.o ft_bzero.o ft_memccpy.o ft_memchr.o ft_memcmp.o ft_memcpy.o ft_memmove.o ft_strlen.o ft_isalnum.o ft_isalpha.o ft_isascii.o ft_isdigit.o ft_isprint.o ft_atoi.o ft_calloc.o ft_strchr.o ft_strdup.o ft_strlcat.o ft_strlcpy.o ft_strncmp.o ft_strnstr.o ft_strrchr.o ft_tolower.o ft_toupper.o ft_substr.o ft_strjoin.o ft_strtrim.o ft_split.o ft_itoa.o ft_strmapi.o ft_putchar_fd.o ft_putstr_fd.o ft_putendl_fd.o ft_putnbr_fd.o ft_strcjoin.o ft_envadd_back.o ft_envclear.o ft_envdelone.o ft_enviter.o ft_envnew.o ft_envsize.o ft_find_env_var.o ft_atol.o ft_str_arr_len.o ft_free_str_arr.o ft_striteri.o ft_pidlstnew.o ft_pidlstsize.o ft_pidlstlast.o ft_pidlstclear.o ft_pidlstadd_back.o ft_doclstnew.o ft_doclstsize.o ft_doclstlast.o ft_doclstclear.o ft_doclstadd_back.o
BONUS = ft_lstadd_back.o ft_lstadd_front.o ft_lstclear.o ft_lstdelone.o ft_lstiter.o ft_lstlast.o ft_lstmap.o ft_lstnew.o ft_lstsize.o
NAME = libft.a

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAG) -c $< 

$(NAME): $(SRCS)
	ar rcs $@ $^

bonus: $(SRCS) $(BONUS)
	ar cr $(NAME) $^

clean: 
	rm -f $(SRCS) $(BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all
