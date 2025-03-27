#include "pch.h"
#include "edge.h"
#include "vertex.h"
#include "graph.h"


template<typename T>
static T vector_length(const sf::Vector2<T>& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

constexpr float TO_DEGREES = 180.f / 3.14159f;


Edge::Edge() {
    shape.setFillColor(sf::Color::Black);

    point.setFillColor(sf::Color::Black);
    point.setPointCount(3);
    point.setRadius(7);
    point.setOrigin(7, 7);
}


Edge Edge::from_vertices(const Vertex& from, const Vertex& to, bool directed) {
    auto result = Edge();
    const sf::Vector2f begin = from.get_position();
    const sf::Vector2f end = to.get_position();
    const auto dir = end - begin;
    const auto pos = begin.x < end.x ? begin : end;
    const auto len = vector_length(dir);
    result.shape.setSize(sf::Vector2f(len, LINEWEIGHT));
    result.shape.setPosition(pos);

    if (std::abs(dir.x) < 0.01f) {
        if (end.y > begin.y) {
            result.shape.move(LINEWEIGHT * -0.5f, 0.f);
            result.shape.setRotation(-90.f);
            result.point.setRotation(-90.f + 30.f);
        } else {
            result.shape.move(LINEWEIGHT * 0.5f, 0.f);
            result.shape.setRotation(90.f);
            result.point.setRotation(90.f + 30.f);
        }
    } else {
        const float calc = std::atan(dir.y / dir.x);
        result.shape.setRotation(TO_DEGREES * calc);
        result.point.setRotation(TO_DEGREES * calc + 30.f);
    }
    if (dir.x > 0.1f) {
        result.point.setRotation(result.point.getRotation() + 180.f);
    }

    if (directed) {
        result.point.setFillColor(sf::Color(0, 0, 0, 255));
        const auto rev = (to.edges.find(from.id) != to.edges.end()) ? (sf::Vector2f(-dir.y, dir.x) / len * 5.f) : sf::Vector2f(0, 0);
        result.shape.move(rev);
        result.point.setPosition(end - (dir / len * (45.f + LINEWEIGHT)) + rev);
    } else {
        result.point.setFillColor(sf::Color(0, 0, 0, 0));
    }

    return result;
}


std::string Edge::create_svg(const Vertex& from, const Vertex& to, bool directed, const sf::Vector2f& offset) {
    const auto e = Edge::from_vertices(from, to, directed);
    const auto begin = from.get_position();
    const auto end = to.get_position();

    const auto dir = end - begin;
    const auto len = vector_length(dir);

    if (directed && !to.get_hidden()) {
        const auto rev = (to.edges.find(from.id) != to.edges.end()) ? (sf::Vector2f(-dir.y, dir.x) / len * 5.f) : sf::Vector2f(0, 0);
        const auto p1 = begin + rev;
        const auto p2 = end + rev;
        std::string elem =  "\t<line x1=\"" + std::to_string(p1.x - offset.x)
                               + "\" x2=\"" + std::to_string(p2.x - offset.x)
                               + "\" y1=\"" + std::to_string(p1.y - offset.y)
                               + "\" y2=\"" + std::to_string(p2.y - offset.y) + "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";

        elem += "\t<polygon points=\"";
        for (size_t i = 0; i < e.point.getPointCount(); i++) {
            elem += std::to_string(e.point.getPoint(i).x) + " " + std::to_string(e.point.getPoint(i).y);
            if (i != e.point.getPointCount() - 1) {
                elem += ", ";
            }
        }
        elem += "\" transform=\"translate(" + std::to_string(e.point.getPosition().x - offset.x)
                                     + ", " + std::to_string(e.point.getPosition().y - offset.y)
                                     + ") rotate(" + std::to_string(e.point.getRotation()) + ") translate(-5, -7)\"";

        return elem + " fill=\"black\" />\n";
    } else {
        return "\t<line x1=\"" + std::to_string(begin.x - offset.x)
                  + "\" x2=\"" + std::to_string(end.x - offset.x)
                  + "\" y1=\"" + std::to_string(begin.y - offset.y)
                  + "\" y2=\"" + std::to_string(end.y - offset.y) + "\"  stroke=\"black\" stroke-width=\"1.5\"/>\n";
    }
}

