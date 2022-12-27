CXX_WARNINGS = -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Weffc++ -Wvexing-parse -Wfloat-equal -Wundef -Wwrite-strings -Wunused-macros -Wmissing-declarations -Wcast-qual -Warith-conversion

CXX_FLAGS = ${CXX_WARNINGS} -std=c++20 -Werror

setup:
	@[ -d bin ] || mkdir bin

dummy1:
	@g++ ${CXX_FLAGS} -o $@ $@.cpp
	@mv $@ bin/

all: setup dummy1
	@echo "done!"
