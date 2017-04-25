#include <stdio.h>

typedef enum { false, true } bool;

struct coord scan(char, int, int);



void move();

char oppositeDirection(char);

void updateTile(struct coord, char d);


/*====================
TILE
======================*/

struct tile {
	/*
	TRUE: Blocked
	FALSE: Open
	*/
	bool type;
	bool went[4];
	bool wasAlreadyOn;
	bool isFlagged;
	int timesTraveled;
	int x;
	int y;
};

struct coord {
	int x;
	int y;
};

struct tile maze[7][13];
//Robot vars
int robotX;
int robotY;

int mazeSizeX = 7;
int mazeSizeY = 13;
/*
OPTION 1:
0: Left
1: Right
2: Up
3: Down
*/
//int robotD;
/*
'U': Up
'D': Down
'L': Left
'R': Right
*/
char robotD; // Virtual direction used by algo
char hwRobotD;//Actual current robot direction

char directions[4] = { 'R', 'D', 'L', 'U' };


main() {
	robotD = 'R';
	hwRobotD = 'R';

	while (true) {
		//Simulation of entering an intersection
		printf("Next Intersection(X: %d , y: %d)", robotX, robotY);
		printf("\nPress any key to simulate entering the intersection...\n");
		getchar();
		algo();
	}
	
	printf("helloworld\n");
	getchar();
}


algo() {
	bool isRunning = true;
	while (isRunning) {
		printf("Start The Loop\n");
		getchar();
		//struct coord robotTile = maze[robotX][robotY];
		struct coord robotCoord = { robotX, robotY };
		if (maze[robotCoord.x][robotCoord.y].wasAlreadyOn) {
			printf("Was already on\n");
			int amountOpenNotBeenOn = 0;
			/*

			0: None
			1: Found, but not open
			2: Found and open
			*/
			int foundRight = 0;
			int foundLeft = 0;
			int foundUp = 0;
			int foundDown = 0;

			int currentIndex = directionToIndex(robotD);
			int totalIndex = 0;

			while (totalIndex <= 3) {
				char d = directions[currentIndex];
				struct coord mazeTile = scan(d, robotX, robotY);

				if (mazeTile.x == -1 || mazeTile.y == -1) {
					//The tile is null (INDEX)
				}
				else {
					if (!maze[mazeTile.x][mazeTile.y].isFlagged) {
						//Do the HW Scan and see if it is open or closed
						updateTile(mazeTile, robotD);
					}


				}


				currentIndex += 1;
				if (currentIndex >= 3) {
					currentIndex = 0;
				}
				totalIndex++;
			}

		}
		else {
			//Move Straight 
			printf("Moving straight...");
			struct coord nextTile = scan(robotD, robotX, robotY);

			if (nextTile.x == -1 || nextTile.y == -1) {
				//The tile is null (INDEX)
				printf("The tile is null\n");
				robotD = oppositeDirection(robotD);
				maze[robotCoord.x][robotCoord.y].wasAlreadyOn = true;
				//Do not move
			}
			else {
				//Tile is not null
				printf("The tile is not null\n");
				if (!maze[nextTile.x][nextTile.y].isFlagged) {
					printf("The tile is not flagged\n");
					//Do the HW Scan and see if it is open or closed
					updateTile(nextTile, robotD);
				}
				if (maze[nextTile.x][nextTile.y].type == 'B') {
					printf("The tile is blocked\n");
					//Cannot move straight, path is blocked
					robotD = oppositeDirection(robotD);
					maze[robotCoord.x][robotCoord.y].wasAlreadyOn = true;
				}
				else if (checkTile(maze[nextTile.x][nextTile.y], robotD)) {
					printf("The tile is a dead branch\n");
					//Cannot move straight, dead branch
					robotD = oppositeDirection(robotD);
					maze[robotCoord.x][robotCoord.y].wasAlreadyOn = true;
				}
				else {
					printf("moving...\n");
					//Move 
					maze[robotCoord.x][robotCoord.y].wasAlreadyOn = true;
					move();

					isRunning = false;
				}



			}
		}

		if (robotX == mazeSizeX && robotY == mazeSizeY) {
			//TODO: End Maze...
		}

	}
}

