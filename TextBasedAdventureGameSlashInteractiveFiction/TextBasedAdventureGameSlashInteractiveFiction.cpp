#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;

/* Constants */

const string kMapFile = "map.txt";
const int kMapSize = 10;

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

		void setPosition(Coord pos) {
			position = pos;
		}

		void move(int x, int y) {
			position.x += x;
			position.y += y;
		}

		Coord getPosition() {
			return position;
		}

	private:
		Coord position;
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
					
					map[y][x] = (Tile)tile;
				}
			}

			file.close();
		}

		void display() {
			for (int y = 0; y < kMapSize; y++) {
				for (int x = 0; x < kMapSize; x++)
					cout << (int)map[y][x] << " ";

				cout << endl;
			}
		}

		Coord getPlayerPosition() {
			return playerPos;
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
		cout << "3 = Right\n";

		int dir;
		cin >> dir;

		switch (dir) {
		case 1: pos.x--; break;
		case 2: pos.y++; break;
		case 3: pos.x++; break;

		default:
			error = true;
		}

		if (!error)
			break;
	}

	return pos;
}

int main() {
	cout << "Welcome to TextAdventureInteractiveFiction!!!\n\n";

	GameMap gameMap(kMapFile);
	gameMap.display();

	Player player;
	player.setPosition(gameMap.getPlayerPosition());

	while (true) {
		Coord nPos = moveDirection();
		Coord playerPos = player.getPosition();

		nPos.x += playerPos.x;
		nPos.y += playerPos.y;

		player.setPosition(nPos);

		cout << "x: " << nPos.x << " y: " << nPos.y << endl;
	}
	
	cout << "\n\n";
	system("pause");

    return 0;
}