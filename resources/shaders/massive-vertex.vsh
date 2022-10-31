#version 330 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexNormal;
uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;
out vec3 fragmentPosition;
out vec3 fragmentNormal;


void main() {
    gl_Position = cameraMatrix * modelMatrix * vec4(position, 1.0);
    fragmentPosition = position;
    fragmentNormal = vertexNormal;
}
