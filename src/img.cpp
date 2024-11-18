#include "img.h"
#include <SFML/Graphics.hpp>


static sf::Image cropped(const sf::Image& img, uint32_t margin);

static sf::IntRect get_bounds(const sf::Image& img);


void export_image(const std::string& filename, const sf::Texture& texture, uint32_t margin) {
    sf::Image img = texture.copyToImage();
    sf::Image crop = cropped(img, margin);
    crop.saveToFile(filename);
}


static sf::Image cropped(const sf::Image& img, uint32_t margin) {
    const auto bounds = get_bounds(img);
    auto result = sf::Image();
    result.create(bounds.width + (int)margin * 2, bounds.height + (int)margin * 2, sf::Color::White);

    for (int y = 0; y < bounds.height; y++) {
        for (int x = 0; x < bounds.width; x++) {
            const auto p = img.getPixel(x + bounds.left, y + bounds.top);
            result.setPixel(x + margin, y + margin, p);
        }
    }

    return result;
}


static sf::IntRect get_bounds(const sf::Image& img) {
    auto bounds = sf::IntRect(0, 0, 0, 0);

    for (uint32_t x = 0; x < img.getSize().x; x++) {
        for (uint32_t y = 0; y < img.getSize().y; y++) {
            if (img.getPixel(x, y) != sf::Color::White) {
                bounds.left = x;
                goto next1;
            }
        }
    }
next1:

    for (uint32_t y = 0; y < img.getSize().y; y++) {
        for (uint32_t x = 0; x < img.getSize().x; x++) {
            if (img.getPixel(x, y) != sf::Color::White) {
                bounds.top = y;
                goto next2;
            }
        }
    }
next2:

    for (int x = (int)img.getSize().x - 1; x >= 0; x--) {
        for (uint32_t y = 0; y < img.getSize().y; y++) {
            if (img.getPixel((uint32_t)x, y) != sf::Color::White) {
                bounds.width = x - bounds.left;
                goto next3;
            }
        }
    }
next3:

    for (int y = (int)img.getSize().y - 1; y >= 0; y--) {
        for (uint32_t x = 0; x < img.getSize().x; x++) {
            if (img.getPixel(x, y) != sf::Color::White) {
                bounds.height = y - bounds.top;
                goto next4;
            }
        }
    }
next4:

    return bounds;
}

