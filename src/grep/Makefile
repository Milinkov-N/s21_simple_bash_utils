CC=gcc
SOURCES=main.c s21_grep.c matches.c ../common/vec.c ../common/utils.c ../common/args.c 
CFLAGS=
ERR_FLAGS=-Wall -Wextra -Werror
BIN=s21_grep
TEST_BIN=tests/run

all: cppcheck s21_grep build_tests run_tests

s21_grep: $(SOURCES)
	$(CC) $(ERR_FLAGS) $(CFLAGS) $(SOURCES) -o $(BIN)

build_tests:
	$(CC) $(ERR_FLAGS) $(CFLAGS) ../common/tests.c ../common/utils.c tests/main.c -o $(TEST_BIN)

run_tests:
	$(TEST_BIN)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ./

rebuild: clean all

clean:
	rm -rf ./$(BIN) \
	$(TEST_BIN) \
	./*.out \
	./*.o \
	./*.a \
	./*.so \
