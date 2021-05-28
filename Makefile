main:
	${CXX} main.cpp -o main -lGL -lGLU -lglut -lGLEW -lglfw

clean:
	rm ./main

all:
	${CXX} main.cpp -o main -lGL -lGLU -lglut -lGLEW -lglfw