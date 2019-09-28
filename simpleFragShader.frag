#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float offsetx;
uniform float offsety;

void main()
{
    vec2 vt = vec2(TexCoord.x + offsetx, TexCoord.y + offsety);
    FragColor = texture(ourTexture, vt);
}