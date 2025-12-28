#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

// ব্লক টাইপ enum
enum BlockType {
    BLOCK_AIR = 0,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_WOOD,
    BLOCK_LEAF,
    BLOCK_WATER,
    BLOCK_COUNT
};

// ব্লক রং (RGBA)
const float blockColors[BLOCK_COUNT][4] = {
    {0.0f, 0.0f, 0.0f, 0.0f},       // AIR - transparent
    {0.2f, 0.6f, 0.2f, 1.0f},       // GRASS - green
    {0.5f, 0.35f, 0.2f, 1.0f},      // DIRT - brown
    {0.5f, 0.5f, 0.5f, 1.0f},       // STONE - gray
    {0.6f, 0.4f, 0.2f, 1.0f},       // WOOD - light brown
    {0.2f, 0.7f, 0.2f, 1.0f},       // LEAF - light green
    {0.2f, 0.3f, 0.8f, 0.7f}        // WATER - blue with transparency
};

// ওয়ার্ল্ড সেটিংস
const int WORLD_SIZE_X = 16;
const int WORLD_SIZE_Y = 16;
const int WORLD_SIZE_Z = 16;

// প্লেয়ার অবস্থান
float playerX = 8.0f;
float playerY = 20.0f;
float playerZ = 8.0f;
float playerRotation = 0.0f;
float playerTilt = 0.0f;

// টাচ কন্ট্রোল ভেরিয়েবল
double touchStartX = 0, touchStartY = 0;
double touchCurrentX = 0, touchCurrentY = 0;
bool isMoving = false;
bool isCrouching = false;
bool touchRightSide = false; // ডান পাশের টাচ

// ওয়ার্ল্ড ডেটা
BlockType world[WORLD_SIZE_X][WORLD_SIZE_Y][WORLD_SIZE_Z];

// শেডার সোর্স কোড
const char* vertexShaderSource = R"(
    #version 300 es
    precision mediump float;
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec4 aColor;
    out vec4 ourColor;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 300 es
    precision mediump float;
    in vec4 ourColor;
    out vec4 FragColor;
    void main() {
        FragColor = ourColor;
    }
)";

// শেডার কম্পাইল ফাংশন
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // এরর চেক
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}

// শেডার প্রোগ্রাম ক্রিয়েট ফাংশন
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // এরর চেক
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking error: " << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// ওয়ার্ল্ড জেনারেট ফাংশন
void generateWorld() {
    for (int x = 0; x < WORLD_SIZE_X; x++) {
        for (int z = 0; z < WORLD_SIZE_Z; z++) {
            // ভূমির উচ্চতা
            int height = 10 + (sin(x * 0.3f) + cos(z * 0.3f)) * 2;
            
            for (int y = 0; y < WORLD_SIZE_Y; y++) {
                if (y > height) {
                    world[x][y][z] = BLOCK_AIR;
                } else if (y == height) {
                    world[x][y][z] = BLOCK_GRASS;
                } else if (y > height - 3) {
                    world[x][y][z] = BLOCK_DIRT;
                } else {
                    world[x][y][z] = BLOCK_STONE;
                }
                
                // পানির ব্লক
                if (y < 5) {
                    world[x][y][z] = BLOCK_WATER;
                }
            }
        }
    }
}

