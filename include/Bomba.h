#pragma once
#include <SFML/Graphics.hpp>
#include "Mapa.h"
#include <iostream> // Agregar este encabezado para usar std::cerr

class Bomba {
public:
    sf::Vector2i gridPos;
    sf::Clock timer;
    bool explotada = false;

    Bomba(sf::Vector2i pos)
        : gridPos(pos)
    {
        if (!texture.loadFromFile("assets/images/bomba.png")) {
            // Manejo de error
            std::cerr << "Error: No se pudo cargar la textura de la bomba." << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(gridPos.x * Mapa::tile, gridPos.y * Mapa::tile); // Centrado en el tile
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }

    void explotar(Mapa& mapa) {
        // Destruir el bloque donde está la bomba y los bloques adyacentes
        mapa.destruirBloque(gridPos.x, gridPos.y);
        mapa.destruirBloque(gridPos.x + 1, gridPos.y);
        mapa.destruirBloque(gridPos.x - 1, gridPos.y);
        mapa.destruirBloque(gridPos.x, gridPos.y + 1);
        mapa.destruirBloque(gridPos.x, gridPos.y - 1);
        explotada = true; // Marcar la bomba como explotada
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    int frameWidth = 16;
    int frameHeight = 16;
};