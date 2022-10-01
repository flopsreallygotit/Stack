CXX=g++
CXXFLAGS=#-Wall -Wextra -ggdb3 -fsanitize=leak -lasan -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE

SOURCES=main.cpp stackUtils.cpp configStack.cpp universalUtils.cpp
LOGFILE=logOutput.txt

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=main

all: $(EXECUTABLE) oclear
	
$(EXECUTABLE): $(OBJECTS)
	@$(CXX) $(OBJECTS) -o $@

clear:
	@rm -rf $(OBJECTS) $(EXECUTABLE)

clean:
	@rm -rf $(OBJECTS) $(EXECUTABLE)

oclear:
	@rm -rf $(OBJECTS)

lclear:
	@rm -rf $(LOGFILE)
