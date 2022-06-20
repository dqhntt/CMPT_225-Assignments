## For assignment 1 of CMPT 225.

CXXFLAGS := -g -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
CXXFLAGS += -Wshadow -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wcast-qual
CXXFLAGS += -Wconversion -Wno-error=conversion 
# https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
CXXFLAGS += -Wnon-virtual-dtor -Wold-style-cast -Wuseless-cast -Weffc++ -Wno-error=effc++
# Sanitizers:
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

## Different for each assignment. ##
MAIN := a1main
TEST := test_main
ARCH := a1.zip

SRCS  := $(wildcard *.cpp *.c)
TESTS := $(filter-out $(MAIN).cpp, $(SRCS))
SRCS  := $(filter-out $(TEST).cpp, $(SRCS))

# $(MAIN): $(SRCS) # No .o object files.
$(MAIN): $(filter %.o, $(SRCS:.cpp=.o) $(SRCS:.c=.o)) # With object files.
$(TEST): $(filter %.o, $(TESTS:.cpp=.o) $(TESTS:.c=.o))

run: $(MAIN)
	./$<

test: $(TEST)
	./$<

clean:
	rm -f *.o *.gcno *.gcda *.gcov coverage.* $(MAIN) $(TEST)

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html

dist: clean
## Different for each assignment. ##
	zip $(ARCH) $(MAIN).cpp PlayList.*

distclean:
	rm -f $(ARCH)
