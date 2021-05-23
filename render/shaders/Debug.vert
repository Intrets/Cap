#version 330 core

layout(location = 0) in vec2 vertex;

uniform mat4 VP;

void main(){
	gl_Position = VP * vec4(vertex, 0.0, 1.0);
}

