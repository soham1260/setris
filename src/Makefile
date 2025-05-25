CXX = g++
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

OBJ = main.o grid.o colors.o block.o position.o game.o toggleButton.o

all: main

main: $(OBJ)
	$(CXX) $(OBJ) -o main $(LDFLAGS)

main.o : main.cpp
	$(CXX) -c main.cpp -o main.o

grid.o : grid.cpp grid.h
	$(CXX) -c grid.cpp -o grid.o

colors.o : colors.cpp colors.h
	$(CXX) -c colors.cpp -o colors.o

block.o : block.cpp block.h
	$(CXX) -c block.cpp -o block.o

position.o : position.cpp position.h
	$(CXX) -c position.cpp -o position.o

game.o : game.cpp game.h
	$(CXX) -c game.cpp -o game.o

toggleButton.o : toggleButton.cpp toggleButton.h
	$(CXX) -c toggleButton.cpp -o toggleButton.o

clean:
	del /F /Q -f main.exe $(OBJ)
