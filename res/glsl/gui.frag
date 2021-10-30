#version 330


out vec4 color;

in vec2 pass_uv;

uniform sampler2D tex;

void main()
{
	color = texture(tex, pass_uv);
}