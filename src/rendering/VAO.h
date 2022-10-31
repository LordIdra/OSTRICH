#pragma once

#include <util/Types.h>
#include <rendering/structures/VertexAttribute.h>



class VAO {
private:
    unsigned int vbo;
    unsigned int vao;
    unsigned int vertexCount;

public:
    VAO();
    
    auto Init() -> void;

    auto Bind() const -> void;
    static auto Unbind() -> void;

    auto AddVertexAttribute(const VertexAttribute &attribute) const -> void;
    auto Data(const vector<VERTEX_DATA_TYPE> &data, unsigned int vertexCount, unsigned int mode) -> void;

    auto Render(unsigned int geometryType) const -> void;

};