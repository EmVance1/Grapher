#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "vertex.h"
#include "edge.h"


#define LINEWEIGHT 1.5f


class Graph {
private:
    std::unordered_map<std::string, Vertex> m_vertices;
    bool m_directed;

    inline static uint32_t s_fontsize = 26;

    inline static constexpr uint32_t maj_ver = 0;
    inline static constexpr uint32_t min_ver = 1;

    bool changed = false;

public:
    inline static sf::Font* CMU_SERIF;
    inline static sf::Font* GUI_FONT;

    Graph(bool directed = false): m_directed(directed) {}

    static void init_fonts();
    static const sf::Font& get_font();
    static const sf::Font& get_gui_font();

    static uint32_t get_fontsize() { return s_fontsize; };

    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename) const;
    void export_svg(const std::string& filename) const;
    void reset() { m_vertices.clear(); m_directed = false; s_fontsize = 26; changed = false; };
    bool is_changed() const { return changed; }

    void set_directed(bool directed) { m_directed = directed; }
    bool is_directed() const { return m_directed; }

    Vertex* add_vertex(const sf::Vector2f& position, const std::string& value = "");
    Vertex* add_vertex_with_id(const std::string& id, const sf::Vector2f& position, const std::string& value = "");
    bool has_vertex(const std::string& id) const;
    const Vertex* get_vertex(const std::string& id) const;
    Vertex* get_vertex_mut(const std::string& id);
    std::unordered_set<std::string> remove_vertex(const std::string& id);

    Vertex* pick_vertex(const sf::Vector2f& position);

    void connect(const std::string& v, const std::string& w);
    void connect(Vertex* v, Vertex* w);
    void disconnect(const std::string& v, const std::string& w);
    void disconnect(Vertex* v, Vertex* w);

    size_t size() const { return m_vertices.size(); }

    void draw(sf::RenderTarget& target, bool no_hidden = false) const;

    friend class GraphSettings;
    friend class GraphEditor;
};

