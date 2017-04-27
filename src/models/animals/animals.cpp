#include "../../../inc/models/animals/animals.hpp"
#include "ai/ai.cpp"
#include "eating.cpp"
#define square(x) ((x) * (x))

list<Animal*> Animal::animals;

Animal::Animal(World* world, pair<int, int> coords, AnimalType type) {
    this->world = world;
    this->type = type;
    x = coords.a;
    y = coords.b;
}

float Animal::getX() {return x;}
float Animal::getY() {return y;}
int Animal::getHunger() {return hunger;}
Animal::~Animal() {}

float Animal::getSpeed() {
    float res = 1 - (float)hunger / maxHunger;
    if (! world->isLandAt(round(x), round(y))) {
        res /= 5;
    }
    return res;
}

void Animal::die() {
    // cout << animals.size() << " little niggers left\n";
    alive = false;
}

bool Animal::isAlive() {
    return alive;
}



void Animal::takeTurn() { // decide where to go, become hungry, die etc.
    increaseHunger(hungerRate);

    eat();

    //std::cout << hunger << "\n";

    auto vec = decideWhereToGo();

    //std::cout << (int)vec.a << " " << (int)vec.b << "\n";

    x += vec.a * getSpeed();
    y += vec.b * getSpeed();
}

int Animal::getFieldOfView() {
    return 50;
}

void Animal::takeTurns() {
    for(auto it = animals.begin(); it != animals.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->takeTurn();
        }
    }

    removeDead();
}

void Animal::removeDead() {
    list<Animal*> upd;

    for(auto it = animals.begin(); it != animals.end(); it++) {
        if ((*it)->isAlive()) {
            upd.push_back(*it);
        } else {
            delete *it;
        }
    }

    animals = upd;
}

int Animal::getType() {
    return type;
}

void Animal::spawnAnimals(World* world) {
    const int N = 50;

    for(auto it=animals.begin(); it != animals.end(); it++) {
        delete *it;
    }

    animals = list<Animal*>();

    pair<int, int> coords;

    for (int i=0; i < N; i++) {
        coords = world->getRandomLand();
        animals.push_back(new Animal(world, coords, AnimalType::HARE));

        coords = world->getRandomLand();
        animals.push_back(new Animal(world, coords, AnimalType::WOLF));
    }
}


list<Animal*>::iterator Animal::begin() {
    return animals.begin();
}

list<Animal*>::iterator Animal::end() {
    return animals.end();
}

void Animal::decreaseHunger(int delta) {
    hunger = ((hunger - delta) < 0) ? : (hunger - delta);
}

void Animal::increaseHunger(int delta) {
    hunger += delta;
    if (hunger > maxHunger) die();
}

bool Animal::isHare() {
    return type == AnimalType::HARE;
}

bool Animal::isWolf() {
    return type == AnimalType::WOLF;
}