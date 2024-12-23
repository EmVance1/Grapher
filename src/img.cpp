#include "pch.h"
#include "img.h"


static sf::Image cropped(const sf::Image& img, uint32_t margin);

static sf::IntRect get_bounds(const sf::Image& img);


bool export_image(const std::string& filename, const sf::Texture& texture, uint32_t margin) {
    sf::Image img = texture.copyToImage();
    sf::Image crop = cropped(img, margin);
    return crop.saveToFile(filename);
}


static sf::Image cropped(const sf::Image& img, uint32_t margin) {
    const auto bounds = get_bounds(img);
    auto result = sf::Image();
    result.resize({(uint32_t)bounds.size.x + margin * 2, (uint32_t)bounds.size.y + margin * 2}, sf::Color::White);

    for (int y = -1; y < bounds.size.y + 2; y++) {
        for (int x = -1; x < bounds.size.x + 2; x++) {
            const auto p = img.getPixel({(uint32_t)(x + bounds.position.x), (uint32_t)(y + bounds.position.y)});
            result.setPixel({x + margin, y + margin}, p);
        }
    }

    return result;
}


static sf::IntRect get_bounds(const sf::Image& img) {
    auto bounds = sf::IntRect({0, 0}, {0, 0});

    for (uint32_t x = 0; x < img.getSize().x; x++) {
        for (uint32_t y = 0; y < img.getSize().y; y++) {
            if (img.getPixel({x, y}) != sf::Color::White) {
                bounds.position.x = x;
                goto next1;
            }
        }
    }
next1:

    for (uint32_t y = 0; y < img.getSize().y; y++) {
        for (uint32_t x = 0; x < img.getSize().x; x++) {
            if (img.getPixel({x, y}) != sf::Color::White) {
                bounds.position.y = y;
                goto next2;
            }
        }
    }
next2:

    for (int x = (int)img.getSize().x - 1; x >= 0; x--) {
        for (uint32_t y = 0; y < img.getSize().y; y++) {
            if (img.getPixel({(uint32_t)x, y}) != sf::Color::White) {
                bounds.size.x = x - bounds.position.x;
                goto next3;
            }
        }
    }
next3:

    for (int y = (int)img.getSize().y - 1; y >= 0; y--) {
        for (uint32_t x = 0; x < img.getSize().x; x++) {
            if (img.getPixel({x, (uint32_t)y}) != sf::Color::White) {
                bounds.size.y = y - bounds.position.y;
                goto next4;
            }
        }
    }
next4:

    return bounds;
}

