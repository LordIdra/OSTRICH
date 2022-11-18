#include "VAO.h"
#include "util/Log.h"
#include "util/Types.h"

#include <glad/glad.h>



VAO::VAO() : vbo(0), vao(0), vertexCount(0) {}

auto VAO::Init() -> void {
    // Generate VAO
    glGenVertexArrays(1, &vao);
    Bind();

    // Generate VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Unbind VAO
    Unbind();
}

auto VAO::Bind() const -> void {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

auto VAO::Unbind() -> void {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto VAO::AddVertexAttribute(const VertexAttribute &a) const -> void {
    // Set and enable vertex attribute
    Bind();
    glVertexAttribPointer(a.index, a.size, a.type, a.normalised, a.stride, a.offset);
    glEnableVertexAttribArray(a.index);
    Unbind();
}

auto VAO::Data(const vector<VERTEX_DATA_TYPE> &data, const unsigned int vertexCount, const unsigned int mode) -> void {
    // Set buffer data
    Bind();
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VERTEX_DATA_TYPE), &data[0], mode);
    Unbind();

    // size will be needed later to specify the number of vertices in render calls
    this->vertexCount = vertexCount;
}

auto VAO::Render(unsigned int geometryType) const -> void {
    Bind();
    glDrawArrays(geometryType, 0, vertexCount);
    Unbind();
}