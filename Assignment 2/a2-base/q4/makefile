CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

all: main

main: Queue.o main.o

Queue.o: Queue.cpp Queue.h

main.o:	main.cpp Queue.h

test_main: test_main.o Queue.o

run: main
	./$<

test: test_main
	./$<

clean:
	rm -f *.o *.gcno *.gcda *.gcov coverage.* main test_main

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html
