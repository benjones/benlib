
CC=clang++
FLAGS=-Wall -Wextra -Wno-c++11-extensions -Wno-c++98-compat -std=c++1y -stdlib=libc++ -O3 

TEST_INPUTS = $(wildcard *Test.cpp)
TESTS=$(TEST_INPUTS:.cpp=.run)

all: $(TESTS) testImmediateGl


%.run: %.cpp
	$(CC) $(FLAGS) $< -o $@
	./$@

clean:
	rm -rfv *.run testImmediateGl *.dSYM *~

testImmediateGl: testImmediateGl.cpp
	$(CC) $(FLAGS) testImmediateGl.cpp ../types/immediateGl.cpp -o testImmediateGl -framework SDL2 -framework OpenGl -F/Library/Frameworks -I/Library/Frameworks/SDL2.framework/Headers
