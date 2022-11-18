#include "VertexQueue.h"



VertexQueue::VertexQueue(const unsigned int maxSize) 
    : maxSize(maxSize) {}

auto VertexQueue::AddVertex(vector<VERTEX_DATA_TYPE> &vertices, const vec3 position, const vec3 color) -> void {
    ZoneScoped;
    vertices.push_back(position.x);
    vertices.push_back(position.y);
    vertices.push_back(position.z);
    vertices.push_back(color.r);
    vertices.push_back(color.g);
    vertices.push_back(color.b);
}