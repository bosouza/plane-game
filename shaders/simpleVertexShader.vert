#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 rotationMatrix;

out vec2 TexCoord;

uniform float positionx;
uniform float positiony;

void main()
{
    vec4 pos = rotationMatrix * vec4(aPos, 1.0);
    gl_Position = vec4(pos.x + positionx,pos.y + positiony,pos.z,pos.w);
    TexCoord = aTexCoord;
}