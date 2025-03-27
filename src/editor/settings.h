#pragma once
#include <SFML/Graphics.hpp>
#include "graph/graph.h"

#include "gui/toggle.h"
#include "gui/slider.h"
#include "gui/dropdown.h"


class GraphSettings {
private:
    sf::RectangleShape toolbar;
    Graph* graph;
    const sf::RenderWindow* window;

    ToggleButton directed;
    Slider fontsize;
    Dropdown loadsave;


public:
    GraphSettings(Graph* _graph, const sf::RenderWindow* window);

    void handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target);
};

