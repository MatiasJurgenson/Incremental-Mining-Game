#version 400

in vec2 interpolatedUv;
out vec4 fragColor;

uniform sampler2D blockTexture;
uniform int areaMask;

void main() {


    vec2 uv = interpolatedUv;

    // All 9 positions are solid: 2^9 - 1 = 511
    if (areaMask == 511) {
        uv = vec2(uv.x * 0.5, uv.y * 0.5);
    } else {
        // Calculate the offset based on the areaMask
        int index = 0;
        for (int i = 0; i < 9; ++i) {
            if ((areaMask & (1 << i)) != 0) {
                index = i;
                break;
            }
        }

        // Calculate the UV offset based on the index
        float offsetX = float(index % 3) * 0.5;
        float offsetY = float(index / 3) * 0.5;

        uv = vec2(uv.x * 0.5 + offsetX, uv.y * 0.5 + offsetY);
    }

    fragColor = texture(blockTexture, uv);
}
