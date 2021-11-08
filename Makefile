#* ************************************************************************** *#
#*                                                                            *#
#*                                                                            *#
#*   Makefile                                                                 *#
#*                                                                            *#
#*   By: yhetman <yhetman@student.unit.ua>                                    *#
#*                                                                            *#
#*   Created: 2021/11/08 15:19:26 by yhetman                                  *#
#*   Updated: 2021/11/08 15:19:27 by yhetman                                  *#
#*                                                                            *#
#* ************************************************************************** *#

SRCS = sha-256.c test_collision.c

SRCS_DIR = src/

OBJ_DIR = obj/

OBJ = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

OBJ_DIR = obj/

LIB_NAME = libSHA256.a

SHA256 = SHA256

FLAGS = -I includes -Wall -Wextra -Werror

DEBUG_FLAGS = -g3 -fsanitize=address

SHA256_MAIN = src/main.c

all: make_obj_dir $(SHA256)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	gcc $(FLAGS) -c $< -o $@

$(LIB_NAME): $(OBJ)
	ar -rv $(LIB_NAME) $^
	ranlib $(LIB_NAME)

$(SHA256): $(LIB_NAME) $(SHA256_MAIN)
	gcc $(FLAGS) $(SHA256_MAIN) $(LIB_NAME) -o $(SHA256)

make_obj_dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(SHA256)
	rm -f $(LIB_NAME)

re: fclean all

.PHONY: all clean flcean re debug