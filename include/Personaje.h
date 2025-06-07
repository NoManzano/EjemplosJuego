#pragma once
#include <SFML/Graphics.hpp>
#include "Mapa.h"

class Personaje {
public:
    Personaje(sf::Vector2f position, const std::string& imgPath = "assets/images/animacion.png") {
        if (!texture.loadFromFile(imgPath)) {
            // Manejo de error
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        
        // Reducir el tamaño del personaje
        sprite.setScale(Mapa::tile / 17.f, Mapa::tile / 17.f); // Escala ajustada a 15
    }

    void move(float offsetX, float offsetY, Mapa& mapa) {
        sf::Vector2f next = sprite.getPosition() + sf::Vector2f(offsetX, offsetY);

        // Verificar las cuatro esquinas del sprite
        sf::Vector2f topLeft(next.x, next.y);
        sf::Vector2f topRight(next.x + frameWidth * sprite.getScale().x, next.y);
        sf::Vector2f bottomLeft(next.x, next.y + frameHeight * sprite.getScale().y);
        sf::Vector2f bottomRight(next.x + frameWidth * sprite.getScale().x, next.y + frameHeight * sprite.getScale().y);

        int gridTopLeftX = static_cast<int>(topLeft.x / Mapa::tile);
        int gridTopLeftY = static_cast<int>(topLeft.y / Mapa::tile);

        int gridTopRightX = static_cast<int>(topRight.x / Mapa::tile);
        int gridTopRightY = static_cast<int>(topRight.y / Mapa::tile);

        int gridBottomLeftX = static_cast<int>(bottomLeft.x / Mapa::tile);
        int gridBottomLeftY = static_cast<int>(bottomLeft.y / Mapa::tile);

        int gridBottomRightX = static_cast<int>(bottomRight.x / Mapa::tile);
        int gridBottomRightY = static_cast<int>(bottomRight.y / Mapa::tile);

        // Verificar si alguna esquina está en un bloque sólido
        if (!mapa.esSolido(gridTopLeftX, gridTopLeftY) &&
            !mapa.esSolido(gridTopRightX, gridTopRightY) &&
            !mapa.esSolido(gridBottomLeftX, gridBottomLeftY) &&
            !mapa.esSolido(gridBottomRightX, gridBottomRightY)) {
            sprite.move(offsetX, offsetY);

            // Cambiar la dirección y animar según el movimiento
            if (offsetX > 0) // Derecha
                updateAnimation(0);
            else if (offsetX < 0) // Izquierda
                updateAnimation(1);
            else if (offsetY > 0) // Abajo
                updateAnimation(2);
            else if (offsetY < 0) // Arriba
                updateAnimation(3);
        }
    }

    void updateAnimation(int direction) {
        // Actualizar el frame de la animación según la dirección
        if (clock.getElapsedTime().asSeconds() >= frameTime) {
            currentFrame = (currentFrame + 1) % numFrames;
            int frameX = currentFrame * (frameWidth + frameSpacing); // Espacio entre frames
            sprite.setTextureRect(sf::IntRect(frameX, direction * frameHeight, frameWidth, frameHeight));
            clock.restart();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void aumentarAlcance() {
        alcanceExplosiones += 1; // Incrementar el alcance de las explosiones
    }

    void aumentarBombas() {
        maxBombas += 1; // Incrementar el número máximo de bombas que puede colocar
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock clock;
    float frameTime = 0.1f; // Tiempo entre cada frame en segundos
    int currentFrame = 0;
    int numFrames = 3; // Número total de frames en la animación
    int frameWidth = 16;
    int frameHeight = 16;
    int frameSpacing = 1; // Espacio entre frames
    int alcanceExplosiones = 0; // Alcance de las explosiones
    int maxBombas = 0; // Número máximo de bombas que puede colocar
};