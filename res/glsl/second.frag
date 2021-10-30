#version 330

in vec2 pass_uv;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, pass_uv);
}