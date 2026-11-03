CXX = c++
CPPFLAGS = -Iinclude -Iasm/include -Ispu/include -Istack/include
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g

SOURCES = asm/src/onegin.cpp asm/src/assem.cpp spu/src/spu.cpp spu/src/funk.cpp stack/src/stack.cpp
HEADERS = include/calc.h asm/include/assem.h spu/include/spu.h stack/include/stack.h

.PHONY: all clean
all: calc_app

calc_app: $(SOURCES) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SOURCES) -o $@

stack_test: stack/src/main.cpp stack/src/stack.cpp stack/include/stack.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) stack/src/main.cpp stack/src/stack.cpp -o $@

clean:
	$(RM) calc_app stack_test
