#include "pch.h"
#include "graph/graph.h"
#include "editor/editor.h"
#include "editor/settings.h"
#include "util/files.h"
#include "util/grid.h"
#include "util/img.h"

#include "log/log.h"


int main(int argc, char** argv) {
    // FILE* _junk = nullptr;
    // freopen_s(&_junk, "res/info_log.txt", "w", stdout);
    // freopen_s(&_junk, "res/err_log.txt", "w", stderr);

    sf::ContextSettings ctx;
    ctx.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Vertex", sf::Style::Default, ctx);
    window.setFramerateLimit(60);
    sf::Event event;
    sf::RenderTexture texture;
    texture.create(1920, 1080, ctx);
    texture.setSmooth(true);

    auto grid = get_grid(sf::Vector2f(0.f, 0.f), sf::Vector2f(1600.f, 1000.f));

    sf::Font _font1;
    sf::Font _font2;
    Graph::CMU_SERIF = &_font1;
    Graph::GUI_FONT = &_font2;
    Graph::init_fonts();
    Graph graph = Graph(false);
    if (argc == 2) {
        graph.load_from_file(argv[1]);
        log_info("loaded graph file: \"" << argv[1] << "\"");
    } else {
#ifdef DEBUG
        graph.load_from_file("res/test.graph");
        log_info("loaded graph file: \"${app}/res/test.graph\"");
#else
        graph.load_from_file("res/empty.graph");
#endif
    }
    GraphEditor editor = GraphEditor(&graph, &texture);
    GraphSettings settings = GraphSettings(&graph, &window);
    sf::View graphview = texture.getDefaultView();
    const sf::View guiview = texture.getDefaultView();
    sf::View windowview = window.getDefaultView();

    texture.clear(sf::Color::White);
    texture.setView(graphview);
    texture.draw(grid);
    graph.draw(texture);
    editor.draw(texture);
    texture.setView(guiview);
    settings.draw(texture);
    texture.display();

    window.draw(sf::Sprite(texture.getTexture()));
    window.display();

    while (window.waitEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                return 69;
            case sf::Event::Resized:
                windowview.setSize((float)event.size.width, (float)event.size.height);
                windowview.setCenter((float)event.size.width * 0.5f, (float)event.size.height * 0.5f);
                grid = get_grid(graphview.getCenter() - graphview.getSize() * 0.5f, graphview.getCenter() + graphview.getSize() * 0.5f);
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::S && event.key.control) {
                    auto f = saveFileName(window.getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                    auto stem = f.path.replace_extension().generic_string();
                    if (!stem.empty()) {
                        graph.save_to_file(stem + ".graph");
                    }
                }
                if (event.key.code == sf::Keyboard::E && event.key.control) {
                    auto f = saveFileName(window.getSystemHandle(), "SVG Image (*.svg)\0*.svg\0PNG Image (*.png)\0*.png\0");
                    auto stem = f.path.replace_extension().generic_string();
                    if (!stem.empty()) {
                        if (f.type == 1) {
                            graph.export_svg(stem + ".svg");
                        } else if (f.type == 2) {
                            auto temp = texture.getDefaultView();
                            temp.zoom(0.8f);
                            texture.setView(temp);
                            texture.clear(sf::Color::White);
                            graph.draw(texture, true);
                            texture.display();
                            export_image(stem + ".png", texture.getTexture(), 20);
                        }
                    }
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.delta > 0) {
                    auto p = sf::Vector2f((float)event.mouseWheelScroll.x, (float)event.mouseWheelScroll.y);
                    auto begin = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                    graphview.zoom(0.9f);
                    auto end = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                    graphview.move(begin - end);
                    grid = get_grid(graphview.getCenter() - graphview.getSize() * 0.5f, graphview.getCenter() + graphview.getSize() * 0.5f);
                } else {
                    auto p = sf::Vector2f((float)event.mouseWheelScroll.x, (float)event.mouseWheelScroll.y);
                    auto begin = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                    graphview.zoom(1.1f);
                    auto end = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                    graphview.move(begin - end);
                    grid = get_grid(graphview.getCenter() - graphview.getSize() * 0.5f, graphview.getCenter() + graphview.getSize() * 0.5f);
                }
                break;
            default:
                break;
        }

        editor.handle_event(event, graphview);
        settings.handle_event(event);

        texture.clear(sf::Color::White);
        texture.setView(graphview);
        texture.draw(grid);
        graph.draw(texture);
        texture.setView(guiview);
        editor.draw(texture);
        settings.draw(texture);
        texture.display();

        window.setView(windowview);
        window.draw(sf::Sprite(texture.getTexture()));
        window.display();
    }
}

