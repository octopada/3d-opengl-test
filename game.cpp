#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include "game_level.h"
#include "game_object.h"

#include <iostream>
#include <sstream>

int cameraIndex = 4;

bool jumpready = false, win = false;

float unitt = 1.0f, adj = 0.055f, diff = 0.3f; 
float p_speed = 1.5f, p_ulimit = 5.0f, p_llimit = -5.0f, jump = 4.5f;
float b_ulimit = 2.0f, b_llimit = -2.0f; 
float gravity = 10.0f;

SpriteRenderer *Renderer;
TextRenderer *Text;
Camera* cam[5];

GameLevel first;
GameObject *player;

Game::Game(GLuint width, GLuint height) 
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Text;
    delete player;
}

void createPlayer()
{
    if (player != NULL)
    {
        delete player;
        player = NULL;
    }
    
    glm::vec3 pos(0, 1, 0);
    glm::vec3 size(unitt/2, unitt/2, unitt/2);

    player = new GameObject (-2, pos, size, ResourceManager::GetTexture("player"), 0);
}

void Game::Init(Camera* camera)
{
    // Load Shader
    ResourceManager::LoadShader("shaders/cube.v", "shaders/cube.f", nullptr, "cube");

    Renderer = new SpriteRenderer(ResourceManager::GetShader("cube"));
    Renderer->SetCamera(camera);

    // camera
    cam[0] = new Camera();
    cam[1] = new Camera();

    cam[2] = new Camera(glm::vec3(-1.0f, 10.0f, -1.0f)); // tower
    cam[2]->Yaw = 45.0f;
    cam[2]->Pitch = -60.0f;
    cam[2]->updateCameraVectors();

    cam[3] = new Camera(glm::vec3(5.0f, 20.0f, 5.0f)); // top
    cam[3]->Yaw = 45.0f;
    cam[3]->Pitch = -89.9f;
    cam[3]->updateCameraVectors();

    cam[4] = camera;

    // Load Textures
    ResourceManager::LoadTexture("textures/block.png", GL_TRUE, "floor");
    ResourceManager::LoadTexture("textures/mblock.png", GL_TRUE, "move");
    ResourceManager::LoadTexture("textures/player.png", GL_TRUE, "player");
    ResourceManager::LoadTexture("textures/end.png", GL_TRUE, "end");

    first.Load("levels/1.lvl");

    createPlayer(); 
}

