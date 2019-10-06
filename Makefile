.PHONY: build
build: bin/game.exe

bin/%.o: src/%.cpp
	g++ $< -std=c++11 -c -Iinc -o $@

bin/game.exe: $(patsubst src/%.cpp,bin/%.o,$(wildcard src/*.cpp))
	g++ $^ -std=c++11 -o $@ -lglew32 -lglfw3 -lopengl32 -lgdi32