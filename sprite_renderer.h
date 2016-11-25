#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "camera.h"


class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer(const Shader &shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(const Texture2D &texture, glm::vec3 position, glm::vec3 size);
    void SetCamera(Camera* cam);
private:
    // Render state
    Shader shader; 
    GLuint VAO;
    Camera* camera;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif
