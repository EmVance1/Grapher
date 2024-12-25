#include "pch.h"
#include "graph/graph.h"
#include "editor/editor.h"
#include "editor/settings.h"
#include "files.h"
#include "img.h"


float down_to_nearest(float x, int m) {
    return x - (float)((int)x % m);
}


sf::VertexArray get_grid(const sf::Vector2f& begin, const sf::Vector2f& end) {
    sf::VertexArray va = sf::VertexArray(sf::PrimitiveType::Lines);
    float diff = 50.f;
    if (end.x - begin.x > 2000.f) {
        diff = 100.f;
    }
    if (end.x - begin.x > 4000.f) {
        diff = 200.f;
    }

    // horizontal
    for (float y = down_to_nearest(begin.y, (int)diff) + 25.f; y < end.y; y += diff) {
        va.append({ sf::Vector2f(begin.x, y), sf::Color(150, 150, 150) });
        va.append({ sf::Vector2f(end.x,   y), sf::Color(150, 150, 150) });
    }
    // vertical
    for (float x = down_to_nearest(begin.x, (int)diff) + 25.f; x < end.x; x += diff) {
        va.append({ sf::Vector2f(x, begin.y), sf::Color(150, 150, 150) });
        va.append({ sf::Vector2f(x, end.y),   sf::Color(150, 150, 150) });
    }

    return va;
}

sf::VertexArray get_hex() {
    sf::VertexArray va = sf::VertexArray(sf::PrimitiveType::Lines);

    for (int i = -400; i < 1200; i += 50) {
        va.append({ sf::Vector2f(0, (float)i),         sf::Color(150, 150, 150) });
        va.append({ sf::Vector2f(800, (float)i + 400), sf::Color(150, 150, 150) });
    }
    for (int i = -400; i < 1200; i += 50) {
        va.append({ sf::Vector2f(0, (float)i + 400), sf::Color(150, 150, 150) });
        va.append({ sf::Vector2f(800, (float)i),     sf::Color(150, 150, 150) });
    }
    for (int i = 0; i < 800; i += 50) {
        va.append({ sf::Vector2f((float)i, 0),   sf::Color(150, 150, 150) });
        va.append({ sf::Vector2f((float)i, 800), sf::Color(150, 150, 150) });
    }

    return va;
}


int main(int argc, char** argv) {
    sf::ContextSettings ctx;
    ctx.antiAliasingLevel = 16;
    auto window = sf::RenderWindow(sf::VideoMode({1280, 720}), "Graphs", sf::Style::Default, sf::State::Windowed, ctx);
    window.setFramerateLimit(60);
    auto texture = sf::RenderTexture();
    std::ignore = texture.resize({1920, 1280}, ctx);
    texture.setSmooth(true);

    auto grid = get_grid(sf::Vector2f(0.f, 0.f), sf::Vector2f(1280.f, 720.f));

    sf::Font _font;
    Graph::CMU_SERIF = &_font;
    Graph::init_font();
    Graph graph = Graph(false);
    if (argc == 2) {
        graph.load_from_file(argv[1]);
    } else {
        graph.load_from_file("res/test.graph");
    }
    GraphEditor editor = GraphEditor(&graph, &texture);
    GraphSettings settings = GraphSettings(&graph);
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

    while (const auto event = window.waitEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return 69;

        } else if (const auto resize = event->getIf<sf::Event::Resized>()) {
            windowview.setSize({(float)resize->size.x, (float)resize->size.y});
            windowview.setCenter({(float)resize->size.x* 0.5f, (float)resize->size.y * 0.5f});
            grid = get_grid(graphview.getCenter() - graphview.getSize() * 0.5f, graphview.getCenter() + graphview.getSize() * 0.5f);

        } else if (const auto press = event->getIf<sf::Event::KeyPressed>()) {
            if (press->code == sf::Keyboard::Key::S && press->control) {
                auto f = saveFileName(window.getNativeHandle(), "Graph File (*.graph)\0*.graph\0");
                auto stem = f.path.replace_extension().generic_string();
                if (!stem.empty()) {
                    graph.save_to_file(stem + ".graph");
                }
            }
            if (press->code == sf::Keyboard::Key::E && press->control) {
                auto f = saveFileName(window.getNativeHandle(), "PNG Image (*.png)\0*.png\0SVG Image (*.svg)\0*.svg\0");
                auto stem = f.path.replace_extension().generic_string();
                if (!stem.empty()) {
                    texture.clear(sf::Color::White);
                    graph.draw(texture);
                    texture.display();
                    if (f.type == 0) {
                        export_image(stem + ".png", texture.getTexture(), 20);
                    } else {
                        graph.export_svg(stem + ".svg");
                    }
                }
            }

        } else if (const auto scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (scroll->delta > 0) {
                auto p = sf::Vector2f((float)scroll->position.x, (float)scroll->position.y);
                auto begin = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                graphview.zoom(0.9f);
                auto end = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                graphview.move(begin - end);
                grid = get_grid(graphview.getCenter() - graphview.getSize() * 0.5f, graphview.getCenter() + graphview.getSize() * 0.5f);
            } else {
                auto p = sf::Vector2f((float)scroll->position.x, (float)scroll->position.y);
                auto begin = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                graphview.zoom(1.1f);
                auto end = texture.mapPixelToCoords((sf::Vector2i)(p), graphview);
                graphview.move(begin - end);
                grid = get_grid(graphview.getCenter() - graphview.getSize() * 0.5f, graphview.getCenter() + graphview.getSize() * 0.5f);
            }
        }

        if (event.has_value()) {
            editor.handle_event(event.value(), graphview);
            settings.handle_event(event.value());
        }

        texture.clear(sf::Color::White);
        texture.setView(graphview);
        texture.draw(grid);
        graph.draw(texture);
        editor.draw(texture);
        texture.setView(guiview);
        settings.draw(texture);
        texture.display();

        window.setView(windowview);
        window.draw(sf::Sprite(texture.getTexture()));
        window.display();
    }
}

