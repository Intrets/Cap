#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D texture_t;
uniform vec4 c;

in vec2 UV;

void main(){
	color = c * texture(texture_t, UV);
}