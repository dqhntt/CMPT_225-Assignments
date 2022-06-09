CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=address -fsanitize=undefined # --coverage
LDFLAGS  := -fsanitize=address -fsanitize=undefined # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

StkTest: StkTest.o Stack.o

StkTest.o: StkTest.cpp Stack.o

Stack.o: Stack.cpp Stack.h

test: StkTest
	./StkTest

clean:
	rm -f *.o StkTest

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html
