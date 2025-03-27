#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>


enum class GridSnap {
    None = 0,
    Grid,
    Hex,
};


class Edge;
class Graph;


struct VertexDisplay : public sf::Drawable {
    sf::CircleShape circle_shape;
    sf::RectangleShape box_shape;
    sf::Text content;
    bool hidden = false;
    bool highlight = false;

    void set_position(const sf::Vector2f& pos, GridSnap snap = GridSnap::Grid);
    const sf::Vector2f& get_position() const { return circle_shape.getPosition(); }
    void set_value(const std::string& val);
    const sf::String& get_value() const { return content.getString(); }
    void set_content_size(uint32_t size);
    bool contains(const sf::Vector2f& pos) const;
    bool intersects(const sf::FloatRect& rect) const;

    void set_hidden(bool hidden);
    void set_highlighted(bool highlight);
    std::string as_svg_element(const sf::Vector2f& offset) const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(circle_shape);
        target.draw(content);
    }
};


class Vertex {
private:
    VertexDisplay display;
    std::string value;
    bool hidden = false;

public:
    std::string id;
    std::unordered_set<std::string> edges;

public:
    Vertex();
    Vertex(const sf::Vector2f& pos, const std::string& value);
    Vertex(const Vertex& other);
    Vertex& operator=(const Vertex& other);

    void set_position(const sf::Vector2f& pos, GridSnap snap = GridSnap::Grid) { display.set_position(pos, snap); }
    const sf::Vector2f& get_position() const { return display.get_position(); }
    void set_value(const std::string& val) { value = val; display.set_value(val); }
    const std::string& get_value() const { return value; }
    void set_content_size(uint32_t size) { display.set_content_size(size); }
    void set_hidden(bool hidden);
    bool get_hidden() const { return hidden; }
    void set_highlighted(bool highlight) { display.set_highlighted(highlight); }
    bool get_highlighted() const { return display.highlight; }
    bool contains(const sf::Vector2f& pos) const { return display.contains(pos); }
    bool intersects(const sf::FloatRect& rect) const { return display.intersects(rect); }
    void toggle_hidden() { hidden = !hidden; set_hidden(hidden); }
    const VertexDisplay& get_display() const { return display; }

    std::string as_svg_element(const sf::Vector2f& offset) const { return display.as_svg_element(offset); }

    bool goes_to(const std::string& other) const { return edges.find(other) != edges.end(); }

    bool operator==(const Vertex& other) const { return id == other.id; }
    bool operator!=(const Vertex& other) const { return !(*this == other); }
};

