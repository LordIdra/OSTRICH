#pragma once

#include <rendering/structures/Material.h>



const Material planetMaterial {
    .ambient = vec3(0.02, 0.05, 0.07),
    .diffuse = vec3(0.1, 0.25, 0.35),
    .specular = vec3(0.2, 0.5, 0.7),
    .shine = 64.0
};
