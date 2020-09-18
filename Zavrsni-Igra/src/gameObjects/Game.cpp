#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "ParticleGenerator.h"

/* Game-related State data */
SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
std::vector<Texture> Backgrounds;

/* Initial size of the player paddle */
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
/* Initial velocity of the player paddle */
const float PLAYER_VELOCITY(500.0f);

/* Initial velocity of the Ball */
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
/* Radius of the ball object */
const float BALL_RADIUS = 12.5f;

Game::Game(unsigned int width, unsigned int height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
}

void Game::Init()
{
    /* Load shaders */
    ResourceManager::LoadShader("res/shaders/VertexShader.glsl", "res/shaders/FragmentShader.glsl", "sprite");
    ResourceManager::LoadShader("res/shaders/VertexShaderParticle.glsl", "res/shaders/FragmentShaderParticle.glsl", "particle");
	
    /* Configure shaders */
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Bind();
    ResourceManager::GetShader("sprite").SetUniform1i("u_Image", 0);
    ResourceManager::GetShader("sprite").SetUniformMat4f("u_Projection", projection);
    ResourceManager::GetShader("particle").Bind();
    ResourceManager::GetShader("particle").SetUniform1i("u_Sprite", 0);
    ResourceManager::GetShader("particle").SetUniformMat4f("u_Projection", projection);
	
    /* Load textures */
    //ResourceManager::LoadTexture("res/textures/awesomeface.png", "face");
    ResourceManager::LoadTexture("res/textures/ball.png", "ball");

    ResourceManager::LoadTexture("res/textures/background.jpg", "background");
    ResourceManager::LoadTexture("res/textures/background1.jpg", "background1");
    ResourceManager::LoadTexture("res/textures/background2.jpg", "background2");
    ResourceManager::LoadTexture("res/textures/background3.jpg", "background3");
    ResourceManager::LoadTexture("res/textures/background4.jpg", "background4");

    ResourceManager::LoadTexture("res/textures/particleGreen.png", "particleGreen");
    ResourceManager::LoadTexture("res/textures/particleRed.png", "particleRed");
    ResourceManager::LoadTexture("res/textures/block.png", "block");
    ResourceManager::LoadTexture("res/textures/block_solid.png", "block_solid");
    ResourceManager::LoadTexture("res/textures/paddle.png", "paddle");

    /* Set render-specific controls */
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particleGreen"), 1000);

    Backgrounds.push_back(ResourceManager::GetTexture("background1"));
    Backgrounds.push_back(ResourceManager::GetTexture("background2"));
    Backgrounds.push_back(ResourceManager::GetTexture("background3"));
    Backgrounds.push_back(ResourceManager::GetTexture("background4"));

    /* Load levels */
    GameLevel one;
	one.Load("src/levels/one.lvl", Width, Height / 2);
    GameLevel two;
	two.Load("src/levels/two.lvl", Width, Height / 2);
    GameLevel three;
	three.Load("src/levels/three.lvl", Width, Height / 2);
    GameLevel four;
	four.Load("src/levels/four.lvl", Width, Height / 2);

    Levels.push_back(one);
    Levels.push_back(two);
    Levels.push_back(three);
    Levels.push_back(four);
    Level = 0;

    /* Configure game objects */
    glm::vec2 playerPosition = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y * 1.5f);
    Player = new GameObject(playerPosition, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPosition = playerPosition + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPosition, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
}

void Game::Update(float deltaTime)
{
	/* Update objects */
    Ball->Move(deltaTime, Width);
	
    /* Check for collisions */
    DoCollisions();

    /* Update particles */
    Particles->Update(deltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));
	
    /* Check for GAME LOSE condition */
    if (Ball->Position.y >= Height) // Ball reached bottom edge
    {
        ResetLevel();
        ResetPlayer();
    }
}

void Game::ProcessInput(float deltaTime)
{
    if (State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * deltaTime;
        /* Move player */
        if (Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck) Ball->Position.x -= velocity;
            }
        }
        if (Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= (Width - Player->Size.x))
            {
                Player->Position.x += velocity;
                if (Ball->Stuck) Ball->Position.x += velocity;
            }
        }
    	/* Unstick ball with space bar */
        if (Keys[GLFW_KEY_SPACE]) Ball->Stuck = false;
    	
    	/* VERTICAL MOVEMENT - FOR TESTING
        if (Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 0.0f)
            {
                Player->Position.y -= velocity;
            }
        }
        if (Keys[GLFW_KEY_S])
        {
            if (Player->Position.y <= (Height - Player->Size.y))
            {
                Player->Position.y += velocity;
            }
        }
    	*/
    }
}

