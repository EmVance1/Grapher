#pragma once
#include <SFML/Graphics.hpp>

class Vertex;
class Graph;

class Edge {
private:
    sf::RectangleShape shape;
    sf::CircleShape point;
    inline static float RADIUS = 3;

private:
    void place_edge(const sf::Vector2f& begin, const sf::Vector2f& end, bool directed);

public:
    Edge() = default;
    Edge(const Vertex& _from, const Vertex& _to, bool directed);
    Edge(const Edge& other);

    std::string from;
    std::string to;

    void update(const Graph* graph);

    bool operator==(const Edge& other) const { return from == other.from && to == other.to; }
    bool operator!=(const Edge& other) const { return !(*this == other); }

    friend class GraphSettings;
    friend class Graph;
    friend class Vertex;
};


namespace std {
    template<> struct hash<Edge> {
        size_t operator()(const Edge& val) const {
            return hash<std::string>()(val.from) * hash<std::string>()(val.to);
        }
    };
}


