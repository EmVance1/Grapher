#include <SFML/Graphics.hpp>
#include "graph/graph.h"
#include "editor.h"
#include "settings.h"
#include "files.h"
#include "img.h"


sf::VertexArray get_grid() {
    sf::VertexArray va = sf::VertexArray(sf::Lines);

    for (int i = 25; i < 800; i += 50) {
        va.append(sf::Vertex(sf::Vector2f((float)i, 0), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f((float)i, 800), sf::Color(150, 150, 150)));
    }
    for (int i = 25; i < 800; i += 50) {
        va.append(sf::Vertex(sf::Vector2f(0, (float)i), sf::Color(150, 150, 150)));
        va.append(sf::Vertex(sf::Vector2f(800, (float)i), sf::Color(150, 150, 150)));
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
    texture.create(800, 800, ctx);
    texture.setSmooth(true);

    const auto grid = get_grid();

    Graph::init_font();
    Graph graph = Graph(false);
    if (argc == 2) {
        graph.load_from_file(argv[1]);
    } else {
        graph.load_from_file("res/test.graph");
    }
    GraphEditor editor = GraphEditor(&graph);
    GraphSettings settings = GraphSettings(&graph);

    texture.clear(sf::Color::White);
    graph.draw(texture);
    editor.draw(texture);
    settings.draw(texture);
    texture.display();

    window.draw(sf::Sprite(texture.getTexture()));
    window.display();

    while (window.waitEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                return 0;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::S && event.key.control) {
                    auto f = saveFileName(window.getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                    auto stem = f.path.replace_extension().generic_string();
                    if (!stem.empty()) {
                        graph.save_to_file(stem + ".graph");
                    }
                }
                if (event.key.code == sf::Keyboard::E && event.key.control) {
                    auto f = saveFileName(window.getSystemHandle(), "PNG Image (*.png)\0*.png\0");
                    auto stem = f.path.replace_extension().generic_string();
                    if (!stem.empty()) {
                        texture.clear(sf::Color::White);
                        graph.draw(texture);
                        texture.display();
                        export_image(stem + ".png", texture.getTexture(), 20);
                    }
                }
                break;
            default:
                break;
        }
        editor.handle_event(event);
        settings.handle_event(event);

        texture.clear(sf::Color::White);
        texture.draw(grid);
        graph.draw(texture);
        editor.draw(texture);
        settings.draw(texture);
        texture.display();

        window.draw(sf::Sprite(texture.getTexture()));
        window.display();
    }
}

