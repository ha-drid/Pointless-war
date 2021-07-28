#version 330 

out vec4 color;

in vec4 pass_color;

void main()
{
	color = pass_color;
}