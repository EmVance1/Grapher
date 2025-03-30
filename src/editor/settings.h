#pragma once
#include <SFML/Graphics.hpp>
#include <variant>

#include "graph/graph.h"

#include "gui/toggle.h"
#include "gui/slider.h"
#include "gui/dropdown.h"


class GraphSettings {
private:
    sf::RectangleShape toolbar;
    Graph* graph;
    const sf::RenderWindow* window;

    Dropdown loadsave;
    ToggleButton directed;
    Slider fontsize;

public:
    std::string current_file = "";

public:
    GraphSettings(Graph* _graph, const sf::RenderWindow* window);

    void handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target);
};

