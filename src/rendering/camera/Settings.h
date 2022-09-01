#pragma once

#include <util/Constants.h>



const float STARTING_ZOOM = 0.05;

const float MIN_ZOOM = 0.002;
const float MAX_ZOOM = 95.0;

const float MAX_ANGLE = PI * 0.45F;

const float MAX_ANGLE_SPEED = 0.01;
const float MAX_ZOOM_SPEED = 0.1;

const float ANGLE_SENSITIVITY = 20.0;
const float ZOOM_SENSITIVITY = 0.03;

const float ANGLE_DAMPING = 0.001;
const float ZOOM_DAMPING = 0.01;

const float FIELD_OF_VIEW = 45.0;

const float NEAR_PLANE = 0.0001;
const float FAR_PLANE = 100.00;