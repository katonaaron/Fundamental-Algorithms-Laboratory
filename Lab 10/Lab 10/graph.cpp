#include "graph.h"

Graph::Graph(size_t v) : adj(v), time(0) {
    for (size_t i = 0; i < v; i++)
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

void Graph::DFS(Operation* op)
{
    if (op != nullptr) op->count();
    for (auto& v : vertex)
    {
        v->color = Color::WHITE;
        v->parent = nullptr;
        if (op != nullptr) op->count(3);
    }

    time = 0;
    if (op != nullptr) op->count();

    for (auto& v : vertex)
    {
        if (op != nullptr) op->count();
        if (v->color == Color::WHITE)
        {
            DFSVist(v->id, op);
        }
    }
}

void Graph::DFSVist(unsigned src, Operation* op)
{
    std::shared_ptr<Vertex> u = vertex[src];
    u->d = ++time;
    u->color = Color::GRAY;
    if (op != nullptr) op->count(4);

    for (auto& v : adj[src])
    {
        if (op != nullptr) op->count();
        if (v->color == Color::WHITE)
        {
            if (op != nullptr) op->count();
            v->parent = u;
            DFSVist(v->id, op);
        }
    }

    u->color = Color::BLACK;
    u->f = ++time;
    if (op != nullptr) op->count(3);
}

std::list<unsigned> Graph::TopologicalSort()
{
    std::list<unsigned> result;

    //DFS
    for (auto& v : vertex)
    {
        v->color = Color::WHITE;
    }

    for (auto& v : vertex)
    {
        if (v->color == Color::WHITE)
        {
            if(!TopologicalSortVisit(v->id, result))
                return std::list<unsigned>();
        }
    }
    return result;
}

bool Graph::TopologicalSortVisit(unsigned src, std::list<unsigned>& result)
{
    std::shared_ptr<Vertex> u = vertex[src];
    u->color = Color::GRAY;

    for (auto& v : adj[src])
    {
        if (v->color == Color::WHITE)
        {
            if (!TopologicalSortVisit(v->id, result))
                return false;
        }
        else if (v->color == Color::GRAY)
        {
            return false;
        }
    }

    u->color = Color::BLACK;
    result.push_front(u->id);

    return true;
}

std::vector<std::vector<unsigned>> Graph::StronglyConnectedComponents()
{
    std::vector<std::vector<unsigned>> result;
    std::stack<std::shared_ptr<Vertex>> stack;

    for (auto& v : vertex)
    {
        v->color = Color::WHITE;
    }

    time = 0;

    for (auto& v : vertex)
    {
        if (v->color == Color::WHITE)
        {
            StronglyConnectedComponentsVisit(v->id, result, stack);
        }
    }

    return result;
}


void Graph::StronglyConnectedComponentsVisit(unsigned src, std::vector<std::vector<unsigned>>& result, std::stack<std::shared_ptr<Vertex>>& stack)
{
    std::shared_ptr<Vertex> u = vertex[src];
    u->d = u->low = ++time;
    u->color = Color::GRAY;
    stack.push(u);

    for (auto& v : adj[src])
    {
        if (v->color == Color::WHITE)
        {
            v->parent = u;
            StronglyConnectedComponentsVisit(v->id, result, stack);
            u->low = min(u->low, v->low);
        }
        else if (v->color == Color::GRAY)
        {
            u->low = min(u->low, v->d);
        }
    }
    
    if (u->low == u->d)
    {
        result.emplace_back();
        std::shared_ptr<Vertex> v = nullptr;

        do
        {
            v = stack.top();
            stack.pop();
            v->color = Color::BLACK;
            result.back().push_back(v->id);
        } while (v->id != u->id);
    }
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
    std::cout << "\n";
}

void Graph::PrintAdjacencyList()
{
    std::cout << "Adjacency list:\n";
    for (unsigned i = 0; i < adj.size(); i++)
    {
        std::cout << vertex[i]->id << ":";
        for (auto& v : adj[i])
        {
            std::cout << "  " << v->id;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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
