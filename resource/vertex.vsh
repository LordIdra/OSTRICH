#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexNormal;
uniform mat4 cameraMatrix;
out vec3 fragmentPosition;
out vec3 fragmentNormal;


void main() {
    gl_Position = cameraMatrix * vec4(position.x, position.y, position.z, 1.0);
    fragmentPosition = position;
    fragmentNormal = vertexNormal;
}
