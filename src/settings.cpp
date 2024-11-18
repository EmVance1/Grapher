#include "settings.h"


GraphSettings::GraphSettings(Graph* _graph): graph(_graph),
    directed(sf::Vector2f(10, 10), "Directed", Graph::get_font()),
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

    directed.handle_event(event);
    lineweight.handle_event(event);
    fontsize.handle_event(event);
}

void GraphSettings::refresh_graph(const sf::Window& window) {
    for (auto& e : graph->edges) {
        e.RADIUS = lineweight.get_value() * 9.f + 1.f;
        e.update(graph);
    }
    for (auto& [_, v] : graph->vertices) {
        v.shape.setOutlineThickness(lineweight.get_value() * 9.f + 1.f);
        v.content.setCharacterSize((int)((fontsize.get_value() * 20.f) + 20.f));
        const auto pos = v.get_position();
        const auto bounds = v.content.getGlobalBounds();
        v.content.setPosition(sf::Vector2f(pos.x - bounds.width / 2, pos.y - bounds.height));
    }
}

void GraphSettings::draw(sf::RenderTarget& target) {
    target.draw(toolbar);
    directed.draw(target);
    lineweight.draw(target);
    fontsize.draw(target);
}

