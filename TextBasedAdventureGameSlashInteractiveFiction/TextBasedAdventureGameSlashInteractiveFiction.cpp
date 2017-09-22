#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;

/* Constants */

const string kMapFile = "map.txt";
const int kMapSize = 10;

struct Item {
	int hp, damage;
	string name;
};

map<string, Item> items;

struct Coord {
	int x, y;
};

/* Living class */

class Living {
	public:
		int hp;
		int damage;
};

class Dragon : public Living {
	public:
		Dragon() {
			hp = 40;
			damage = 6;
		}
};

/* Player class */

class Player : public Living {
	public:
		Player() {
			hp = 100;
			damage = 10;
		}

		void outputStats() {
			cout << "HP = " << hp << ", Damage = " << damage;
		}

		void addHP(int n) {
			hp += n;
		}

		void addDamage(int n) {
			damage += n;
		}

	private:
		
};

/* Map class */

class GameMap {
	public:
		enum class Tile { Empty, Wall, Player, Dragon, Riddle, Item };

		GameMap(string mapFile) {
			map.resize(kMapSize);

			for (int y = 0; y < kMapSize; y++)
				map[y].resize(kMapSize);
			
			ifstream file(mapFile);

			/* Read in map */

			for (int y = 0; y < kMapSize; y++) {
				for (int x = 0; x < kMapSize; x++) {
					int tile;
					file >> tile;

					if ((Tile)tile == Tile::Player)
						playerPos.x = x, playerPos.y = y;
					else
						map[y][x] = (Tile)tile;
				}
			}

			file.close();
		}

		void display() {
			for (int y = 0; y < kMapSize; y++) {
				for (int x = 0; x < kMapSize; x++) {
					if (x == playerPos.x && y == playerPos.y)
						cout << "P ";
					else
						cout << (((int)map[y][x] > 2) ? 0 : (int)map[y][x]) << " ";
				}

				cout << endl;
			}
		}

		// For debugging
		void output() {
			for (int y = 0; y < kMapSize; y++) {
				for (int x = 0; x < kMapSize; x++)
					cout << (int)map[y][x] << " ";

				cout << endl;
			}
		}

		Coord getPlayerPosition() {
			return playerPos;
		}

		void updatePlayerPosition(Coord pos) {
			playerPos = pos;
		}

		Tile getTile(Coord pos) {
			return map[pos.y][pos.x];
		}

		void updateTile(Coord pos, Tile t) {
			map[pos.y][pos.x] = t;
		}

	private:
		vector<vector<Tile>> map;

		Coord playerPos;
};

Coord moveDirection() {
	Coord pos = { 0, 0 };

	while (true) {
		bool error = false;

		cout << "Where do you want to go?\n\n";
		cout << "1 = Left\n";
		cout << "2 = Forward\n";
		cout << "3 = Right\n\n";

		int dir;
		cin >> dir;

		switch (dir) {
			case 1: pos.x--; break;
			case 2: pos.y--; break;
			case 3: pos.x++; break;

			default:
				error = true;
		}

		if (!error)
			break;
	}

	return pos;
}

void pickupItem(shared_ptr<GameMap> gameMap, shared_ptr<Player> player) {
	int prob = rand() % 10;
	Item item;

	if (prob < 5)
		item = items["Sword"];
	else if (prob < 9)
		item = items["Chainmail"];
	else
		item = items["Dildo"];

	cout << "You picked up a " << item.name << endl;

	player->addHP(item.hp);
	player->addDamage(item.damage);
}

void dragonFight(shared_ptr<Player> player) {
	Dragon dragon;

	while (dragon.hp > 0 && player->hp > 0) {
		int damage = (rand() % 20) - 10 + player->damage;
		dragon.hp -= damage;
		cout << "You hit the dragon for " << damage << " damage.\n";
		cout << "The dragon has " << dragon.hp << " hp.\n\n";

		if (dragon.hp > 0) {
			damage = (rand() % 6) + dragon.damage;
			player->hp -= damage;
			cout << "The dragon hit you for " << damage << " damage.\n";
			cout << "you have " << player->hp << " hp.\n\n";
		}
		
		system("pause");
		system("cls");
	}

	if (dragon.hp < 0)
		cout << "You defeated the dragon\n";
	else
		cout << "The dragon defeated you\n";

	
}

void Riddles() {

}

bool checkTile(shared_ptr<GameMap> gameMap, shared_ptr<Player> player, Coord pos, GameMap::Tile tile) {
	switch(tile) {
		case GameMap::Tile::Item:
			cout << "You have landed on an item!" << endl;
			pickupItem(gameMap, player);
			gameMap->updateTile(pos, GameMap::Tile::Empty);
			cout << endl;
			break;

		case GameMap::Tile::Wall:
			return false;
			break;

		case GameMap::Tile::Dragon:
			dragonFight(player);
			break;

		case GameMap::Tile::Riddle:

			break;
	}

	return true;
}

void initItems();

int main() {
	srand(time(0));

	cout << "Welcome to TextAdventureInteractiveFiction!!!\n\n";

	initItems();

	shared_ptr<GameMap> gameMap = make_shared<GameMap>(kMapFile);
	shared_ptr<Player> player = make_shared<Player>();

	while (true) {
		player->outputStats();
		cout << "\n\n";

		gameMap->display();
		cout << endl;

		Coord playerPos = gameMap->getPlayerPosition();
		Coord nPos = moveDirection();

		system("cls");

		nPos.x += playerPos.x;
		nPos.y += playerPos.y;

		if (checkTile(gameMap, player, nPos, gameMap->getTile(nPos)))
			gameMap->updatePlayerPosition(nPos);
		else
			cout << "You can't move that way!\n";
	}
	
	cout << "\n\n";
	system("pause");

    return 0;
}

void initItems() {
	items["Sword"] = { 0, 10, "Sword" };
	items["Chainmail"] = { 50, 0, "Chainmail" };
	items["Dildo"] = { 0, 100, "Dildo" };
}