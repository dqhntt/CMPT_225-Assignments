## For assignment 2 of CMPT 225.

CXXFLAGS := -std=c++11 -Wall -Wextra -g -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=undefined,address # --coverage
LDFLAGS  := -fsanitize=undefined,address # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

## Different for each assignment. ##
MAIN := ??
TEST := foo
ARCH := a2.zip

SRCS  := $(wildcard *.cpp *.c)
TESTS := $(filter-out $(MAIN).cpp, $(SRCS))
SRCS  := $(filter-out $(TEST).cpp, $(SRCS))

$(MAIN): $(SRCS)
$(TEST): $(TESTS)

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
	zip $(ARCH) $(SRCS) *.h Analysis.pdf

distclean:
	rm -f $(ARCH)
