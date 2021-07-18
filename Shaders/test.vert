#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 outuv;

layout(location = 0) in vec3 currentPosition;
layout(location = 1) in vec2 uv;

void main() {
    gl_Position = vec4(currentPosition,1.0);
    fragColor = vec4(1,0,0,1);
    //set uv flags
    outuv = uv;
}
