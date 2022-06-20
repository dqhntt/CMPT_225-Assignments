CXXFLAGS := -g -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CXXFLAGS += -Wshadow -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wcast-qual
CXXFLAGS += -Wconversion -Wno-error=conversion
CXXFLAGS += -Wnon-virtual-dtor -Wold-style-cast -Wuseless-cast -Weffc++ -Wno-error=effc++
# Sanitizers:
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

StkTest: StkTest.o Stack.o

StkTest.o: StkTest.cpp Stack.o

Stack.o: Stack.cpp Stack.h

Test: Test.o Stack.o

run: StkTest
	./$<

test: Test
	./$<

clean:
	rm -f *.o *.gcno *.gcda *.gcov coverage.* StkTest Test

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html
