#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec4 color;

uniform mat4 VP;
uniform float pointSize;

out vec4 c;

void main(){
	c = color;
	gl_Position = VP * vec4(vertex, 0.0, 1.0);
	gl_PointSize = pointSize;
}

