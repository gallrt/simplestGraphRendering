# This is the Simplest Graph Renderer:

Dependencies:

    * GLEW
    * OpenGL
    * GLFW

To build:
    
    mkdir build
    cd build
    cmake ..
    make

To run (in build):

    ./simple -gf ../demo/demo_graph_1.gl

Graph file format: GL

    Number of nodes
    Number of edges
    LAT LONG of node1
    ...
    LAT LONG of nodeN
    nodeFrom nodeTo width color
    ...
    nodeFrom nodeTo width color

Example: Say, we have a graph with two nodes, node `0` at `(1, 1)` and
node `1` at `(2, 2)`. And there is one edge between them of `1px` width
with color `1`. Then this graph would have the following graphfile:

    2
    1
    1 1
    2 2
    0 1 1 2