// ব্লক রেন্ডার ফাংশন
void renderBlock(int x, int y, int z, BlockType type) {
    if (type == BLOCK_AIR) return;
    
    float vertices[] = {
        // সামনের দিক
        x-0.5f, y-0.5f, z+0.5f,
        x+0.5f, y-0.5f, z+0.5f,
        x+0.5f, y+0.5f, z+0.5f,
        x-0.5f, y+0.5f, z+0.5f,
        
        // পিছনের দিক
        x-0.5f, y-0.5f, z-0.5f,
        x-0.5f, y+0.5f, z-0.5f,
        x+0.5f, y+0.5f, z-0.5f,
        x+0.5f, y-0.5f, z-0.5f,
        
        // উপরের দিক
        x-0.5f, y+0.5f, z-0.5f,
        x-0.5f, y+0.5f, z+0.5f,
        x+0.5f, y+0.5f, z+0.5f,
        x+0.5f, y+0.5f, z-0.5f,
        
        // নিচের দিক
        x-0.5f, y-0.5f, z-0.5f,
        x+0.5f, y-0.5f, z-0.5f,
        x+0.5f, y-0.5f, z+0.5f,
        x-0.5f, y-0.5f, z+0.5f,
        
        // ডান দিক
        x+0.5f, y-0.5f, z-0.5f,
        x+0.5f, y+0.5f, z-0.5f,
        x+0.5f, y+0.5f, z+0.5f,
        x+0.5f, y-0.5f, z+0.5f,
        
        // বাম দিক
        x-0.5f, y-0.5f, z-0.5f,
        x-0.5f, y-0.5f, z+0.5f,
        x-0.5f, y+0.5f, z+0.5f,
        x-0.5f, y+0.5f, z-0.5f
    };
    
    // রং অ্যারে তৈরি
    float colorData[24 * 4];
    for (int i = 0; i < 24; i++) {
        colorData[i*4] = blockColors[type][0];
        colorData[i*4+1] = blockColors[type][1];
        colorData[i*4+2] = blockColors[type][2];
        colorData[i*4+3] = blockColors[type][3];
    }
    
    // VBO তৈরি এবং ডেটা লোড
    GLuint VBO[2], VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    
    glBindVertexArray(VAO);
    
    // ভার্টেক্স ডেটা
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // রং ডেটা
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    // ব্লক রেন্ডার
    glDrawArrays(GL_QUADS, 0, 24);
    
    // মেমোরি ক্লিনআপ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
}

// টাচ ইভেন্ট হ্যান্ডলার
void handleTouchEvent(int action, double x, double y, int screenWidth) {
    // স্ক্রিনের ডান অর্ধেক চেক করা
    bool isRightSide = (x > screenWidth / 2);
    
    switch (action) {
        case GLFW_PRESS:
            if (isRightSide) {
                // ডান পাশে টাচ - নিচে যাওয়া
                isCrouching = true;
                touchRightSide = true;
            } else {
                // বাম পাশে টাচ - ক্যামেরা নিয়ন্ত্রণ
                isMoving = true;
                touchStartX = x;
                touchStartY = y;
                touchCurrentX = x;
                touchCurrentY = y;
                touchRightSide = false;
            }
            break;
            
        case GLFW_MOVE:
            if (isMoving && !touchRightSide) {
                touchCurrentX = x;
                touchCurrentY = y;
                
                // ক্যামেরা ঘোরান
                playerRotation += (touchCurrentX - touchStartX) * 0.5f;
                playerTilt += (touchCurrentY - touchStartY) * 0.3f;
                
                // টিল্ট সীমাবদ্ধ করা
                if (playerTilt > 90.0f) playerTilt = 90.0f;
                if (playerTilt < -90.0f) playerTilt = -90.0f;
                
                touchStartX = touchCurrentX;
                touchStartY = touchCurrentY;
            }
            break;
            
        case GLFW_RELEASE:
            if (touchRightSide) {
                isCrouching = false;
            } else {
                isMoving = false;
            }
            break;
    }
}

// ভার্চুয়াল জয়স্টিক রেন্ডার
void renderVirtualJoystick() {
    // বাম পাশে জয়স্টিক বেস
    float baseX = -0.8f;
    float baseY = -0.8f;
    float baseSize = 0.15f;
    
    // জয়স্টিক বেস
    float baseVertices[] = {
        baseX - baseSize, baseY - baseSize, 0.0f,
        baseX + baseSize, baseY - baseSize, 0.0f,
        baseX + baseSize, baseY + baseSize, 0.0f,
        baseX - baseSize, baseY + baseSize, 0.0f
    };
    
    float baseColor[] = {
        0.5f, 0.5f, 0.5f, 0.5f
    };
    
    // জয়স্টিক হ্যান্ডল
    float handleX = baseX;
    float handleY = baseY;
    float handleSize = 0.05f;
    
    if (isMoving) {
        float moveX = (touchCurrentX - touchStartX) / 100.0f;
        float moveY = (touchCurrentY - touchStartY) / 100.0f;
        
        // হ্যান্ডল পজিশন সীমাবদ্ধ করা
        float distance = sqrt(moveX * moveX + moveY * moveY);
        if (distance > baseSize) {
            moveX = moveX * baseSize / distance;
            moveY = moveY * baseSize / distance;
        }
        
        handleX += moveX;
        handleY -= moveY; // Y অক্ষ উল্টো
    }
    
    float handleVertices[] = {
        handleX - handleSize, handleY - handleSize, 0.0f,
        handleX + handleSize, handleY - handleSize, 0.0f,
        handleX + handleSize, handleY + handleSize, 0.0f,
        handleX - handleSize, handleY + handleSize, 0.0f
    };
    
    float handleColor[] = {
        0.8f, 0.8f, 0.8f, 0.7f
    };
    
    // জয়স্টিক রেন্ডার
    GLuint VBO[2], VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    
    glBindVertexArray(VAO);
    
    // বেস রেন্ডার
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseVertices), baseVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseColor) * 4, baseColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    glDrawArrays(GL_QUADS, 0, 4);
    
    // হ্যান্ডল রেন্ডার
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(handleVertices), handleVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(handleColor) * 4, handleColor, GL_STATIC_DRAW);
    
    glDrawArrays(GL_QUADS, 0, 4);
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
}

