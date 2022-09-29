#pragma once

#include <rendering/structures/Material.h>



namespace Materials {
    const Material earth {
        .ambient = vec3(0.02, 0.05, 0.07),
        .diffuse = vec3(0.1, 0.25, 0.35),
        .specular = vec3(0.2, 0.5, 0.7),
        .shine = 64.0
    };

    const Material moon1 {
        .ambient = vec3(0.02, 0.02, 0.02),
        .diffuse = vec3(0.3, 0.3, 0.3),
        .specular = vec3(0.3, 0.3, 0.3),
        .shine = 16.0
    };

    const Material moon2 {
        .ambient = vec3(0.06, 0.02, 0.02),
        .diffuse = vec3(0.6, 0.3, 0.3),
        .specular = vec3(0.6, 0.3, 0.3),
        .shine = 16.0
    };
}