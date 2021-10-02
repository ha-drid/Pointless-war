#version 330 // glsl version

in vec2 position;

// output variable
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
	gl_Position =  (u_ViewProjection * u_Model) * vec4(position, 0, 1);
}