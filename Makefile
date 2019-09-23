p.exe: main.cpp util_opengl.cpp util_opengl.h
	g++ main.cpp util_opengl.cpp -I. -lglew32 -lglfw3 -lopengl32 -lgdi32 -o p.exe