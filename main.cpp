#include "SFML/Graphics.hpp"
#include <algorithm>
#include <iostream>

class Button {
    sf::Texture button_texture_normal;
    sf::Sprite button_sprite_normal;

    sf::Texture button_texture_pressed;
    sf::Sprite button_sprite_pressed;

    bool is_pressed;

public:
    Button(sf::String const &name, sf::Vector2f pos) {
        button_texture_normal.loadFromFile(name + "_normal.png");
        button_sprite_normal.setTexture(button_texture_normal, true);
        button_sprite_normal.setOrigin({button_sprite_normal.getGlobalBounds().width / 2.0f,
                                        button_sprite_normal.getGlobalBounds().height / 2.0f});
        button_sprite_normal.setPosition(pos);

        button_texture_pressed.loadFromFile(name + "_pressed.png");
        button_sprite_pressed.setTexture(button_texture_pressed, true);
        button_sprite_pressed.setOrigin({button_sprite_pressed.getGlobalBounds().width / 2.0f,
                                         button_sprite_pressed.getGlobalBounds().height / 2.0f});
        button_sprite_pressed.setPosition(pos);

        is_pressed = false;
    }

    bool press(sf::Vector2f mouse_pos) {
        if (button_sprite_normal.getGlobalBounds().contains(mouse_pos)) {
            return is_pressed = true;
        }
        return false;
    }

    bool clicked(sf::Vector2f mouse_pos) {
        if (!is_pressed) {
            return false;
        }
        is_pressed = false;
        return button_sprite_pressed.getGlobalBounds().contains(mouse_pos);
    }

    void operator()(sf::RenderWindow &window) {
        window.draw(is_pressed ? button_sprite_pressed : button_sprite_normal);
    }
};

class BarNumerated {

    sf::Texture point_texture_normal;
    sf::Sprite point_sprite_normal;

    sf::Texture point_texture_pressed;
    sf::Sprite point_sprite_pressed;

    sf::Texture bar_texture;
    sf::Sprite bar_sprite;

    sf::Font font;
    sf::Text number;

    sf::Uint64 min_number;
    sf::Uint64 max_number;
    sf::Uint64 now_number;

    bool is_pressed;

public:
    BarNumerated(sf::String const &name, sf::Vector2f pos, sf::Uint64 min, sf::Uint64 max) : min_number{min},
                                                                                             max_number{max} {
        point_texture_normal.loadFromFile(name + "_normal.png");
        point_sprite_normal.setTexture(point_texture_normal, true);
        point_sprite_normal.setOrigin({point_sprite_normal.getGlobalBounds().width / 2.0f,
                                       point_sprite_normal.getGlobalBounds().height / 2.0f});
        point_sprite_normal.setPosition(pos);

        point_texture_pressed.loadFromFile(name + "_pressed.png");
        point_sprite_pressed.setTexture(point_texture_pressed, true);
        point_sprite_pressed.setOrigin({point_sprite_pressed.getGlobalBounds().width / 2.0f,
                                        point_sprite_pressed.getGlobalBounds().height / 2.0f});
        point_sprite_pressed.setPosition(pos);

        bar_texture.loadFromFile(name + "_bar.png");
        bar_sprite.setTexture(bar_texture, true);
        bar_sprite.setOrigin({bar_sprite.getGlobalBounds().width / 2.0f,
                              bar_sprite.getGlobalBounds().height / 2.0f});
        bar_sprite.setPosition(pos);

        now_number = max_number + (max_number - min_number) / 2;
        font.loadFromFile(name + "_font.ttf");

        number.setString(std::to_string(now_number));
        number.setFont(font);
        number.setCharacterSize(25);
        number.setFillColor(sf::Color::Blue);
        number.setOutlineThickness(0.1f);
        number.setOutlineColor(sf::Color::Green);
        number.setOrigin({number.getGlobalBounds().width / 2.0f,
                          number.getGlobalBounds().height / 2.0f});
        number.setPosition(pos.x, pos.y - 50.0f);

        is_pressed = false;
    }

    bool press(sf::Vector2f mouse_pos) {
        if (bar_sprite.getGlobalBounds().contains(mouse_pos)) {
            is_pressed = true;
            return moved(mouse_pos);
        }
        return false;
    }

    sf::Uint64 unpress() {
        is_pressed = false;
        return now_number;
    }

    bool moved(sf::Vector2f mouse_pos) {
        if (!is_pressed) {
            return false;
        }

        float left = bar_sprite.getGlobalBounds().left + 30.0f;
        float right = left + bar_sprite.getGlobalBounds().width - 60.0f;
        float x = mouse_pos.x < left ? left : (mouse_pos.x > right ? right : mouse_pos.x);
        point_sprite_normal.setPosition(x, point_sprite_normal.getPosition().y);
        point_sprite_pressed.setPosition(x, point_sprite_pressed.getPosition().y);
        now_number = min_number +
                     static_cast<sf::Uint64>(static_cast<float>(max_number - min_number) *
                                             ((x - left) / (right - left)));
        number.setString(std::to_string(now_number));
        return true;
    }

    void operator()(sf::RenderWindow &window) {
        window.draw(bar_sprite);
        window.draw(is_pressed ? point_sprite_pressed : point_sprite_normal);
        window.draw(number);
    }
};

int main() {

    float x = 200.0f;
    float y = 200.0f;

    sf::RenderWindow window(sf::VideoMode(1280, 720), " Test");

    sf::Texture background_texture;
    background_texture.loadFromFile("../asphalt.png");
    sf::Sprite background(background_texture);

    sf::Font font;
    font.loadFromFile("../bar_numerated_font.ttf");
    sf::Text answer;
    answer.setFont(font);
    answer.setPosition(100.0f, 100.0f);
    answer.setCharacterSize(30);

    Button button("../button", sf::Vector2f{500.0f, 500.0f});
    BarNumerated bar_numerated("../bar_numerated", sf::Vector2f{300.0f, 300.0f}, 100, 1000);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Button::Left) {
                        sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        button.press(mouse_pos);
                        bar_numerated.press(mouse_pos);
                    }
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    if (event.mouseButton.button == sf::Mouse::Button::Left) {
                        sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        sf::Uint64 unpressed = bar_numerated.unpress();
                        if (button.clicked(mouse_pos)) answer.setString(std::to_string(unpressed));
                    }
                    break;
                }
                case sf::Event::MouseMoved: {
                    bar_numerated.moved(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
                    break;
                }
                default: {
                    break;
                }
            }
        }
        window.clear();
        window.draw(background);
        window.draw(answer);
        button(window);
        bar_numerated(window);
        window.display();
    }
}