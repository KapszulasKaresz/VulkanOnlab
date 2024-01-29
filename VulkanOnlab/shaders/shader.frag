#version 450

layout(location = 0) out vec4 outColor;
layout(location = 1) in vec2 pos;

void main() {
    outColor = vec4(pos.xy, 0.0, 1.0);
}