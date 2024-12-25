#include "pch.h"
#include "vertex.h"
#include "graph.h"
#include "edge.h"


Vertex::Vertex(): id(Uuid::generate_v4()) {}

Vertex::Vertex(const sf::Vector2f& pos, const std::string& value): id(Uuid::generate_v4()) {
    shape.setRadius(40.f);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(LINEWEIGHT);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOrigin(sf::Vector2f(40.f, 40.f));

    content.setString(value);
    content.setFont(Graph::get_font());
    content.setStyle(sf::Text::Regular);
    content.setCharacterSize(Graph::get_fontsize());
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
    p.x = std::floor(p.x);
    p.y = std::floor(p.y);
    shape.setPosition(p);
    const auto bounds = content.getGlobalBounds();
    content.setPosition(sf::Vector2f(p.x - bounds.width / 2, p.y - bounds.height));
}

sf::Vector2f Vertex::get_position() const {
    return shape.getPosition();
}

void Vertex::set_value(const std::string& val) {
    content.setString(val);
    const auto pos = get_position();
    const auto bounds = content.getGlobalBounds();
    content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - bounds.height));
}

bool Vertex::contains(const sf::Vector2f& pos) const {
    const auto center = get_position();
    const auto dir = pos - center;
    return dir.x * dir.x + dir.y * dir.y < 40.f * 40.f;
}


std::string Vertex::as_svg_element(const sf::Vector2f& offset) const {
    std::string elem = "\t<circle cx=\"" + std::to_string(get_position().x - offset.x)
                            + "\" cy=\"" + std::to_string(get_position().y - offset.y) + "\" r=\"42\" stroke=\"black\" stroke-width=\"1.5\" fill=\"white\" />\n";
    elem += "\t<text x=\"" + std::to_string(get_position().x - offset.x)
               + "\" y=\"" + std::to_string(get_position().y - offset.y + 10)
               + "\" font-family=\"CMU Serif\" font-size=\""
               + std::to_string(content.getCharacterSize()) + "\" font-style=\"italic\" text-anchor=\"middle\" fill=\"black\">"
               + content.getString() + "</text>\n";

    // TODO: IMPLEMENT AUTO-CROPPING

    return elem;
}
