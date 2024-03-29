CXXFLAGS := -g -std=c++11 -Wall -Wextra -Wpedantic -Wfatal-errors -Werror
CXXFLAGS += -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wcast-qual
#CXXFLAGS += -Wconversion -Wno-error=conversion
CXXFLAGS += -Wnon-virtual-dtor -Wold-style-cast -Wuseless-cast
# Sanitizers:
CXXFLAGS += -fsanitize=undefined,address #--coverage
LDFLAGS  := -fsanitize=undefined,address #--coverage
CC = $(CXX)

.DELETE_ON_ERROR:
.PHONY: all zip zip_tests test cov clean

all: hashdemo pish setdemo testParse testScanner

hashdemo: hashdemo.o
pish: pish.o Scanner.o Parse.o
setdemo: setdemo.o
testParse: testParse.o Scanner.o Parse.o
testScanner: testScanner.o Scanner.o

hashdemo.o: hashdemo.cpp hash.h
pish.o: pish.cpp hash.h Scanner.h Parse.h
Scanner.o: Scanner.cpp Scanner.h
Parse.o: Parse.cpp Parse.h
setdemo.o: setdemo.cpp set.h
testParse.o: testParse.cpp Scanner.h Parse.h
testScanner.o: testScanner.cpp Scanner.h

test: pish testParse
	cp $^ pish_tests/
	$(MAKE) -C hash_test/ $@
	$(MAKE) -C pish_tests/ $@

zip_tests: makefile pish_tests/ hash_test/
	$(MAKE) clean
	zip -r a5tests.zip $^

zip: hash.h pish.cpp
	zip a5.zip $^

cov:
	@echo ""
	@echo "[NOTE]: See 'coverage.html' for code coverage report."
	@echo ""
	gcovr --html-self-contained --html-details coverage.html


clean:
	rm -f *.o hashdemo pish setdemo testParse testScanner
	rm -f coverage.* *.gcda *.gcno *.gcov
	$(MAKE) -C hash_test/ $@
	$(MAKE) -C pish_tests/ $@
