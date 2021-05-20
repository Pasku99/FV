#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

#define kVel 20

#include "bala.h"

    bala::bala(int xx, int yy){
        tex = new Texture();
        if (!tex->loadFromFile("resources/Sprites/Lara Croft/401003605_atk.png")) {
            std::cerr << "Error cargando la imagen 401003605_atk.png";
            exit(0);
        } 
        spriteb = new Sprite(*tex);
        //Le pongo el centroide donde corresponde
        spriteb->setOrigin(131, 25);
        //Cojo el sprite que me interesa por defecto del sheet
        spriteb->setTextureRect(sf::IntRect(120, 18, 22, 14));
        
        // Lo dispongo a la derecha de la pantalla
        spriteb->setPosition(xx, yy);
    }

    void bala::movimientoBalaIz(){
        spriteb->setTextureRect(sf::IntRect(120, 18, 22, 14));
        spriteb->move(-kVel, 0);
        spriteb->setScale(1, 1);
    }

    void bala::movimientoBalaDe(){
        spriteb->setTextureRect(sf::IntRect(120, 18, 22, 14));
        spriteb->move(kVel, 0);
        spriteb->setScale(1, 1);
    }

    void bala::hacerTransparente(){
        spriteb->setColor(Color::Transparent);
    }

    void bala::Draw(RenderWindow &window){
        window.draw(*spriteb);
    }

    void bala::restartSprite(){
        spriteb->setColor(Color(255,255,255));
    }


    Sprite *bala::getSprite(){
        return spriteb;
    }