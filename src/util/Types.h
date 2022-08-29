#pragma once

#include <vector>
#include <string>

#include "glm/glm.hpp"

#include <boost/multiprecision/cpp_dec_float.hpp>



typedef float ELEMENT_DATA_TYPE;
typedef float VERTEX_DATA_TYPE;

using std::vector; // NOLINT it's used outside of this file
using std::string;

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::uvec3;

using glm::mat2;
using glm::mat3;
using glm::mat4;

typedef boost::multiprecision::cpp_dec_float_50 bfloat;     // bfloat = big float



struct bvec3 {

    bfloat x;
    bfloat y;
    bfloat z;

    bvec3(bfloat x, bfloat y, bfloat z) 
        : x(x), y(y), z(z) {}

    bvec3 operator+(bvec3 v) {
        return bvec3(x+v.x, y+v.y, z+v.z);
    }

    bvec3 operator+(bfloat n) {
        return bvec3(x+n, y+n, z+n);
    }

    bvec3 operator-(bvec3 v) {
        return bvec3(x-v.x, y-v.y, z-v.z);
    }

    bvec3 operator-(bfloat n) {
        return bvec3(x-n, y-n, z-n);
    }

    bvec3 operator*(bvec3 v) {
        return bvec3(x*v.x, y*v.y, z*v.z);
    }

    bvec3 operator*(bfloat n) {
        return bvec3(x*n, y*n, z*n);
    }

    bvec3 operator/(bvec3 v) {
        return bvec3(x/v.x, y/v.y, z/v.z);
    }

    bvec3 operator/(bfloat n) {
        return bvec3(x/n, y/n, z/n);
    }

    vec3 asVec3() {
        return vec3(x, y, z);
    }
};
