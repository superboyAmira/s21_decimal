# Common
SYS = $(shell uname)
CC = gcc
DEBUG_FLAGS = -Wall -Wextra -Werror #-g -fsanitize=address
FLAGS = -c  $(DEBUG_FLAGS) -std=c11
LEAKS = nothing

# Library
LIB_SRC = lib/
SRCS = $(addprefix $(LIB_SRC), s21_add.c s21_sub.c s21_mod.c s21_mul.c s21_div.c s21_help_func.c s21_other_func.c s21_converters.c s21_compare.c)
OBJS = $(SRCS:%.c=%.o)

LIB_NAME = s21_decimal.a
LIB_FLAGS = -lcheck -lm -lpthread

# Testing
TEST_SRC = test_files/*.c
TEST_OBJ = $(TEST_SRC:.c=.o)


TEST_FLAGS_GCOV = --coverage -Wall -Wextra -Werror
REPORT_NAME = gcov_result
GCOV_INFO = gcov_result.info

ifeq ($(SYS), Darwin) # Проверяем ОС
	LIB_FLAGS = -lcheck
	LEAKS = leaks -atExit -- ./gcov_test
else
	LIB_FLAGS = -lcheck -lsubunit -pthread -lrt -lm
	LEAKS = CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

all: clean s21_decimal.a test gcov_report

s21_decimal.a: $(OBJS)
	ar rc $(LIB_NAME) $(OBJS)
	ranlib $(LIB_NAME)

%.o: %.c s21_decimal.h
	$(CC) $(FLAGS) $< -o $@
clean:
	rm -rf lib_obj lib/*.o ./*.a ./*.o ./a.out gcov_test *.html *.css report lib/*.dSYM ./*.gcno ./*.gcda ./*.info ./*.dSYM ./CPPLINT*

test: s21_decimal.a
	$(CC) $(TEST_FLAGS_GCOV) $(TEST_SRC) $(SRCS) $(LIB_NAME) $(LIB_FLAGS) -o gcov_test $(shell pkg-config --libs --cflags check)
	chmod +x *
	./gcov_test

gcov_report: s21_decimal.a
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d . # -t отвечает за имя теста https://stackoverflow.com/questions/837639/is-there-a-way-to-focus-lcov-code-coverage-reports-to-just-one-or-two-directorie
	genhtml -o report/ gcov_test.info # генерирует html страницу
	open ./report/index.html

check: test
	cp ../materials/linters/.clang-format .
	clang-format -style=google -i lib/*.c s21_decimal.h test_files/*.c test_files/*.h
	clang-format -style=google -n lib/*.c s21_decimal.h test_files/*.c test_files/*.h
	rm -rf .clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h lib/*.c
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extension=c lib/*.c lib/*.h
	rm -rf CPPLINT.cfg
	# cp ../materials/linters/.clang-format .
	# clang-format -style=google -i lib/*.c lib/*.h s21_decimal.h test_files/*.c test_files/*.h
	# clang-format -style=google -n lib/*.c lib/*.h s21_decimal.h test_files/*.c test_files/*.h
	# rm -rf .clang-format