void Game::Update(GLfloat dt) 
{
    CameraUpdate();
    if (player != NULL)
    {
        // gravity
        player->Pos.y += (player->Velocity.y * dt); 
        player->Velocity.y -= (gravity * dt); 

        // collisions
        for (auto &block : first.Blocks)
        {
            // Get center point circle first 
            glm::vec3 center (player->Pos.x, player->Pos.y, player->Pos.z);

            // Calculate AABB info (center, half-extents)
            glm::vec3 aabb_half_extents(block.Size.x/2, block.Size.y/2, block.Size.z/2);
            glm::vec3 aabb_center(block.Pos.x, block.Pos.y, block.Pos.z);

            // Get difference vector between both centers
            glm::vec3 difference = center - aabb_center;
            glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

            // Add clamped value to AABB_center and we get the value of box closest to circle
            glm::vec3 closest = aabb_center + clamped;
            difference = center-closest;

            glm::vec3 yaxis( 0.0, 1.0 ,0.0);
            glm::vec3 xaxis( 1.0, 0.0 ,0.0);
            glm::vec3 zaxis( 0.0, 0.0 ,1.0);

            float yot=glm::dot(difference, yaxis);
            float xot=glm::dot(difference, xaxis);
            float zot=glm::dot(difference, zaxis);
            
            if (glm::length(difference) < diff)
            {
                if (xot < 0)
                {
                    player->Pos.x = block.Pos.x - block.Size.x/2 - player->Size.x/2 - adj;
                }
                
                if (xot > 0)
                {
                    player->Pos.x = block.Pos.x + block.Size.x/2 + player->Size.x/2 + adj;
                }

                if (yot < 0)
                {
                    player->Pos.y = block.Pos.y - block.Size.y/2 - player->Size.y/2 - adj;
                    player->Velocity.y = 0;
                }
                
                if (yot > 0)
                {                   
                    player->Pos.y = block.Pos.y + block.Size.y/2 + player->Size.y/2 + adj;
                    player->Velocity.y = 0;
                    jumpready = true;
                }

                if (zot < 0)
                {
                    player->Pos.z = block.Pos.z - block.Size.z/2 - player->Size.z/2 - adj;
                }
                
                if (zot > 0)
                {
                    player->Pos.z = block.Pos.z + block.Size.z/2 + player->Size.z/2 + adj;
                }
                
                if (block.Tag == 3)
                {
                    win = true;
                }
            }
        }

        // out of bounds
        if (player->Pos.y < p_llimit || player->Pos.y > p_ulimit)
        {
            createPlayer();
        }
    }
        
    for (auto &block : first.Blocks) // blocks up and down
    {
        if (block.Tag == 2)
        {
            if ((block.Pos.y > b_ulimit) || (block.Pos.y < b_llimit)) // boundaries
            {
                block.Velocity.y *= -1;
                
                if (block.Pos.y > b_ulimit)
                {
                    block.Pos.y = b_ulimit;
                }
                else if (block.Pos.y < b_llimit)
                {
                    block.Pos.y = b_llimit;
                }
            }
            else if (rand()%10000 < 1) // random change of dir
            {
                block.Velocity.y *= -1;
            }               

            block.Pos.y += (block.Velocity.y * dt);
        }
    }
}

void Game::ProcessInput(GLfloat dt)
{
    if (player != NULL)
    {
        if (this->Keys[GLFW_KEY_UP])
        {
            player->Pos.x += (p_speed * dt);
        }

        if (this->Keys[GLFW_KEY_DOWN])
        {
            player->Pos.x -= (p_speed * dt);
        }

        if (this->Keys[GLFW_KEY_LEFT])
        {
            player->Pos.z -= (p_speed * dt);
        }

        if (this->Keys[GLFW_KEY_RIGHT])
        {
            player->Pos.z += (p_speed * dt);
        }

        if (this->Keys[GLFW_KEY_R]) // reset
        {
            if (win)
            {
                std::cout << "You win! Press ESC to exit.\n";
            }
            else
            {
                createPlayer();
            }
        }

        if (this->Keys[GLFW_KEY_SPACE]) // jump
        {
            if (jumpready)
            {
                player->Velocity.y += jump;
                jumpready = false;
            }
        }

        if (this->Keys[GLFW_KEY_Q]) // change camera
        {
            cameraIndex = (cameraIndex + 1) % 5;
            Renderer->SetCamera(cam[cameraIndex]);
            this->Keys[GLFW_KEY_Q] = false;
        }

        if (this->Keys[GLFW_KEY_F])
        {
            p_speed = 1.5f;
        }

        if (this->Keys[GLFW_KEY_G])
        {
            p_speed = 0.75f;
        }
    }
}

void Game::CameraUpdate()
{
    cam[0]->Position = player->Pos + glm::vec3(1.0f, 0.0f, 0.0f); // adventure
    cam[0]->Yaw = cam[0]->Pitch = 0.0f;
    cam[0]->updateCameraVectors();
    
    cam[1]->Position = player->Pos - glm::vec3(2.0f, -2.0f, 2.0); // follow
    cam[1]->Pitch = -45.0f;
    cam[1]->Yaw = 45.0f;
    cam[1]->updateCameraVectors();
}

void Game::Render()
{
    first.Draw(*Renderer);
    
    if (player != NULL)
    {
        player->Draw(*Renderer);
    }
}
