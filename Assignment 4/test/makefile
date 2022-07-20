CXXFLAGS  := -g -std=c++11 -Wall -Wfatal-errors -Wno-sign-compare
SANITIZER := -fsanitize=undefined,address

.DELETE_ON_ERROR:
.PHONY: all run check test clean zip

all: a4test a4testASAN

# See: https://github.com/doctest/doctest/blob/master/doc/markdown/commandline.md
run: a4test
	@echo
	@echo "[NOTE] First running without the stress test."
	@echo
	@./a4test -tce="*stress*"
	@echo
	@echo "[NOTE] Running the stress test."
	@echo "-- This may take up to a minute to finish."
	@echo "-- Press Ctrl-C to cancel."
	@echo
	@./a4test -ltc -tc="*stress*"
	@./a4test -ni -tc="*stress*"

check: a4test
	valgrind -s --leak-check=full ./a4test --test-case-exclude="*stress*"

test: a4testASAN
	./a4testASAN -tce="*stress*"
	./a4testASAN -tc="*stress*"

clean:
	rm -f a4test a4testASAN debug 

zip: a4test.cpp debug.cpp makefile tools.h doctest.h Guide.pdf
	zip a4test.zip $^

a4test: a4test.cpp AVLTree.h tools.h
	$(CXX) -o $@ $< $(CXXFLAGS)

a4testASAN: a4test.cpp AVLTree.h tools.h
	$(CXX) -o $@ $< $(CXXFLAGS) $(SANITIZER)

debug: debug.cpp AVLTree.h tools.h
	$(CXX) -o $@ $< $(CXXFLAGS)
	./debug