int main() {
    // GLFW ইনিশিয়ালাইজ
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed!" << std::endl;
        return -1;
    }
    
    // উইন্ডো তৈরি
    GLFWwindow* window = glfwCreateWindow(800, 600, "Minecraft-style 3D Game", NULL, NULL);
    if (!window) {
        std::cout << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // OpenGL সেটিংস
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // ওয়ার্ল্ড জেনারেট
    generateWorld();
    
    // শেডার প্রোগ্রাম তৈরি
    GLuint shaderProgram = createShaderProgram();
    
    // স্ক্রিন সাইজ
    int screenWidth, screenHeight;
    
    // গেম লুপ
    while (!glfwWindowShouldClose(window)) {
        // স্ক্রিন সাইজ আপডেট
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        
        // ইভেন্ট প্রসেস
        glfwPollEvents();
        
        // টাচ ইভেন্ট হ্যান্ডলিং
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            handleTouchEvent(GLFW_PRESS, x, y, screenWidth);
        } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            handleTouchEvent(GLFW_RELEASE, x, y, screenWidth);
        } else {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            if (isMoving || isCrouching) {
                handleTouchEvent(GLFW_MOVE, x, y, screenWidth);
            }
        }
        
        // প্লেয়ার মুভমেন্ট
        if (isMoving && !touchRightSide) {
            float moveX = sin(playerRotation * M_PI / 180.0f) * 0.1f;
            float moveZ = cos(playerRotation * M_PI / 180.0f) * 0.1f;
            
            playerX += moveX;
            playerZ += moveZ;
        }
        
        // নিচে যাওয়া
        if (isCrouching) {
            playerY -= 0.1f;
        } else {
            // গ্র্যাভিটি
            playerY -= 0.05f;
        }
        
        // কলিশন ডিটেকশন
        int blockX = static_cast<int>(playerX);
        int blockY = static_cast<int>(playerY);
        int blockZ = static_cast<int>(playerZ);
        
        if (blockY < 0) blockY = 0;
        if (blockY >= WORLD_SIZE_Y) blockY = WORLD_SIZE_Y - 1;
        
        if (world[blockX][blockY][blockZ] != BLOCK_AIR &&
            world[blockX][blockY][blockZ] != BLOCK_WATER) {
            playerY = blockY + 1.0f;
        }
        
        // ক্লিয়ার স্ক্রীন
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // আকাশের রং
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // শেডার প্রোগ্রাম ব্যবহার
        glUseProgram(shaderProgram);
        
        // ওয়ার্ল্ড রেন্ডার
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            for (int y = 0; y < WORLD_SIZE_Y; y++) {
                for (int z = 0; z < WORLD_SIZE_Z; z++) {
                    if (world[x][y][z] != BLOCK_AIR) {
                        renderBlock(x, y, z, world[x][y][z]);
                    }
                }
            }
        }
        
        // ভার্চুয়াল জয়স্টিক রেন্ডার
        renderVirtualJoystick();
        
        // বাফার সোয়াপ
        glfwSwapBuffers(window);
    }
    
    // ক্লিনআপ
    glfwTerminate();
    return 0;
}