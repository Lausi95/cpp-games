#include <World.h>
#include <Input.h>
#include <entities/SimpleEntity.h>
#include <entities/Player.h>
#include <state/Worlds.h>
#include <BackgroundMusic.h>


#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

Worlds currentWorld = WORLD_LEVEL_1;

class Level : public World {
    Player* _player;
    Entity* _boundingEntity;
    Input*  _input;

public:
    Level(Camera* camera, const std::vector<Entity*>& entities, Player* player, Entity* boundingEntity, Input* input) : World(camera, entities) {
        _player = player;
        _boundingEntity = boundingEntity;
        _input = input;
    }

    void initialize() {
    }

    void update(float dt) {
        if (_input->isADown())
            _player->setVx(-1.0f);
        else if (_input->isDDown())
            _player->setVx(1.0f);
        else
            _player->setVx(0.0f);

        if (_input->isWDown() && _player->canJump())
            _player->jump();

        _player->update();

        if (_player->getBottom() >= _boundingEntity->getBottom()) {
            _player->setVy(0);
            _player->enableJump();
            _player->setBottom(_boundingEntity->getBottom());
        } else {
            _player->addVy(0.2f);
        }

        if (_player->getLeft() < _boundingEntity->getLeft())
            _player->setLeft(0.0f);
        if (_player->getRight() > _boundingEntity->getRight())
            _player->setRight(_boundingEntity->getRight());

        lockOn(_player);
    }
};

Camera camera(800, 1000, "You May Not Touch The Ground");
Input input;

Texture texture("assets/images/backgrounds/sky.png");
Tile tile(&texture, 0, 0, 800, 608);
SimpleEntity entity(&tile, 0, 0);


Texture playerBoyLeftTexture("assets/images/tiles/player_boy_left.png");
Tile    playerBoyLeftTile(&playerBoyLeftTexture, 0, 0, 32, 32);
Texture playerBoyRightTexture("assets/images/tiles/player_boy_right.png");
Tile    playerBoyRightTile(&playerBoyRightTexture, 0, 0, 32, 32);
std::array<Tile*, 2> playerTiles {&playerBoyRightTile, &playerBoyLeftTile};
Player player(playerTiles, 0, 0, 32, 32);

std::vector<Entity*> e {&entity, &player};
Level world(&camera, e, &player, &entity, &input);

World* worlds[] {
    &world,
    &world
};

int main() {

    BackgroundMusic backgroundMusic(MP3, "assets/music/Spectra.mp3");
    backgroundMusic.init();

    backgroundMusic.start();

    do {
        input.update();
        Worlds world = currentWorld;

        if (!worlds[world]->initialized()) {
            worlds[world]->initialize();
            worlds[world]->setInitialized(true);
        }

        worlds[world]->update(1.0f /*Unit, as long as the time since last frame is not calculated*/);
        worlds[world]->draw();

        if (world != currentWorld) {
            worlds[currentWorld]->setInitialized(false);
        }
    } while (!input.quitTriggered());

    backgroundMusic.stop();
    backgroundMusic.cleanUp();
}
