p.exe: main.cpp util_opengl.cpp util_opengl.h parallax_loader.cpp parallax_loader.h
	g++ main.cpp util_opengl.cpp parallax_loader.cpp -std=c++11 -I. -lglew32 -lglfw3 -lopengl32 -lgdi32 -o p.exe