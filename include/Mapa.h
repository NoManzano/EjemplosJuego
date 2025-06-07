#pragma once
#include <SFML/Graphics.hpp>

class Mapa {
public:
    static const int ancho = 15, alto = 11;
    static const int tile = 16; // Tamaño de cada tile en píxeles
    int grid[alto][ancho] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,2,2,2,3,3,3,3,3,3,3,1}, // Muros destructibles (2)
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,3,3,3,3,3,3,3,3,3,3,3,3,3,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    sf::Texture texturaTiles;
    sf::Sprite spriteTile;

    Mapa() {
        if (!texturaTiles.loadFromFile("assets/images/tileset.png")) {
            // Manejo de error
        }
        spriteTile.setTexture(texturaTiles);
    }

    void draw(sf::RenderWindow& window) {
        for (int y = 0; y < alto; ++y) {
            for (int x = 0; x < ancho; ++x) {
                if (grid[y][x] == 1) {
                    spriteTile.setTextureRect(sf::IntRect(0, 0, tile, tile)); // Bloques sólidos
                } else if (grid[y][x] == 2) {
                    spriteTile.setTextureRect(sf::IntRect(tile, 0, tile, tile)); // Muros destructibles
                } else if (grid[y][x] == 3) {
                    spriteTile.setTextureRect(sf::IntRect(2 * tile, 0, tile, tile)); // Piso del mapa
                } else {
                    continue; // No dibujar celdas vacías
                }
                spriteTile.setPosition(x * tile, y * tile);
                window.draw(spriteTile);
            }
        }
    }

    bool esSolido(int x, int y) {
        if (x < 0 || y < 0 || x >= ancho || y >= alto) return true; // Fuera del mapa
        return grid[y][x] == 1 || grid[y][x] == 2; // Bloques sólidos o destructibles
    }

    void destruirBloque(int x, int y) {
        if (x >= 0 && y >= 0 && x < ancho && y < alto && grid[y][x] == 2) {
            grid[y][x] = 3; // Cambiar el muro destructible a piso
        }
    }
};