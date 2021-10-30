#version 330 // glsl version

// input variable
in vec3 position;
in vec2 uv;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 positionInstances[670];
uniform vec2 uvInstances[670];

// output variable
out vec2 pass_uv;

void main()
{		   
	mat4 mvp = u_Projection * u_View * u_Model;
	gl_Position = mvp * vec4(position + positionInstances[gl_InstanceID], 1);
	pass_uv = uv + uvInstances[gl_InstanceID];
}