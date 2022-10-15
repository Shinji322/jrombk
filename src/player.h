#define PLAYER_SPRITE '#'

typedef struct Player {
    char sprite;
    float health;
    float speed;
    int y;
    int x;
} Player;


typedef enum MoveDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT
} MoveDirection;

