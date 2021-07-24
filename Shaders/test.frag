#version 450

layout (std140,binding = 3) uniform UniformData
{
    float value;
}data;

layout(location = 0) out vec4 outColor;

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec2 outUV;



void main() {
    outColor = vec4(outUV,sin(data.value),1);
}