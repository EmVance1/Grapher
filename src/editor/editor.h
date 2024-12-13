#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "graph/graph.h"

#include "gui/editbox.h"


class GraphEditor {
private:
    sf::CircleShape highlight;
    bool clicked = false;
    std::vector<Vertex*> selected;
    Graph* graph;

    EditBox nodeval;
    const sf::RenderTexture* renderer;

public:
    GraphEditor(Graph* _graph, const sf::RenderTexture* texture);

    void handle_event(const sf::Event& event, const sf::View& graphview);
    void draw(sf::RenderTarget& target);
};

