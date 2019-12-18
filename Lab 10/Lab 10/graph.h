#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

class Graph {
    enum class Color { WHITE, GRAY, BLACK };

    struct Vertex {
        unsigned id;

        Color color;
        std::shared_ptr<Vertex> parent;
        size_t d;
        size_t f;

        Vertex(int id) : id(id), color(Color::WHITE), d(0), f(0), parent(nullptr) {};
    };

public:
    Graph(size_t v);
    void AddEdge(unsigned src, unsigned dest);
    void DFS();
    void DFSVist(unsigned src);
    //Demo
    void Dump();
private:
    std::vector<std::vector<std::shared_ptr<Vertex>>> adj;
    std::vector<std::shared_ptr<Vertex>> vertex;
    size_t time;

    const char* ColorToString(const Color color);
};

#endif // !GRAPH_H_INCLUDED


