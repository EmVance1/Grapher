#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <variant>
#include <vector>
#include <list>
#include "graph/graph.h"

#include "gui/editbox.h"
#include "gui/list.h"


struct OpCreate {
    std::string vertex;
};

struct OpRename {
    std::string vertex;
    std::string oldname;
};

struct OpHide {
    std::vector<std::string> vertices;
};

struct OpDeletedVertex {
    std::string id;
    sf::Vector2f pos;
    std::string label;
    bool hidden;
    std::unordered_set<std::string> outgoing;
    std::unordered_set<std::string> incoming;
};
struct OpDelete {
    std::vector<OpDeletedVertex> vertices;
};

struct OpJoin {
    std::string A;
    std::string B;
};

struct OpSplit {
    std::string A;
    std::string B;
};

struct OpMove {
    std::vector<std::string> vertices;
    sf::Vector2f diff;
};


using EditOperation = std::variant<OpCreate, OpRename, OpHide, OpDelete, OpJoin, OpSplit, OpMove>;

class GraphEditor {
private:
    Graph* graph;
    std::vector<Vertex*> selected;
    std::list<EditOperation> history;

    bool clicked = false;
    bool held = false;
    sf::Vector2f selectpos;
    size_t selectidx = 0;

    sf::RectangleShape highlighter;
    EditBox nodeval;
    List rightclick;

    const sf::RenderWindow* window;
    const sf::RenderTexture* renderer;

private:
    bool create_vertex(const sf::Vector2f& pos);
    bool rename_vertex();
    bool hide_vertices();
    bool delete_vertices();
    bool join_vertices();
    bool split_vertices();

    void undo();
    void push_history(const EditOperation& op);

public:
    GraphEditor(Graph* _graph, const sf::RenderWindow* window, const sf::RenderTexture* texture);

    void handle_event(const sf::Event& event, const sf::View& graphview);
    void draw(sf::RenderTarget& target);
};

