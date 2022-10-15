#include "game.h"
#include <curses.h>


void movePlayer(MoveDirection direction, Player* player) {
    int y = player->y;
    int x = player->x;
    switch (direction) {
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case RIGHT:
            x++;
            break;
        case LEFT:
            x--;
            break;
    };

    if (y >= MAP_HEIGHT || y < 0) {
        return;
    }
    if (x >= MAP_WIDTH || x < 0) {
        return;
    }

    Tile tile = map[y][x];
    if (tile.type == NONE || tile.type == WALL || tile.isWall) {
        return;
    }
    if (tile.hasPlayer) {
        return;
    }

    map[player->y][player->x].hasPlayer = 0;
    player->x = x;
    player->y = y;
    tile.hasPlayer = 1;
}


void placeWall(Player* player) {
    int x = player->x;
    int y = player->y;
    if (map[y][x].type == WATER || map[y][x].type == LAVA) {
        return;
    }
    map[y][x].isWall = 1;
}


void dropBomb(Player* player) {
    int x = player->x;
    int y = player->y;
    //Tile tile = map[y][x];
    for (int i = 0; i < 3; i++) {
        if (!player->bombs[i].isPlaced && map[y][x].bomb->isNone) {
            map[y][x].bomb = &player->bombs[i];
            map[y][x].bomb->x = x;
            map[y][x].bomb->y = y;
            map[y][x].bomb->startCycles = frameStart;
            map[y][x].bomb->cyclesToBlow = 3 * CLOCKS_PER_SEC;
            map[y][x].bomb->isPlaced = true;

            switch (map[y][x].type) {
                case GRASS:
                    map[y][x].bomb->radius = 3;
                    break;
                case WATER:
                    map[y][x].bomb->radius = 2;
                    break;
                case MOUNTAINS:
                    map[y][x].bomb->radius = 2;
                    break;
                case DESERT:
                    map[y][x].bomb->radius = 4;
                    break;
                case LAVA:
                    map[y][x].bomb->radius = 5;
                    break;
                default:
                    map[y][x].bomb->radius = 0;
            }
        }
    }
}

bool shouldBombBlow(Bomb* bomb) {
    return (bomb->startCycles + bomb->cyclesToBlow <= frameStart);
}

void blowBomb(Bomb* bomb) {
    bomb->isBlowing = 1;
}

void removeBomb(Bomb* bomb) {
    bomb->isBlowing = false;
    bomb->isPlaced = false;
    map[bomb->y][bomb->x].bomb = &noneBomb;


    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            double currentDistanceFromCenter = sqrt(pow((x - bomb->x)/2, 2) + pow(y - bomb->y, 2));
            if (bomb->radius >= currentDistanceFromCenter) {
                map[y][x].isWall = 0;
            }
        }
    }
}

void handlePlayerBombs(Player* player) {
    for (int i = 0; i < 3; i++) {
        if (player->bombs[i].isPlaced) {
            if (player->bombs[i].isBlowing != 0) {
                if (player->bombs[i].isBlowing > 60) {
                    removeBomb(&player->bombs[i]);
                } else {
                    player->bombs[i].isBlowing += 1;
                }
            } else if (shouldBombBlow(&player->bombs[i])) {
                blowBomb(&player->bombs[i]);
            }
        }
    }
}

void handlePlayerInput(int cmd, Player* player) {
    switch (cmd) {
        case KEY_UP:
            movePlayer(UP, player);
            break;
        case KEY_DOWN:
            movePlayer(DOWN, player);
            break;
        case KEY_LEFT:
            movePlayer(LEFT, player);
            break;
        case KEY_RIGHT:
            movePlayer(RIGHT, player);
            break;
        case 'b':
            dropBomb(player);
            break;
        case 'v':
            placeWall(player);
            break;
    }
}


void gameLoop(int input1, int input2) {
    handlePlayerInput(input1, &player1);
    handlePlayerInput(input2, &player2);

    handlePlayerBombs(&player1);
    handlePlayerBombs(&player2);
}
