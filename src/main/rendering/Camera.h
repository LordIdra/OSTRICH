#include "../util/Types.h"



namespace rendering {
    class Camera {
    private:
        double angle;
        double angleAcceleration;
        vec3 position;
        vec3 positionAcceleration;
        double zoom;
        double zoomAcceleration;

    public:
        Camera(double angle, vec3 position);

        // a typical camera system uses three matrices, that are multiplied together to form a final camera matrix
        // this final matrix is used to transform the points of every object on the scene
        // GetMatrix() will return said final matrix
        auto GetMatrix() -> mat4;

        // the ‘angle’ here refers to the angle between the camera’s direction vector and the X-Z plane
        auto SetAngle(double angle) -> void;

        // the ‘target’ refers to the object that the camera is focused on.
        // mathematically, it is the single invariant point under the camera matrix
        auto SetPosition(vec3 position) -> void;

        // in order to provide smooth camera movement, these functions will take a vector and use it to periodically update the position-velocity and angle-velocity in the update function
        auto SetAngleAcceleration(double angle) -> void;
        auto SetPositionAcceleration(vec3 position) -> void;
        auto SetZoomAcceleration(double zoom) -> void;

        // this function will apply the apply the position/angle acceleration to the actual position/angle variables
        auto Update(double deltaTime) -> void;
    };
}