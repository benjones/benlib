
CC=clang++
FLAGS=-Wall -Wno-c++11-extensions -Wno-c++98-compat -std=c++11 -stdlib=libc++ -O3

TEST_INPUTS = $(wildcard *.cpp)
TESTS=$(TEST_INPUTS:.cpp=.run)
all: $(TESTS)


%.run: %.cpp
	$(CC) $(FLAGS) $< -o $@
	./$@
