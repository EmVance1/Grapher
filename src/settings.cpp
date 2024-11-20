#include "settings.h"


GraphSettings::GraphSettings(Graph* _graph): graph(_graph),
    directed(sf::Vector2f(10, 10), "Directed", Graph::get_font(), graph->is_directed()),
    lineweight(sf::Vector2f(160, 10), "Lineweight", Graph::get_font(), 0.2f),
    fontsize(sf::Vector2f(500, 10), "Fontsize", Graph::get_font(), 0.5f)
{
    toolbar.setPosition(sf::Vector2f(0, 0));
    toolbar.setSize(sf::Vector2f(800, 50));
    toolbar.setFillColor(sf::Color(100, 150, 200));
    toolbar.setOutlineThickness(1);
    toolbar.setOutlineColor(sf::Color::Black);
}

void GraphSettings::handle_event(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Up && event.key.control) {
            lineweight.increment();
        }
        if (event.key.code == sf::Keyboard::Down && event.key.control) {
            lineweight.decrement();
        }
        break;
    default:
        break;
    }

    const bool d_change = directed.handle_event(event);
    const bool l_change = lineweight.handle_event(event);
    const bool f_change = fontsize.handle_event(event);

    graph->s_lineweight = lineweight.get_value() * 9.f + 1.f;
    graph->m_directed = directed.get_value();

    for (auto& [_, v] : graph->m_vertices) {
        for (auto& e : v.edges) {
            if (d_change) {
                e.refresh_edge(graph);
            }
            if (l_change) {
                e.refresh_edge(graph);
            }
        }
        if (f_change) {
            v.content.setCharacterSize((int)((fontsize.get_value() * 20.f) + 20.f));
            const auto pos = v.get_position();
            const auto bounds = v.content.getGlobalBounds();
            v.content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - bounds.height));
        }
        if (l_change) {
            v.shape.setOutlineThickness(lineweight.get_value() * 9.f + 1.f);
        }
    }
}

void GraphSettings::draw(sf::RenderTarget& target) {
    target.draw(toolbar);
    directed.draw(target);
    lineweight.draw(target);
    fontsize.draw(target);
}

