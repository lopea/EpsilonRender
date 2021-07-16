#version 460

layout(location = 0) in vec3 currentPosition;

layout(location = 0) out vec4 fragColor;
//const array of positions for the triangle
const vec3 positions[3] = vec3[3](
vec3(1.f,1.f, 0.0f),
vec3(-1.f,1.f, 0.0f),
vec3(0.f,-1.f, 0.0f)
);

void main() {

    //output the position of each vertex
    gl_Position = vec4(positions[gl_VertexID], 1.0f);
    fragColor = vec4(1,0,0,1);
}
