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
    sf::VertexArray va = sf::VertexArray(sf::Lines);
    float diff = 50.f;
    if (end.x - begin.x > 2000.f) {
        diff = 100.f;
    }
    if (end.x - begin.x > 4000.f) {
        diff = 200.f;
    }

    // horizontal
    for (float y = down_to_nearest(begin.y, (int)diff) + 25.f; y < end.y; y += diff) {
        va.append(sf::Vertex(sf::Vector2f(begin.x, y), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f(end.x,   y), sf::Color(150, 150, 150)));
    }
    // vertical
    for (float x = down_to_nearest(begin.x, (int)diff) + 25.f; x < end.x; x += diff) {
        va.append(sf::Vertex(sf::Vector2f(x, begin.y), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f(x, end.y),   sf::Color(150, 150, 150)));
    }

    return va;
}

sf::VertexArray get_hex() {
    sf::VertexArray va = sf::VertexArray(sf::Lines);

    for (int i = -400; i < 1200; i += 50) {
        va.append(sf::Vertex(sf::Vector2f(0, (float)i), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f(800, (float)i + 400), sf::Color(150, 150, 150)));
    }
    for (int i = -400; i < 1200; i += 50) {
        va.append(sf::Vertex(sf::Vector2f(0, (float)i + 400), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f(800, (float)i), sf::Color(150, 150, 150)));
    }
    for (int i = 0; i < 800; i += 50) {
        va.append(sf::Vertex(sf::Vector2f((float)i, 0), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f((float)i, 800), sf::Color(150, 150, 150)));
    }

    return va;
}


int main(int argc, char** argv) {
    sf::ContextSettings ctx;
    ctx.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Graphs", sf::Style::Default, ctx);
    window.setFramerateLimit(60);
    sf::Event event;
    sf::RenderTexture texture;
    texture.create(1920, 1280, ctx);
    texture.setSmooth(true);

    auto grid = get_grid(sf::Vector2f(0.f, 0.f), sf::Vector2f(800.f, 800.f));

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
                    auto f = saveFileName(window.getSystemHandle(), "PNG Image (*.png)\0*.png\0SVG Image (*.svg)\0*.svg\0");
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
        editor.draw(texture);
        texture.setView(guiview);
        settings.draw(texture);
        texture.display();

        window.setView(windowview);
        window.draw(sf::Sprite(texture.getTexture()));
        window.display();
    }
}

