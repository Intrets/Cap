#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D texture_t;
uniform vec4 c;

in vec2 UV;
in vec2 texturePixelSize;
in vec2 screenPixelSize;

vec2 fr(vec2 v, vec2 m){
	return m * floor(v/m);
}

void main(){
	vec2 UV_d = fr(UV + 0.5 * screenPixelSize, texturePixelSize);
	vec2 UV_m = UV - UV_d;

	vec2 scale = clamp(UV_m / screenPixelSize, -0.5, 0.5);
	vec2 UV = UV_d + scale * texturePixelSize;

	color = c * texture(texture_t, UV);
}