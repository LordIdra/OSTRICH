#include "Buffer.h"



namespace rendering {
    // EBO = Element Buffer Object
    class EBO : Buffer {
    public:
        // set the data inside the buffer
        // 'data' is currently passed as a C-style array, but this is likely to be replaced with a vector or std::array
        // this depends on implementation details later down the line
        auto Data(ELEMENT_DATA_TYPE data[], unsigned int size, unsigned int mode) -> void;
    };
}