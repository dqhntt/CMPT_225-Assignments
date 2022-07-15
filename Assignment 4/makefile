CXXFLAGS := -g -std=c++11 -Wall -Wextra -Wpedantic #-Werror -Wfatal-errors
CXXFLAGS += -Wshadow -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wcast-qual
CXXFLAGS += -Wconversion -Wno-error=conversion
CXXFLAGS += -Wnon-virtual-dtor -Wold-style-cast -Wuseless-cast -Weffc++ -Wno-error=effc++
# Sanitizers:
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
CC = $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean

a4main.o: a4main.cpp AVLTree.h

run: a4main
	./a4main

test: a4main
	valgrind -s --leak-check=full ./a4main

cov: run
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html

clean:
	rm -f a4main *.o coverage.* *.gcda *.gcno *.gcov
