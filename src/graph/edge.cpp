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
    shape.setSize(sf::Vector2f(len, Graph::get_lineweight()));
    shape.setPosition(pos);

    if (std::abs(dir.x) < 0.01f) {
        if (end.y > begin.y) {
            shape.move(Graph::get_lineweight() * -0.5f, 0.f);
            shape.setRotation(-90.f);
            point.setRotation(-90.f + 30.f);
        } else {
            shape.move(Graph::get_lineweight() * 0.5f, 0.f);
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
        point.setPosition(end - (dir / len * (45.f + Graph::get_lineweight())) + rev);
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
    shape.setSize(sf::Vector2f(shape.getSize().x, Graph::get_lineweight()));
}

