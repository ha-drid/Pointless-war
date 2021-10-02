#version 330 // glsl version

// input variable
attribute vec3 position;
attribute vec2 uv;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform vec3 positionInstances[670];
uniform vec2 uvInstances[670];

// output variable
out vec2 pass_uv;
uniform vec2 u_uv;

void main()
{		   
	//gl_Position = (u_ViewProjection * u_Model) * vec4(position + positionInstances[gl_InstanceID], 1);
	//pass_uv = uv + uvInstances[gl_InstanceID];
	gl_Position = (u_ViewProjection * u_Model) * vec4(position + positionInstances[gl_InstanceID], 1);
	pass_uv = uv + uvInstances[gl_InstanceID];
}