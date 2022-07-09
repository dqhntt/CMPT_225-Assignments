CXXFLAGS := -g -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CXXFLAGS += -Wshadow -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wcast-qual
CXXFLAGS += -Wnon-virtual-dtor -Wold-style-cast -Wuseless-cast -Weffc++ -Wno-error=effc++
# Prevent UB overflow.
CXXFLAGS += -O1 -fwrapv
# Sanitizers:
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
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
