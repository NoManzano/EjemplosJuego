#include <SFML/Graphics.hpp>
#include <vector>
#include "Mapa.h"
#include "Personaje.h"
#include "Bomba.h"
#include "Mejora.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(272, 208), "Bomberman 2 Jugadores");
    Mapa mapa;

    // Cambiar la imagen de los jugadores a "animacion.png"
    Personaje jugador1(sf::Vector2f(Mapa::tile, Mapa::tile), "assets/images/animacion.png");
    Personaje jugador2(sf::Vector2f((Mapa::ancho - 2) * Mapa::tile, (Mapa::alto - 2) * Mapa::tile), "assets/images/animacion.png");

    std::vector<Bomba> bombas; // Lista de bombas

    double velocidad = 0.06;

    // Añadir mejoras al inicio del juego
    mapa.mejoras.emplace_back(sf::Vector2f(3 * Mapa::tile, 3 * Mapa::tile), Mapa::VELOCIDAD);
    mapa.mejoras.emplace_back(sf::Vector2f(5 * Mapa::tile, 5 * Mapa::tile), Mapa::ALCANCE);
    mapa.mejoras.emplace_back(sf::Vector2f(7 * Mapa::tile, 7 * Mapa::tile), Mapa::BOMBA_EXTRA);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Colocar bombas
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    sf::Vector2i gridPos(
                        static_cast<int>(jugador1.getPosition().x / Mapa::tile),
                        static_cast<int>(jugador1.getPosition().y / Mapa::tile)
                    );
                    bombas.emplace_back(gridPos);
                }
                if (event.key.code == sf::Keyboard::RShift) {
                    sf::Vector2i gridPos(
                        static_cast<int>(jugador2.getPosition().x / Mapa::tile),
                        static_cast<int>(jugador2.getPosition().y / Mapa::tile)
                    );
                    bombas.emplace_back(gridPos);
                }
            }
        }

        // Movimiento jugador 1 (flechas)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            jugador1.move(-velocidad, 0, mapa);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            jugador1.move(velocidad, 0, mapa);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            jugador1.move(0, -velocidad, mapa);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            jugador1.move(0, velocidad, mapa);

        // Movimiento jugador 2 (WASD)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            jugador2.move(-velocidad, 0, mapa);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            jugador2.move(velocidad, 0, mapa);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            jugador2.move(0, -velocidad, mapa);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            jugador2.move(0, velocidad, mapa);

        // Explosión de bombas
        for (auto& bomba : bombas) {
            if (bomba.timer.getElapsedTime().asSeconds() > 3.0f) { // Explota después de 3 segundos
                bomba.explotar(mapa);
            }
        }
        bombas.erase(std::remove_if(bombas.begin(), bombas.end(),
            [](const Bomba& b) { return b.explotada; }), bombas.end());

        // Mejora de jugadores
        for (auto it = mapa.mejoras.begin(); it != mapa.mejoras.end();) {
            if (jugador1.getPosition().x < it->getPosition().x + Mapa::tile &&
                jugador1.getPosition().x + Mapa::tile > it->getPosition().x &&
                jugador1.getPosition().y < it->getPosition().y + Mapa::tile &&
                jugador1.getPosition().y + Mapa::tile > it->getPosition().y) {
                // Aplicar la mejora al jugador 1
                switch (it->getTipo()) {
                case Mapa::VELOCIDAD:
                    velocidad += 0.02;
                    break;
                case Mapa::ALCANCE:
                    jugador1.aumentarAlcance();
                    break;
                case Mapa::BOMBA_EXTRA:
                    jugador1.aumentarBombas();
                    break;
                }
                it = mapa.mejoras.erase(it); // Eliminar la mejora del mapa
            } else if (jugador2.getPosition().x < it->getPosition().x + Mapa::tile &&
                       jugador2.getPosition().x + Mapa::tile > it->getPosition().x &&
                       jugador2.getPosition().y < it->getPosition().y + Mapa::tile &&
                       jugador2.getPosition().y + Mapa::tile > it->getPosition().y) {
                // Aplicar la mejora al jugador 2
                switch (it->getTipo()) {
                case Mapa::VELOCIDAD:
                    velocidad += 0.02;
                    break;
                case Mapa::ALCANCE:
                    jugador2.aumentarAlcance();
                    break;
                case Mapa::BOMBA_EXTRA:
                    jugador2.aumentarBombas();
                    break;
                }
                it = mapa.mejoras.erase(it); // Eliminar la mejora del mapa
            } else {
                ++it;
            }
        }

        window.clear();
        mapa.draw(window);
        jugador1.draw(window);
        jugador2.draw(window);
        for (auto& bomba : bombas) {
            bomba.draw(window);
        }
        for (auto& mejora : mapa.mejoras) {
            mejora.draw(window);
        }
        window.display();
    }

    return 0;
}