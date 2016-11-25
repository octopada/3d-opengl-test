#include "game_object.h"

GameObject::GameObject()
    : Tag(-1), Pos(0, 0, 0), Size(1, 1, 1), Velocity(0, 0, 0), Texture(), DontDraw(0) {}

GameObject::GameObject(int tag, glm::vec3 pos, glm::vec3 size, Texture2D texture, GLboolean dontdraw, glm::vec3 velocity)
    : Tag(tag), Pos(pos), Size(size), Texture(texture), DontDraw(dontdraw), Velocity(velocity) {}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Texture, this->Pos, this->Size);
}
