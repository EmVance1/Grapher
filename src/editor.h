#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "graph.h"

#include "gui/editbox.h"


class GraphEditor {
private:
    sf::CircleShape highlight;
    bool clicked = false;
    std::vector<Vertex*> selected;
    Graph* graph;

    EditBox nodeval;

public:
    GraphEditor(Graph* _graph);

    void handle_event(const sf::Event& event);
    void refresh_graph(const sf::Window& window);
    void draw(sf::RenderTarget& target);
};