void Game::Render()
{
    if (State == GAME_ACTIVE)
    {
        /* Draw background */
        Renderer->DrawSprite(Backgrounds[Level], glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), 0.0f);
        /* Draw level */
        Levels[Level].Draw(*Renderer);
        /* Draw player */
        Player->Draw(*Renderer);
        /* Draw particles */
        Particles->Draw();
        /* Draw ball */
        Ball->Draw(*Renderer);
    }
}

void Game::ResetLevel()
{
	/* Workaround */
    if (Level != 3) Level++;
    else Level = 0;

    GameLevel one;
    one.Load("src/levels/one.lvl", Width, Height / 2);
    GameLevel two;
    two.Load("src/levels/two.lvl", Width, Height / 2);
    GameLevel three;
    three.Load("src/levels/three.lvl", Width, Height / 2);
    GameLevel four;
    four.Load("src/levels/four.lvl", Width, Height / 2);

    Levels.clear();

    Levels.push_back(one);
    Levels.push_back(two);
    Levels.push_back(three);
    Levels.push_back(four);
	
	/*
    if (Level == 0)
    {
        Levels[0].Bricks.clear();
	    Levels[0].Load("levels/one.lvl", Width, Height / 2);
        Level++;
    }
    else if (Level == 1)
    {
	    Levels[1].Load("levels/two.lvl", Width, Height / 2);
        Level++;
    }
    else if (Level == 2)
    {
        Levels[2].Load("levels/three.lvl", Width, Height / 2);
        Level++;
    }
    else if (Level == 3)
    {
        Levels[3].Load("levels/four.lvl", Width, Height / 2);
        Level++;
    }
	*/
}

void Game::ResetPlayer()
{
    /* Reset player and ball stats */
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(Width / 2.0f - PLAYER_SIZE.x / 2.0f, Height - PLAYER_SIZE.y * 1.5f);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

void Game::DoCollisions()
{
    /* Check collisions for bricks */
    for (GameObject& box : Levels[Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
        	
            if (std::get<0>(collision)) // If collision is true
            {
                /* Destroy block if not solid */
                if (!box.IsSolid) box.Destroyed = true;
            	
                /* Collision resolution */
                Direction dir = std::get<1>(collision);
                glm::vec2 diffVector = std::get<2>(collision);
            	
                if (dir == LEFT || dir == RIGHT) // Horizontal collision
                {
                    /* Reverse horizontal velocity */
                    Ball->Velocity.x = -Ball->Velocity.x;
                	
                    /* Relocate */
                    float penetration = Ball->Radius - std::abs(diffVector.x);
                    if (dir == LEFT) Ball->Position.x += penetration; // Move ball to right
                    else Ball->Position.x -= penetration; // Move ball to left;
                }
                else // Vertical collision
                {
                    /* Reverse vertical velocity */
                    Ball->Velocity.y = -Ball->Velocity.y;
                    /* Relocate */
                    float penetration = Ball->Radius - std::abs(diffVector.y);
                    if (dir == UP) Ball->Position.y -= penetration; // move ball back up
                    else Ball->Position.y += penetration; // move ball back down
                }
            }
        }
    }
    /* Check collisions for player pad - unless stuck */
    Collision collision = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(collision))
    {
        /* Check where it hit the board, and change velocity based on where it hit the board */
        float centerBoard = Player->Position.x + Player->Size.x / 2.0f;
        float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->Size.x / 2.0f);
    	
        /* Then move accordingly */
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;

        /* Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed) */
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    	
        /* Fix sticky paddle */
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
    }
}

bool Game::CheckCollision(GameObject& a, GameObject& b) // AABB - "axis-aligned bounding box" collision
{
    /* Collision x-axis */
    bool collisionX = (a.Position.x + a.Size.x >= b.Position.x && b.Position.x + b.Size.x >= a.Position.x);
    /* Collision y-axis */
    bool collisionY = (a.Position.y + a.Size.y >= b.Position.y && b.Position.y + b.Size.y >= a.Position.y);

    return (collisionX && collisionY);
}

Collision Game::CheckCollision(BallObject& circle, GameObject& box) // AABB - Circle collision
{
    /* Get center point of the circle */
    glm::vec2 center(circle.Position + circle.Radius);
	
    /* Calculate AABB info (center, half-extents) */
    glm::vec2 aabb_half_extents(box.Size.x / 2.0f, box.Size.y / 2.0f);
    glm::vec2 aabb_center(
        box.Position.x + aabb_half_extents.x,
        box.Position.y + aabb_half_extents.y
    );
	
    /* Get difference vector between both centers */
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

    /* Add clamped value to AABB_center and we get the value of point closest to circle */
    glm::vec2 closest = aabb_center + clamped;
	
    /* Retrieve vector between center circle and closest point AABB and check if length <= radius */
    difference = closest - center;
	
    if (glm::length(difference) < circle.Radius) // not <= since in that case a collision also occurs when first object exactly touches second object, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            bestMatch = i;
        }
    }
    return (Direction)bestMatch;
}