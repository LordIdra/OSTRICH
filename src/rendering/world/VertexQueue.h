#pragma once



#include "util/Types.h"
class VertexQueue {
private:
    unsigned int maxSize;
    std::vector<VERTEX_DATA_TYPE> vertices;

public:
    VertexQueue(const unsigned int maxSize);

    auto AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec3 position, const vec3 color) -> void;
};