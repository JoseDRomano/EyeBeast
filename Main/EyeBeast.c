/*
 max width = 80 columns
 tab = 4 spaces
 01234567890123456789012345678901234567890123456789012345678901234567890123456789
 */

/*  Linguagens e Ambientes de Programação - Projeto de 2021/2022

 Eye Beast

 Program written in C/C ++ over the wxWidget platform.
 The wxWidget platform runs on Windows, MacOS and Linux.

 This file is only a starting point fo your work. The entire file can
 be changed, starting with this comment.


 AUTHORS IDENTIFICATION
 Student 1: 59241, José Diogo Teotónio Pinto Romano
 Student 2: 60291, Gonçalo Pereira Duarte

 Comments:
 In this project, besides the code we were given we added some variables to
 the actor structure, such as pushable, to give information if the actor is
 pushable by the hero or not, and evil, if the actor can kill the hero.
 This helps making the code more extensible.

 The monsters are also moderately smart, and this is due to, when stuck,
 they will move to a random position, and since in every position they will
 evaluate what direction to make towards the hero, this random positioning
 may or may not unstuck them and give a clear sight.

 For the extra we decided to implement the second option: the level system.
 - There are 7 levels;
 - When a level is passed, a window pops-up alerting the level is complete,
 and upon closing it, the next level starts immediately with one more monster;
 - When completing the seventh level, a window pops-up alerting the game
 has been completed, and when closed, the game closes;
 - If the player is caught or runs into a monster, a window shows telling the
 player he got caught, and when closed it restarts the game at level 1.

 Place here the names and numbers of the authors, plus some comments, as
 asked in the listing of the project. Do not deliver an anonymous file with
 unknown authors.
 */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* IDENTIFICATION */

#define APP_NAME    "Eye Beast"

#define AUTHOR1     "Gonçalo Duarte (60291)"
#define AUTHOR2     "José Romano (59241)"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* INCLUDES */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "wxTiny.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* STRINGS */

#define MAX_STRING  256
#define MAX_LINE    1024

typedef char String[MAX_STRING];
typedef char Line[MAX_LINE];

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* IMAGES */

/******************************************************************************
 * The images are specified in the XPM format [http://en.wikipedia.org/wiki/X_PixMap]
 * After created, each image is represented by an integer in the wxTiny library.
 ******************************************************************************/

typedef int Image;

static Image emptyImg, heroImg, chaserImg, blockImg, boundaryImg, invalidImg;

/* XPM */
static tyImage empty_xpm = { "16 16 2 1", "   c None", ".  c #FFFFFF",
        "................", "................", "................",
        "................", "................", "................",
        "................", "................", "................",
        "................", "................", "................",
        "................", "................", "................",
        "................" };

/* XPM */
static tyImage hero_xpm = { "16 16 3 1", "   c None", ".  c #FFFFFF",
        "+  c #000000", "................", "...+++...+++....",
        "..+...+.+...+...", ".+.....+.....+..", ".+...+++++...+..",
        ".+...+++++...+..", ".+...+++++...+..", ".+.....+.....+..",
        "..+...+.+...+...", "...+++...+++....", "................",
        ".....+++++......", "....+.....+.....", "................",
        "................", "................" };

/* XPM */
static tyImage chaser_xpm = { "16 16 3 1", "   c None", ".  c #FFFFFF",
        "+  c #000000", "................", "...+++...+++....",
        "..++.++.+.+++...", ".+..+++++++..+..", ".+...+++++...+..",
        ".+...+++++...+..", ".+...+++++...+..", "..+...+.+...+...",
        "...+++...+++....", "................", "................",
        "....+++++++.....", "................", "................",
        "................", "................" };

/* XPM */
static tyImage block_xpm = { "16 16 3 1", "   c None", ".  c #000000",
        "+  c #FFFFFF", "................", "................",
        "..............+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
        "....+.+.+.+.+...", "...+.+.+.+.+..+.", "....+.+.+.+.+...",
        "...+.+.+.+.+..+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
        "....+.+.+.+.+...", "...+.+.+.+.+..+.", "................",
        "..+.+.+.+.+.+.+.", "................" };

/* XPM */
static tyImage boundary_xpm = { "16 16 3 1", "   c None", ".  c #000000",
        "+  c #FFFFFF", "................", "................",
        "..............+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
        "....+.+.+.+.+...", "...+.+.+.+.+..+.", "....+.+.+.+.+...",
        "...+.+.+.+.+..+.", "....+.+.+.+.+...", "...+.+.+.+.+..+.",
        "....+.+.+.+.+...", "...+.+.+.+.+..+.", "................",
        "..+.+.+.+.+.+.+.", "................" };

