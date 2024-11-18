#pragma once
#include <SFML/Graphics.hpp>


class ToggleButton {
private:
    bool set = false;
    sf::FloatRect bounds;
    sf::RectangleShape shape;
    sf::Text label;

public:
    ToggleButton(const sf::Vector2f& pos, const std::string& value, const sf::Font& font, bool init = false);

    bool get_value() const { return set; }

    void handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};

