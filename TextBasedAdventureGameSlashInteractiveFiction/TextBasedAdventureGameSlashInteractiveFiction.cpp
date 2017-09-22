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
			return map[pos.x][pos.y];
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

void pickupItem(shared_ptr<Player> player) {
	int prob = rand() % 10;
	Item item;

	if (prob < 5)
		item = items["Sword"];
	else if (prob < 9)
		item = items["Chainmail"];
	else
		item = items["Dildo"];
}

void checkTile(shared_ptr<Player> player, GameMap::Tile tile) {
	switch(tile) {
		case GameMap::Tile::Item: pickupItem(player); break;
	}
}

void initItems();

int main() {
	srand(time(0));

	cout << "Welcome to TextAdventureInteractiveFiction!!!\n\n";

	initItems();

	GameMap gameMap(kMapFile);
	shared_ptr<Player> player = make_shared<Player>();

	while (true) {
		player->outputStats();
		cout << "\n\n";

		gameMap.display();
		cout << endl;

		Coord nPos = moveDirection();
		Coord playerPos = gameMap.getPlayerPosition();

		nPos.x += playerPos.x;
		nPos.y += playerPos.y;

		gameMap.updatePlayerPosition(nPos);

		checkTile(player, gameMap.getTile(nPos));

		system("cls");
	}
	
	cout << "\n\n";
	system("pause");

    return 0;
}

void initItems() {
	items["Sword"] = { 0, 10 };
	items["Chainmail"] = { 50, 0 };
	items["Dildo"] = { 0, 100 };
}