/* XPM */
static tyImage invalid_xpm = { "16 16 2 1", "   c None", ".  c #FFFFFF",
        "................", "................", "................",
        "................", "................", "................",
        "................", "................", "................",
        "................", "................", "................",
        "................", "................", "................",
        "................" };

/******************************************************************************
 * imagesCreate - Converts all the XPM images to the type Image
 ******************************************************************************/
void imagesCreate(void) {
    emptyImg = tyCreateImage(empty_xpm);
    heroImg = tyCreateImage(hero_xpm);
    chaserImg = tyCreateImage(chaser_xpm);
    blockImg = tyCreateImage(block_xpm);
    boundaryImg = tyCreateImage(boundary_xpm);
    invalidImg = tyCreateImage(invalid_xpm);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* ACTORS */

/******************************************************************************
 * Constants, types and functions for all kinds of actors
 ******************************************************************************/

#define ACTOR_PIXELS_X  16
#define ACTOR_PIXELS_Y  16
int counter = 0;
int level = 0;
int gameStatus = 0;

typedef enum {
    EMPTY, HERO, CHASER, BLOCK, BOUNDARY
} ActorKind;

typedef struct {
// specific fields can go here, but probably none will be needed
} Hero;

typedef struct {
// specific fields can go here, but probably none will be needed
} Chaser;

typedef struct {
// specific fields can go here, but probably none will be needed
} Block;

typedef struct {
// specific fields can go here, but probably none will be needed
} Boundary;

typedef struct {
// factored common fields
    ActorKind kind;
    Image image;
    bool pushable;
    bool isEvil;
    int x, y;
    union {
// specific fields for each kind
        Hero hero;
        Chaser chaser;
        Block block;
        Boundary boundary;
    } u;
} ActorStruct, *Actor;


#define WORLD_SIZE_X    31
#define WORLD_SIZE_Y    18
#define N_MONSTERS      5
#define MAXMONSTERS     12

typedef struct {
    Actor world[WORLD_SIZE_X][WORLD_SIZE_Y];
    Actor hero;
    Actor monsters[MAXMONSTERS];
} GameStruct, *Game;

/******************************************************************************
 * actorImage - Get the screen image corresponding to some kind of actor
 ******************************************************************************/
Image actorImage(ActorKind kind) {
    switch (kind) {
        case EMPTY:
            return emptyImg;
        case HERO:
            return heroImg;
        case CHASER:
            return chaserImg;
        case BLOCK:
            return blockImg;
        case BOUNDARY:
            return boundaryImg;
        default:
            return invalidImg;
    }
}

/******************************************************************************
 * cellSet - Useful function to update one cell in the matrix and in the screen
 ******************************************************************************/
void cellSet(Game g, Actor a, int x, int y, Image img) {
    tyDrawImage(img, x * ACTOR_PIXELS_X, y * ACTOR_PIXELS_Y);
    g->world[x][y] = a;
}

/******************************************************************************
 * cellIsEmpty - Check in the matrix if a cell is empty (if contains NULL)
 ******************************************************************************/
bool cellIsEmpty(Game g, int x, int y) {
    return g->world[x][y] == NULL;
}

/******************************************************************************
 * actorShow - Install an actor in the matrix and in the screen
 ******************************************************************************/
void actorShow(Game g, Actor a) {
    cellSet(g, a, a->x, a->y, a->image);
}

/******************************************************************************
 * actorHide - Remove an actor from the matrix and from the screen
 ******************************************************************************/
void actorHide(Game g, Actor a) {
    cellSet(g, NULL, a->x, a->y, emptyImg);
}

/******************************************************************************
 * actorMove - Move an actor to a new position
 * pre: the new position is empty
 ******************************************************************************/
void actorMove(Game g, Actor a, int nx, int ny) {
    actorHide(g, a);
    a->x = nx;
    a->y = ny;
    actorShow(g, a);
}

/******************************************************************************
 * isPushable - Verifies if the actor a is pushable by the hero.
 ******************************************************************************/
bool isPushable(Actor a) {
    return a->pushable;
}

/******************************************************************************
 * isEvil - Verifies if the actor can kill the hero.
 ******************************************************************************/
bool isEvil(Actor a){
    return a->isEvil;
}

/******************************************************************************
 * isBlockMovable - Verifies if a block or row of blocks is/are movable.
 ******************************************************************************/
bool isBlockMovable(Game g, int x, int y, int dx, int dy) {
    int ax = x + dx;
    int ay = y + dy;
    if(!cellIsEmpty(g, ax, ay) && isPushable(g->world[ax][ay]))
        return isBlockMovable(g, ax, ay, dx, dy);
    return cellIsEmpty(g, ax, ay);
}

/******************************************************************************
 * moveBlocks - Moves the block or row of blocks in the
 * direction in which the hero is going.
 ******************************************************************************/
void moveBlocks(Game g, int x, int y, int dx, int dy) {
    int ax = x + dx;
    int ay = y + dy;
    if(!cellIsEmpty(g, ax, ay) && isPushable(g->world[ax][ay]))
        moveBlocks(g, ax, ay, dx, dy);
    actorMove(g, g->world[x][y], ax, ay);
}

/******************************************************************************
 * actorNew - Creates a new actor and installs it in the matrix and the screen
 ******************************************************************************/
Actor actorNew(Game g, ActorKind kind, int x, int y, bool push, bool evil) {
    Actor a = malloc(sizeof(ActorStruct));
    a->kind = kind;
    a->x = x;
    a->y = y;
    a->image = actorImage(kind);
    a->pushable=push;
    a->isEvil = evil;
    actorShow(g, a);
    return a;
}

/******************************************************************************
 * heroAnimation - The hero moves using the cursor keys
 ******************************************************************************/
void heroAnimation(Game g, Actor a) {
    int dx = tyKeyDeltaX(), dy = tyKeyDeltaY();
    int nx = a->x + dx, ny = a->y + dy;
    if (cellIsEmpty(g, nx, ny))
        actorMove(g, a, nx, ny);
    else if(isEvil(g->world[nx][ny]))
        gameStatus = -1;
    else if (isPushable(g->world[nx][ny]) &&
    isBlockMovable(g, nx, ny, dx, dy)) {
        moveBlocks(g, nx, ny, dx, dy);
        actorMove(g, a, nx, ny);
    }
}

/******************************************************************************
 * canChaserMove - checks if the chaser can move to the hero
 ******************************************************************************/

bool canChaserMove(Game g, Actor a) {
    int cx = a->x;
    int cy = a->y;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (cellIsEmpty(g, cx + i, cy + j) || g->world[cx+i][cy+j] == g->hero)
                return true;
        }
    }
    return false;
}

