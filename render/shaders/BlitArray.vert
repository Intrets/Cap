#version 330 core

layout(location = 0) in vec2 vertex;

layout(location = 1) in vec4 position;
layout(location = 2) in int texture_rotation;
layout(location = 3) in int layer;

uniform float depth;
uniform mat4 VP;

out vec3 UVW;

uniform mat2 rotations[5] = mat2[](
	mat2(1,0,
	     0,-1),
	mat2(0,-1,
	     -1,0),
	mat2(-1,0,
	     0,1),
	mat2(0,1,
	     1,0),
	mat2(1,0,
		 0,1)
);


void main(){
	gl_Position = VP * vec4(vertex * position.zw + position.xy, depth, 1);

	UVW = vec3(vertex * rotations[texture_rotation], float(layer));
}

