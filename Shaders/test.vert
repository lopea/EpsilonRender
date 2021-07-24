#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (std140, binding = 3) uniform UniformData
{
    float value;
};

layout(location = 0) out vec4 fragColor;

layout(location = 0) in vec3 currentPosition;
layout(location = 1) in vec2  uv;

layout(location = 1) out vec2 outUV;

void main() {
    gl_Position = vec4(currentPosition,1.0);
    fragColor = vec4(1,value,0,1);
    //set uv flags
    outUV = uv;
}
