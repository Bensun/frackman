#include "StudentWorld.h"
#include "Actor.h"
using namespace std;
class Dirt;
class GraphObject;
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::StudentWorld(string assetDir) 
	: GameWorld(assetDir)
{
}
StudentWorld::~StudentWorld() {
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			delete m_dirt[i][j];
		}
	}
	delete m_man;
}
int StudentWorld::init()
{
	m_ticks = 0;
	int B = fmin(getLevel() / 2 + 2, 6);
	int G = fmax(5 - getLevel() / 2, 2);
	int L = fmin(2 + getLevel(), 20);
	m_barrels = L;
	int Bcount = 0;
	int Gcount = 0;
	int Lcount = 0;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			m_dirt[i][j] = nullptr;
		}
	}
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 60; j++) {
			m_dirt[i][j] = new Dirt(this, i, j);
		}
	}
	for (int i = 30; i < 34; i++) {
		for (int j = 0; j < 4; j++) {
			m_dirt[i][j] = new Dirt(this, i, j);
		}
	}
	for (int i = 34; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			m_dirt[i][j] = new Dirt(this, i, j);
		}
	}
	while (Bcount < B) {
		int x = randInt(0, 60);
		int y = randInt(20, 56);
		if (canBePlaced(x, y)) {
			Object* a = new Boulder(this, x, y);
			for (int j = a->getX(); j < a->getX() + 4; j++) {
				for (int k = a->getY(); k < a->getY() + 4; k++) {
					removeDirt(j, k);
				}
			}
			addObject(a);
			Bcount++;
		}
	}
	while (Gcount < G) {
		int x = randInt(0, 60);
		int y = randInt(0, 56);
		if (canBePlaced(x, y)) {
			Object* a = new GoldNugget(this, x, y);
			addObject(a);
			Gcount++;
		}
	}
	while (Lcount < L) {
		int x = randInt(0, 60);
		int y = randInt(0, 56);
		if (canBePlaced(x, y)) {
			Object* a = new OilBarrel(this, x, y);
			addObject(a);
			Lcount++;
		}
	}
	/*Object *a = new RegularProtester(this, 60, 60);
	addObject(a);*/
	m_man = new FrackMan(this, 30, 60);
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move() 
{
	m_ticks++;
	updateDisplayText();
	//TODO: add protesters
	int g = getLevel()*25 + 300;
	if (randInt(1, 1) == 1) {
		if (randInt(1, 5) == 0) {
			Object *a = new SonarKit(this, 0, 60);
			addObject(a);
		}
		else {
			int x = randInt(0, 60);
			int y = randInt(0, 60);
			while (dirtBlockingObject(m_man, x, y)) {
				if (!dirtBlockingObject(m_man, x, y)) {
					Object *a = new WaterPool(this, x, y);
					addObject(a);
				}
				else {
					x = randInt(0, 60);
					y = randInt(0, 60);
				}
			}
		}
	}
	m_man->doSomething();
	for (int i = 0; i < m_objects.size(); i++) {
		if (m_objects[i]->isAlive()) {
			m_objects[i]->doSomething();
		}
		if (!m_man->isAlive()) {
			return GWSTATUS_PLAYER_DIED;
		}
		if (getBarrels() == 0) {
			return GWSTATUS_FINISHED_LEVEL;
		}
	}
	std::vector<Object*>::iterator it = m_objects.begin();
	while (it != m_objects.end()) {
		if ((*it)->isAlive() == false) {
			delete (*it);
			it = m_objects.erase(it);
		}
		else {
			it++;
		}
	}
	if (!m_man->isAlive()) {
		return GWSTATUS_PLAYER_DIED;
	}
	if (getBarrels() == 0) {
		return GWSTATUS_FINISHED_LEVEL;
	}
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
	std::vector<Object*>::iterator it = m_objects.begin();
	while (it != m_objects.end()) {
		delete (*it);
		it = m_objects.erase(it);
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			delete m_dirt[i][j];
		}
	}
	delete m_man;
}
void StudentWorld::removeDirt(int x, int y) {
	if (x >= 0 && x < 64 &&
		y >= 0 && y < 60) {
		if (m_dirt[x][y] != nullptr) {
			delete m_dirt[x][y];
			m_dirt[x][y] = nullptr;
			playSound(SOUND_DIG);
		}
	}
}
bool StudentWorld::containsDirt(int x, int y) const{
	if (x < 0 || x >= 60 || y < 0 || y >= 60) {
		return false;
	}
	if (m_dirt[x][y] != nullptr) {
		return true;
	}
	return false;
}
void StudentWorld::addObject(Object* a) {
	m_objects.push_back(a);
}
bool StudentWorld::canFrackManMoveTo(Object *frackman, int x, int y) const {
	if (x < 0 || x > 60 || y < 0 || y > 60) {
		return false;
	}
	for (int i = 0; i < m_objects.size(); i++) {
		if (frackman != m_objects[i] &&
			m_objects[i]->canActorsPassThroughMe() == false &&
			isBlocked(m_objects[i], x, y)) {
			return false;
		}
	}
	return true;
}
bool StudentWorld::canObjectMoveTo(Object* a, int x, int y) const {
	if (x < 0 || x > 60 || y < 0 || y > 60) {
		return false;
	}
	if (dirtBlockingObject(a, x, y)) {
		return false;
	}
	for (int i = 0; i < m_objects.size(); i++) {
		if (a != m_objects[i] &&
			m_objects[i]->canActorsPassThroughMe() == false &&
			isBlocked(m_objects[i], x, y)) {
			return false;
		}
	}
	return true;
}
int StudentWorld::annoyAllNearbyObjects(Object* annoyer, int points,
	int radius) {
	//TODO
	return 0;
}
void StudentWorld::revealAllNearbyObjects(int x, int y, int radius) {
	for (int i = 0; i < m_objects.size(); i++) {
		if (isWithinRadius(x, y, m_objects[i], radius)) {
			m_objects[i]->setVisible(true);
		}
	}
}
Object* StudentWorld::findNearbyFrackMan(Object* a, int radius) const {
	//TODO
	return nullptr;
}
Object* StudentWorld::findNearbyPickerUpper(Object* a, int radius) const {
	//TODO
	return nullptr;
}
void StudentWorld::annoyFrackMan(int amt) {
	m_man->annoy(amt);
}
void StudentWorld::giveFrackManGold() {
	m_man->addGold();
}
void StudentWorld::giveFrackManSonar() {
	m_man->addSonar();
}
void StudentWorld::giveFrackManWater() {
	m_man->addWater();
}
bool StudentWorld::facingTowardFrackMan(Object* a) const {
	if (a->getDirection() == GraphObject::Direction::left && 
		m_man->getX() < a->getX()) {
		return true;
	}
	if (a->getDirection() == GraphObject::Direction::right && 
		m_man->getX() > a->getX()) {
		return true;
	}
	if (a->getDirection() == GraphObject::Direction::down && 
		m_man->getY() < a->getY()) {
		return true;
	}
	if (a->getDirection() == GraphObject::Direction::up && 
		m_man->getX() > a->getX()) {
		return true;
	}
	return false;
}
int StudentWorld::lineOfSightToFrackMan(Object* a) const {
	if (a->getX() == m_man->getX()) {
		if (a->getY() > m_man->getY()) {
			for (int i = m_man->getY(); i < a->getY(); i++) {
				for (int j = a->getX(); j < a->getX() + 4; j++) {
					if (containsDirt(i, j)) {
						return 0;
					}
				}
			}
		}
		return 1;	//left
	}
	if (a->getX() == m_man->getX()) {
		if (a->getY() < m_man->getY()) {
			for (int i = a->getY(); i < m_man->getY(); i++) {
				for (int j = a->getX(); j < a->getX() + 4; j++) {
					if (containsDirt(i, j)) {
						return 0;
					}
				}
			}
		}
		return 2;	//right
	}
	if (a->getY() == m_man->getY()) {
		if (a->getX() > m_man->getX()) {
			for (int i = m_man->getX(); i < a->getX(); i++) {
				for (int j = a->getY(); j < a->getY() + 4; j++) {
					if (containsDirt(i, j)) {
						return 0;
					}
				}
			}
		}
		return 3;	//down
	}
	if (a->getY() == m_man->getY()) {
		if (a->getX() < m_man->getX()) {
			for (int i = a->getX(); i < m_man->getX(); i++) {
				for (int j = a->getY(); j < a->getY() + 4; j++) {
					if (containsDirt(i, j)) {
						return 0;
					}
				}
			}
		}
		return 4;	//up
	}
	return 0;
}
bool StudentWorld::isNearFrackman(Object* a, int radius) const {
	if (isWithinRadius(a->getX(), a->getY(), m_man, radius)) {
		return true;
	}
	return false;
}
//GraphObject::Direction StudentWorld::determineFirstMoveToExit(int x, 
//	int y) {
//	//TODO
//}
//GraphObject::Direction StudentWorld::determineFirstMoveToFrackMan(int x, 
//	int y) {
//	//TODO
//}
bool StudentWorld::isWithinRadius(int x, int y, Object *a, int radius) const {
	int lengthX = x - a->getX();
	int lengthY = y - a->getY();
	if (radius >= sqrt((lengthX*lengthX) + (lengthY*lengthY))) {
		return true;
	}
	return false;
}
bool StudentWorld::canBePlaced(int x, int y) const{
	if (x < 0 || x > 60 || y < 0 || y > 60) {
		return false;
	}
	for (int i = 0; i < m_objects.size(); i++) {
		if (isWithinRadius(x, y, m_objects[i], 6)) {
			return false;
		}
	}
	if (x >= 26 && x < 34 && y >= 3 && y < 60) {
		return false;
	}
	return true;
}
bool StudentWorld::isBlocked(Object *blocker, int x, int y) const {
	for (int i = blocker->getX() - 3; i < blocker->getX() + 4; i++) {
		for (int j = blocker->getY() - 3; j < blocker->getY() + 4; j++) {
			if (x == i && y == j) {
				return true;
			}
		}
	}
	return false;
}
bool StudentWorld::dirtBlockingObject(Object* a, int x, int y) const {
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (containsDirt(i, j)) {
				return true;
			}
		}
	}

	return false;
}
void StudentWorld::updateDisplayText() {
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = m_man->getHitPoints();
	int squirts = m_man->getWater();
	int gold = m_man->getGold();
	int sonar = m_man->getSonar();
	int barrelsLeft = getBarrels();
	string s = formatScore(score, level, lives,
		health, squirts, gold, sonar, barrelsLeft);
	setGameStatText(s);
}
int StudentWorld::getBarrels() {
	return m_barrels;
}
void StudentWorld::pickedUpBarrel() {
	m_barrels--;
}
string StudentWorld::formatScore(int score, int level, int lives,
	int health, int squirts, int gold, int sonar, int barrelsLeft) {
	stringstream t;
	t << "Scr: " << setw(6) << right << setfill('0') << score << "  Lvl: " <<
		setw(2) << setfill(' ') << level << "  Lives: " << setw(1) <<
		lives << "  Hlth: " << setw(2) << health / .1 << '%' << "  Wtr: " <<
		squirts << "  Gld: " << gold << "  Sonar: " << sonar << 
		"  Oil Left: " << barrelsLeft;
	string s = t.str();
	return s;
}