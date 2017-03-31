#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <string>
#include <cmath>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Object : public GraphObject {
	bool m_alive;
	StudentWorld* m_world;
public:
	Object(StudentWorld* world, int imageID, int x, int y, 
		Direction dir = right, double size = 1.0, unsigned int depth = 0);
	~Object();
	virtual void doSomething() = 0;
	StudentWorld* getWorld();
	bool isAlive() const;
	void setDead();
	virtual bool canActorsPassThroughMe() const;
	virtual bool canDigThroughDirt() const;
	virtual bool canPickThingsUp() const;
	virtual bool huntsFrackMan() const;
	virtual bool needsToBePickedUpToFinishTheLevel() const;
	bool moveToIfPossible(int x, int y);
};
class Agent : public Object {
	int m_hitPoints;
public:
	Agent(StudentWorld* world, int imageID, int startX, int startY,
		Direction startDir, unsigned int hitPoints);
	virtual void addGold() = 0;
	unsigned int getHitPoints() const;
	virtual bool annoy(unsigned int amount);
	virtual bool canPickThingsUp() const;
};
class FrackMan : public Agent {
	int m_gold;
	int m_sonarCharges;
	int m_waterSquirts;
public:
	FrackMan(StudentWorld* world, int x, int y);
	~FrackMan();
	virtual void doSomething();
	virtual bool annoy(unsigned int amount);
	virtual void addGold();
	virtual bool canDigThroughDirt() const;
	void addSonar();
	void addWater();
	unsigned int getGold() const;
	unsigned int getSonar() const;
	unsigned int getWater() const;
};
class Protester : public Agent {
	bool m_leaveOilFieldState;
	int m_ticks;
	int m_left;
public:
	Protester(StudentWorld* world, int startX, int startY, int imageID,
		unsigned int hitPoints);
	virtual void doSomething();
	virtual bool annoy(unsigned int amount);
	virtual void addGold();
	virtual bool huntsFrackMan() const;
	void setMustLeaveOilField();
	bool isLeave();
};
class RegularProtester : public Protester {
	int m_ticks;
	int m_move;
	int m_shoutTicks;
public:
	RegularProtester(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	virtual void addGold();
};
class HardcoreProtester : public Protester {
public:
	HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
	virtual void doSomething();
	virtual void addGold();
};
class Dirt : public Object {
public:
	Dirt(StudentWorld* world, int x, int y);
	~Dirt();
	virtual void doSomething();
};
class Boulder : public Object {
	int m_waitingCount;
	std::string m_state;
public:
	Boulder(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	virtual bool canActorsPassThroughMe() const;
};
class Squirt : public Object {
	int m_distanceTraveled;
public:
	Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
	virtual void doSomething();
};
class ActivatingObject : public Object {
	int m_sound;
	bool m_visible;
public:
	ActivatingObject(StudentWorld* world, int startX, int startY, 
		int imageID, int soundToPlay, bool isVisible);
	virtual void doSomething();
	bool isVisible();
	bool discovered();
};
class OilBarrel : public ActivatingObject {
public:
	OilBarrel(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
};
class GoldNugget : public ActivatingObject {
public:
	GoldNugget(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
};
class BribeNugget : public ActivatingObject {
	int m_ticks;
public:
	BribeNugget(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
};
class SonarKit : public ActivatingObject{
	int m_ticks;
public:
	SonarKit(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
};
class WaterPool : public ActivatingObject{
	int m_ticks;
public:
	WaterPool(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
};

#endif // ACTOR_H_
