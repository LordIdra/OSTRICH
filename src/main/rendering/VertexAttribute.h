#pragma once



struct VertexAttribute {
    const unsigned int index;
    const int size;
    const unsigned int type;
    const unsigned char normalised;
    const int stride;
    const void *offset;
};