.PHONY: build
build: bin/game.exe

bin/%.o: %.cpp
	g++ $< -std=c++11 -c -I. -o $@

bin/game.exe: $(patsubst %.cpp,bin/%.o,$(wildcard *.cpp))
	g++ $^ -std=c++11 -o $@ -lglew32 -lglfw3 -lopengl32 -lgdi32