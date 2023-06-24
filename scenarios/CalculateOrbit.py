from scipy.spatial.transform import Rotation

class BodyParameters:
    def __init__(self, mass, periapsis, speed, argument_of_periapsis, inclination, longitude_of_ascending_node):
        self.mass = mass
        self.periapsis = periapsis
        self.speed = speed
        self.argument_of_periapsis = argument_of_periapsis
        self.inclination = inclination
        self.longitude_of_ascending_node = longitude_of_ascending_node

bodies = {
    "mercury": BodyParameters(
        0.330e+24,
        46.0e+9,
        58.98e+3,
        77.45645,
        7.004,
        48.33167),
    "venus": BodyParameters(
        4.87e+24,
        107.480e+9,
        35.26e+3,
        131.53298,
        3.395,
        76.68069),
    "earth": BodyParameters(
        5.97e+24,
        147.0e+9,
        30.29e+3,
        85.9,
        0.0,
        -11.26064),
    "mars": BodyParameters(
        0.642e+24,
        206.650e+9,
        26.50e+3,
        336.04084,
        1.848,
        49.57854),
    "jupiter": BodyParameters(
        1898e+24,
        740.595e+9,
        13.72e+3,
        14.75385,
        1.304,
        100.55615),
    "saturn": BodyParameters(
        568e+24,
        1357.554e+9,
        10.18e+3,
        92.43194,
        2.486,
        113.71504),
    "uranus": BodyParameters(
        86.8e+24,
        2732.696e+9,
        7.11e+3,
        170.96424,
        0.770,
        74.22988),
    "neptune": BodyParameters(
        102e+24,
        4471.050e+9,
        5.50e+3,
        44.97135,
        1.770,
        131.72169),
}

momentum = [0, 0, 0]
SUN_MASS = 1988500e24

for bodyId, body in bodies.items():
    rotation_matrix1 = Rotation.from_euler('xyz', [0, body.longitude_of_ascending_node, 0], degrees = True)
    rotation_matrix2 = Rotation.from_euler('xyz', [0, 0, body.inclination], degrees = True)
    rotation_matrix3 = Rotation.from_euler('xyz', [0, body.argument_of_periapsis, 0], degrees = True)

    x = [body.periapsis, 0, 0]
    x = rotation_matrix1.apply(x)
    x = rotation_matrix2.apply(x)
    x = rotation_matrix3.apply(x)

    v = [0, 0, body.speed]
    v = rotation_matrix1.apply(v)
    v = rotation_matrix2.apply(v)
    v = rotation_matrix3.apply(v)

    print(bodyId)
    print(str(x[0]) + ", " + str(x[1]) + ", " + str(x[2]))
    print(str(v[0]) + ", " + str(v[1]) + ", " + str(v[2]))
    print("")

    momentum[0] += v[0] * body.mass
    momentum[1] += v[1] * body.mass
    momentum[2] += v[2] * body.mass

sun_velocity = [-momentum[0] / SUN_MASS, -momentum[1] / SUN_MASS, -momentum[2] / SUN_MASS]

print("Total momentum: " + str(momentum))
print("Sun velocity for zero momentum: " + str(sun_velocity))