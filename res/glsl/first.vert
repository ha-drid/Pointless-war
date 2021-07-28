#version 330 // glsl version

// input variable
attribute vec3 position;

uniform vec3 camera_direction;
uniform vec3 camera_position;
uniform vec3 u_translatef;
uniform vec3 u_rotate;
uniform float angle_radians;
uniform vec3 positionInstances[670];
uniform vec3 colorInstances[670];

// output variable
out vec4 pass_color;

//	functions
mat4 lookAt(vec3 eye, vec3 center, vec3 up);
mat4 perpective(float angle_radians, float ratio, float near, float far);
mat4 translate(vec3 vector);
mat4 setMat4Value(float x);
mat4 rotate(mat4 matrix, float angle, vec3 vector);

mat4 projection = perpective(((3.14 / 180.0) * 45), 4.0 / 3.0, 0.1, 200);

// 
void main()
{	
	mat4 model = translate(u_translatef);
	mat4 view = lookAt(camera_position,
	       		   camera_direction,
			       vec3(0, 1, 0));
				   
	mat4 mvp = projection * view * model;
	
	gl_Position = mvp * vec4(position + positionInstances[gl_InstanceID], 1);
	pass_color = vec4(colorInstances[gl_InstanceID], 1);
}

mat4 rotate(mat4 matrix, float angle, vec3 vector)
{
	float a = angle;
	float c = cos(a);
	float s = sin(a);

	vec3 axis = normalize(vector);
	vec3 temp = vec3((1.0 - c) * axis);

	mat4 rotate_matrix;
	rotate_matrix[0][0] = c + temp[0] * axis[0];
	rotate_matrix[0][1] = temp[0] * axis[1] + s * axis[2];
	rotate_matrix[0][2] = temp[0] * axis[2] - s * axis[1];

	rotate_matrix[1][0] = temp[1] * axis[0] - s * axis[2];
	rotate_matrix[1][1] = c + temp[1] * axis[1];
	rotate_matrix[1][2] = temp[1] * axis[2] + s * axis[0];

	rotate_matrix[2][0] = temp[2] * axis[0] + s * axis[1];
	rotate_matrix[2][1] = temp[2] * axis[1] - s * axis[0];
	rotate_matrix[2][2] = c + temp[2] * axis[2];

	mat4 result;
	result[0] = matrix[0] * rotate_matrix[0][0] + matrix[1] * rotate_matrix[0][1] + matrix[2] * rotate_matrix[0][2];
	result[1] = matrix[0] * rotate_matrix[1][0] + matrix[1] * rotate_matrix[1][1] + matrix[2] * rotate_matrix[1][2];
	result[2] = matrix[0] * rotate_matrix[2][0] + matrix[1] * rotate_matrix[2][1] + matrix[2] * rotate_matrix[2][2];
	result[3] = matrix[3];
	
	return result;
}

mat4 setMat4Value(float x)
{
	mat4 result;
	result[0] = vec4(x, 0, 0, 0);
	result[1] = vec4(0, x, 0, 0);
	result[2] = vec4(0, 0, x, 0);
	result[3] = vec4(0, 0, 0, x);
	
	return result;
}

mat4 translate(vec3 vector)
{
	mat4 matrix = mat4(1.0);
	
	matrix[3][0] = vector.x;
	matrix[3][1] = vector.y;
	matrix[3][2] = vector.z;
	
	return matrix;
}

mat4 perpective(float angle_radians, float ratio, float near, float far)
{
	float tanHalfFovy = tan(angle_radians / 2.0);

	mat4 result = setMat4Value(0);
	result[0][0] = 1.0 / (ratio * tanHalfFovy);
	result[1][1] = 1.0 / (tanHalfFovy);
	result[2][2] = - (far + near) / (far - near);
	result[2][3] = - 1;
	result[3][2] = - (2 * far * near) / (far - near);
	return result;
}

mat4 lookAt(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = normalize(center - eye);
	vec3 s = normalize(cross(f, up));
	vec3 u = cross(s, f);

	mat4 result = setMat4Value(1);
	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] =-f.x;
	result[1][2] =-f.y;
	result[2][2] =-f.z;
	result[3][0] =-dot(s, eye);
	result[3][1] =-dot(u, eye);
	result[3][2] = dot(f, eye);
	
	return result;
}
