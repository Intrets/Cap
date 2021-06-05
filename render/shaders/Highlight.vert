#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 position;
layout(location = 2) in vec2 scale;
layout(location = 3) in vec4 color;

out vec4 c;

uniform mat4 VP;

void main(){
	c = color;
	gl_Position = VP * vec4(pos * scale + position, 0, 1);
}

