INCS = exec.h test_msdscript.hpp test_Cmdline.hpp

OBJS = main.o exec.o test_msdscript.o test_Cmdline.o

CXXFLAGS = --std=c++14 -O2

test_msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o test_msdscript $(OBJS)

main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

exec.o: exec.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

test_msdscript.o: test_msdscript.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<
	
test_Cmdline.o: test_Cmdline.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<
