#pragma once

#include "VertexAttribute.h"
#include "../util/Types.h"



// VBO = Vertex Buffer Object
class VAO {
private:
    unsigned int vbo;
    unsigned int id;
    unsigned int vertexCount;

public:
    VAO();
    
    auto Init() -> void;

    auto Bind() const -> void;
    auto Unbind() const -> void;

    // describe an attribute of a vertex
    // for example, an attribute could be added to indicate that the first 3 numbers in a vertex are floats, and represent the position of the vertex
    auto AddVertexAttribute(const VertexAttribute &attribute) const -> void;

    // set the data inside the VBO
    // vectors are easier to manipulate, safer, and more modern than C-style arrays, which is why they're used here
    auto Data(const vector<VERTEX_DATA_TYPE> &data, unsigned int vertexCount, unsigned int mode) -> void;

    auto Render() -> void;

};