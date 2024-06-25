# B-Tree Visualization with Raylib

Welcome to the **B-Tree Visualization** project! This project is a C-based application that visualizes B-trees using the Raylib graphics library for the user interface. This tool aims to provide an educational resource for understanding B-trees, their structure, and their traversal.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Building the Project](#building-the-project)
- [Usage](#usage)

## Introduction

B-trees are a self-balancing tree data structure that maintains sorted data and allows for efficient insertion, deletion, and search operations. This project provides a graphical representation of B-trees, making it easier to understand their operations and structure. It leverages Raylib for rendering graphics and user interaction.

When using the application, you can begin visualizing and interacting with your B-tree. The interface allows you to insert, delete, and search for records within the tree. The graphical display will update to reflect the current state of the B-tree, showing the relationships between pages and their parent nodes.

## Features

- [x] **Graphical Visualization**: See your B-tree in a graphical format.
- [ ] **User Interface**: Manage and interact with your B-tree through a user interface.
- [ ] **Traversal Display**: Walk through the B-tree and view each page, along with its parent-child relationships.

## Installation

### Prerequisites

Before building and running the project, ensure you have the following installed:

- **GCC**: GNU Compiler Collection
- **CMake**: Build system generator

### Building the Project

1. **Clone the repository:**

    ```bash
    git clone https://github.com/yourusername/btree-ui.git
    cd btree-ui
    ```

2. **Create a build directory and navigate into it:**

    ```bash
    mkdir build
    cd build
    ```

3. **Generate build files with CMake:**

    ```bash
    cmake ..
    ```

4. **Build the project:**

    ```bash
    cmake --build .
    ```

## Usage

After building the project, you can run the executable to start the application:

```bash
./bin/app
```
