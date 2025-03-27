#include "pch.h"
#include "dropdown.h"


Dropdown::Dropdown(const sf::Vector2f& pos, const std::vector<std::string>& values, const sf::Font& font, float width, bool dynwidth)
    : top(pos, values[0], font, width)
{
    top.shape.setOutlineThickness(1);
    top.shape.setOutlineColor(sf::Color::Black);
    float h = 30.f;
    for (size_t i = 1; i < values.size(); i++) {
        options.push_back(ListItem(pos + sf::Vector2f(0, h), values[i], font, width));
        h += 30.f;
    }
}


bool Dropdown::handle_event(const sf::Event& event) {
    if (top.handle_event(event)) {
        open = !open;
    } else if (open) {
        for (size_t i = 0; i < options.size(); i++) {
            if (options[i].handle_event(event)) {
                selected = i;
                open = false;
                return true;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            open = false;
        }
    }

    return false;
}

void Dropdown::draw(sf::RenderTarget& target) const {
    if (open) {
        for (const auto& op : options) {
            op.draw(target);
        }
    }
    top.draw(target);
}



DropdownSelect::DropdownSelect(const sf::Vector2f& pos, const std::vector<std::string>& values, const sf::Font& font, float width, bool dynwidth)
    : active(pos, values[0], font, width)
{
    active.shape.setOutlineThickness(1);
    active.shape.setOutlineColor(sf::Color::Black);
    float h = 30.f;
    for (const auto& v : values) {
        options.push_back(ListItem(pos + sf::Vector2f(0, h), v, font, width));
        h += 30.f;
    }
}


bool DropdownSelect::handle_event(const sf::Event& event) {
    if (active.handle_event(event)) {
        open = !open;
    } else if (open) {
        for (size_t i = 0; i < options.size(); i++) {
            if (options[i].handle_event(event)) {
                selected = i;
                active.value = options[i].value;
                active.label.setString(options[i].value);
                open = false;
                return true;
            }
        }
    }

    return false;
}

void DropdownSelect::draw(sf::RenderTarget& target) const {
    if (open) {
        for (const auto& op : options) {
            op.draw(target);
        }
    }
    active.draw(target);
}

