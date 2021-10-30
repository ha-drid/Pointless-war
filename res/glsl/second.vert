#version 330 // glsl version

in vec2 position;
in vec2 uv;

// output variable
uniform mat4 u_ViewProjection;

out vec2 pass_uv;

void main()
{
	gl_Position =  u_ViewProjection * vec4(position, 0, 1);
	pass_uv = uv;
}