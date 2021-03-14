#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2DArray texture_t;

in vec3 UVW;

void main(){
	color = texture(texture_t, UVW);
	if (color.a < 0.5){
		discard;
	}
}