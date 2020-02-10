E_PATH=../engine/
3D_SRC_PATH=../src/3D/
2D_SRC_PATH=../src/2D/
UTILS_SRC_PATH=../src/utils/
OBJ_PATH=../obj/

all_libs: 3D 2D utils

3D: edge2i face3i frame matrix point2f point3f simple_mesh
	ar -r $(E_PATH)lib3dengine.a $(OBJ_PATH)edge2i.o $(OBJ_PATH)face3i.o $(OBJ_PATH)frame.o $(OBJ_PATH)matrix.o $(OBJ_PATH)point2f.o $(OBJ_PATH)point3f.o $(OBJ_PATH)simple_mesh.o
	
edge2i: $(3D_SRC_PATH)edge2i.cpp
	g++ -c $(3D_SRC_PATH)edge2i.cpp -lm -o $(OBJ_PATH)edge2i.o

face3i: $(3D_SRC_PATH)face3i.cpp
	g++ -c $(3D_SRC_PATH)face3i.cpp -lm -o $(OBJ_PATH)face3i.o

frame: $(3D_SRC_PATH)frame.cpp
	g++ -c $(3D_SRC_PATH)frame.cpp -lm -o $(OBJ_PATH)frame.o

matrix: $(3D_SRC_PATH)matrix.cpp
	g++ -c $(3D_SRC_PATH)matrix.cpp -lm -o $(OBJ_PATH)matrix.o

point2f: $(3D_SRC_PATH)point2f.cpp
	g++ -c $(3D_SRC_PATH)point2f.cpp -lm -o $(OBJ_PATH)point2f.o

point3f: $(3D_SRC_PATH)point3f.cpp
	g++ -c $(3D_SRC_PATH)point3f.cpp -lm -o $(OBJ_PATH)point3f.o

simple_mesh: $(3D_SRC_PATH)simple_mesh.cpp
	g++ -c $(3D_SRC_PATH)simple_mesh.cpp -lm -o $(OBJ_PATH)simple_mesh.o

2D: interface vector_font
	ar -r $(E_PATH)lib2dinterface.a $(OBJ_PATH)interface.o $(OBJ_PATH)vector_font.o

interface: $(2D_SRC_PATH)interface.cpp
	g++ -c $(2D_SRC_PATH)interface.cpp -lm -o $(OBJ_PATH)interface.o

vector_font: $(2D_SRC_PATH)vector_font.cpp
	g++ -c $(2D_SRC_PATH)vector_font.cpp -lm -o $(OBJ_PATH)vector_font.o

utils: bmpreader options
	ar -r $(E_PATH)utils.a $(OBJ_PATH)bmpreader.o $(OBJ_PATH)options.o

bmpreader: $(UTILS_SRC_PATH)bmpreader.cpp
	g++ -c $(UTILS_SRC_PATH)bmpreader.cpp -lm -o $(OBJ_PATH)bmpreader.o

options: $(UTILS_SRC_PATH)options.cpp
	g++ -c $(UTILS_SRC_PATH)options.cpp -lm -o $(OBJ_PATH)options.o
clean:
	rm -f $(OBJ_PATH)*.o
	rm -f $(E_PATH)*.a

