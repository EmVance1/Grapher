#pragma once
#include <SFML/Graphics.hpp>


enum class GridSnap {
    None = 0,
    Grid,
    Hex,
};


class Edge;
class Graph;

class Vertex {
private:
    sf::CircleShape shape;
    sf::Text content;

public:
    Vertex();
    Vertex(const sf::Vector2f& pos, const std::string& value);
    Vertex(const Vertex& other);
    friend class Graph;

    std::string id;
    std::vector<Edge> edges;

    void set_position(const sf::Vector2f& pos, GridSnap snap = GridSnap::Grid);
    sf::Vector2f get_position() const;
    void set_value(const std::string& val);
    const sf::String& get_value() const { return content.getString(); }
    bool contains(const sf::Vector2f& pos) const;

    std::string as_svg_element(const sf::Vector2f& offset) const;

    bool operator==(const Vertex& other) const { return id == other.id; }
    bool operator!=(const Vertex& other) const { return !(*this == other); }

    friend class GraphSettings;
};

