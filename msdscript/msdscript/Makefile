
INCS = cmdline.hpp expr.hpp

OBJS = main.o cmdline.o expr.o parse.o

TESTINCS = ../../testMsdscript/testMsdscript/exec.h ../../testMsdscript/testMsdscript/test_msdscript.hpp


OBJS = main.o cmdline.o expr.o parse.o

TESTOBJS = testMain.o exec.o testMsdscript.o


CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

testMsdscript: $(TESTOBJS)
	$(CXX) $(CXXFLAGS) -o testMsdscript $(TESTOBJS)

.PHONY: test
test: msdscript
	./msdscript --test

main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

cmdline.o: cmdline.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

expr.o:	expr.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

parse.o: parse.cpp $(INCS)

	$(CXX) $(CXXFLAGS) -c $<

testMain.o: ../../testMsdscript/testMsdscript/main.cpp $(TESTINCS)
	$(CXX) $(CXXFLAGS) -o testMain.o -c $<

exec.o: ../../testMsdscript/testMsdscript/exec.h $(TESTINCS)
	$(CXX) $(CXXFLAGS) -o exec.o -c $<

testcmdline.o: ../../testMsdscript/testMsdscript/test_Cmdline.h $(TESTINCS)
	$(CXX) $(CXXFLAGS) -o exec.o -c $<
	
testMsdscript.o: ../../testMsdscript/testMsdscript/test_msdscript.hpp $(TESTINCS)
	$(CXX) $(CXXFLAGS) -o testMsdscript.o -c $<
