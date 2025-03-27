#include "pch.h"
#include "settings.h"
#include "util/files.h"
#include "util/img.h"


GraphSettings::GraphSettings(Graph* _graph, const sf::RenderWindow* _window): graph(_graph), window(_window),
    directed(sf::Vector2f(10, 10), "Directed", Graph::get_gui_font(), graph->is_directed()),
    fontsize(sf::Vector2f(250, 10), "Fontsize", Graph::get_gui_font(), ((float)graph->get_fontsize()-1.f) / 39.f),
    loadsave(sf::Vector2f(600, 10), { "File", "Save", "Save As", "Open", "Export" }, Graph::get_gui_font(), 150)
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
    if (loadsave.handle_event(event)) {
        switch (loadsave.get_index()) {
            case 0: case 1: {
                auto f = saveFileName(window->getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                auto stem = f.path.replace_extension().generic_string();
                if (!stem.empty()) {
                    graph->save_to_file(stem + ".graph");
                }
                break; }
            case 2:{
                // TODO
                break; }
            case 3: {
                auto f = saveFileName(window->getSystemHandle(), "SVG Image (*.svg)\0*.svg\0PNG Image (*.png)\0*.png\0");
                auto stem = f.path.replace_extension().generic_string();
                if (!stem.empty()) {
                    if (f.type == 1) {
                        graph->export_svg(stem + ".svg");
                    } else if (f.type == 2) {
                        auto texture = sf::RenderTexture();
                        texture.create(1920, 1080);
                        auto temp = texture.getDefaultView();
                        temp.zoom(0.8f);
                        texture.setView(temp);
                        texture.clear(sf::Color::White);
                        graph->draw(texture, true);
                        texture.display();
                        export_image(stem + ".png", texture.getTexture(), 20);
                    }
                }
                break; }
        }
    }

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
    loadsave.draw(target);
}

