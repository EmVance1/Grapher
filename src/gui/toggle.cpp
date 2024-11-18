#include "toggle.h"


ToggleButton::ToggleButton(const sf::Vector2f& pos, const std::string& value, const sf::Font& font, bool init): set(init) {
    shape.setPosition(pos);
    shape.setSize(sf::Vector2f(30, 30));
    shape.setOutlineThickness(1);
    shape.setOutlineColor(sf::Color::Black);
    if (set) {
        shape.setFillColor(sf::Color(50, 75, 100));
    } else {
        shape.setFillColor(sf::Color::White);
    }

    label.setPosition(pos + sf::Vector2f(40, 0));
    label.setCharacterSize(20);
    label.setFillColor(sf::Color::Black);
    label.setFont(font);
    label.setString(value);

    bounds.left = pos.x;
    bounds.top = pos.y;
    bounds.width = 30;
    bounds.height = 30;
}

void ToggleButton::handle_event(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (bounds.contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
            set = !set;
            if (set) {
                shape.setFillColor(sf::Color(50, 75, 100));
            } else {
                shape.setFillColor(sf::Color::White);
            }
        }
        break;
    default:
        break;
    }
}

void ToggleButton::draw(sf::RenderTarget& target) const {
    target.draw(shape);
    target.draw(label);
}

