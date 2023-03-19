# Common
SYS = $(shell uname)
CC = gcc
DEBUG_FLAGS =#-g -fsanitize=adress # dont work with gcov and verter faild with him
FLAGS = -c -Wall -Wextra -Werror $(DEBUG_FLAGS) -std=c11
LEAKS = nothing

# Library
LIB_SRC = lib/
SRCS = $(addprefix $(LIB_SRC), s21_add)
OBJS = $(SRCS:%.c=%.o)

LIB_NAME = s21_decimal.a
LIB_FLAGS = -lcheck -lm -lpthread

# Testing
TEST_SRC = test_files/s21_test_decimal.c
TEST_OBJ = $(TEST_SRC:.c=.o)


TEST_FLAGS_GCOV = --coverage -Wall -Wextra -Werror
REPORT_NAME = gcov_result
GCOV_INFO = gcov_result.info

ifeq ($(SYS), Darwin) # Проверяем ОС
	LIB_FLAGS = -lcheck
	LEAKS = leaks --atExit -- test
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
	rm -rf lib_obj ./lib/*.o ./*.a ./*.o ./a.out gcov_test *.html *.css report ./*.gcno ./*.gcda ./*.info ./math_o ./*.dSYM ./CPPLINT* test ./a.out

test: s21_decimal.a
	$(CC) $(TEST_FLAGS_GCOV) $(TEST_SRC) $(SRCS) $(LIB_NAME) $(LIB_FLAGS) -o gcov_test $(shell pkg-config --libs --cflags check)
	chmod +x *
	./gcov_test

gcov_report: s21_decimal.a
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d . # -t отвечает за имя теста https://stackoverflow.com/questions/837639/is-there-a-way-to-focus-lcov-code-coverage-reports-to-just-one-or-two-directorie
	genhtml -o report/ gcov_test.info # генерирует html страницу
	open ./report/index.html

check: test
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extension=c lib/*.c lib/*.h
	rm -rf CPPLINT.cfg
	cp ../materials/linters/.clang-format .
	clang-format -style=google -i lib/*.c lib/*.h s21_decimal.h test_files/*.c test_files/*.h
	clang-format -style=google -n lib/*.c lib/*.h s21_decimal.h test_files/*.c test_files/*.h
	rm -rf .clang-format