char oppositeDirection(char direction) {
	if (direction == 'L') return 'R';
	if (direction == 'R') return 'L';
	if (direction == 'U') return 'D';
	if (direction == 'D') return 'U';
	return direction;
}



//Returns tile from memory, does not HW scan.
struct coord scan(char d, int x, int y) {
	struct coord scannedTile = { -1, -1 };
	if (d == 'U' && (x - 1) > -1) {
		scannedTile.x = x - 1;
		scannedTile.y = y;
		//return getTile(x - 1, y);
	}
	else if (d == 'D' && (x + 1) < mazeSizeX) {
		scannedTile.x = x + 1;
		scannedTile.y = y;
		//return getTile(x + 1, y);
	}
	else if (d == 'L' && (y - 1) > -1) {
		scannedTile.x = x;
		scannedTile.y = y - 1;
		//return getTile(x, y - 1);
	}
	else if (d == 'R' && (y + 1) < mazeSizeY) {
		scannedTile.x = x;
		scannedTile.y = y + 1;
		//return getTile(x, y + 1);
	}
	return scannedTile;
	//Horrible workaround in order to return null if the tile is not found...
	//struct tile nullTile;
	//nullTile.x = -1;
	//nullTile.y = -1;
	//return &nullTile;
}

void updateTile(struct coord mazeTile, char d) {
	if (hwScan(d)) {
		maze[mazeTile.x][mazeTile.y].type = true;
	}
	else {
		maze[mazeTile.x][mazeTile.y].type = false;
	}
	//maze[mazeTile.x][mazeTile.y] = mazeTile;
}

//Turns, returns TRUE if blocked, FALSE if closed
bool hwScan(char d) {
	//doTurn(hwRobotD, d);
	//Todo: Call ryans method here.
	//return isBlocked();
	//printf("The tile is blocked: Y for true\n");
	//char isBlocked = getchar();
	//if (isBlocked == 'Y') {
	//	return true;
	//}
	return false;
}

bool checkTile(struct tile mazeTile, char direction) {
	if (direction == 'U')  return mazeTile.went[0];
	if (direction == 'D') return mazeTile.went[1];
	if (direction == 'L') return mazeTile.went[2];
	if (direction == 'R') return mazeTile.went[3];
	return false;
}

void move() {
	if (robotD == 'U') {
		struct coord mazeTile = { robotX - 1 ,robotY };
		
		maze[mazeTile.x][mazeTile.y].went[0] = true;
		maze[mazeTile.x][mazeTile.y].timesTraveled += 1;

		robotX -= 2;
	}
	else if (robotD == 'D') {
		struct coord mazeTile = { robotX + 1, robotY };
		maze[mazeTile.x][mazeTile.y].went[1] = true;
		maze[mazeTile.x][mazeTile.y].timesTraveled += 1;

		robotX += 2;
	}
	else if (robotD == 'L') {
		struct coord mazeTile = { robotX, robotY - 1 };
		maze[mazeTile.x][mazeTile.y].went[2] = true;
		maze[mazeTile.x][mazeTile.y].timesTraveled += 1;

		robotY -= 2;
	}
	else if (robotD == 'R') {
		struct coord mazeTile = { robotX, robotY + 1 };
		maze[mazeTile.x][mazeTile.y].went[3] = true;
		maze[mazeTile.x][mazeTile.y].timesTraveled += 1;

		robotY += 2;
	}
	//robot.move(robot.direction);
	//Todo
	//struct coord mazeTile = { robotX, robotY };
	//maze[mazeTile.x][mazeTile.y].wasAlreadyOn = true;
	//isRunning = false;
	//robot.move(robot.direction);
}
//{'R', 'D', 'L', 'U'};
int directionToIndex(char d) {
	if (d == 'U') return 3;
	else if (d == 'D') return 1;
	else if (d == 'L') return 2;
	else if (d == 'R') return 0;
	return -1;
}