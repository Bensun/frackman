#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <iomanip>
// Students:  Add code to this file, StudentWorld.cpp, Object.h, and Object.cpp
class Object;
class FrackMan;
class Dirt;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void removeDirt(int x, int y);
	bool containsDirt(int x, int y) const;
	void addObject(Object* a);
	bool canFrackManMoveTo(Object *frackman, int x, int y) const;
	bool canObjectMoveTo(Object* a, int x, int y) const;
	int annoyAllNearbyObjects(Object* annoyer, int points, int radius);
	void revealAllNearbyObjects(int x, int y, int radius);
	Object* findNearbyFrackMan(Object* a, int radius) const;
	Object* findNearbyPickerUpper(Object* a, int radius) const;
	void annoyFrackMan(int amt);
	void giveFrackManGold();
	void giveFrackManSonar();
	void giveFrackManWater();
	bool facingTowardFrackMan(Object* a) const;
	bool isNearFrackman(Object* a, int radius) const;
	//GraphObject::Direction determineFirstMoveToExit(int x, int y);
	//GraphObject::Direction determineFirstMoveToFrackMan(int x, int y);
	bool isWithinRadius(int x, int y, Object *b, int radius) const;
	bool canBePlaced(int x, int y) const;
	bool isBlocked(Object *blocker, int x, int y) const;
	bool dirtBlockingObject(Object *a, int x, int y) const;
	void updateDisplayText();
	int getBarrels();
	void pickedUpBarrel();
	std::string formatScore(int score, int level, int lives,
		int health, int squirts, int gold, int sonar, int barrelsLeft);
	int lineOfSightToFrackMan(Object* a) const;
private:
	std::vector<Object*> m_objects;
	Dirt *m_dirt[64][64];
	FrackMan *m_man;
	int m_barrels;
	int m_ticks;
};
int randInt(int x, int y);
#endif // STUDENTWORLD_H_
