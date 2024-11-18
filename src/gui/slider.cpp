#include "slider.h"


void Slider::apply_value() {
    if (bounds.left < scale.getPosition().x) {
        bounds.left = scale.getPosition().x;
    }
    if (bounds.left > scale.getPosition().x + 180.f) {
        bounds.left = scale.getPosition().x + 180.f;
    }
    shape.setPosition(sf::Vector2f(bounds.left, shape.getPosition().y));
    value = (bounds.left - scale.getPosition().x) / 180.f;
}


Slider::Slider(const sf::Vector2f& pos, const std::string& _value, const sf::Font& font, float init): value(init) {
    scale.setPosition(pos);
    scale.setSize(sf::Vector2f(200, 30));
    scale.setOutlineThickness(1);
    scale.setOutlineColor(sf::Color::Black);
    scale.setFillColor(sf::Color::White);

    shape.setPosition(pos);
    shape.move(sf::Vector2f(180.f * value, 0));
    shape.setSize(sf::Vector2f(20, 30));
    shape.setOutlineThickness(1);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(sf::Color(50, 75, 100));

    label.setPosition(pos + sf::Vector2f(210, 0));
    label.setCharacterSize(20);
    label.setFillColor(sf::Color::Black);
    label.setFont(font);
    label.setString(_value);

    bounds.left = pos.x;
    bounds.top = pos.y;
    bounds.width = 20;
    bounds.height = 30;
    bounds.left += 180.f * value;
}


void Slider::increment() {
    bounds.left += 10.f;
    apply_value();
}

void Slider::decrement() {
    bounds.left -= 10.f;
    apply_value();
}


void Slider::handle_event(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (bounds.contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
            held = true;
        }
        break;
    case sf::Event::MouseButtonReleased:
        held = false;
        break;
    case sf::Event::MouseMoved:
        if (held) {
            bounds.left = (float)event.mouseMove.x;
            apply_value();
        }
        break;
    default:
        break;
    }
}

void Slider::draw(sf::RenderTarget& target) const {
    target.draw(scale);
    target.draw(shape);
    target.draw(label);
}

