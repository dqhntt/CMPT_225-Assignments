CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g
CXXFLAGS += -fsanitize=address -fsanitize=undefined # --coverage
LDFLAGS  := -fsanitize=address -fsanitize=undefined # --coverage
CC := $(CXX)

.DELETE_ON_ERROR:
.PHONY: run test clean cov dist distclean

eval: Scanner.o eval.o

Scanner.o: Scanner.cpp Scanner.h

eval.o: eval.cpp Scanner.h Stack.h

run: eval
	./eval

clean:
	rm -f *.o eval

cov: test
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html
