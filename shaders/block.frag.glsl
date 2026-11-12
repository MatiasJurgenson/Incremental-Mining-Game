#version 400

in vec2 interpolatedUv;
out vec4 fragColor;

uniform sampler2D blockTexture;
uniform int tileIndexX;
uniform int tileIndexY;

void main() {
    const vec2 spriteSize = vec2(12.0, 6.0);
    vec2 uv = (interpolatedUv + vec2(tileIndexX, tileIndexY)) /spriteSize;
    fragColor = texture(blockTexture, uv);
}
