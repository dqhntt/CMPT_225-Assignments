CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CXXFLAGS += -O1 -fwrapv # --coverage
CC = $(CXX)

.DELETE_ON_ERROR:
.PHONY: run check clean

run: test
	./test

check: test
	valgrind -s --leak-check=full ./test

cov: run
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html

clean:
	rm -f test *.o coverage.* *.gcda *.gcno *.gcov
