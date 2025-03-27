#include "pch.h"
#include "editor.h"


GraphEditor::GraphEditor(Graph* _graph, const sf::RenderTexture* texture)
    : graph(_graph),
    nodeval(sf::Vector2f(250, 200), "new value", Graph::get_gui_font(), ""),
    rightclick(sf::Vector2f(600, 10), { "Create", "Rename", "Hide", "Delete", "Connect", "Disconnect" }, Graph::get_gui_font(), 150),
    renderer(texture)
{}


bool GraphEditor::create_vertex(const sf::Vector2f& pos) {
    Vertex* s = graph->add_vertex(pos);
    for (auto& v : selected) {
        graph->connect(v, s);
        v->set_highlighted(false);
    }
    selected.clear();
    selected.push_back(s);
    s->set_highlighted(true);
    return true;
}

bool GraphEditor::rename_vertex() {
    if (selected.size() == 1) {
        nodeval.set_active(selected[0]->get_value());
        return true;
    }
    return false;
}

bool GraphEditor::hide_vertices() {
    if (!selected.empty()) {
        for (const auto& v : selected) {
            graph->get_vertex_mut(v->id)->toggle_hidden();
        }
        return true;
    }
    return false;
}

bool GraphEditor::delete_vertices() {
    if (!selected.empty()) {
        for (const auto& v : selected) {
            graph->remove_vertex(v->id);
        }
        selected.clear();
        return true;
    }
    return false;
}

bool GraphEditor::join_vertices() {
    if (selected.size() == 2) {
        graph->connect(selected[0], selected[1]);
        return true;
    }
    return false;
}

bool GraphEditor::split_vertices() {
    if (selected.size() == 2) {
        graph->disconnect(selected[0], selected[1]);
        return true;
    }
    return false;
}


void GraphEditor::handle_event(const sf::Event& event, const sf::View& graphview) {
    if (nodeval.is_focused()) {
        nodeval.handle_event(event);
        if (!nodeval.is_focused()) {
            graph->get_vertex_mut(selected[0]->id)->set_value(nodeval.get_value());
        }
        return;
    }
    if (rightclick.handle_event(event)) {
        switch (rightclick.get_index()) {
            case 0: {
                const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
                create_vertex(mapped);
                break; }
            case 1:
                rename_vertex();
                break;
            case 2:
                hide_vertices();
                break;
            case 3:
                delete_vertices();
                break;
            case 4:
                join_vertices();
                break;
            case 5:
                split_vertices();
                break;
            default:
                break;
        }
    }
    if (rightclick.is_open()) {
        return;
    }

    switch (event.type) {
    case sf::Event::KeyPressed:
        switch (event.key.code) {
        case sf::Keyboard::J:
            join_vertices();
            break;
        case sf::Keyboard::X:
            if (event.key.control) {
                delete_vertices();
            } else {
                split_vertices();
            }
            break;
        case sf::Keyboard::I:
            if (event.key.control) {
                rename_vertex();
            }
            break;
        case sf::Keyboard::H:
            hide_vertices();
            break;
        default:
            break;
        }
        break;
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
                create_vertex(mapped);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
                Vertex* s = graph->pick_vertex(mapped);
                if (s && std::find(selected.begin(), selected.end(), s) == selected.end()) {
                    selected.push_back(s);
                    s->set_highlighted(true);
                }
            } else {
                for (auto& v : selected) {
                    v->set_highlighted(false);
                }
                selected.clear();
                const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
                Vertex* s = graph->pick_vertex(mapped);
                if (s) {
                    selected.push_back(s);
                    s->set_highlighted(true);
                }
                clicked = true;
            }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            rightclick.set_position(sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y));
            rightclick.set_open(true);
        }
        break;
    case sf::Event::MouseButtonReleased:
        clicked = false;
        break;
    case sf::Event::MouseMoved:
        if (selected.size() == 1) {
            if (clicked) {
                const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), graphview);
                selected[0]->set_position(mapped);
            }
        }
        break;
    default:
        break;
    }
}

void GraphEditor::draw(sf::RenderTarget& target) {
    if (nodeval.is_focused()) {
        nodeval.draw(target);
    }
    rightclick.draw(target);
}

