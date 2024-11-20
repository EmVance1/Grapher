#include "vertex.h"
#include "edge.h"
#include "graph.h"
#include "uuid.h"


Vertex::Vertex(): id(uuid::generate_v4()) {}

Vertex::Vertex(const sf::Vector2f& pos, const std::string& value): id(uuid::generate_v4()) {
    shape.setRadius(Graph::get_vertex_radius());
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(Graph::get_lineweight());
    shape.setOutlineColor(sf::Color::Black);
    shape.setOrigin(sf::Vector2f(Graph::get_vertex_radius(), Graph::get_vertex_radius()));

    content.setString(value);
    content.setFont(Graph::get_font());
    content.setStyle(sf::Text::Italic);
    content.setCharacterSize(40);
    content.setFillColor(sf::Color::Black);

    set_position(pos);
}

Vertex::Vertex(const Vertex& other): shape(other.shape), content(other.content), id(other.id) {}


void Vertex::set_position(const sf::Vector2f& pos, GridSnap snap) {
    sf::Vector2f p = pos;
    switch (snap) {
    case GridSnap::Grid: {
        float xdiff = (float)((int)p.x % 50);
        float ydiff = (float)((int)p.y % 50);
        p.x += 25.f - xdiff;
        p.y += 25.f - ydiff;
        break; }
    case GridSnap::Hex: {
        int x = (int)p.x - ((int)p.x % 50);
        p.x = (float)x;
        p.y -= (float)((int)p.y % 50);
        if ((x / 50) % 2 == 0) {
            p.y += 25.f;
        }
        break; }
    default:
        break;
    }
    shape.setPosition(p);
    const auto bounds = content.getGlobalBounds();
    content.setPosition(sf::Vector2f(p.x - bounds.width / 2, p.y - bounds.height));
}

sf::Vector2f Vertex::get_position() const {
    return shape.getPosition();
}

void Vertex::set_value(const std::string& val) {
    content.setString(val);
    const auto bounds = content.getGlobalBounds();
    const auto pos = get_position();
    content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - bounds.height));
}

bool Vertex::contains(const sf::Vector2f& pos) const {
    const auto center = get_position();
    const auto dir = pos - center;
    return dir.x * dir.x + dir.y * dir.y < Graph::get_vertex_radius() * Graph::get_vertex_radius();
}

