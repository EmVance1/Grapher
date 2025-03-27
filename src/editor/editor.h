#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "graph/graph.h"

#include "gui/editbox.h"
#include "gui/list.h"


class GraphEditor {
private:
    bool clicked = false;
    std::vector<Vertex*> selected;
    Graph* graph;

    sf::RectangleShape highlighter;
    bool held = false;

    EditBox nodeval;
    List rightclick;
    const sf::RenderTexture* renderer;

private:
    bool create_vertex(const sf::Vector2f& pos);
    bool rename_vertex();
    bool hide_vertices();
    bool delete_vertices();
    bool join_vertices();
    bool split_vertices();

public:
    GraphEditor(Graph* _graph, const sf::RenderTexture* texture);

    void handle_event(const sf::Event& event, const sf::View& graphview);
    void draw(sf::RenderTarget& target);
};

