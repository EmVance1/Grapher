#include "pch.h"
#include "editbox.h"


EditBox::EditBox(const sf::Vector2f& pos, const std::string& _value, const sf::Font& font, const sf::Font& guifont, std::string init) {
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
    label.setCharacterSize(25);
    label.setFillColor(sf::Color::Black);
    label.setFont(guifont);
    label.setString(_value);

    value.setPosition(pos + sf::Vector2f(30, 40));
    value.setCharacterSize(25);
    value.setFillColor(sf::Color::Black);
    value.setFont(font);
    value.setString(init);

    bounds.left = pos.x + 25;
    bounds.top = pos.y + 40;
    bounds.width = 250;
    bounds.height = 30;

    cancel = init;
}

void EditBox::set_position(const sf::Vector2f& pos) {
    window.setPosition(pos);
    shape.setPosition(pos + sf::Vector2f(25, 40));
    label.setPosition(pos + sf::Vector2f(30, 0));
    value.setPosition(pos + sf::Vector2f(30, 40));
    cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().width, 0));
    bounds.left = pos.x + 25;
}

void EditBox::set_active(const std::string& val) {
    focus = true;
    value.setString(val);
    cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().width, 0));
    cancel = val;
}

bool EditBox::handle_event(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
        switch (event.key.code) {
            case sf::Keyboard::Return:
                focus = false;
                break;
            case sf::Keyboard::BackSpace:
                if (!value.getString().isEmpty()) {
                    value.setString(value.getString().substring(0, value.getString().getSize() - 1));
                    cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().width, 0));
                    // pos--;
                }
                break;
            case sf::Keyboard::Escape:
                value.setString(cancel);
                focus = false;
                break;
            case sf::Keyboard::Left:
                // if (pos > 0) {
                //     pos--;
                // }
                break;
            case sf::Keyboard::Right:
                // if (pos < value.getString().getSize()) {
                //     pos++;
                // }
                break;
            default:
                break;
        }
        return true;
    case sf::Event::TextEntered:
        if (event.text.unicode >= 32) {
            auto temp = value.getString() + event.text.unicode;
            value.setString(temp);
            cursor.setPosition(value.getPosition() + sf::Vector2f(value.getGlobalBounds().width, 0));
            // pos++;
        }
        return true;
    default:
        return false;
    }
}

void EditBox::draw(sf::RenderTarget& target) const {
    target.draw(window);
    target.draw(label);
    target.draw(shape);
    target.draw(value);
    target.draw(cursor);
}


