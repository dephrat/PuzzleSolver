CXXFLAGS = -std=c++17 -Wall -g -MMD -pthread -O3
CXXCMD = g++ $(CXXFLAGS)

STAGE_NUMS = 0 1 2 3
STAGE_NAMES = $(STAGE_NUMS:%=stage%)

SRC_FILENAMES = display main messages piece solver solver_threaded solutions $(STAGE_NAMES)
INC_FILENAMES = display messages piece solutions solver task $(STAGE_NAMES)

SOURCES = $(SRC_FILENAMES:%=src/%.cpp)
OBJECTS = $(SRC_FILENAMES:%=build/%.o)
DEPFILES = $(OBJECTS:%.o=%.d)

all: puzzleSolver

-include $(DEPFILES)

puzzleSolver: $(OBJECTS)
	$(CXXCMD) -o puzzleSolver $(OBJECTS)

build/%.o: src/%.cpp | build
	$(CXXCMD) -c -o $@ $<

build:
	mkdir -p build

.PHONY: clean

clean:
	rm -f build/*.o build/*.d puzzleSolver
