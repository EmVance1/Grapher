#include "pch.h"
#include "settings.h"


GraphSettings::GraphSettings(Graph* _graph): graph(_graph),
    directed(sf::Vector2f(10, 10), "Directed", Graph::get_gui_font(), graph->is_directed()),
    fontsize(sf::Vector2f(250, 10), "Fontsize", Graph::get_gui_font(), ((float)graph->get_fontsize()-1.f) / 39.f)
{
    toolbar.setPosition(sf::Vector2f(0, 0));
    toolbar.setSize(sf::Vector2f(2000, 50));
    toolbar.setFillColor(sf::Color(100, 150, 200));
    toolbar.setOutlineThickness(1);
    toolbar.setOutlineColor(sf::Color::Black);

    for (auto& [_, v] : graph->m_vertices) {
        v.set_content_size((int)((fontsize.get_value() * 20.f) + 20.f));
    }
}

void GraphSettings::handle_event(const sf::Event& event) {
    directed.handle_event(event);

    graph->m_directed = directed.get_value();
    Graph::s_fontsize = (int)(fontsize.get_value() * 20.f + 20.f);

    if (fontsize.handle_event(event)) {
        for (auto& [_, v] : graph->m_vertices) {
            v.set_content_size(Graph::s_fontsize);
        }
    }
}

void GraphSettings::draw(sf::RenderTarget& target) {
    target.draw(toolbar);
    directed.draw(target);
    fontsize.draw(target);
}

