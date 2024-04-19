# OOD-OOP-Comparison-in-Parallel

## Description
This project aims to investigate the use of object-oriented design (ECS) and classical object hierarchies in systems using parallel computing. Initially, the system will be implemented with a large number of different objects and their instances using inheritance. Later, the same system will be implemented using components (aggregation) instead of inheritance. Parallel updating of all objects will be implemented for both versions. The results will be compared by qualitative and quantitative characteristics (system update time, individual object update time, cache misses), as well as by abstract indicators (system complexity, cognitive load, flexibility).

### Inheritance Version
This version implements an object hierarchy using inheritance. It includes:
- `main.cc`: C++ source code for utilizing the object hierarchy.
- `CMakeLists.txt`: CMake configuration file for building the C++ code.

### Component Version
This version implements a component-based architecture. It includes:
- `main.cc`: C++ source code for utilizing the component-based architecture.
- `CMakeLists.txt`: CMake configuration file for building the C++ code.

## Setup and Usage

### Prerequisites
- CMake
- C++ compiler (e.g., g++)

### Installation and Configuration
1. Clone this repository to your local machine.
2. Navigate to the root directory of the project.
3. Follow the instructions specific to the version you want to set up (inheritance or component).

### Running the Application

#### Inheritance Version
1. Navigate to the `inheritance_version` directory.
2. Configure the CMake project:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
3. Build the C++ code:
    ```bash
    cmake --build .
    ```
4. Run the executable:
    ```bash
    ./inheritance_version/inheritance_version
    ```

#### Component Version
1. Navigate to the `component_version` directory.
2. Configure the CMake project:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
3. Build the C++ code:
    ```bash
    cmake --build .
    ```
4. Run the executable:
    ```bash
    ./component_version/component_version
    ```
