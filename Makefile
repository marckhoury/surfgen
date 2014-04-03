OS = $(shell uname -s)

ifeq ($(OS),Darwin)
    CC = clang++
    LIBS = -framework GLUT -framework OpenGL -framework Cocoa
    OPT = -O2 -std=c++11 -stdlib=libc++
else
    CC = g++
    LIBS = -lGL -lglut -lGLU 
    OPT = -O2 -std=c++0x
endif

OBJECTS = io.o mesh.o grid.o gengrid.o cutfunc.o marchingcubes.o mathutil.o
TARGET = surfgen

surfgen: $(OBJECTS)
	$(CC) $(OPT) -o $(TARGET) main.cpp $(OBJECTS) $(LIBS)

io.o: io.h io.cpp
	$(CC) $(OPT) -c io.cpp

mesh.o: mesh.h mesh.cpp
	$(CC) $(OPT) -c mesh.cpp

grid.o: grid.h grid.cpp
	$(CC) $(OPT) -c grid.cpp

gengrid.o: gengrid.h gengrid.cpp
	$(CC) $(OPT) -c gengrid.cpp

cutfunc.o: cutfunc.h cutfunc.cpp
	$(CC) $(OPT) -c cutfunc.cpp

marchingcubes.o: marchingcubes.h marchingcubes.cpp
	$(CC) $(OPT) -c marchingcubes.cpp

mathutil.o: mathutil.h mathutil.cpp
	$(CC) $(OPT) -c mathutil.cpp

clean:
	rm *.o $(TARGET) *~
