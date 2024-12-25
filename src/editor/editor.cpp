#include "pch.h"
#include "editor.h"


GraphEditor::GraphEditor(Graph* _graph, const sf::RenderTexture* texture)
    : graph(_graph),
    nodeval(sf::Vector2f(250, 200), "new value", Graph::get_font(), ""),
    renderer(texture)
{
    highlight.setRadius(40);
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineColor(sf::Color::Cyan);
    highlight.setOutlineThickness(3);
}

void GraphEditor::handle_event(const sf::Event& event, const sf::View& graphview) {
    if (nodeval.is_focused()) {
        nodeval.handle_event(event);
        if (!nodeval.is_focused()) {
            graph->get_vertex_mut(selected[0]->id)->set_value(nodeval.get_value());
        }
        return;
    }

    switch (event.type) {
    case sf::Event::KeyPressed:
        switch (event.key.code) {
        case sf::Keyboard::J:
            if (selected.size() == 2) {
                graph->connect(selected[0], selected[1]);
            }
            break;
        case sf::Keyboard::X:
            if (selected.size() == 2) {
                graph->disconnect(selected[0], selected[1]);
            } else if (selected.size() == 1 && event.key.control) {
                graph->remove_vertex(selected[0]->id);
                selected.clear();
            }
            break;
        case sf::Keyboard::I:
            if (selected.size() == 1 && event.key.control) {
                nodeval.set_active(selected[0]->get_value());
            }
        default:
            break;
        }
        break;
    case sf::Event::MouseButtonPressed:
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
            Vertex* s = graph->add_vertex(mapped);
            for (auto& v : selected) {
                graph->connect(v, s);
            }
            selected.clear();
            selected.push_back(s);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
            Vertex* s = graph->pick_vertex(mapped);
            if (s && std::find(selected.begin(), selected.end(), s) == selected.end()) {
                selected.push_back(s);
            }
        } else {
            selected.clear();
            const auto mapped = renderer->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), graphview);
            Vertex* s = graph->pick_vertex(mapped);
            if (s) {
                selected.push_back(s);
            }
            clicked = true;
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
                for (auto& [_, v] : graph->m_vertices) {
                    for (auto& e : v.edges) {
                        if (e.to == selected[0]->id || e.from == selected[0]->id) {
                            e.refresh_edge(graph);
                        }
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}

void GraphEditor::draw(sf::RenderTarget& target) {
    for (const auto& s : selected) {
        const auto pos = s->get_position();
        highlight.setPosition(sf::Vector2f(pos.x - 40, pos.y - 40));
        target.draw(highlight);
    }
    if (nodeval.is_focused()) {
        nodeval.draw(target);
    }
}

