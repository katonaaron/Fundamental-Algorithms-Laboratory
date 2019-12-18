#include "graph.h"

Graph::Graph(size_t v) : adj(v), time(0) {
    for (int i = 0; i < v; i++)
    {
        vertex.push_back(std::make_shared<Vertex>(i));
    }
};

void Graph::AddEdge(unsigned src, unsigned dest)
{
    if (src >= vertex.size() || dest >= vertex.size())
        throw new std::out_of_range("src and dest must be in the range [0, V-1]");

    adj[src].push_back(vertex[dest]);
}

void Graph::DFS()
{
    for (auto& v : vertex)
    {
        v->color = Color::WHITE;
        v->parent = nullptr;
    }

    time = 0;

    for (auto& v : vertex)
    {
        if (v->color == Color::WHITE)
        {
            DFSVist(v->id);
        }
    }
}

void Graph::DFSVist(unsigned src)
{
    std::shared_ptr<Vertex> u = vertex[src];
    u->d = ++time;
    u->color = Color::GRAY;

    for (auto& v : adj[src])
    {
        if (v->color == Color::WHITE)
        {
            v->parent = u;
            DFSVist(v->id);
        }
    }

    u->color = Color::BLACK;
    u->f = ++time;
}

void Graph::Dump()
{
    std::cout << "id:";
    for (auto& v : vertex)
    {
        std::cout << "\t" << v->id;
    }
    std::cout << "\ncolor:";
    for (auto& v : vertex)
    {
        std::cout << "\t" << ColorToString(v->color);
    }
    std::cout << "\nd:";
    for (auto& v : vertex)
    {
        std::cout << "\t" << v->d;
    }
    std::cout << "\nf:";
    for (auto& v : vertex)
    {
        std::cout << "\t" << v->f;
    }
    std::cout << "\nparent:";
    for (auto& v : vertex)
    {
        if (v->parent == nullptr)
            std::cout << "\t-1";
        else
            std::cout << "\t" << v->parent->id;
    }
}

const char* Graph::ColorToString(const Color color)
{
    if (color == Color::WHITE)
        return "w";
    if (color == Color::GRAY)
        return "g";
    if (color == Color::BLACK)
        return "b";
    return nullptr;
}
