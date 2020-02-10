EPATH=../../engine/
ENGINE=$(EPATH)lib2dinterface.a	$(EPATH)lib3dengine.a $(EPATH)utils.a
SRC=about_form.cpp game.cpp lmain.cpp my_form1.cpp my_interface.cpp opt_form.cpp

labirint:
	g++  $(SRC) $(ENGINE) -L/usr/X11/lib64 -lGL -lGLU -lX11 -o game

clean:
	rm -f *.o
	rm -f game

