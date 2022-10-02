#version 330 core


in vec3 fragmentColor;
in vec2 fragmentTexturePosition;
out vec4 FragColor;

uniform sampler2D inputTexture;


void main() {
    if (texture(inputTexture, fragmentTexturePosition).a == 0.0) {
        discard;
    }
    FragColor = vec4(fragmentColor, 1.0);
}
