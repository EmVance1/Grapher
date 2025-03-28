#include "pch.h"
#include "vertex.h"
#include "graph.h"
#include "edge.h"


static bool circle_point(const sf::Vector2f& center, float radius, const sf::Vector2f& p) {
    const auto v = p - center;
    const auto l = v.x * v.x + v.y * v.y;
    return l <= radius * radius;
}

static bool circle_AABB(const sf::Vector2f& center, float radius, const sf::Vector2f& pos, const sf::Vector2f& size) {
    const auto a = pos;
    const auto b = pos + sf::Vector2f(size.x, 0);
    const auto c = pos + size;
    const auto d = pos + sf::Vector2f(0, size.y);
    if (circle_point(center, radius, a) || circle_point(center, radius, b) || circle_point(center, radius, c) || circle_point(center, radius, d)) {
        return true;
    } else if (center.y > pos.y && center.y < pos.y + size.y) {
        if (center.x >= pos.x - radius && center.x <= pos.x + size.x + radius) {
            return true;
        }
    } else if (center.x > pos.x && center.x < pos.x + size.x) {
        if (center.y >= pos.y - radius && center.y <= pos.y + size.y + radius) {
            return true;
        }
    }
    return false;
}


void VertexDisplay::set_position(const sf::Vector2f& pos, GridSnap snap) {
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
    circle_shape.setPosition(p);
    const auto bounds = content.getGlobalBounds();
    const auto height = (float)content.getCharacterSize() * 0.3f;
    content.setPosition(sf::Vector2f(p.x - bounds.width / 2, p.y - height));
}

void VertexDisplay::set_value(const std::string& val) {
    content.setString(val);
    const auto pos = get_position();
    const auto bounds = content.getGlobalBounds();
    const auto height = (float)content.getCharacterSize() * 0.3f;
    content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - height));
}

void VertexDisplay::set_content_size(uint32_t size) {
    content.setCharacterSize(size * 2);
    content.setScale(0.5f, 0.5f);
    const auto pos = get_position();
    const auto bounds = content.getGlobalBounds();
    const auto height = (float)content.getCharacterSize() * 0.3f;
    content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - height));
}


bool VertexDisplay::contains(const sf::Vector2f& pos) const {
    const auto radius = hidden ? 20.f : 40.f;
    return circle_point(get_position(), radius, pos);
}

bool VertexDisplay::intersects(const sf::FloatRect& rect) const {
    const auto radius = hidden ? 20.f : 40.f;
    return circle_AABB(get_position(), radius, { rect.left, rect.top }, { rect.width, rect.height });
}


void VertexDisplay::set_hidden(bool _hidden) {
    hidden = _hidden;
    if (hidden) {
        circle_shape.setFillColor(sf::Color(255, 255, 255, 50));
        circle_shape.setRadius(20);
        circle_shape.setOrigin(20, 20);
        content.setFillColor(sf::Color::Transparent);
        if (!highlight) {
            circle_shape.setOutlineColor(sf::Color(0, 0, 0, 50));
        }
    } else {
        circle_shape.setFillColor(sf::Color(255, 255, 255, 255));
        circle_shape.setRadius(40);
        circle_shape.setOrigin(40, 40);
        content.setFillColor(sf::Color::Black);
        if (!highlight) {
            circle_shape.setOutlineColor(sf::Color(0, 0, 0, 255));
        }
    }
}

void VertexDisplay::set_highlighted(bool _highlight) {
    highlight = _highlight;
    if (highlight) {
        circle_shape.setOutlineColor(sf::Color(0, 220, 255, 255));
        circle_shape.setOutlineThickness(3);
    } else if (hidden) {
        circle_shape.setOutlineColor(sf::Color(0, 0, 0, 50));
        circle_shape.setOutlineThickness(LINEWEIGHT);
    } else {
        circle_shape.setOutlineColor(sf::Color(0, 0, 0, 255));
        circle_shape.setOutlineThickness(LINEWEIGHT);
    }
}


std::string VertexDisplay::as_svg_element(const sf::Vector2f& offset) const {
    if (!hidden) {
        std::string elem = "\t<circle cx=\"" + std::to_string(get_position().x - offset.x)
                                + "\" cy=\"" + std::to_string(get_position().y - offset.y) + "\" r=\"42\" stroke=\"black\" stroke-width=\"1.5\" fill=\"white\" />\n";
        elem += "\t<text x=\"" + std::to_string(get_position().x - offset.x)
                   + "\" y=\"" + std::to_string(get_position().y - offset.y + 10)
                   + "\" font-family=\"CMU Serif\" font-size=\""
                   + std::to_string(content.getCharacterSize() * 0.5) + "\" text-anchor=\"middle\" fill=\"black\">"
                   // + std::to_string(content.getCharacterSize()) + "\" font-style=\"italic\" text-anchor=\"middle\" fill=\"black\">"
                   + content.getString() + "</text>\n";
        return elem;
    } else {
        return "";
    }

}


Vertex::Vertex(): id(Uuid::generate_v4()) {}

Vertex::Vertex(const sf::Vector2f& pos, const std::string& value): id(Uuid::generate_v4()) {
    display.circle_shape.setRadius(40.f);
    display.circle_shape.setFillColor(sf::Color::White);
    display.circle_shape.setOutlineThickness(LINEWEIGHT);
    display.circle_shape.setOutlineColor(sf::Color::Black);
    display.circle_shape.setOrigin(sf::Vector2f(40.f, 40.f));

    display.content.setFont(Graph::get_font());
    display.content.setStyle(sf::Text::Regular);
    display.content.setFillColor(sf::Color::Black);

    set_content_size(Graph::get_fontsize());
    set_position(pos);
    set_value(value);
}

Vertex::Vertex(const Vertex& other): display(other.display), value(other.value), hidden(other.hidden), edges(other.edges), id(other.id) {}

Vertex& Vertex::operator=(const Vertex& other) {
    display = other.display;
    value = other.value;
    hidden = other.hidden;
    edges = other.edges;
    id = other.id;
    return *this;
}


void Vertex::set_hidden(bool _hidden) {
    hidden = _hidden;
    display.set_hidden(hidden);
}

