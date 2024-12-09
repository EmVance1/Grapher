#include "pch.h"
#include "graph.h"
#include "editor/settings.h"


static sf::Font CMU_SERIF;


void Graph::load_from_file(const std::string& filename) {
    std::ifstream f(filename);
    std::string line;
    int count = 0;
    std::vector<Vertex*> verts;
    for (int i = 0; std::getline(f, line); i++) {
        if (i == 0) {
            maj_ver = line[0] - '0';
            min_ver = line[2] - '0';
        } else if (i == 1) {
            s_fontsize = std::atoi(line.c_str());
        } else if (i == 2) {
            m_directed = (bool)std::atoi(line.c_str());
        } else if (i == 3) {
            count = std::atoi(line.c_str());
        } else if (i-3 > count) {
            std::stringstream stream(line);
            int v = 0;
            int w = 0;
            stream >> v >> w;
            connect(verts[v], verts[w]);
        } else {
            std::stringstream stream(line);
            float x = 0;
            float y = 0;
            stream >> x >> y;
            std::string val = "";
            std::getline(stream, val);
            while (val[0] == ' ') val.erase(val.begin());
            Vertex* v = add_vertex(sf::Vector2f(x, y), val);
            verts.push_back(v);
        }
    }
}

void Graph::save_to_file(const std::string& filename) const {
    std::ofstream f(filename);
    f << maj_ver << '.' << maj_ver << '\n';
    f << s_fontsize << '\n';
    f << m_directed << '\n' << m_vertices.size() << '\n';
    std::unordered_map<std::string, size_t> indices;
    size_t i = 0;
    for (const auto& [_, v] : m_vertices) {
        if (v.content.getString() == "") {
            f << v.get_position().x << " " << v.get_position().y << " " << "$\n";
        } else {
            f << v.get_position().x << " " << v.get_position().y << " " << v.content.getString().toUtf8().c_str() << "\n";
        }
        indices[v.id] = i;
        i++;
    }
    for (const auto& [_, v] : m_vertices) {
        for (const auto& e : v.edges) {
            f << indices[e.from] << " " << indices[e.to] << '\n';
        }
    }
}

void Graph::export_svg(const std::string& filename) const {
    sf::Vector2f min = sf::Vector2f(10000.f, 10000.f);
    sf::Vector2f max = sf::Vector2f(0.f, 0.f);
    for (const auto& [_, v] : m_vertices) {
        if (v.get_position().x - 60.f < min.x) {
            min.x = v.get_position().x - 60.f;
        }
        if (v.get_position().y - 60.f < min.y) {
            min.y = v.get_position().y - 60.f;
        }
        if (v.get_position().x + 60.f > max.x) {
            max.x = v.get_position().x + 60.f;
        }
        if (v.get_position().y + 60.f > max.y) {
            max.y = v.get_position().y + 60.f;
        }
    }

    std::ofstream f(filename);
    f << "<svg version=\"1.1\" width=\"" + std::to_string((max - min).x)
                        + "\" height=\"" + std::to_string((max - min).y) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n\n";
    for (const auto& [_, v] : m_vertices) {
        for (const auto& e : v.edges) {
            f << e.as_svg_element(this, min);
        }
    }
    for (const auto& [_, v] : m_vertices) {
        f << v.as_svg_element(min);
    }
    f << "\n</svg>";
    f.close();
}


Vertex* Graph::add_vertex(const sf::Vector2f& position, const std::string& value) {
    Vertex vertex = Vertex(position, value);
    m_vertices[vertex.id] = vertex;
    return &m_vertices[vertex.id];
}

bool Graph::has_vertex(const std::string& id) const {
    return m_vertices.find(id) != m_vertices.end();
}

const Vertex* Graph::get_vertex(const std::string& id) const {
    auto n = m_vertices.find(id);
    if (n != m_vertices.end()) {
        return &n->second;
    } else {
        return nullptr;
    }
}

Vertex* Graph::get_vertex_mut(const std::string& id) {
    auto n = m_vertices.find(id);
    if (n != m_vertices.end()) {
        return &n->second;
    } else {
        return nullptr;
    }
}

void Graph::remove_vertex(const std::string& id) {
    for (auto& [_, v] : m_vertices) {
        for (int i = (int)v.edges.size() - 1; i >= 0; i--) {
            if (v.edges[i].to == id) {
                v.edges.erase(v.edges.begin() + i);
            }
        }
    }
    m_vertices.erase(id);
}

Vertex* Graph::pick_vertex(const sf::Vector2f& position) {
    for (auto& [_, v] : m_vertices) {
        if (v.contains(position)) {
            return &v;
        }
    }
    return nullptr;
}


void Graph::connect(const std::string& v, const std::string& w) {
    connect(get_vertex_mut(v), get_vertex_mut(w));
}

void Graph::connect(Vertex* v, Vertex* w) {
    if (!v || !w) return;
    Edge to = Edge{ *v, *w, m_directed };
    if (std::find(v->edges.begin(), v->edges.end(), to) == v->edges.end()) {
        v->edges.push_back(to);
    }
    if (!m_directed) {
        Edge from = Edge{ *w, *v, m_directed };
        if (std::find(w->edges.begin(), w->edges.end(), from) == w->edges.end()) {
            w->edges.push_back(from);
        }
    }
}

void Graph::disconnect(const std::string& v, const std::string& w) {
    disconnect(get_vertex_mut(v), get_vertex_mut(w));
}

void Graph::disconnect(Vertex* v, Vertex* w) {
    if (!v || !w) return;
    auto to = std::find(v->edges.begin(), v->edges.end(), Edge{ *v, *w, m_directed });
    if (to != v->edges.end()) {
        v->edges.erase(to);
    }
    if (!m_directed) {
        auto from = std::find(w->edges.begin(), w->edges.end(), Edge{ *w, *v, m_directed });
        if (from != w->edges.end()) {
            w->edges.erase(from);
        }
    }
}


void Graph::draw(sf::RenderTarget& target) const {
    for (const auto& [_, v] : m_vertices) {
        for (const auto& e : v.edges) {
            target.draw(e.shape);
            if (m_directed) {
                target.draw(e.point);
            }
        }
    }
    for (const auto& [_, v] : m_vertices) {
        target.draw(v.shape);
        target.draw(v.content);
    }
}


void Graph::init_font() {
    WCHAR path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::filesystem::path p(path);
    p = p.parent_path();
    if (!CMU_SERIF.loadFromFile(p.generic_string() + "/res/cmunrm.ttf")) {
        CMU_SERIF.loadFromFile(p.parent_path().parent_path().generic_string() + "/res/cmunrm.ttf");
    }
}

const sf::Font& Graph::get_font() {
    return CMU_SERIF;
}

