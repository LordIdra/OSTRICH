#include "../util/Types.h"



struct Material {

    // this website lists some real-world values of materials - it might come in useful later
    // http://devernay.free.fr/cours/opengl/materials.html

    // the vec3 data type of these variables represents rgb
    vec3 ambient;   // intensity of the 'background' lighting applied to everything
    vec3 diffuse;   // intensity of the light reflected according to the angle between the surface normal and the light source
    vec3 specular;  // the intensity of the bright spot of light on shiny objects - a phenomenom known as specular reflection
    float shine;    // how shiny the surface is - this is used to calculate the radius of the specular reflection
};
