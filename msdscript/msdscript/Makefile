
msdscript: main.cpp cmdline.cpp expr.cpp parse.cpp cmdline.hpp expr.hpp parse.hpp catch.h Val.hpp Val.cpp
	c++ --std=c++14 -O2 -o msdscript main.cpp cmdline.cpp expr.cpp parse.cpp Val.cpp

.PHONY: test
test: msdscript
	./msdscript --test
