# Compiler options
CPPFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CPPFLAGS += --coverage
LDLIBS   := --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run clean cov dist distclean

MAIN := a1main
ARCH := a1.zip

SRCS := $(wildcard *.cpp *.c)
HDRS := $(wildcard *.hpp *.h)

# $(MAIN): $(SRCS) # No .o object files.
$(MAIN): $(filter %.o, $(SRCS:.cpp=.o) $(SRCS:.c=.o)) # With object files.

run: $(MAIN)
	valgrind ./$<

clean:
	rm -f *.o *.gcno *.gcda coverage.* $(MAIN)

cov: $(MAIN)
	@echo "\n""[NOTE]: Code coverage is only accurate after running the program once.""\n"
	gcovr --html-self-contained --html-details coverage.html

dist:
	zip $(ARCH) $(SRCS) $(HDRS) makefile

distclean:
	rm -f $(ARCH)
