#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec4 UVSource;
layout(location = 2) in vec4 worldTarget;


uniform float UVflip;
uniform float depth;
uniform vec2 offset;

out vec2 UV;

void main(){
	gl_Position = vec4(vertex * worldTarget.zw + worldTarget.xy + offset, depth, 1);

	UV = vertex * vec2(1, UVflip) + vec2(0,1);
	UV = (UV * UVSource.zw) + UVSource.xy;
}

