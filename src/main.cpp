#include <SFML/Graphics.hpp>
#include "graph.h"
#include "editor.h"
#include "settings.h"
#include "files.h"
#include "img.h"


int main(int argc, char** argv) {
    sf::ContextSettings ctx;
    ctx.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Graphs", sf::Style::Default, ctx);
    window.setFramerateLimit(60);
    sf::Event event;
    sf::RenderTexture texture;
    texture.create(800, 800, ctx);
    texture.setSmooth(true);

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
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::S && event.key.control) {
                    auto f = saveFileName(window.getSystemHandle(), "Graph File (*.graph)\0*.graph\0");
                    graph.save_to_file(f.path.string());
                }
                if (event.key.code == sf::Keyboard::E && event.key.control) {
                    auto f = saveFileName(window.getSystemHandle(), "PNG Image (*.png)\0*.png\0");
                    texture.clear(sf::Color::White);
                    graph.draw(texture);
                    texture.display();
                    export_image(f.path.string(), texture.getTexture(), 20);
                    // export image
                }
                break;
            default:
                break;
        }
        editor.handle_event(event);
        settings.handle_event(event);

        editor.refresh_graph(window);
        settings.refresh_graph(window);

        texture.clear(sf::Color::White);
        graph.draw(texture);
        editor.draw(texture);
        settings.draw(texture);
        texture.display();

        window.draw(sf::Sprite(texture.getTexture()));
        window.display();
    }
}

