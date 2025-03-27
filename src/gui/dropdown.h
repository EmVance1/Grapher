#pragma once
#include "list.h"


class Dropdown {
private:
    ListItem top;
    std::vector<ListItem> options;
    size_t selected = 0;
    bool open = false;

public:
    Dropdown(const sf::Vector2f& pos, const std::vector<std::string>& values, const sf::Font& font, float width, bool dynwidth = false);

    const std::string& get_value() const { return options[selected].value; }
    size_t get_index() const { return selected; }

    bool handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};


class DropdownSelect {
private:
    ListItem active;
    std::vector<ListItem> options;
    size_t selected = 0;
    bool open = false;

public:
    DropdownSelect(const sf::Vector2f& pos, const std::vector<std::string>& values, const sf::Font& font, float width, bool dynwidth = false);

    const std::string& get_value() const { return options[selected].value; }
    size_t get_index() const { return selected; }

    bool handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target) const;
};

