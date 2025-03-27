#pragma once
#include <SFML/Graphics.hpp>


class Vertex;
class Graph;

struct Edge : public sf::Drawable {
    sf::RectangleShape shape;
    sf::CircleShape point;

    Edge();

    static Edge from_vertices(const Vertex& from, const Vertex& to, bool directed);
    static std::string create_svg(const Vertex& from, const Vertex& to, bool directed, const sf::Vector2f& offset);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(shape);
        target.draw(point);
    }
};

