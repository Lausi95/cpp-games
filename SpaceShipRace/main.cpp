#include <vector>
#include "stdafx.h"

class Background : public Entity {
public:
    Background(float x, float y, Tile* tile) : Entity(x, y) {
        this->tile = tile;
    }
    Tile* getTile() {
        return tile;
    }
private:
    Tile* tile;
};

class BackgroundScroller {
public:
    BackgroundScroller(Tile* entity, int vx) {
        background1 = new Background(0, 0, entity);
        background2 = new Background(background1->getWidth(), 0, entity);
        this->vx = vx;
    }

    ~BackgroundScroller() {
        delete background1;
        delete background2;
    }

    void update() {
        background1->setX(background1->getX() - vx);
        background2->setX(background2->getX() - vx);
        resetIfOutOfScreen(background1);
        resetIfOutOfScreen(background2);
    }

    void render(Window* window) {
        window->renderEntity(background1);
        window->renderEntity(background2);
    }

private:
    Background* background1;
    Background* background2;
    int vx = 1;

    void resetIfOutOfScreen(Background* background) {
        if (background->getX() <= -background->getWidth()) {
            background->setX(background->getWidth());
        }
    }
};

class SpaceShip : public Entity {
public:
    SpaceShip(float x, float y, Tile* tile) : Entity(x, y) {
        this->tile = tile;
    }

    Tile* getTile() {
        return tile;
    }

private:
    Tile* tile;
};

class EnemySpaceShip : public SpaceShip {
public:
    EnemySpaceShip(float x, float y, Tile* tile) : SpaceShip(x, y, tile) {

    }

    void moveRandom() {

        bool moveX = (bool) (rand() % 2);
        bool moveY = (bool) (rand() % 2);

        //values same as players ship
        int xSpeed = 15;
        int ySpeed = 10;

        if(moveX) {
            bool moveLeft = (bool) (rand() % 2);

            if(moveLeft) {
                this->setX(this->getX() - xSpeed);
            }
            else {
                this->setX(this->getX() + xSpeed);
            }
        }

        if(moveY) {
            bool moveTop = (bool) (rand() % 2);

            if(moveTop) {
                this->setY(this->getY() - ySpeed);
            }
            else {
                this->setY(this->getY() + ySpeed);
            }
        }
    }

private:

};

int main(int argc, char** argv) {

    srand (time(NULL));

    Window window("SpaceShipRace", 800, 600);

    Texture texture_background("assets/background.png");
    Texture texture_spaceships("assets/spaceships.png");
    Texture texture_stars1("assets/stars1.png");
    Texture texture_stars2("assets/stars2.png");
    Texture texture_stars3("assets/stars3.png");

    Tile tile_background(&texture_background, 0, 0, 1024, 800);
    Tile tile_spaceship(&texture_spaceships, 0, 32, 32, 32);
    Tile tile_enemy_spaceship(&texture_spaceships, 0, 224, 32, 32);
    Tile tile_stars1(&texture_stars1, 0, 0, 800, 600);
    Tile tile_stars2(&texture_stars2, 0, 0, 800, 600);
    Tile tile_stars3(&texture_stars3, 0, 0, 800, 600);

    Background background(0, 0, &tile_background);

    BackgroundScroller scroller1(&tile_stars1, 5);
    BackgroundScroller scroller2(&tile_stars2, 4);
    BackgroundScroller scroller3(&tile_stars3, 2);

    std::vector<BackgroundScroller*> backgroundScroller;
    backgroundScroller.push_back(&scroller1);
    backgroundScroller.push_back(&scroller2);
    backgroundScroller.push_back(&scroller3);

    SpaceShip ship(32, 300, &tile_spaceship);

    EnemySpaceShip enemyShip1(736, 150, &tile_enemy_spaceship);
    EnemySpaceShip enemyShip2(736, 250, &tile_enemy_spaceship);
    EnemySpaceShip enemyShip3(736, 350, &tile_enemy_spaceship);
    EnemySpaceShip enemyShip4(736, 450, &tile_enemy_spaceship);

    std::vector<EnemySpaceShip*> enemies;
    enemies.push_back(&enemyShip1);
    enemies.push_back(&enemyShip2);
    enemies.push_back(&enemyShip3);
    enemies.push_back(&enemyShip4);


    InputHandler inputHandler;
    bool running = true;
    Timer timer;

    const int MAX_CYCLES = 5;
    int cyclesToWaitBeforeUpdateMap = MAX_CYCLES;

    while (running) {

        if(--cyclesToWaitBeforeUpdateMap == -1) {
            cyclesToWaitBeforeUpdateMap = MAX_CYCLES;
        }

        // render enteties
        window.renderEntity(&background);

        window.renderEntity(&ship);

        for(BackgroundScroller* scroller: backgroundScroller) {
            scroller->render(&window);
            scroller->update();
        }

        for(EnemySpaceShip* enemy: enemies) {
            window.renderEntity(enemy);
            if(cyclesToWaitBeforeUpdateMap == 0) {
                enemy->moveRandom();
            }
        }

        // input
        while (inputHandler.pollEvent()) {

            int xSpeed = 15;
            int ySpeed = 10;
            int framePixelBuffer = std::max(ySpeed, xSpeed);

            if (inputHandler.isKeyPressed(SDLK_w) && ship.getY() >= (0 + framePixelBuffer)) {
                ship.setY(ship.getY() - ySpeed);
            }
            if (inputHandler.isKeyPressed(SDLK_s) && (ship.getY() + ship.getHeight()) <= background.getHeight() - framePixelBuffer) {
                ship.setY(ship.getY() + ySpeed);
            }
            if (inputHandler.isKeyPressed(SDLK_a) && ship.getX() >= (0 + framePixelBuffer)) {
                ship.setX(ship.getX() - xSpeed);
            }
            if (inputHandler.isKeyPressed(SDLK_d) && (ship.getX() + ship.getWidth()) <= background.getWidth() - framePixelBuffer) {
                ship.setX(ship.getX() + xSpeed);
            }

            if (inputHandler.isQuitEvent()) {
                INFO("Quit event triggered")
                running = false;
            }
        }
        // update

        window.update();
        timer.sleep(8);
        timer.update();
    }

    return 0;
}
