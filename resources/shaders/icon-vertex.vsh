#version 330 core



layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texturePosition;


out vec3 fragmentColor;
out vec2 fragmentTexturePosition;


void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    fragmentColor = color;
    fragmentTexturePosition = texturePosition;
}
