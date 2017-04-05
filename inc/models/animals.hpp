#pragma once
#include "../models/world.hpp"
#include "../tools/containers.hpp"
#include "../tools/random.hpp"
#include "../models/ai.hpp"

using std::cout;
using std::list;

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))



class Animal {
private:
    static list<Animal*> animals;

    BaseAI* ai=nullptr;
    World* world;
    float x, y;
    int hunger=0;
    bool alive=true;
public:
	static const int maxHunger = 1000;
	static const int hungerRate = 10; // how many hunger points the hare gains per turn

    Animal(World* world, BaseAI* ai, pair<int, int> coords);

    float getX();
    float getY();
    int getHunger();
    ~Animal();

    float getSpeed();
    bool isAlive();
    void die();
    void eat();
	void takeTurn();
    static void takeTurns();
    static void spawnAnimals(World* world);
    static void removeDead();
    static list<Animal*>::iterator begin();
    static list<Animal*>::iterator end();
};

