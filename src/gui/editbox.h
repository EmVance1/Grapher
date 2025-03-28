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
    std::string cancel;

public:
    EditBox(const sf::Vector2f& pos, const std::string& _label, const sf::Font& font, const sf::Font& guifont, std::string init = "");

    const sf::String& get_value() const { return value.getString(); }

    void set_position(const sf::Vector2f& pos);
    void set_active(const std::string& val);
    bool is_focused() { return focus; }

    bool handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};

