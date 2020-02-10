SRC=game.cpp

glgame:
	g++  -pthread -Wall -g $(SRC) -L/usr/X11/lib64 -lGL -lGLU -lX11 -lpthread -o game

clean:
	rm -f *.o
	rm -f game

