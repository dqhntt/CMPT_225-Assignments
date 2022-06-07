## For assignment 1 of CMPT 225.

CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=address -fsanitize=undefined # --coverage
LDFLAGS  := -fsanitize=address -fsanitize=undefined # --coverage
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
