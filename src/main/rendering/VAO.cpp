#include "VAO.h"

#include "../util/logging.h"
#include "glad/glad.h"



VAO::VAO() {}

auto VAO::Init() -> void {
    glGenVertexArrays(1, &id);
    Bind();
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Unbind();
    logging::Info("Created VAO " + std::to_string(id));
}

auto VAO::Bind() const -> void {
    glBindVertexArray(id);
}

auto VAO::Unbind() const -> void {
    glBindVertexArray(0);
}

auto VAO::AddVertexAttribute(const VertexAttribute &a) const -> void {
    Bind();
    glVertexAttribPointer(a.index, a.size, a.type, a.normalised, a.stride, a.offset);
    glEnableVertexAttribArray(a.index);
    Unbind();
    logging::Info("Added vertex attribute " + std::to_string(a.index) + " to VAO " + std::to_string(id));
}

auto VAO::Data(const vector<VERTEX_DATA_TYPE> &data, unsigned int mode) -> void {
    Bind();
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VERTEX_DATA_TYPE), &data[0], mode);
    Unbind();
    size = data.size();
}

auto VAO::Render() -> void {
    Bind();
    glDrawArrays(GL_TRIANGLES, 0, size);
    Unbind();
}