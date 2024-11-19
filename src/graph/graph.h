#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "vertex.h"
#include "edge.h"


class Graph {
private:
    std::unordered_map<std::string, Vertex> vertices;
    std::vector<Edge> edges;
    bool directed;

public:
    Graph(bool _directed = false): directed(_directed) {}

    static void init_font();
    static const sf::Font& get_font();

    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);

    void set_is_directed(bool _directed);
    bool is_directed() const { return directed; }

    Vertex* add_vertex(const sf::Vector2f& position, const std::string& value = "");
    bool has_vertex(const std::string& id) const;
    const Vertex* get_vertex(const std::string& id) const;
    Vertex* get_vertex_mut(const std::string& id);
    void remove_vertex(const std::string& id);

    Vertex* pick_vertex(const sf::Vector2f& position);
    void update_edges();

    void connect(const std::string& v, const std::string& w);
    void connect(Vertex* v, Vertex* w);
    void disconnect(const std::string& v, const std::string& w);
    void disconnect(Vertex* v, Vertex* w);

    size_t size() const { return vertices.size(); }

    void draw(sf::RenderTarget& target) const;

    friend class GraphSettings;
};

