#version 330

uniform vec3 my_color;
 
layout(location = 0) out vec4 out_color;
 
void main()
{
    out_color = vec4(my_color, 0);
}