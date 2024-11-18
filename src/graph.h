#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>


enum class GridSnap {
    None = 0,
    Grid,
    Hex,
};


class Graph;

class Vertex {
private:
    sf::CircleShape shape;
    sf::Text content;
    static inline constexpr uint32_t RADIUS = 40;

public:
    Vertex();
    Vertex(const sf::Vector2f& pos, const std::string& value);
    Vertex(const Vertex& other);
    friend class Graph;

    std::string id;

    void set_position(const sf::Vector2f& pos, GridSnap snap = GridSnap::Grid);
    sf::Vector2f get_position() const;
    void set_value(const std::string& val);
    const sf::String& get_value() const { return content.getString(); }
    bool contains(const sf::Vector2f& pos) const;

    bool operator==(const Vertex& other) const { return id == other.id; }
    bool operator!=(const Vertex& other) const { return !(*this == other); }

    friend class GraphSettings;
};

class Edge {
private:
    sf::RectangleShape shape;
    inline static float RADIUS = 3;

public:
    Edge() = default;
    Edge(const Vertex& _from, const Vertex& _to);
    Edge(const Edge& other);
    friend class Graph;

    std::string from;
    std::string to;

    void update(const Graph* graph);

    bool operator==(const Edge& other) const { return from == other.from && to == other.to; }
    bool operator!=(const Edge& other) const { return !(*this == other); }

    friend class GraphSettings;
    friend class Vertex;
};


namespace std {
    template<> struct hash<Edge> {
        size_t operator()(const Edge& val) const {
            return hash<std::string>()(val.from) * hash<std::string>()(val.to);
        }
    };
}


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

