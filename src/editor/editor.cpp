#include "pch.h"
#include <variant>
#include "editor.h"


GraphEditor::GraphEditor(Graph* _graph, const sf::RenderWindow* _window, const sf::RenderTexture* texture)
    : graph(_graph),
    nodeval(sf::Vector2f(250, 200), "new value", Graph::get_font(), Graph::get_gui_font(), ""),
    rightclick(sf::Vector2f(600, 10), { "Create", "Rename", "Hide", "Delete", "Connect", "Disconnect" }, Graph::get_gui_font(), 200),
    window(_window), renderer(texture)
{
    highlighter.setFillColor(sf::Color(200, 200, 200, 100));
    highlighter.setOutlineThickness(1.5f);
}


bool GraphEditor::create_vertex(const sf::Vector2f& pos) {
    Vertex* s = graph->add_vertex(pos);
    for (auto& v : selected) {
        graph->connect(v, s);
        v->set_highlighted(false);
    }
    selected.clear();
    selected.push_back(s);
    s->set_highlighted(true);
    push_history(OpCreate{ s->id });
    return true;
}

bool GraphEditor::rename_vertex() {
    if (selected.size() == 1) {
        push_history(OpRename{ selected[0]->id, selected[0]->get_value() });
        nodeval.set_active(selected[0]->get_value());
        nodeval.set_position(sf::Vector2f(window->getSize()) * 0.5f - sf::Vector2f(300, 80) * 0.5f);
        return true;
    }
    return false;
}

bool GraphEditor::hide_vertices() {
    if (!selected.empty()) {
        auto op = OpHide{};
        for (const auto& v : selected) {
            graph->get_vertex_mut(v->id)->toggle_hidden();
            op.vertices.push_back(v->id);
        }
        push_history(op);
        return true;
    }
    return false;
}

bool GraphEditor::delete_vertices() {
    if (!selected.empty()) {
        auto op = OpDelete{};
        for (const auto& v : selected) {
            op.vertices.push_back(OpDeletedVertex{
                v->id,
                v->get_position(),
                v->get_value(),
                v->get_hidden(),
                v->edges,
                {}
            });
            op.vertices.back().incoming = graph->remove_vertex(v->id);
        }
        selected.clear();
        push_history(op);
        return true;
    }
    return false;
}

bool GraphEditor::join_vertices() {
    if (selected.size() == 2) {
        graph->connect(selected[0], selected[1]);
        push_history(OpJoin{ selected[0]->id, selected[1]->id });
        return true;
    }
    return false;
}

bool GraphEditor::split_vertices() {
    if (selected.size() == 2) {
        graph->disconnect(selected[0], selected[1]);
        push_history(OpSplit{ selected[0]->id, selected[1]->id });
        return true;
    }
    return false;
}


void GraphEditor::undo() {
    if (history.empty()) {
        return;
    }
    const auto op = history.back();
    if (auto create = std::get_if<OpCreate>(&op)) {
        graph->remove_vertex(create->vertex);
        selected.clear();
    } else if (auto rename = std::get_if<OpRename>(&op)) {
        graph->m_vertices[rename->vertex].set_value(rename->oldname);
    } else if (auto hide = std::get_if<OpHide>(&op)) {
        for (const auto& v : hide->vertices) {
            graph->m_vertices[v].toggle_hidden();
        }
    } else if (auto del = std::get_if<OpDelete>(&op)) {
        for (auto v = del->vertices.rbegin(); v != del->vertices.rend(); v++) {
            Vertex* s = graph->add_vertex_with_id(v->id, v->pos);
            s->set_value(v->label);
            s->set_hidden(v->hidden);
            for (auto& e : v->outgoing) {
                graph->connect(s->id, e);
            }
            for (auto& e : v->incoming) {
                graph->connect(e, s->id);
            }
        }
    } else if (auto join = std::get_if<OpJoin>(&op)) {
        graph->disconnect(join->A, join->B);
    } else if (auto split = std::get_if<OpSplit>(&op)) {
        graph->connect(split->A, split->B);
    } else if (auto move = std::get_if<OpMove>(&op)) {
        for (const auto& v : move->vertices) {
            graph->m_vertices[v].set_position(graph->m_vertices[v].get_position() - move->diff);
        }
    }
    history.pop_back();
}