/******************************************************************************
 * moveX and moveY - These two auxiliar methods were made to complement the
 * chaserAnimation method. They receive two integers that are the
 * coordinates - in case of moveX is the x coordinate and in moveY is the
 * y coordinate - one from the chaser and one from the hero, respectively,
 * and by comparing them they give the considered "best" option the chaser
 * could do in order to catch the hero.
 ******************************************************************************/

/******************************************************************************
 * moveX - Return the direction of the hero according to the X axis, this is,
 * where the chaser needs to move towards to get to him.
 ******************************************************************************/

int moveX(int cx, int hx) {
    if (cx < hx)
        return 1;
    else if (cx > hx)
        return -1;
    else
        return 0;
}

/******************************************************************************
 * moveY - Return the direction of the hero according to the Y axis, this is,
 * where the chaser needs to move towards to get to him.
 ******************************************************************************/
int moveY(int cy, int hy) {
    if (cy < hy)
        return 1;
    else if (cy > hy)
        return -1;
    return 0;
}

/******************************************************************************
 * moveChaserRandom - Code to randomize the movements
 * of the chasers when they can't get to the hero.
 ******************************************************************************/
void moveChaserRandom(Game g, Actor a) {
    int cx = a->x;
    int cy = a->y;
    int dx = tyRand(2) - 1;
    int dy = tyRand(2) - 1;

    if(cellIsEmpty(g,cx + dx, cy + dy))
        actorMove(g, a, cx + dx, cy + dy);
}

/******************************************************************************
 * chaserAnimation - Animates the movements of the chaser towards the hero,
 * and, if not possible, it randomizes them.
 ******************************************************************************/
void chaserAnimation(Game g, Actor a) {
    int cx = a->x;
    int cy = a->y;
    int hx = g->hero->x;
    int hy = g->hero->y;
    int dx = moveX(cx, hx);
    int dy = moveY(cy, hy);

    if(!cellIsEmpty(g, cx + dx, cy + dy) && g->world[cx-dx][cy-dy] == g->hero) {
        gameStatus = -1;
        actorMove(g, a, cx + dx, cy + dy);
    }
    else if (cellIsEmpty(g, cx + dx, cy + dy))
        actorMove(g, a, cx + dx, cy + dy);
    else
        moveChaserRandom(g, a);
}

/******************************************************************************
 * actorAnimation - The actor behaves according to its kind
 ******************************************************************************/
