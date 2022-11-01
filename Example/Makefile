CXX=g++
CXXFLAGS=-c -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE

SOURCES=../Stack/Config/configStack.cpp ../Stack/StackUtils/stackUtils.cpp ../Stack/Universal/universalUtils.cpp
OBJECTS=$(SOURCES:.cpp=.o)

MAIN=main.cpp
MAINOBJECTS=$(MAIN:.cpp=.o)
MAINEXECUTABLE=main

DIRFLAGS=-I../Stack/Universal -I../Stack/Config -I../Stack/StackUtils

# STACKFLAGS=-DDATACANARY -DSTRUCTCANARY -DHASH -DBIRTHINFO -DMAXCAPACITY

all: $(SOURCES) MAINCOMPILE
	
MAINCOMPILE: $(OBJECTS) $(MAINOBJECTS)
	@$(CXX) -g $(OBJECTS) $(MAINOBJECTS) -o $(MAINEXECUTABLE)

.cpp.o:
	@$(CXX) $(CXXFLAGS) $(DIRFLAGS) $(STACKFLAGS) $< -o $@

clear:
	@rm -rf $(OBJECTS) $(MAINOBJECTS) $(MAINEXECUTABLE)