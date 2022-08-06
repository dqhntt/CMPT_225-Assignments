CXXFLAGS := -g -std=c++11 -Wall -Wextra -Wpedantic -Wfatal-errors
ASAN := -fsanitize=undefined,address

CXXFLAGS += $(ASAN)
LDFLAGS := $(ASAN)
INCL := -I./multiprecision-1.79/include
CC = $(CXX)

.DELETE_ON_ERROR:
.PHONY: test run clean

test: run
run: hash_test
	./$<

hash_test.o: hash_test.cpp ../hash.h
	@echo "Please be patient!"
	$(CXX) $(CXXFLAGS) $(INCL) $< -c -o $@

clean:	
	rm -f *.o hash_test
