#pragma once
#include <SFML/Graphics.hpp>


struct ListItem {
    sf::FloatRect bounds;
    sf::RectangleShape shape;
    sf::Text label;
    std::string value;
    bool hovered = false;

    ListItem(const sf::Vector2f& pos, const std::string& value, const sf::Font& font, float width);

    bool handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};


class List {
private:
    std::vector<ListItem> options;
    size_t selected = 0;
    bool open = false;

public:
    List(const sf::Vector2f& pos, const std::vector<std::string>& values, const sf::Font& font, float width, bool dynwidth = false);

    void set_position(const sf::Vector2f& pos);
    void set_open(bool _open) { open = _open; }
    bool is_open() const { return open; }

    const std::string& get_value() const { return options[selected].value; }
    size_t get_index() const { return selected; }

    bool handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};

