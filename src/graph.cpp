#include "graph.h"
#include "uuid.h"
#include <fstream>
#include <iostream>
#include <sstream>


static sf::Font CMU_SERIF;
constexpr float TO_DEGREES = 180.f / 3.14159f;


template<typename T>
static T vector_length(const sf::Vector2<T>& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}


Vertex::Vertex(): id(uuid::generate_v4()) {}

Vertex::Vertex(const sf::Vector2f& pos, const std::string& value): id(uuid::generate_v4()) {
    shape.setRadius(RADIUS);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(Edge::RADIUS);
    shape.setOutlineColor(sf::Color::Black);

    content.setString(value);
    content.setFont(CMU_SERIF);
    content.setStyle(sf::Text::Italic);
    content.setCharacterSize(40);
    content.setFillColor(sf::Color::Black);

    set_position(pos);
}

Vertex::Vertex(const Vertex& other): shape(other.shape), content(other.content), id(other.id) {}


void Vertex::set_position(const sf::Vector2f& pos, GridSnap snap) {
    sf::Vector2f p = pos;
    switch (snap) {
    case GridSnap::Grid:
        p.x -= (float)((int)p.x % 50);
        p.y -= (float)((int)p.y % 50);
        break;
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
    shape.setPosition(sf::Vector2f(p.x - RADIUS, p.y - RADIUS));
    const auto bounds = content.getGlobalBounds();
    content.setPosition(sf::Vector2f(p.x - bounds.width / 2, p.y - bounds.height));
}

sf::Vector2f Vertex::get_position() const {
    return sf::Vector2f(shape.getPosition().x + RADIUS, shape.getPosition().y + RADIUS);
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
    return dir.x * dir.x + dir.y * dir.y < RADIUS * RADIUS;
}


Edge::Edge(const Vertex& _from, const Vertex& _to): from(_from.id), to(_to.id) {
    const auto start = _from.get_position();
    const auto end = _to.get_position();
    const auto dir = end - start;
    const auto pos = start.x < end.x ? start : end;
    shape.setSize(sf::Vector2f(vector_length(dir), RADIUS));
    shape.setPosition(pos);
    if (std::abs(dir.x) < 0.01f) {
        if (end.y > start.y) {
            shape.move(RADIUS * -0.5f, 0.f);
            shape.setRotation(-90);
        } else {
            // shape.setFillColor(sf::Color::Transparent);
            // shape.setOutlineColor(sf::Color::Transparent);
            shape.move(RADIUS * 0.5f, 0.f);
            shape.setRotation(90.f);
        }
    } else {
        shape.setRotation(TO_DEGREES * std::atan(dir.y / dir.x));
    }
    shape.setFillColor(sf::Color::Black);
}

Edge::Edge(const Edge& other): shape(other.shape), from(other.from), to(other.to) {}


void Edge::update(const Graph* graph) {
    const auto start = graph->get_vertex(from)->get_position();
    const auto end = graph->get_vertex(to)->get_position();
    const auto dir = end - start;
    const auto pos = start.x < end.x ? start : end;
    shape.setSize(sf::Vector2f(vector_length(dir), RADIUS));
    shape.setPosition(pos);
    if (std::abs(dir.x) < 0.01f) {
        if (end.y > start.y) {
            shape.move(RADIUS * -0.5f, 0.f);
            shape.setRotation(-90);
        } else {
            shape.move(RADIUS * 0.5f, 0.f);
            shape.setRotation(90.f);
        }
    } else {
        shape.setRotation(TO_DEGREES * std::atan(dir.y / dir.x));
    }
}


void Graph::load_from_file(const std::string& filename) {
    std::ifstream f(filename);
    std::string line;
    int count = 0;
    std::vector<Vertex*> verts;
    for (int i = 0; std::getline(f, line); i++) {
        if (i == 0) {
            count = std::atoi(line.c_str());
        } else if (i > count) {
            std::stringstream stream(line);
            int v = 0;
            int w = 0;
            stream >> v >> w;
            connect(verts[v], verts[w]);
        } else {
            std::stringstream stream(line);
            float x = 0;
            float y = 0;
            std::string val = "";
            stream >> x >> y >> val;
            if (val == "$") {
                Vertex* v = add_vertex(sf::Vector2f(x, y));
                verts.push_back(v);
            } else {
                Vertex* v = add_vertex(sf::Vector2f(x, y), val);
                verts.push_back(v);
            }
        }
    }
}

void Graph::save_to_file(const std::string& filename) {
    std::ofstream f(filename);
    f << vertices.size() << '\n';
    std::unordered_map<std::string, size_t> indices;
    size_t i = 0;
    for (const auto& [_, v] : vertices) {
        if (v.content.getString() == "") {
            f << v.get_position().x << " " << v.get_position().y << " " << "$\n";
        } else {
            f << v.get_position().x << " " << v.get_position().y << " " << v.content.getString().toUtf8().c_str() << "\n";
        }
        indices[v.id] = i;
        i++;
    }
    for (const auto& e : edges) {
        f << indices[e.from] << " " << indices[e.to] << '\n';
    }
}


void Graph::set_is_directed(bool _directed) {
    directed = _directed;
}


Vertex* Graph::add_vertex(const sf::Vector2f& position, const std::string& value) {
    Vertex vertex = Vertex(position, value);
    vertices[vertex.id] = vertex;
    return &vertices[vertex.id];
}

bool Graph::has_vertex(const std::string& id) const {
    return vertices.find(id) != vertices.end();
}

const Vertex* Graph::get_vertex(const std::string& id) const {
    auto n = vertices.find(id);
    if (n != vertices.end()) {
        return &n->second;
    } else {
        return nullptr;
    }
}

Vertex* Graph::get_vertex_mut(const std::string& id) {
    auto n = vertices.find(id);
    if (n != vertices.end()) {
        return &n->second;
    } else {
        return nullptr;
    }
}

void Graph::remove_vertex(const std::string& id) {
    for (int i = (int)edges.size() - 1; i >= 0; i--) {
        if (edges[i].to == id || edges[i].from == id) {
            edges.erase(edges.begin() + i);
        }
    }
    vertices.erase(id);
}

Vertex* Graph::pick_vertex(const sf::Vector2f& position) {
    for (auto& [_, v] : vertices) {
        if (v.contains(position)) {
            return &v;
        }
    }
    return nullptr;
}

void Graph::update_edges() {
    for (auto& e : edges) {
        e.update(this);
    }
}


void Graph::connect(const std::string& v, const std::string& w) {
    connect(get_vertex_mut(v), get_vertex_mut(w));
}

void Graph::connect(Vertex* v, Vertex* w) {
    if (!v || !w) return;
    Edge to = Edge{ *v, *w };
    if (std::find(edges.begin(), edges.end(), to) == edges.end()) {
        edges.push_back(to);
    }
    if (!directed) {
        Edge from = Edge{ *w, *v };
        if (std::find(edges.begin(), edges.end(), from) == edges.end()) {
            edges.push_back(from);
        }
    }
}

void Graph::disconnect(const std::string& v, const std::string& w) {
    disconnect(get_vertex_mut(v), get_vertex_mut(w));
}

void Graph::disconnect(Vertex* v, Vertex* w) {
    if (!v || !w) return;
    auto to = std::find(edges.begin(), edges.end(), Edge{ *v, *w });
    if (to != edges.end()) {
        edges.erase(to);
    }
    if (!directed) {
        auto from = std::find(edges.begin(), edges.end(), Edge{ *w, *v });
        if (from != edges.end())
            edges.erase(from);
    }
}


void Graph::draw(sf::RenderTarget& target) const {
    for (const auto& e : edges) {
        target.draw(e.shape);
    }
    for (const auto& [_, v] : vertices) {
        target.draw(v.shape);
        target.draw(v.content);
    }
}


void Graph::init_font() {
    CMU_SERIF.loadFromFile("C:/users/ayv20/cpp/graphs/res/cmunbx.ttf");
}

const sf::Font& Graph::get_font() {
    return CMU_SERIF;
}

