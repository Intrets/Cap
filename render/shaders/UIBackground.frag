#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform vec4 panelColor;

in vec4 c;

void main(){
	color = c;
}