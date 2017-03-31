#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
Object::Object(StudentWorld* world, int imageID, int x, int y, Direction dir, double size, unsigned int depth)
	:GraphObject(imageID, x, y, dir, size, depth)
{
	m_world = world;
}
Object::~Object() {

}
void Object::doSomething() {

}
StudentWorld* Object::getWorld() {
	return m_world;
}
bool Object::isAlive() const {
	return m_alive;
}
void Object::setDead() {
	m_alive = false;
}
bool Object::canActorsPassThroughMe() const {
	return true;
}
bool Object::canDigThroughDirt() const {
	//TODO
	return false;
}
bool Object::canPickThingsUp() const {
	//TODO
	return false;
}
bool Object::huntsFrackMan() const {
	//TODO
	return false;
}
bool Object::needsToBePickedUpToFinishTheLevel() const {
	//TODO
	return false;
}
bool Object::moveToIfPossible(int x, int y) {
	if (getWorld()->canObjectMoveTo(this, x, y)) {
		moveTo(x, y);
		return true;
	}
	return false;
}
Agent::Agent(StudentWorld* world, int imageID, int startX, int startY,
	Direction startDir, unsigned int hitPoints) 
	:Object(world, imageID, startX, startY, startDir, 1, 0)
{
	m_hitPoints = hitPoints;
	setVisible(true);
}
unsigned int Agent::getHitPoints() const {
	return m_hitPoints;
}
bool Agent::annoy(unsigned int amount) {
	m_hitPoints -= amount;
	if (m_hitPoints <= 0) {
		setDead();
		return true;
	}
	return false;
}
bool Agent::canPickThingsUp() const {
	//TODO
	return false;
}
FrackMan::FrackMan(StudentWorld* world, int x, int y)
	:Agent(world, IID_PLAYER, x, y, right, 10)
{
	m_gold = 0;
	m_sonarCharges = 1;
	m_waterSquirts = 5;
}
FrackMan::~FrackMan() {

}
void FrackMan::doSomething() {
	for (int i = getX(); i < getX() + 4; i++) {
		for (int j = getY(); j < getY() + 4; j++) {
			getWorld()->removeDirt(i, j);
		}
	}
	int press;
	if (getWorld()->getKey(press) == true) {
		switch (press) {
		case KEY_PRESS_LEFT:
			if (getDirection() == left) {
				if (getWorld()->canFrackManMoveTo(this, getX() - 1, getY())) {
					this->moveTo(getX() - 1, getY());
				}
				if (getX() == 0) {
					this->moveTo(getX(), getY());
				}
			}
			else {
				this->setDirection(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() == right) {
				if (getWorld()->canFrackManMoveTo(this, getX() + 1, getY())) {
					this->moveTo(getX() + 1, getY());
				}
				if (getX() == 60) {
					this->moveTo(getX(), getY());
				}
			}
			else {
				this->setDirection(right);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() == down) {
				if (getWorld()->canFrackManMoveTo(this, getX(), getY() - 1)) {
					this->moveTo(getX(), getY() - 1);
				}
				if (getY() == 0) {
					this->moveTo(getX(), getY());
				}
			}
			else {
				this->setDirection(down);
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() == up) {
				if (getWorld()->canFrackManMoveTo(this, getX(), getY() + 1)) {
					this->moveTo(getX(), getY() + 1);
				}
				if (getY() == 60) {
					this->moveTo(getX(), getY());
				}
			}
			else {
				this->setDirection(up);
			}
			break;
		case KEY_PRESS_ESCAPE:
			getWorld()->annoyFrackMan(100);
			break;
		case KEY_PRESS_SPACE:
			if (m_waterSquirts > 0) {
				if (getDirection() == left) {
					getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					m_waterSquirts--;
					if (getWorld()->canObjectMoveTo(this, getX() - 4, getY())) {
						Object *a = new Squirt(getWorld(), getX() - 4,
							getY(), left);
						getWorld()->addObject(a);
					}
				}
				if (getDirection() == right) {
					getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					m_waterSquirts--;
					if (getWorld()->canObjectMoveTo(this, getX() + 4, getY())) {
						Object *a = new Squirt(getWorld(), getX() + 4,
							getY(), right);
						getWorld()->addObject(a);
					}
				}
				if (getDirection() == up) {
					getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					m_waterSquirts--;
					if (getWorld()->canObjectMoveTo(this, getX(),
						getY() + 4)) {
						Object *a = new Squirt(getWorld(), getX(),
							getY() + 4, up);
						getWorld()->addObject(a);
					}
				}
				if (getDirection() == down) {
					getWorld()->playSound(SOUND_PLAYER_SQUIRT);
					m_waterSquirts--;
					if (getWorld()->canObjectMoveTo(this, getX(),
						getY() - 4)) {
						Object *a = new Squirt(getWorld(), getX(),
							getY() - 4, down);
						getWorld()->addObject(a);
					}
				}
			}
			break;
		case 'Z':
		case 'z':
			if (getSonar() > 0) {
				m_sonarCharges--;
				getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
			}
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0) {
				m_gold--;
				Object *a = new BribeNugget(getWorld(), getX(), getY());
				getWorld()->addObject(a);
			}
		}
	}
}
bool FrackMan::annoy(unsigned int amount) {
	if (Agent::annoy(amount)) {
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		return true;
	}
	return false;
}
bool FrackMan::canDigThroughDirt() const {
	//TODO
	return false;
}
void FrackMan::addGold() {
	m_gold++;
}
void FrackMan::addSonar() {
	m_sonarCharges++;
}
void FrackMan::addWater() {
	m_waterSquirts += 5;
}
unsigned int FrackMan::getGold() const {
	return m_gold;
}
unsigned int FrackMan::getSonar() const {
	return m_sonarCharges;
}
unsigned int FrackMan::getWater() const {
	return m_waterSquirts;
}
Protester::Protester(StudentWorld* world, int startX, int startY,
	int imageID, unsigned int hitPoints)
	:Agent(world, startX, startY, IID_PROTESTER, left, 5)
{
	m_leaveOilFieldState = false;
}
void Protester::doSomething() {
	//TODO
}
bool Protester::annoy(unsigned int amount) {
	//TODO
	return false;
}
void Protester::addGold() {
	//TODO
}
bool Protester::huntsFrackMan() const {
	//TODO
	return false;
}
void Protester::setMustLeaveOilField() {
	m_leaveOilFieldState = true;
}
bool Protester::isLeave() {
	return m_leaveOilFieldState;
}
RegularProtester::RegularProtester(StudentWorld* world, int startX, 
	int startY) 
	:Protester(world, startX, startY, IID_PROTESTER, 10)
{
	m_ticks = fmax(0, 3 - getWorld()->getLevel() / 4);
	m_move = randInt(8, 60);
	m_shoutTicks = 15;
}
void RegularProtester::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (m_ticks > 0) {
		m_ticks--;
		return;
	}
	if (isLeave()) {
		if (getX() == 60 && getY() == 60) {
			setDead();
			return;
		}
		//TODO queue-based maze-searching algorithm
	}
	/*if (getWorld()->isNearFrackman(this, 4) &&
		getWorld()->facingTowardFrackMan(this)) {
		if (m_shoutTicks == 15) {
			m_shoutTicks--;
			getWorld()->annoyFrackMan(2);
			return;
		}
		m_shoutTicks--;
		if (m_shoutTicks <= 0) {
			m_shoutTicks = 15;
			return;
		}
	}*/
	/*if (getWorld()->lineOfSightToFrackMan(this) != 0 &&
		!getWorld()->isNearFrackman(this, 4)) {
		if (getWorld()->lineOfSightToFrackMan(this) == 1) {
			moveTo(getX() - 1, getY());
		}
		if (getWorld()->lineOfSightToFrackMan(this) == 2) {
			moveTo(getX() + 1, getY());
		}
		if (getWorld()->lineOfSightToFrackMan(this) == 3) {
			moveTo(getX(), getY() - 1);
		}
		if (getWorld()->lineOfSightToFrackMan(this) == 1) {
			moveTo(getX(), getY() + 1);
		}
		m_move = 0;
		return;
	}*/
	m_move--;
	/*if (m_move == 15) {
		int d = randInt(1, 4);
		if (d == 1) {

		}
	}*/
}
void RegularProtester::addGold() {
	//TODO
}
HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX,
	int startY, int imageID) 
	:Protester(world, startX, startY, IID_HARD_CORE_PROTESTER, 20)
{

}
void HardcoreProtester::doSomething() {
	//TODO
}
void HardcoreProtester::addGold() {
	//TODO
}
Dirt::Dirt(StudentWorld* world, int startX, int startY)
	:Object(world, IID_DIRT, startX, startY, right, .25, 3)
{
	setVisible(true);
}
Dirt::~Dirt() {

}
void Dirt::doSomething() {

}
Boulder::Boulder(StudentWorld* world, int startX, int startY)
	:Object(world, IID_BOULDER, startX, startY, down, 1.0, 1)
{
	m_state = "stable";
	m_waitingCount = 0;
	setVisible(true);
}
void Boulder::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (m_state == "stable") {
		if (!(getWorld()->dirtBlockingObject(this, getX(), getY() - 1)))
		{
			m_state = "waiting";
			return;
		}
	}
	if (m_state == "waiting") {
		if (m_waitingCount == 30) {
			m_state = "falling";
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		else {
			m_waitingCount++;
		}
	}
	if (m_state == "falling") {
		if (!getWorld()->canObjectMoveTo(this, getX(), getY() - 1)) {
			setDead();
			return;
		}
		if (getWorld()->isNearFrackman(this, 4)) {
			getWorld()->annoyFrackMan(100);
		}
		//TODO annoy protesters
		moveTo(getX(), getY() - 1);
	}
}
bool Boulder::canActorsPassThroughMe() const {
	return false;
}
Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir) 
	:Object(world, IID_WATER_SPURT, startX, startY, startDir, 1.0, 1)
{
	m_distanceTraveled = 0;
	setVisible(true);
}
void Squirt::doSomething() {
	//TODO: if within radius of protester
	if (m_distanceTraveled == 4) {
		setDead();
		return;
	}
	if (getDirection() == left) {
		if (getWorld()->canObjectMoveTo(this, getX() - 1, getY())) {
			moveTo(getX() - 1, getY());
			m_distanceTraveled++;
			return;
		}
		setDead();
		return;
	}
	if (getDirection() == right) {
		if (getWorld()->canObjectMoveTo(this, getX() + 1, getY())) {
			moveTo(getX() + 1, getY());
			m_distanceTraveled++;
			return;
		}
		else {
			setDead();
			return;
		}
	}
	if (getDirection() == down) {
		if (getWorld()->canObjectMoveTo(this, getX(), getY() - 1)) {
			moveTo(getX(), getY() - 1);
			m_distanceTraveled++;
			return;
		}
		else {
			setDead();
			return;
		}
	}
	if (getDirection() == up) {
		if (getWorld()->canObjectMoveTo(this, getX() + 1, getY() + 1)) {
			moveTo(getX(), getY() + 1);
			m_distanceTraveled++;
			return;
		}
		else {
			setDead();
			return;
		}
	}
}
ActivatingObject::ActivatingObject(StudentWorld* world, int startX,
	int startY, int imageID, int soundToPlay, bool isVisible) 
	:Object(world, imageID, startX, startY, right, 1.0, 2)
{
	m_sound = soundToPlay;
	m_visible = isVisible;
}
void ActivatingObject::doSomething() {
	
}
bool ActivatingObject::isVisible() {
	return m_visible;
}
bool ActivatingObject::discovered() {
	m_visible = true;
	return m_visible;
}
OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY) 
	:ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, 
		false)
{
}
void OilBarrel::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (getWorld()->isNearFrackman(this, 4) && isVisible() == false) {
		setVisible(true);
		discovered();
		return;
	}
	if (getWorld()->isNearFrackman(this, 3)) {
		setDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->pickedUpBarrel();
	}
	return;
}
GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY) 
	:ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE,
		false)
{
	
}
void GoldNugget::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (getWorld()->isNearFrackman(this, 4) && isVisible() == false) {
		setVisible(true);
		discovered();
		return;
	}
	if (getWorld()->isNearFrackman(this, 3)) {
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->giveFrackManGold();
	}
	return;
}
BribeNugget::BribeNugget(StudentWorld* world, int startX, int startY)
	:ActivatingObject(world, startX, startY, IID_GOLD, 
		SOUND_PROTESTER_FOUND_GOLD, true)
{
	m_ticks = 0;
	setVisible(true);
}
void BribeNugget::doSomething() {
	//TODO
}
SonarKit::SonarKit(StudentWorld* world, int startX, int startY) 
	:ActivatingObject(world, startX, startY, IID_SONAR,
		SOUND_GOT_GOODIE, true)
{
	m_ticks = fmax(100, 300 - 10 * world->getLevel());
	setVisible(true);
}
void SonarKit::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (getWorld()->isNearFrackman(this, 3)) {
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->giveFrackManSonar();
		return;
	}
	if (m_ticks == 0) {
		setDead();
		return;
	}
	m_ticks--;
}
WaterPool::WaterPool(StudentWorld* world, int startX, int startY)
	:ActivatingObject(world, startX, startY, IID_WATER_POOL,
		SOUND_GOT_GOODIE, true)
{
	m_ticks = fmax(100, 300 - 10 * getWorld()->getLevel());
	setVisible(true);
}
void WaterPool::doSomething() {
	if (!isAlive()) {
		return;
	}
	if (getWorld()->isNearFrackman(this, 3)) {
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(100);
		getWorld()->giveFrackManWater();
		return;
	}
	if (m_ticks == 0) {
		setDead();
		return;
	}
	m_ticks--;
}