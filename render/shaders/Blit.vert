#version 330 core

layout(location = 0) in vec2 vertex;

layout(location = 1) in vec4 UVSource;
layout(location = 2) in vec4 worldTarget;
layout(location = 3) in int rot;

mat2 rotations[4] = mat2[](
	mat2(1,0,
	     0,1),
	mat2(0,1,
	     -1,0),
	mat2(-1,0,
	     0,-1),
	mat2(0,-1,
	     1,0)
);

uniform float UVflip;
uniform float depth;
uniform vec2 offset;

out vec2 UV;

void main(){
	vec2 v = vertex;
	v = v - vec2(0.5, 0.5);
	v = rotations[rot] * v ;
	v = v + vec2(0.5, 0.5);

	gl_Position = vec4(vertex * worldTarget.zw + worldTarget.xy + offset, depth, 1);

	UV = v * vec2(1, UVflip) + vec2(0,1);
	UV = (UV * UVSource.zw) + UVSource.xy;
}

