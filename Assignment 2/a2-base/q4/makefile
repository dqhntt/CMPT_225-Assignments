CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=address -fsanitize=undefined # --coverage
LDFLAGS  := -fsanitize=address -fsanitize=undefined # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

all: x

x: Queue.o main.o

Queue.o: Queue.cpp Queue.h

main.o:	main.cpp Queue.h

run: x
	./x

clean:
	rm -f x *.o

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html
