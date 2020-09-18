#include "GameLevel.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    /* Clear old data */
    Bricks.clear();
    /* load from file */
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        /* Read each line from level file */
        while (std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            /* Read each word seperated by spaces */
            while (sstream >> tileCode)
            {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
        {
            init(tileData, levelWidth, levelHeight);
        }
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (GameObject& tile : Bricks)
    {
		if (!tile.Destroyed) tile.Draw(renderer);
	}
}

bool GameLevel::IsCompleted()
{
    for (GameObject& tile : Bricks)
    {
        if (!tile.IsSolid && !tile.Destroyed) return false;
	}
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    /* Calculate dimensions */
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // We can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width); // Fix for right edge
	float unit_height = levelHeight / height;
	
    /* Initialize level tiles based on tileData */
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            /* Check block type from level data (2D level array) */
            if (tileData[y][x] == 1) // Solid
            {
                glm::vec2 position(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(position, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.8f));
                obj.IsSolid = true;
                Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1) // Non-solid; Needs to determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // Original: white
                if (tileData[y][x] == 2) color = glm::vec3(0.15f, 0.15f, 0.95f); // Blue
                else if (tileData[y][x] == 3) color = glm::vec3(0.0f, 0.9f, 0.0f); // Green
                else if (tileData[y][x] == 4) color = glm::vec3(0.9f, 0.9f, 0.2f); // Yelow
                else if (tileData[y][x] == 5) color = glm::vec3(0.8f, 0.1f, 0.1f); // Red

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                Bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
}