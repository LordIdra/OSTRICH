#include "../util/Types.h"

#include "Shader.h"


namespace rendering {
    class Program {
    private:
       // OpenGL assigns an ID to this program
       unsigned int id;

    public:
       // the constructor will link two shaders (the vertex shader and fragment shader) into a single program, which can run on the GPU
       Program(Shader vertex, Shader fragment);

       // mark the program to be used in subsequent render calls
       auto Use() -> void;

       // lots of overloads for setting uniform values in the shaders
       // a template is not used here because each type requires a different OpenGL function call
       auto Set(string key, bool  value) -> void;
       auto Set(string key, int   value) -> void;
       auto Set(string key, float value) -> void;
       auto Set(string key, vec2  value) -> void;
       auto Set(string key, vec3  value) -> void;
       auto Set(string key, vec4  value) -> void;
       auto Set(string key, mat2  value) -> void;
       auto Set(string key, mat3  value) -> void;
       auto Set(string key, mat4  value) -> void;
    };
}