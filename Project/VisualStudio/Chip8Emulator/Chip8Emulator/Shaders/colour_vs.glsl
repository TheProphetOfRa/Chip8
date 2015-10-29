#version 400
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertCol;

out vec3 colour;

void main ()
{
    colour = vertCol;
    gl_Position = vec4(vertPos, 1.0);
}