## Plane-game
This is a simple game using opengl. Control the left plane with `A`+`S`+`left-shift` and the right one with `K`+`L`+`right-shift`.

### Build
A Makefile is provided, you just have to run `make` in the project's root. Requires some dependencies to be installed:
* glfw
* glew
* stb_image

### Running the game
After successfully building it, you have to run the game binary from the project's root, so the shaders under the `shaders` folder can be found. Run `bin/game.exe`