#include "pch.h"
#include "editbox.h"


EditBox::EditBox(const sf::Vector2f& pos, const std::string& _value, const sf::Font& font, std::string init)
    : value(font, init, 25),
    label(font, _value, 25)
{
    window.setPosition(pos);
    window.setSize(sf::Vector2f(300, 80));
    window.setOutlineThickness(1);
    window.setFillColor(sf::Color(150, 150, 150));
    window.setOutlineColor(sf::Color::Black);

    shape.setPosition(pos + sf::Vector2f(25, 40));
    shape.setSize(sf::Vector2f(250, 30));
    shape.setOutlineThickness(1);
    shape.setFillColor(sf::Color::White);
    if (focus) {
        shape.setOutlineColor(sf::Color(50, 75, 100));
    } else {
        shape.setOutlineColor(sf::Color::Black);
    }

    cursor.setPosition(pos + sf::Vector2f(30, 40));
    cursor.setSize(sf::Vector2f(1, 30));
    cursor.setFillColor(sf::Color::Black);

    label.setPosition(pos + sf::Vector2f(30, 0));
    label.setFillColor(sf::Color::Black);

    value.setPosition(pos + sf::Vector2f(30, 40));
    value.setFillColor(sf::Color::Black);

    bounds.position.x = pos.x + 25;
    bounds.position.y = pos.y + 40;
    bounds.size.x = 250;
    bounds.size.y = 30;
}

void EditBox::set_active(const std::string& val) {
    focus = true;
    value.setString(val);
    cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().size.x, 0));
}

bool EditBox::handle_event(const sf::Event& event) {
    if (const auto press = event.getIf<sf::Event::KeyPressed>()) {
        if (press->code == sf::Keyboard::Key::Enter) {
            focus = false;
        } else if (press->code == sf::Keyboard::Key::Backspace) {
            value.setString(value.getString().substring(0, value.getString().getSize() - 1));
            cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().size.x, 0));
        }
        return true;
    } else if (const auto text = event.getIf<sf::Event::TextEntered>()) {
        if (text->unicode >= 32) {
            value.setString(value.getString() + text->unicode);
            cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().size.x, 0));
        }
        return true;
    }

    return false;
}

void EditBox::draw(sf::RenderTarget& target) const {
    target.draw(window);
    target.draw(label);
    target.draw(shape);
    target.draw(value);
    target.draw(cursor);
}

