#version 330 core


in vec3 fragmentColor;
in vec2 fragmentTexturePosition;
out vec4 FragColor;


void main() {
    FragColor = vec4(fragmentColor, 1.0);
}
