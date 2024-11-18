#pragma once
#include <SFML/Graphics.hpp>


class EditBox {
private:
    bool focus = false;
    sf::FloatRect bounds;
    sf::RectangleShape window;
    sf::RectangleShape shape;
    sf::RectangleShape cursor;
    sf::Text value;
    sf::Text label;

public:
    EditBox(const sf::Vector2f& pos, const std::string& _label, const sf::Font& font, std::string init = "");

    const sf::String& get_value() const { return value.getString(); }

    void set_active(const std::string& val);
    bool is_focused() { return focus; }

    void handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};

