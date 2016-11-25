#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

class GameObject
{
public:
    int Tag;

    glm::vec3 Pos, Size, Velocity;
    
    Texture2D Texture;

    GLboolean DontDraw;    
    
    GameObject();
    GameObject(int tag, glm::vec3 pos, glm::vec3 size, Texture2D texture, GLboolean dontdraw, glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));

    virtual void Draw(SpriteRenderer &renderer);
};

#endif 
