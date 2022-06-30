#include "../util/Types.h"



namespace rendering {
    class Buffer {
    private:
        // OpenGL assigns an ID to this buffer
        unsigned int id;
    
    public:
        // generate a buffer on the GPU and assign the ID of said buffer to 'id'
        void Init();
    
        void Bind();
        void Unbind();
    };
}