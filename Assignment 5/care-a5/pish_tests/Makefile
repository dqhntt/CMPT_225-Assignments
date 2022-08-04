CXXFLAGS := -O3 #-g -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Werror
CC = $(CXX)

.DELETE_ON_ERROR:
.PHONY: test run clean

test: run

# https://stackoverflow.com/a/29747593
run: pish_test.sh pish testParse transform
	@sed -i -e 's/\r$$//' $<
	chmod +x $<
	./$<

pish:
	$(MAKE) -C .. $@
	cp ../$@ .

testParse:
	$(MAKE) -C .. $@
	cp ../$@ .

clean:	
	rm -f testParse transform pish
	rm -f *.pish.py *.py.expected *.got *.diff *.parsed
