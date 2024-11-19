#include "graph.h"
#include <fstream>
#include <iostream>
#include <sstream>


static sf::Font CMU_SERIF;


void Graph::load_from_file(const std::string& filename) {
    std::ifstream f(filename);
    std::string line;
    int count = 0;
    std::vector<Vertex*> verts;
    for (int i = 0; std::getline(f, line); i++) {
        if (i == 0) {
            directed = (bool)std::atoi(line.c_str());
        } else if (i == 1) {
            count = std::atoi(line.c_str());
        } else if (i-1 > count) {
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
    f << directed << '\n' << vertices.size() << '\n';
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
    Edge to = Edge{ *v, *w, directed };
    if (std::find(edges.begin(), edges.end(), to) == edges.end()) {
        edges.push_back(to);
    }
    if (!directed) {
        Edge from = Edge{ *w, *v, directed };
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
    auto to = std::find(edges.begin(), edges.end(), Edge{ *v, *w, directed });
    if (to != edges.end()) {
        edges.erase(to);
    }
    if (!directed) {
        auto from = std::find(edges.begin(), edges.end(), Edge{ *w, *v, directed });
        if (from != edges.end())
            edges.erase(from);
    }
}


void Graph::draw(sf::RenderTarget& target) const {
    for (const auto& e : edges) {
        target.draw(e.shape);
        if (directed) {
            target.draw(e.point);
        }
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

