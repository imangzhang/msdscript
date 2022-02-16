
msdscript: main.cpp cmdline.cpp expr.cpp parse.cpp cmdline.hpp expr.hpp parse.hpp catch.h
	c++ --std=c++14 -O2 -o msdscript main.cpp cmdline.cpp expr.cpp parse.cpp

.PHONY: test
test: msdscript
	./msdscript --test
