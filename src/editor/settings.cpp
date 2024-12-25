#include "pch.h"
#include "settings.h"


GraphSettings::GraphSettings(Graph* _graph): graph(_graph),
    directed(sf::Vector2f(10, 10), "Directed", Graph::get_font(), graph->is_directed()),
    fontsize(sf::Vector2f(300, 10), "Fontsize", Graph::get_font(), ((float)graph->get_fontsize()-1.f) / 39.f)
{
    toolbar.setPosition(sf::Vector2f(0, 0));
    toolbar.setSize(sf::Vector2f(2000, 50));
    toolbar.setFillColor(sf::Color(100, 150, 200));
    toolbar.setOutlineThickness(1);
    toolbar.setOutlineColor(sf::Color::Black);

    for (auto& [_, v] : graph->m_vertices) {
        for (auto& e : v.edges) {
            e.refresh_edge(graph);
        }
        v.content.setCharacterSize((int)((fontsize.get_value() * 20.f) + 20.f));
        const auto pos = v.get_position();
        const auto bounds = v.content.getGlobalBounds();
        v.content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - bounds.height));
    }
}

void GraphSettings::handle_event(const sf::Event& event) {
    const bool d_change = directed.handle_event(event);
    const bool f_change = fontsize.handle_event(event);

    graph->m_directed = directed.get_value();
    Graph::s_fontsize = (uint32_t)(fontsize.get_value() * 39.f + 1.f);

    for (auto& [_, v] : graph->m_vertices) {
        for (auto& e : v.edges) {
            if (d_change) {
                e.refresh_edge(graph);
            }
        }
        if (f_change) {
            v.content.setCharacterSize((int)((fontsize.get_value() * 20.f) + 20.f));
            const auto pos = v.get_position();
            const auto bounds = v.content.getGlobalBounds();
            v.content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - bounds.height));
        }
    }
}

void GraphSettings::draw(sf::RenderTarget& target) {
    target.draw(toolbar);
    directed.draw(target);
    fontsize.draw(target);
}

