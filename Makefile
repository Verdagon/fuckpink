current_dir = $(shell pwd)

CC = clang++
CFLAGS = -std=c++11 -stdlib=libc++ -g -I$(current_dir)/glew-1.10.0/include -I$(current_dir)/glm/glm -I/Users/verdagon/Library/Frameworks/SDL2.framework/Headers -I/opt/local/include/libpng16
LDFLAGS = -stdlib=libc++ -F/Users/verdagon/Library/Frameworks
LIBS =  -framework SDL2 -framework OpenGL glew-1.10.0/lib/libGLEW.a -lpng -L/opt/local/lib

SRCS = src/utilities.cpp $(filter-out src/utilities.cpp, $(wildcard src/*.cpp))
OBJS = $(patsubst src/%, build/%, $(patsubst %.cpp, %.o, $(SRCS)))

all:	fuckpink

fuckpink:	$(OBJS)
	$(CC) $^ -o fuckpink $(LIBS) $(LDFLAGS)

build/%.o:	src/%.cpp src/*.h
	$(CC) $< -c -o $@ $(CFLAGS)

run:	fuckpink
	./fuckpink

debug:	fuckpink
	gdb ./fuckpink

clean:
	rm -f fuckpink
	rm -f build/*.o