void actorAnimation(Game g, Actor a) {
    switch (a->kind) {
        case HERO:
            heroAnimation(g, a);
            break;
        case CHASER:
            if(counter%10 == 0)
                chaserAnimation(g, a);
            break;
        default:
            break;
    }
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* GAME */

/******************************************************************************
 * gameClearWorld - Clear the matrix and the screen
 ******************************************************************************/
void gameClearWorld(Game g) {
    for (int y = 0; y < WORLD_SIZE_Y; y++)
        for (int x = 0; x < WORLD_SIZE_X; x++)
            cellSet(g, NULL, x, y, emptyImg);
}

/******************************************************************************
 * gameInstallBoundaries - Install the boundary blocks
 ******************************************************************************/
void gameInstallBoundaries(Game g) {
    for (int y = 0; y < WORLD_SIZE_Y; y++)
        for (int x = 0; x < WORLD_SIZE_X; x++)
            if (x == 0 || x == WORLD_SIZE_X - 1 || y == 0
                    || y == WORLD_SIZE_Y - 1) {
                actorNew(g, BOUNDARY, x, y, false, false);
            }
}

/******************************************************************************
 * gameInstallBlocks - Install the movable blocks
 ******************************************************************************/
void gameInstallBlocks(Game g) {
    int x = 0;
    int y = 0;
    for (int i = 0; i < 110; i++) {
        do {
            x = tyRand(31);
            y = tyRand(17);
        } while (!cellIsEmpty(g, x, y));
        actorNew(g, BLOCK, x, y, true, false);
    }
}

/******************************************************************************
 * chaserHasDistance - Verifies if the distance of the chaser is
 * 4 or more blocks away from the hero (spawn purposes).
 ******************************************************************************/
bool chaserHasDistance(Game g, int x, int y) {
    int hx = g->hero->x;
    int hy = g->hero->y;
    return tyDistance(x, y, hx, hy) > 4;
}

/******************************************************************************
 * gameInstallMonsters - Install the monsters
 * INCOMPLETE!
 ******************************************************************************/
void gameInstallMonsters(Game g) {
    int x = 0;
    int y = 0;
    for (int i = 0; i < N_MONSTERS+level; i++) {
        do {
            x = tyRand(31);
            y = tyRand(17);
        } while (!(cellIsEmpty(g, x, y) && chaserHasDistance(g, x, y)));
        g->monsters[i] = actorNew(g, CHASER, x, y, false, true);
    }
}

/******************************************************************************
 * gameInstallHero - Install the hero
 * INCOMPLETE! This code is to change
 ******************************************************************************/
void gameInstallHero(Game g) {
    int x,y= 0;
    do {
        x = tyRand(31);
        y = tyRand(17);
    } while (!cellIsEmpty(g, x, y));
    g->hero = actorNew(g, HERO, x, y, false, false);
}

/******************************************************************************
 * gameInit - Initialize the matrix and the screen
 ******************************************************************************/
Game gameInit(Game g) {
    if (g == NULL)
        g = malloc(sizeof(GameStruct));
    imagesCreate();
    gameClearWorld(g);
    gameInstallBoundaries(g);
    gameInstallBlocks(g);
    gameInstallHero(g);
    gameInstallMonsters(g);
    return g;
}

/******************************************************************************
 * gameRedraw - Redraw the entire scenario. This function is called by
 * tyHandleRedraw in very specific circumstances. It should not be used anywhere
 * else because you don't want to be constantly redrawing the whole scenario.
 ******************************************************************************/
void gameRedraw(Game g) {
    for (int y = 0; y < WORLD_SIZE_Y; y++)
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            Actor a = g->world[x][y];
            if (!cellIsEmpty(g, x, y))
                actorShow(g, a);
        }
}

/******************************************************************************
 * winBox - Prints a box with a message when the player passed a x level.
 ******************************************************************************/
void winBox(Game g){
    tyAlertDialog("Congratulations!", "You trapped all the monsters in level %d!", level+1);
}

/******************************************************************************
 * wingGameBox - Shows the box when the game has been fully completed.
 ******************************************************************************/
void winGameBox(Game g){
tyAlertDialog("Congratulations!", "You beat the game!");
}

/******************************************************************************
 * loseBox - Prints a box with a message when the player loses.
 ******************************************************************************/
void loseBox(Game g){
    tyAlertDialog("DEAD MEAT!", "The monsters have got you...");
}

/******************************************************************************
 * gameAnimation - based on the current gameStatus, this method will act
 * as a manager as to what should happen in every game situation (if the player
 * either loses, wins  or is still playing).
 ******************************************************************************/
