#version 330 core


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};


in vec3 fragmentNormal;
in vec3 fragmentPosition;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform Material material;
out vec4 FragColor;


vec3 normal = normalize(fragmentNormal);
vec3 lightDirection = normalize(lightPosition - fragmentPosition);
vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
vec3 halfwayDirection = normalize(lightDirection - cameraDirection);


vec3 Ambient() {
    return material.ambient;
}

vec3 Diffuse() {
    return material.diffuse * max(dot(normal, lightDirection), 0.0);
}

vec3 Specular() {
    return material.specular * pow(max(dot(normal, halfwayDirection), 0.0), material.shine);
}

void main() {
    vec3 lighting = Ambient() + Diffuse();
    FragColor = vec4(Ambient() + Diffuse() + Specular(), 1.0);
}
