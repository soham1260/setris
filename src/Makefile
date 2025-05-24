CXX = g++
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

OBJ = main.o grid.o colors.o block.o position.o game.o

all: main

main: $(OBJ)
	$(CXX) $(OBJ) -o main $(LDFLAGS)

main.o : main.cpp
	$(CXX) -c main.cpp -o main.o

grid.o : grid.cpp
	$(CXX) -c grid.cpp -o grid.o

colors.o : colors.cpp
	$(CXX) -c colors.cpp -o colors.o

block.o : block.cpp
	$(CXX) -c block.cpp -o block.o

position.o : position.cpp
	$(CXX) -c position.cpp -o position.o

game.o : game.cpp
	$(CXX) -c game.cpp -o game.o

clean:
	del /F /Q -f main.exe $(OBJ)
