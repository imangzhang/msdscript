
msdscript: main.cpp cmdline.cpp expr.cpp parse.cpp Val.cpp env.cpp cmdline.hpp expr.hpp parse.hpp catch.h Val.hpp env.hpp pointer.h
	c++ --std=c++14 -O2 -o msdscript main.cpp cmdline.cpp expr.cpp parse.cpp Val.cpp env.cpp

.PHONY: test
test: msdscript
	./msdscript --test
