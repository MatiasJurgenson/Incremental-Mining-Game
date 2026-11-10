#version 400

layout (location = 0) in vec2 vertex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 interpolatedUv;

void main() {
    gl_Position = projection * view * model * vec4(vertex, 0.0, 1.0);
    interpolatedUv = vertex;
}
