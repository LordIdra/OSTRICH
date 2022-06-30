#include "../util/Types.h"



namespace rendering {
    class Shader {
    private:
        // OpenGL assigns an ID to this shader
        unsigned int id;

    public:
        // the constructor will compile the source into a shader managed by OpenGL
        Shader(string source);

        // after being linked into a program, the shaders are not needed any more, so this destructor can be called
        // the reason a custom destructor is needed is to delete the internal shaders managed by OpenGL; not doing so could be considered a memory leak
        ~Shader();
    };
}