void gameAnimation(Game g) {
    int trapped = 0;
    switch(gameStatus){
        //win
        case 1:
            if(level != 7) {
                winBox(g);
                gameRedraw(g);
                level++;
                comandRestart();
                gameStatus = 0;
            } else {
                winGameBox(g);
                tyQuit();
            }
            break;
    //during game
        case 0:
            actorAnimation(g, g->hero);
            for (int i = 0; i < N_MONSTERS+level; i++){
                if(!canChaserMove(g, g->monsters[i]))
                    trapped++;
                actorAnimation(g, g->monsters[i]);
            }
            if(trapped == N_MONSTERS+level)
                gameStatus = 1;
            break;
    //lose
        case -1:
            loseBox(g);
            level = 0;
            comandRestart();
            gameStatus = 0;
            break;
    }
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* STATUS BAR */

/******************************************************************************
 * The function status populates the status bar, at the bottom of the window
 ******************************************************************************/

#define STATUS_ITEMS    5

void status(void) {
    String s;
    String l;
    sprintf(l, "LEVEL = %d", level+1);
    sprintf(s, "TIME = %d seg.", tySeconds());
    tySetStatusText(0, l);
    tySetStatusText(4, s);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* MENU COMMANDS */

/******************************************************************************
 * Each function handles one menu command
 ******************************************************************************/

void comandAbout(void) {
    tyAlertDialog("About", "%s\n%s\n\n%s\n%s",
    APP_NAME, "(Reimplementation of the original game of Chuck Shotton)",
    AUTHOR1,
    AUTHOR2);
}

void comandRestart(void) {
    tyHandleStart();
}

void comandFinish(void) {
    tyAlertDialog("Quit", "See you later!");
    tyQuit();
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* FUNCTIONS REQUIRED BY THE FRAMEWORK wxTiny */

/******************************************************************************
 * Section with all the "root" "functions required by the wxTiny support system.
 *
 * There is no function "main" in a wxTiny program, as the main function is
 * hidden within the wxTiny library. The most important thing you need to know
 * is that the function "tyHandleStart" is called when the program begins,
 * and that function "tyHandleTime" is called periodically 10 times per second,
 * to make the program progress. In some sense, this pair of functions makes a kind
 * of main function.
 *
 * More information about the wxTiny functions in the file "wxTiny.h".
 ******************************************************************************/

static Game game = NULL; // defined here to be available to the "root" functions

/******************************************************************************
 * tyAppName - specify the name of the app
 ******************************************************************************/
tyStr tyAppName(void) {
    return APP_NAME;
}

/******************************************************************************
 * tyWidth - specify the width of the window
 ******************************************************************************/
int tyWidth(void) {
    return WORLD_SIZE_X * ACTOR_PIXELS_X;
}

/******************************************************************************
 * tyHeight - specify the height of the window
 ******************************************************************************/
int tyHeight(void) {
    return WORLD_SIZE_Y * ACTOR_PIXELS_Y;
}

/******************************************************************************
 * tyStatusItems - specify the number of slots available in the status bar
 ******************************************************************************/
int tyStatusItems(void) {
    return STATUS_ITEMS;
}

/******************************************************************************
 * tyMenus - specify the menus
 ******************************************************************************/
tyStr tyMenus(void) {
    return ":+Menu"
            ":&About\tCtrl-A"
            ":-"
            ":&Restart\tCtrl-R"
            ":-"
            ":&Quit\tCtrl-Q";
}

/******************************************************************************
 * tyHandleMenuCommand - Handle the menu commands
 ******************************************************************************/
void tyHandleMenuCommand(tyStr command) {
    if (strcmp(command, "About") == 0)
        comandAbout();
    else if (strcmp(command, "Restart") == 0)
        comandRestart();
    else if (strcmp(command, "Quit") == 0)
        comandFinish();
    else
        tyFatalError("Unknown command: \"%s\"", command);
}

/******************************************************************************
 * tyHandleRedraw - redraw the window
 *
 * Automatically called in rare situations, for example if the game window is
 * minimized and then maximized
 ******************************************************************************/
void tyHandleRedraw(void) {
    gameRedraw(game);
}

/******************************************************************************
 * tyHandleTime - Called periodically, around 10 time per second
 *
 * This function does the following: animate the actors; update the status bar;
 *  possibly more things
 ******************************************************************************/
void tyHandleTime(void) {
    status();
    counter++;
    gameAnimation(game);
}

/******************************************************************************
 * tyHandleStart - Program initialization
 ******************************************************************************/
void tyHandleStart(void) {
    tySecondsSetZero();
    game = gameInit(game);
}