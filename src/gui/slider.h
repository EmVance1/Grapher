#pragma once
#include <SFML/Graphics.hpp>


class Slider {
private:
    float value;
    bool held = false;
    sf::FloatRect bounds;
    sf::RectangleShape scale;
    sf::RectangleShape shape;
    sf::Text label;

private:
    void apply_value();

public:
    Slider(const sf::Vector2f& pos, const std::string& value, const sf::Font& font, float init = 0);

    float get_value() const { return value; }

    void increment();
    void decrement();

    bool handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};

