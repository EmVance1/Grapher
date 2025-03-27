#include "pch.h"
#include "list.h"


ListItem::ListItem(const sf::Vector2f& pos, const std::string& _value, const sf::Font& font, float width) {
    shape.setPosition(pos);
    shape.setSize(sf::Vector2f(width, 30));
    // shape.setOutlineThickness(1);
    // shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(sf::Color(140, 200, 240));

    label.setPosition(pos + sf::Vector2f(10, 0));
    label.setCharacterSize(25);
    label.setFillColor(sf::Color::Black);
    label.setFont(font);
    label.setString(_value);

    bounds.left = pos.x;
    bounds.top = pos.y;
    bounds.width = width;
    bounds.height = 30;

    value = _value;
}


bool ListItem::handle_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::MouseButtonReleased:
            return hovered;

        case sf::Event::MouseMoved: {
            const auto mouse = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
            hovered = bounds.contains(mouse);
            if (hovered) {
                shape.setFillColor(sf::Color(170, 220, 255));
            } else {
                shape.setFillColor(sf::Color(140, 200, 240));
            }
            break; }

        default:
            break;
    }

    return false;
}

void ListItem::draw(sf::RenderTarget& target) const {
    target.draw(shape);
    target.draw(label);
}


List::List(const sf::Vector2f& pos, const std::vector<std::string>& values, const sf::Font& font, float width, bool dynwidth) {
    float h = 0.f;
    for (const auto& v : values) {
        options.push_back(ListItem(pos + sf::Vector2f(0, h), v, font, width));
        h += 30.f;
    }
}


void List::set_position(const sf::Vector2f& pos) {
    float h = 0.f;
    for (auto& op : options) {
        op.shape.setPosition(sf::Vector2f(pos.x, pos.y + h));
        op.label.setPosition(sf::Vector2f(pos.x + 10, pos.y + h));
        op.bounds.left = pos.x;
        op.bounds.top = pos.y + h;
        h += 30.f;
    }
}

bool List::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
        open = true;
        return false;
    }

    if (open) {
        size_t i = 0;
        for (auto& op : options) {
            if (op.handle_event(event)) {
                selected = i;
                open = false;
                return true;
            }
            i++;
        }
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            open = false;
        }
    }

    return false;
}

void List::draw(sf::RenderTarget& target) const {
    if (open) {
        for (const auto& op : options) {
            op.draw(target);
        }
    }
}

