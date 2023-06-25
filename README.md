## Overview
OSTRICH (Orbital Simulation with Transient Rectilinear Interactions In C++ with Heuristics) is an interactive orbital simulator. This project was created as coursework for A-Level Computer Science and received full marks pre-moderation (still waiting on the final result).

![Screenshot from 2023-06-25 18-18-15](https://github.com/LordIdra/OSTRICH/assets/35176119/6f0f55b9-aa24-4b64-ba6e-34f849f8a310)
![Screenshot from 2023-06-25 18-18-00](https://github.com/LordIdra/OSTRICH/assets/35176119/056cb934-2576-49d2-bb5e-d1f04b9568d1)
![Screenshot from 2023-06-25 18-17-52](https://github.com/LordIdra/OSTRICH/assets/35176119/ad8770c0-fc4f-47b9-ba75-324c27e80eb6)
![Screenshot from 2023-06-25 18-16-57](https://github.com/LordIdra/OSTRICH/assets/35176119/bcb7dbc8-2f07-443e-bc1d-337f6570b62f)

## Compilation
The project is built with CMake. Please note that this project assumes you are running on a Linux system and have vcpkg installed in your home directory. You will need to modify the build scripts to suit your system if this is not the case (compatibility with Windows / other environments is planned hopefully in the near future). You'll need to `vcpkg install` GLAD, GLFW, GLM, imgui, and yaml-cpp to compile the project.  Otherwise, simply build the project as any other CMake project by entering the directory and running:
`cmake .`
`make`
and run it using
`./OSTRICH`

## Usage
The simulator comes with two pre-built scenarios; the Earth-Moon system with a spacecraft, and the Solar System. Scenarios are stored in .yml files under `scenarios`, and can be edited as you please. The examples provided should be sufficient to understand how the .yml files must be structured. A CalculateOrbit.py file is included which I used to create the solar system scenario; you may find this useful in creating your own scenarios.

## Notes
There are still substantial issues with the software (such as more frequent crashes than I would like), but it can be considered largely complete and usable. If you have any interest in the project or wish to contribute, please don't hesitate to get in touch.
