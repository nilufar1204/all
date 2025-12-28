#include <iostream>
#include <vector>
#include <cmath>

// Simple software rendering version for CxxDroid
// (যদি OpenGL ইন্সটল করতে সমস্যা হয়)

class Simple3DGame {
private:
    // ব্লক টাইপ enum
    enum BlockType {
        BLOCK_AIR = 0,
        BLOCK_GRASS,
        BLOCK_DIRT,
        BLOCK_STONE,
        BLOCK_COUNT
    };

    // ব্লক রং (RGB)
    const char* blockColors[BLOCK_COUNT] = {
        " ",       // AIR - transparent
        "\033[32m█\033[0m", // GRASS - green
        "\033[33m█\033[0m", // DIRT - brown
        "\033[37m█\033[0m"  // STONE - gray
    };

    // ওয়ার্ল্ড সেটিংস
    const int WORLD_SIZE_X = 10;
    const int WORLD_SIZE_Y = 5;
    const int WORLD_SIZE_Z = 10;

    // প্লেয়ার অবস্থান
    float playerX = 5.0f;
    float playerY = 10.0f;
    float playerZ = 5.0f;
    float playerRotation = 0.0f;

    // ওয়ার্ল্ড ডেটা
    BlockType world[10][5][10];

public:
    Simple3DGame() {
        generateWorld();
    }

    // ওয়ার্ল্ড জেনারেট ফাংশন
    void generateWorld() {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            for (int z = 0; z < WORLD_SIZE_Z; z++) {
                int height = 2 + (sin(x * 0.5f) + cos(z * 0.5f));
                
                for (int y = 0; y < WORLD_SIZE_Y; y++) {
                    if (y > height) {
                        world[x][y][z] = BLOCK_AIR;
                    } else if (y == height) {
                        world[x][y][z] = BLOCK_GRASS;
                    } else if (y > height - 2) {
                        world[x][y][z] = BLOCK_DIRT;
                    } else {
                        world[x][y][z] = BLOCK_STONE;
                    }
                }
            }
        }
    }

    // রেন্ডার ফাংশন
    void render() {
        // ক্লিয়ার স্ক্রীন
        std::cout << "\033[2J\033[H";
        
        // সিম্পল 3D ভিউ রেন্ডার
        std::cout << "Minecraft-style 3D Game (Text Mode)" << std::endl;
        std::cout << "Use W/A/S/D to move, Q/E to look around" << std::endl;
        std::cout << "Player Position: X=" << playerX << " Y=" << playerY << " Z=" << playerZ << std::endl;
        std::cout << "Rotation: " << playerRotation << " degrees" << std::endl;
        std::endl;
        
        // মাটির স্তর রেন্ডার
        std::cout << "Ground View:" << std::endl;
        for (int z = 0; z < WORLD_SIZE_Z; z++) {
            for (int x = 0; x < WORLD_SIZE_X; x++) {
                int y = 0;
                while (y < WORLD_SIZE_Y && world[x][y][z] == BLOCK_AIR) {
                    y++;
                }
                
                if (y < WORLD_SIZE_Y) {
                    std::cout << blockColors[world[x][y][z]];
                } else {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    // মুভমেন্ট হ্যান্ডলিং
    void moveForward() {
        playerX += sin(playerRotation * M_PI / 180.0f) * 0.5f;
        playerZ += cos(playerRotation * M_PI / 180.0f) * 0.5f;
    }

    void moveBackward() {
        playerX -= sin(playerRotation * M_PI / 180.0f) * 0.5f;
        playerZ -= cos(playerRotation * M_PI / 180.0f) * 0.5f;
    }

    void moveLeft() {
        playerX += cos(playerRotation * M_PI / 180.0f) * 0.5f;
        playerZ -= sin(playerRotation * M_PI / 180.0f) * 0.5f;
    }

    void moveRight() {
        playerX -= cos(playerRotation * M_PI / 180.0f) * 0.5f;
        playerZ += sin(playerRotation * M_PI / 180.0f) * 0.5f;
    }

    void lookLeft() {
        playerRotation -= 10.0f;
        if (playerRotation < 0.0f) playerRotation += 360.0f;
    }

    void lookRight() {
        playerRotation += 10.0f;
        if (playerRotation >= 360.0f) playerRotation -= 360.0f;
    }

    void jump() {
        playerY += 1.0f;
    }

    void crouch() {
        playerY -= 1.0f;
        if (playerY < 1.0f) playerY = 1.0f;
    }
};

int main() {
    Simple3DGame game;
    
    std::cout << "Simple Minecraft-style 3D Game" << std::endl;
    std::cout << "Controls: W/A/S/D to move, Q/E to look, Space to jump, C to crouch" << std::endl;
    std::cout << "Press X to exit" << std::endl;
    
    game.render();
    
    // সিম্পল ইনপুট লুপ
    char input;
    while (true) {
        std::cin >> input;
        
        switch (input) {
            case 'w': case 'W':
                game.moveForward();
                break;
            case 's': case 'S':
                game.moveBackward();
                break;
            case 'a': case 'A':
                game.moveLeft();
                break;
            case 'd': case 'D':
                game.moveRight();
                break;
            case 'q': case 'Q':
                game.lookLeft();
                break;
            case 'e': case 'E':
                game.lookRight();
                break;
            case ' ':
                game.jump();
                break;
            case 'c': case 'C':
                game.crouch();
                break;
            case 'x': case 'X':