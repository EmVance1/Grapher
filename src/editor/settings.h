#pragma once
#include <SFML/Graphics.hpp>
#include "graph/graph.h"

#include "gui/toggle.h"
#include "gui/slider.h"


class GraphSettings {
private:
    sf::RectangleShape toolbar;
    Graph* graph;

    ToggleButton directed;
    Slider fontsize;

public:
    GraphSettings(Graph* _graph);

    void handle_event(const sf::Event& event);
    void draw(sf::RenderTarget& target);
};

