#include "Buffer.h"



namespace rendering {
    // VBO = Vertex Buffer Object
    class VBO : Buffer {
    public:
        // set the data inside the buffer
        // 'data' is currently passed as a C-style array, but this is likely to be replaced with a vector or std::array
        // this depends on implementation details later down the line
        auto Data(VERTEX_DATA_TYPE data[], unsigned int size, unsigned int mode) -> void;
    };
}
