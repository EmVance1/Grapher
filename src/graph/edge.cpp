#include "pch.h"
#include "edge.h"
#include "vertex.h"
#include "graph.h"


template<typename T>
static T vector_length(const sf::Vector2<T>& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

constexpr float TO_DEGREES = 180.f / 3.14159f;


void Edge::place_edge(const sf::Vector2f& begin, const sf::Vector2f& end, bool directed) {
    const auto dir = end - begin;
    const auto pos = begin.x < end.x ? begin : end;
    const auto len = vector_length(dir);
    shape.setSize(sf::Vector2f(len, LINEWEIGHT));
    shape.setPosition(pos);

    if (std::abs(dir.x) < 0.01f) {
        if (end.y > begin.y) {
            shape.move(LINEWEIGHT * -0.5f, 0.f);
            shape.setRotation(-90.f);
            point.setRotation(-90.f + 30.f);
        } else {
            shape.move(LINEWEIGHT * 0.5f, 0.f);
            shape.setRotation(90.f);
            point.setRotation(90.f + 30.f);
        }
    } else {
        const float calc = std::atan(dir.y / dir.x);
        shape.setRotation(TO_DEGREES * calc);
        point.setRotation(TO_DEGREES * calc + 30.f);
    }
    if (dir.x > 0.1f) {
        point.setRotation(point.getRotation() + 180.f);
    }

    if (directed) {
        const auto rev = sf::Vector2f(-dir.y, dir.x) / len * 5.f;
        shape.move(rev);
        point.setPosition(end - (dir / len * (45.f + LINEWEIGHT)) + rev);
    }
}

Edge::Edge(const Vertex& _from, const Vertex& _to, bool directed): from(_from.id), to(_to.id) {
    shape.setFillColor(sf::Color::Black);

    point.setFillColor(sf::Color::Black);
    point.setPointCount(3);
    point.setRadius(7);
    point.setOrigin(7, 7);

    place_edge(_from.get_position(), _to.get_position(), directed);
}

Edge::Edge(const Edge& other): shape(other.shape), point(other.point), from(other.from), to(other.to) {}


void Edge::refresh_edge(const Graph* graph) {
    place_edge(
        graph->get_vertex(from)->get_position(),
        graph->get_vertex(to)->get_position(),
        graph->is_directed()
    );
}

void Edge::refresh_lineweight() {
    shape.setSize(sf::Vector2f(shape.getSize().x, LINEWEIGHT));
}

std::string Edge::as_svg_element(const Graph* graph, const sf::Vector2f& offset) const {
    const auto begin = graph->get_vertex(from)->get_position();
    const auto end = graph->get_vertex(to)->get_position();
    const auto directed = graph->is_directed();

    const auto dir = end - begin;
    const auto len = vector_length(dir);

    if (directed) {
        const auto rev = sf::Vector2f(-dir.y, dir.x) / len * 5.f;
        const auto p1 = begin + rev;
        const auto p2 = end + rev;
        std::string elem =  "\t<line x1=\"" + std::to_string(p1.x - offset.x)
                               + "\" x2=\"" + std::to_string(p2.x - offset.x)
                               + "\" y1=\"" + std::to_string(p1.y - offset.y)
                               + "\" y2=\"" + std::to_string(p2.y - offset.y) + "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";

        elem += "\t<polygon points=\"";
        for (size_t i = 0; i < point.getPointCount(); i++) {
            elem += std::to_string(point.getPoint(i).x) + " " + std::to_string(point.getPoint(i).y);
            if (i != point.getPointCount() - 1) {
                elem += ", ";
            }
        }
        elem += "\" transform=\"translate(" + std::to_string(point.getPosition().x - offset.x)
                                     + ", " + std::to_string(point.getPosition().y - offset.y)
                                     + ") rotate(" + std::to_string(point.getRotation()) + ") translate(-7, -7)\"";

        return elem + " fill=\"black\" />\n";
    } else {
        return "\t<line x1=\"" + std::to_string(begin.x - offset.x)
                  + "\" x2=\"" + std::to_string(end.x - offset.x)
                  + "\" y1=\"" + std::to_string(begin.y - offset.y)
                  + "\" y2=\"" + std::to_string(end.y - offset.y) + "\"  stroke=\"black\" stroke-width=\"1.5\"/>\n";
    }
}

