#include "Darkrai.h"
#define PI 3.1415926536

Darkrai::Darkrai(int posx, int posy, float speed, sf::Sprite &personaje) : Enemigo(2) {
    this->speed = speed;
    this->personaje = &personaje;

    // Cargamos el fantasma...
    textura.loadFromFile("resources/Darkrai.png");
    textura.setSmooth(true);

    body = new Sprite(textura);
    body->setPosition(posx, posy);
    // Creamos la animacion...
    animacion = new AnimationDarkrai(&textura, sf::Vector2u(3, 5), 0.4f);

    // Modificamos el centro de masas...
    body->setOrigin(animacion->getOrigin());

    // Creamos los limitadores colisionables...
    boundingSet[0].setSize(sf::Vector2f(28, 30));
    boundingSet[1].setSize(sf::Vector2f(23, 30));
    boundingSet[2].setSize(sf::Vector2f(21, 30));
    boundingSet[3].setSize(sf::Vector2f(21, 26));
    boundingSet[4].setSize(sf::Vector2f(16, 29));
    for (unsigned int i=0; i<5; i++) {
      boundingSet[i].setOrigin(boundingSet[i].getSize().x/2, boundingSet[i].getSize().y);
      boundingSet[i].setFillColor(sf::Color::Red);
    }

    // La caja inicial...
    boundingBox = &boundingSet[0];
    boundingBox->setPosition(body->getPosition().x, body->getPosition().y-6);
}


Darkrai::~Darkrai() {
    boundingBox = NULL;
    personaje = NULL;
    delete body;
    body = NULL;
    delete animacion;
    animacion = NULL;
}


void Darkrai::Update(float deltaTime) {
    sf::Vector2f movimiento(0.0f, 0.0f);
    sf::Vector2i distancia(0, 0);

    // Distancia de perseguimiento y vision...
    distancia.x = abs(body->getPosition().x) - abs(personaje->getPosition().x);

    if(restartear == true){
      impactado.restart();
      restartear = false;
    }
    this->impactoProyectil();
    if(golpeado == false){
      if (follow) {

        sf::Vector2i distancia(0, 0);
        sf::Vector2f angulo(0.0f, 0.0f);

        // Calculamos la distancia...
        distancia.x = abs(body->getPosition().x) - abs(personaje->getPosition().x);
        distancia.y = abs(body->getPosition().y) - abs(personaje->getPosition().y);

        if (distancia.x > 0) {
          miraIzquierda = true;
          movimiento.x -= speed * deltaTime;
        }
        if (distancia.x < 0) {
          miraIzquierda = false;
          movimiento.x += speed * deltaTime;
        }
        if (distancia.y > 0) {
          rowSheet = 4;
          movimiento.y -= speed * deltaTime;
        }
        if (distancia.y < 0) {
          rowSheet = 0;
          movimiento.y += speed * deltaTime;
        }

        // Resolvemos el movimiento diagonal...
        if (movimiento.x != 0.0f  &&  movimiento.y != 0.0f) {

          // Calculamos el angulo en radianes que forman...
          angulo.x = atan(abs(distancia.y)/abs(distancia.x));
          angulo.y = atan(abs(distancia.x)/abs(distancia.y));
          movimiento.x *= float(cos(angulo.x));
          movimiento.y *= float(cos(angulo.y));

          // Pasamos los angulos a grados para la orientacion...
          angulo.x = angulo.x*(180/PI);
          angulo.y = angulo.y*(180/PI);

          // Orientamos la animacion...
          if (angulo.x >= 0  &&  angulo.x < 22.5) {
            rowSheet = 2;
          } else if (angulo.x < 67.5  &&  angulo.x >= 22.5) {
            if (movimiento.y > 0) {
              rowSheet = 1;
            } else {
              rowSheet = 3;
            }
          }

        }
      }else {

      if (abs(distancia.x) < 300.f) {
        follow = true;
      }
    }

    // Actualizamos animacion...
    animacion->Update(rowSheet, deltaTime);
    body->setTextureRect(animacion->getCoordenadasRect());

    if ((rowSheet == 1 || rowSheet == 2 || rowSheet == 3)  &&  !miraIzquierda) {
      body->setScale(-1, 1);
    } else {
      body->setScale(1, 1);
    }

    // Asignamos el movimiento al body...
    body->setPosition(body->getPosition() + movimiento);

    // Movemos el collider...
    boundingBox = &boundingSet[rowSheet];
    boundingBox->setPosition(body->getPosition().x, body->getPosition().y-6);
  }
}


sf::RectangleShape Darkrai::getBoundingBox() {
    return *boundingBox;
}

bool Darkrai::colisionProyectil(Proyectil *p1){
    bool x = false;
    if(p1->get_sprite().getGlobalBounds().intersects(boundingBox->getGlobalBounds()) && golpeado == false){
        this->perderVida();
        x = true;
        golpeado = true;
        restartear = true;           
    }
    return x;
}

void Darkrai::impactoProyectil(){
    float sgs = impactado.getElapsedTime().asSeconds();
    if(golpeado == true){
      this->recibeGolpe(sgs);
    }
    
}

bool Darkrai::colisionProtagonista(Jugador *j, bool esGuerrera){
  bool x = false;

  if((boundingBox->getGlobalBounds().intersects(j->spr_player->getGlobalBounds()) || j->spr_player->getGlobalBounds().contains(boundingBox->getOrigin()))  &&  !golpeado){
    if(j->atacando  &&  esGuerrera) {
      //std::cout << "holaaaaaa" << std::endl;
      golpeado = true;
      restartear = true;
      this->perderVida();
      //j->atacando = false;
    } else { x = true; }
  }

  return x;
}

void Darkrai::recibeGolpe(float sgs) {
  
  if(contando % 2 == 0){
      body->setColor(Color::Red);
      contando++;
  }
  else{
      this->hacerTransparente();
      contando++;
  }
  if(sgs >= 1){
      this->restartSprite();
      golpeado = false;
  }
}

bool Darkrai::morir(){
  bool x = false;
  if(this->getMuerte()){
    x = true;
  }
  return x;
}

void Darkrai::hacerTransparente(){
    body->setColor(Color::Transparent);
}

void Darkrai::restartSprite(){
    body->setColor(Color(255,255,255));
}

sf::Sprite* Darkrai::getSprite() {
  return body;
}


void Darkrai::Draw(sf::RenderWindow& window) {
  if (esGolpeado) {
    esGolpeado = false;
  } else {
    window.draw(*body);
  }
}
