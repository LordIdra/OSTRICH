#include "../util/Types.h"



struct Material {

    // http://devernay.free.fr/cours/opengl/materials.html

    // intensity of the 'background' lighting applied to everything
    vec3 ambient;

    // intensity of the light reflected according to the angle between the surface normal and the light source   
    vec3 diffuse;

    // the intensity of the bright spot of light on shiny objects - a phenomenom known as specular reflection
    vec3 specular;

    // how shiny the surface is - this is used to calculate the radius of the specular reflection
    float shine;
};