void GraphEditor::push_history(const EditOperation& op) {
    history.push_back(op);
    if (history.size() > 32) {
        history.pop_front();
    }
}


void GraphEditor::handle_event(const sf::Event& event, const sf::View& graphview) {
    if (nodeval.is_focused()) {
        nodeval.handle_event(event);
        graph->get_vertex_mut(selected[0]->id)->set_value(nodeval.get_value());
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
        case sf::Keyboard::Z:
            undo();
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
                const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
                Vertex* s = graph->pick_vertex(mapped);
                if (s) {
                    const auto pos = std::find(selected.begin(), selected.end(), s);
                    if (pos == selected.end()) {
                        for (auto& v : selected) {
                            v->set_highlighted(false);
                        }
                        selected.clear();
                        selected.push_back(s);
                        s->set_highlighted(true);
                        selectidx = 0;
                    } else {
                        selectidx = pos - selected.begin();
                    }
                    clicked = true;
                    selectpos = selected[selectidx]->get_position();
                } else {
                    for (auto& v : selected) {
                        v->set_highlighted(false);
                    }
                    selected.clear();
                }
            }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            rightclick.set_position(sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y));
            rightclick.set_open(true);
        }
        if (!clicked) {
            held = true;
            const auto pos = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
            highlighter.setPosition(pos);
            highlighter.setSize(sf::Vector2f(0, 0));
        }
        break;
    case sf::Event::MouseButtonReleased:
        if (held) {
            held = false;
            highlighter.setFillColor(sf::Color::Transparent);
            highlighter.setOutlineColor(sf::Color::Transparent);
            const auto bounds = highlighter.getGlobalBounds();
            const auto map_tl = renderer->mapPixelToCoords(sf::Vector2i((int)bounds.left, (int)bounds.top), graphview);
            const auto map_br = renderer->mapPixelToCoords(sf::Vector2i((int)bounds.width, (int)bounds.height), graphview);
            const auto rect = sf::FloatRect(map_tl.x, map_tl.y, map_br.x, map_br.y);
            if (rect.width * rect.height > 4.f) {
                for (auto& v : selected) {
                    v->set_highlighted(false);
                }
                selected.clear();
                for (auto& [_, v] : graph->m_vertices) {
                    if (v.intersects(rect)) {
                        v.set_highlighted(true);
                        selected.push_back(&v);
                    }
                }
            }
        } else if (clicked) {
            const auto diff = selected[selectidx]->get_position() - selectpos;
            if (std::abs(diff.x) > 0.0001f || std::abs(diff.y) > 0.0001f) {
                auto op = OpMove{ std::vector<std::string>(), diff };
                for (const auto& s : selected) {
                    op.vertices.push_back(s->id);
                }
                push_history(op);
            }
            clicked = false;
        }
        break;
    case sf::Event::MouseMoved:
        if (clicked) {
            const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), graphview);
            const auto last = selected[selectidx]->get_position();
            selected[selectidx]->set_position(mapped);
            const auto diff = selected[selectidx]->get_position() - last;
            size_t i = 0;
            for (auto& v : selected) {
                if (i != selectidx) {
                    v->set_position(v->get_position() + diff);
                }
                i++;
            }
        } else if (held) {
            const auto pos = sf::Vector2f((float)event.mouseMove.x, (float)event.mouseMove.y);
            highlighter.setOutlineColor(sf::Color(0, 220, 255, 255));
            highlighter.setFillColor(sf::Color(200, 200, 200, 100));
            highlighter.setSize(pos - highlighter.getPosition());
        }
        break;
    default:
        break;
    }
}

void GraphEditor::draw(sf::RenderTarget& target) {
    target.draw(highlighter);
    if (nodeval.is_focused()) {
        nodeval.draw(target);
    }
    rightclick.draw(target);
}

