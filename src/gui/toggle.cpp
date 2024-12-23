#include "pch.h"
#include "toggle.h"


ToggleButton::ToggleButton(const sf::Vector2f& pos, const std::string& value, const sf::Font& font, bool init)
    : set(init),
    label(font, value, 25)
{
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
    label.setFillColor(sf::Color::Black);

    bounds.position.x = pos.x;
    bounds.position.y = pos.y;
    bounds.size.x = 30;
    bounds.size.y = 30;
}

bool ToggleButton::handle_event(const sf::Event& event) {
    if (const auto press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (bounds.contains({(float)press->position.x, (float)press->position.y})) {
            set = !set;
            if (set) {
                shape.setFillColor(sf::Color(50, 75, 100));
            } else {
                shape.setFillColor(sf::Color::White);
            }
            return true;
        }
    }
    return false;
}

void ToggleButton::draw(sf::RenderTarget& target) const {
    target.draw(shape);
    target.draw(label);
}

