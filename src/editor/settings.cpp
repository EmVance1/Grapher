#include "pch.h"
#include "settings.h"
#include "util/files.h"
#include "util/img.h"


GraphSettings::GraphSettings(Graph* _graph, const sf::RenderWindow* _window): graph(_graph), window(_window),
    loadsave(sf::Vector2f(10, 10), { "File", "New", "Open", "Save", "Save As", "Export" }, Graph::get_gui_font(), 150),
    directed(sf::Vector2f(200, 10), "Directed", Graph::get_gui_font(), graph->is_directed()),
    fontsize(sf::Vector2f(380, 10), "Fontsize", Graph::get_gui_font(), ((float)graph->get_fontsize()-1.f) / 39.f)
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
            case 0: {
                if (graph->is_changed()) {
                    // do save
                }
                if (!graph->is_changed()) {
                    current_file = "";
                    graph->reset();
                    directed.set_value(false);
                    graph->changed = false;
                }
                break; }
            case 1: {
                if (graph->is_changed()) {
                    // do save
                }
                if (!graph->is_changed()) {
                    auto f = openFileName(window->getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                    current_file = f.path.generic_string();
                    graph->load_from_file(current_file);
                    directed.set_value(graph->is_directed());
                    graph->changed = false;
                }
                break; }
            case 2: {
                if (current_file == "") {
                    auto f = saveFileName(window->getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                    auto stem = f.path.replace_extension().generic_string();
                    if (!stem.empty()) {
                        current_file = stem + ".graph";
                    }
                }
                graph->save_to_file(current_file);
                graph->changed = false;
                break; }
            case 3: {
                auto f = saveFileName(window->getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                auto stem = f.path.replace_extension().generic_string();
                if (!stem.empty()) {
                    current_file = stem + ".graph";
                    graph->save_to_file(current_file);
                }
                graph->changed = false;
                break; }
            case 4: {
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

    if (fontsize.handle_event(event)) {
        Graph::s_fontsize = (int)(fontsize.get_value() * 20.f + 20.f);
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

