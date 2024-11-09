all: project.bin

project.bin: Project4.o

%.o: %.cpp
	g++ $< -o ../bin/project.bin -Wall -lGL -lGLU -lglut -lGLEW

start: project.bin
	../bin/project.bin

format:
	clang-format -i *.cpp