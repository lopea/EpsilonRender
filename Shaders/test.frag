#version 450

layout (binding = 3) uniform UniformData
{
    float value;
};

layout(location = 0) out vec4 outColor;

layout (location = 0) in vec4 fragColor;
layout (location = 1) in vec2 outUV;

void main() {
    outColor = vec4(outUV,value,1);
}