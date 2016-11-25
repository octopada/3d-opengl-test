#include "game_level.h"

#include <iostream>
#include <fstream>
#include <sstream>

float unit = 1.0f, b_speed = 1.5f; 

GameLevel::GameLevel()
    : Blocks() {}

void GameLevel::Load(const GLchar *file)
{
    // Clear old data
    this->Blocks.clear();
    // Load from file
    GLuint blockCode;

    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLuint>> blockMatrix;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> blockCode) // Read each word seperated by spaces
                row.push_back(blockCode);
            blockMatrix.push_back(row);
        }
        if (blockMatrix.size() > 0)
            this->init(blockMatrix);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &block : this->Blocks)
    { 
        if (!block.DontDraw)
        {            
            block.Draw(renderer);
        }
    }
}

void GameLevel::init(std::vector<std::vector<GLuint>> blockMatrix)
{
    for (GLuint x = 0; x < 10; ++x)
    {
        for (GLuint y = 0; y < 10; ++y)
        {
            if (blockMatrix[x][y] == 1) // Normal Block
            {
                glm::vec3 pos(x*unit, 0*unit, y*unit);
                glm::vec3 size(unit, unit, unit);
                GameObject block (blockMatrix[x][y], pos, size, ResourceManager::GetTexture("floor"), 0);
                this->Blocks.push_back(block);
            }
            else if (blockMatrix[x][y] == 2) // Moving Block
            {
                glm::vec3 pos(x*unit, 0*unit, y*unit);
                glm::vec3 size(unit, unit, unit);
                glm::vec3 vel(0, b_speed, 0);
                GameObject block (blockMatrix[x][y], pos, size, ResourceManager::GetTexture("move"), 0, vel);
                this->Blocks.push_back(block);
            }
            else if (blockMatrix[x][y] == 3) // Final Block
            {
                glm::vec3 pos(x*unit, 0*unit, y*unit);
                glm::vec3 size(unit, unit, unit);
                GameObject block (blockMatrix[x][y], pos, size, ResourceManager::GetTexture("end"), 0);
                this->Blocks.push_back(block);
            }
        }
    }
}
