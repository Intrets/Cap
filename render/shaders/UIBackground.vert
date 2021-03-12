#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 scale;
layout(location = 3) in vec4 color;
layout(location = 4) in int depth;


// Output data ; will be interpolated for each fragment.
//in int layer;

out vec4 c;


void main(){
	c = color;
	gl_Position = vec4(vertexPosition_modelspace*vec3(scale,1) + position, 1);
}

