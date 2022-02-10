msdscript: main.cpp cmdline.cpp cmdline.hpp expr.hpp expr.cpp catch.h parse.cpp parse.hpp
	c++ --std=c++14 -O2 -o msdscript main.cpp cmdline.cpp expr.cpp parse.cpp
.PHONY:test
test: msdscript
	./msdscript --test
