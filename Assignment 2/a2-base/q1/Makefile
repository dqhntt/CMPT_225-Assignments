CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

StkTest: StkTest.o Stack.o

StkTest.o: StkTest.cpp Stack.o

Stack.o: Stack.cpp Stack.h

Test: Test.cpp Stack.o

run: StkTest
	./$<

test: Test
	./$<

clean:
	rm -f *.o StkTest Test

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html
