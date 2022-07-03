#version 450 core
layout (location = 8) in vec3 position;
layout (location = 9) in vec3 color;
layout (location = 10) in vec3 normal;
layout (location = 20) uniform mat4 vp;
layout(location = 22) uniform mat4 model;
layout (location = 23) uniform mat4 normal_matrix;
layout (location = 24) uniform vec3 campos;
layout (location = 25) uniform float alpha;

out vec4 vcolor;

void main(void){
    gl_Position = vp* model *vec4(position, 1.0f);
    vcolor = vec4(color, alpha);
}