#version 330 core

in vec2 TexCoordsOut;
out vec4 colour;

uniform sampler2D image;

void main()
{    
    colour = vec4(1.0) * texture(image, TexCoordsOut);
}