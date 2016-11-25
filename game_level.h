#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

class GameLevel
{
public:
    std::vector<GameObject> Blocks;

    GameLevel();

    void Load(const GLchar *file);

    void Draw(SpriteRenderer &renderer);
private:
    void init(std::vector<std::vector<GLuint>> blockMatrix);
};

#endif

