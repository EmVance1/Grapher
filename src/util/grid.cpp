#include "pch.h"
#include "grid.h"


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


