# kds-spatial-project

## Overview
The `kds-spatial-project` is a C++ project designed to manage and represent spatial coordinates and objects in three-dimensional space. It provides a framework for defining points, lines, and surfaces using a class hierarchy.

## Classes

### KDCoord
- **Description**: Represents spatial coordinates in 3D space.
- **Properties**:
  - `x`: The x-coordinate.
  - `y`: The y-coordinate.
  - `z`: The z-coordinate.
- **Methods**:
  - Setters and getters for each coordinate.

### KDSData
- **Description**: Base class for spatial objects that stores properties in a key-value format.
- **Properties**:
  - `map<string, string>`: A map to store object properties.
- **Methods**:
  - Add, retrieve, and remove properties.

### KDSNode
- **Description**: Derived from `KDSData`, represents a point in space.
- **Methods**:
  - Methods specific to point operations.

### KDSWay
- **Description**: Derived from `KDSData`, represents a line in space.
- **Methods**:
  - Methods specific to line operations.

### KDSArea
- **Description**: Derived from `KDSData`, represents a surface in space.
- **Methods**:
  - Methods specific to area operations.

## Build Instructions
To build the project, use CMake. Navigate to the project directory and run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage
After building the project, you can run the executable to create instances of the spatial objects and demonstrate their functionality. The main entry point is located in `src/main.cpp`. 

## License
This project is licensed under the MIT License. See the LICENSE file for more details.