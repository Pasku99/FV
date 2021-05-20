#ifndef mojon_h
#define mojon_h

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemigo.h"
#include "Proyectil.h"
#include "spritePersonaje.h"
#include "Jugador.h"

using namespace std;
using namespace sf;

class mojon : public Enemigo{
    public:
        int direccion;
        Texture *tex;
        Clock reloj, impactado;
        int numVidas, avansa, posx = 0, posxx = 0, x = 0, y = 0;
        int contando = 0;
        bool golpeado = false, restartear = false;
        bool grande = false;
        mojon(int, int, int, int, bool, int);
        ~mojon();
        void Draw(RenderWindow &);
        void Update(float nada);
        void cambiarSprite(int);
        void cambiarSpriteDos(int);
        bool colisionProyectil(Proyectil *);
        void impactoProyectil();
        bool colisionProtagonista(Jugador *, bool);
        Sprite getSprite();
        void recibeGolpe();
        void hacerTransparente();
        void restartSprite();
        bool morir();
    private:
        Sprite *sprite;
};

#endif