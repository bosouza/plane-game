#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float positionx;
uniform float positiony;

void main()
{
    gl_Position = vec4(aPos.x + positionx,aPos.y + positiony,0, 1.0);
    TexCoord = aTexCoord;
}