#include "pch.h"
#include "slider.h"


void Slider::apply_value() {
    if (bounds.position.x < scale.getPosition().x) {
        bounds.position.x = scale.getPosition().x;
    }
    if (bounds.position.x > scale.getPosition().x + 180.f) {
        bounds.position.x = scale.getPosition().x + 180.f;
    }
    shape.setPosition({bounds.position.x, shape.getPosition().y});
    value = (bounds.position.x - scale.getPosition().x) / 180.f;
}


Slider::Slider(const sf::Vector2f& pos, const std::string& _value, const sf::Font& font, float init)
    : value(init),
    label(font, _value, 25)
{
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
    label.setFillColor(sf::Color::Black);

    bounds.position = pos;
    bounds.position.x += 180.f * value;
    bounds.size.x = 20;
    bounds.size.y = 30;
}


void Slider::increment() {
    bounds.position.x += 10.f;
    apply_value();
}

void Slider::decrement() {
    bounds.position.x -= 10.f;
    apply_value();
}


bool Slider::handle_event(const sf::Event& event) {
    if (const auto press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (bounds.contains({(float)press->position.x, (float)press->position.x})) {
            held = true;
        } else if (scale.getGlobalBounds().contains({(float)press->position.x, (float)press->position.x})) {
            held = true;
            bounds.position.x = (float)press->position.x;
            apply_value();
            return true;
        }
    } else if (event.is<sf::Event::MouseButtonReleased>()) {
        held = false;
    } else if (const auto move = event.getIf<sf::Event::MouseMoved>()) {
        if (held) {
            bounds.position.x = (float)move->position.x;
            apply_value();
            return true;
        }
    }

    return false;
}

void Slider::draw(sf::RenderTarget& target) const {
    target.draw(scale);
    target.draw(shape);
    target.draw(label);
}

