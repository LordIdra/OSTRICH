#include "Types.h"



class Transition {
private:
    vec3 start;
    vec3 end;

    float totalTime;
    float currentTime;


public:
    Transition(vec3 vector, vec3 end, float totalTime);

    vec3 Step(float deltaTime);
    bool Finished();
};
