//Opis: Primjer ucitavanja modela upotrebom ASSIMP biblioteke
//Preuzeto sa learnOpenGL

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "model.hpp"

#define CRES 100 // Circle Resolution = Rezolucija kruga


static unsigned loadImageToTexture(const char* filePath);

const unsigned int wWidth = 800;
const unsigned int wHeight = 600;

unsigned int moonVAO, moonVBO;
unsigned moonTex;
float circle[30 * 2 + 4];

float gas = 0;
float rpm = 0;
float maxRPM = 5000;
int gear = 0;
float totalKM = 0;
bool blending = false;
bool checkEngine = false;
bool checkBattery = false;
glm::vec3 batPos = glm::vec3(0.25f, -0.32f, 0.98f);


void createMoon() {

    float r = 0.5;

    circle[0] = 0;
    circle[1] = 0;

    int i;
    for (i = 0; i <= 30; i++)
    {

        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / 30)); //Xi
        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / 30)); //Yi
    }
    glGenVertexArrays(1, &moonVAO);
    glGenBuffers(1, &moonVBO);
    glBindVertexArray(moonVAO);
    glBindBuffer(GL_ARRAY_BUFFER, moonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    moonTex = loadImageToTexture("res/moon.png");
    glBindTexture(GL_TEXTURE_2D, moonTex); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
}



int main()
{
    if(!glfwInit())
    {
        std::cout << "GLFW fail!\n" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Window fail!\n" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() !=GLEW_OK)
    {
        std::cout << "GLEW fail! :(\n" << std::endl;
        return -3;
    }


    float nameVertices[] = {
        //X     Y     S    T
        -1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 0.0, 1.0,
        -1.0, -1.0, 1.0, 0.0,
        1.0, -1.0, 0.0, 0.0
    };


    float windShieldVertices[] = {
        -1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 0.0, 1.0,
        -1.0, -1.0, 1.0, 0.0,
        1.0, -1.0, 0.0, 0.0
    };

    float batVertices[] = {
        -0.15625, 0.64, 0.0, 0.0, 0.0, 0.5, //0
        -0.09375, 0.625, 0.0, 0.0, 0.0, 0.5, //1
        -0.125, 0.5625, 0.0, 0.0, 0.0, 0.5, //2
        -0.1, 0.45, 0.0, 0.0, 0.0, 0.5, //3
        -0.0625, 0.4375, 0.0, 0.0, 0.0, 0.5, //4
        -0.0835, 0.5625, 0.0, 0.0, 0.0, 0.5, //5
        -0.025, 0.525, 0.0, 0.0, 0.0, 0.5, //6
        0.0, 0.35, 0.0, 0.0, 0.0, 0.5, //7
        0.025, 0.525, 0.0, 0.0, 0.0, 0.5, //8
        0.0625, 0.4375, 0.0, 0.0, 0.0, 0.5, //9
        0.0835, 0.5625, 0.0, 0.0, 0.0, 0.5, //10
        0.1, 0.45, 0.0, 0.0, 0.0, 0.5, //11
        0.09375, 0.625, 0.0, 0.0, 0.0, 0.5, //12
        0.125, 0.5625, 0.0, 0.0, 0.0, 0.5, //13
        0.15625, 0.64, 0.0, 0.0, 0.0, 0.5, //14
        -0.025, 0.6, 0.0, 0.0, 0.0, 0.5, //15
        -0.015, 0.5625, 0.0, 0.0, 0.0, 0.5, //16
        0.0, 0.525, 0.0, 0.0, 0.0, 0.5, //17
        0.015, 0.5625, 0.0, 0.0, 0.0, 0.5, //18
        0.025, 0.6, 0.0, 0.0, 0.0, 0.5 //19
    };


    float minX = batVertices[0];
    float maxX = batVertices[0];
    float minY = batVertices[1];
    float maxY = batVertices[1];

    for (int i = 0; i < sizeof(batVertices) / sizeof(float); i += 6) {
        float x = batVertices[i];
        float y = batVertices[i + 1];

        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }


    float padding = 0.3;

    float displayVertices[] = {

        minX - padding, maxY, 0.0, 0.0, 0.0, -1.0, // UL
        maxX + padding, maxY, 0.0, 0.0, 0.0, -1.0,// UR
        minX - padding, minY - padding, 0.0, 0.0, 0.0, -1.0,// DL
        maxX + padding, minY - padding, 0.0, 0.0, 0.0, -1.0,// DR
    };

    float tachometerVertices[] = {
        -0.8f, 0.0f, 0.0, 1.0,
        -0.3f, 0.0f, 1.0, 1.0,
        -0.8f, -0.5f, 0.0, 0.0,
        -0.3f, -0.5f, 1.0, 0.0
    };

    float tachLineVertices[] = {
        -0.55f, -0.45f, 1.0, 0.0, 0.0, 1.0,
        -0.73f, -0.65f, 1.0, 0.0, 0.0, 1.0
    };

    float boxCenterX = 0.55f;
    float boxCenterY = -0.45f;

    float circle[CRES * 6 + 12];
    float r = 0.25;

    circle[0] = boxCenterX;
    circle[1] = boxCenterY;

    circle[2] = 0.0f;
    circle[3] = 0.0f;
    circle[4] = 0.0f;
    circle[5] = 1.0f;

    for (int i = 0; i <= CRES; ++i) {
        circle[6 + 6 * i] = boxCenterX - r * cos((3.141592 / 180) * (i * 360 / CRES));
        circle[6 + 6 * i + 1] = boxCenterY +  r * sin((3.141592 / 180) * (i * 360 / CRES));

        circle[6 + 6 * i + 2] = 0.0f;
        circle[6 + 6 * i + 3] = 0.0f;
        circle[6 + 6 * i + 4] = 0.0f;
        circle[6 + 6 * i + 5] = 1.0f;
    }

    float xLeft = maxX + padding + 0.1;
    float xRight = maxX + padding + 0.3;
    float yMax = maxY;
    float yMin = minY - padding;

    float step = (yMax - yMin) / 10;

    float progressBar[8 * 7] = {
        xLeft + 0.005 , yMax - 0.005, 0.0, 0.0, 0.0, -1.0,  //0
        xRight - 0.005, yMax - 0.005, 0.0, 0.0, 0.0, -1.0,  //6
        xLeft + 0.005, yMin + 0.005, 0.0, 0.0, 0.0, -1.0,  //12
        xRight - 0.005, yMin + 0.005, 0.0, 0.0, 0.0, -1.0,  //18

        xLeft , yMax, 0.0, 0.0, 0.0, -1.0,  //24
        xRight, yMax,0.0, 0.0, 0.0, -1.0,  //30
        xLeft, yMin,0.0, 0.0, 0.0, -1.0,  //36
        xRight, yMin,0.0, 0.0, 0.0, -1.0,  //42
        //48
    };

    float lights[] = {
        // check engine
        -0.1, -0.1, 0.0, 1.0,
        0.1, -0.1, 1.0, 1.0,
        -0.1, -0.3, 0.0, 0.0,
        0.1, -0.3, 1.0, 0.0,

        // battery light
        -0.08, -0.5, 0.0, 1.0,
        0.08, -0.5, 1.0, 1.0,
        -0.08, -0.7, 0.0, 0.0,
        0.08, -0.7, 1.0, 0.0
    };

    float dashboardVertices[]{
        -0.2f, 0.1f, 0.0, 0.0, 0.0, -1.0,
        0.2f, 0.1f, 0.0, 0.0, 0.0, -1.0,
        -0.2f, -0.1f, 0.0, 0.0, 0.0, -1.0,
        0.2f, -0.1f, 0.0, 0.0, 0.0, -1.0,
    };

    unsigned int indices[] = // Indeksi tjemena
    {
        0, 1, 2, //Prvi trougao sacinjen od tjemena 0, tjemena 1 i tjemena 2 (Prednja strana mu je naprijed)
        1, 2, 3, //Drugi trougao (Zadnja strana mu je naprijed)
        1, 3, 4,
        4, 5, 6,
        4, 6, 7,
        6, 7, 8,
        7, 8, 9,
        8, 9, 10,
        9, 11, 12,
        11, 12, 13,
        12, 13, 14,
        6, 15, 16,
        6, 16, 17,
        16, 17, 18,
        8, 17, 18,
        8, 18, 19

    };
    unsigned int stride = (2 + 4) * sizeof(float);
    unsigned int textureStride = (2 + 2) * sizeof(float);
    unsigned int VAO[10];
    glGenVertexArrays(10, VAO);

    unsigned int VBO[10];
    glGenBuffers(10, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(batVertices), batVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(displayVertices), displayVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tachometerVertices), tachometerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tachLineVertices), tachLineVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(progressBar), progressBar, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windShieldVertices), windShieldVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lights), lights, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dashboardVertices), dashboardVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, textureStride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned tachometerTexture = loadImageToTexture("res/tachometer.png"); //Ucitavamo teksturu
    glBindTexture(GL_TEXTURE_2D, tachometerTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned nameTexture = loadImageToTexture("res/name.png"); //Ucitavamo teksturu
    glBindTexture(GL_TEXTURE_2D, nameTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned engineTexture = loadImageToTexture("res/check-engine.png");
    glBindTexture(GL_TEXTURE_2D, engineTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned batteryTexture = loadImageToTexture("res/battery-problem.png");
    glBindTexture(GL_TEXTURE_2D, batteryTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned windshieldTexture = loadImageToTexture("res/wing.png");
    glBindTexture(GL_TEXTURE_2D, windshieldTexture);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);

    createMoon();
    //createMoon(1.0f, 50, 50);

    Model scene("res/zgrade.obj");
    Model wheel("res/merc steering.obj");
    Model dashboard("res/dashboard.obj");
    Model windshield("res/windshield.obj");
    //Tjemena i baferi su definisani u model klasi i naprave se pri stvaranju objekata

    Shader unifiedShader("basic.vert", "basic.frag");
    Shader moonShader("moon.vert", "moon.frag");
    Shader texShader("tex.vert", "tex.frag");
    Shader dashboardShader("dashboard.vert", "dashboard.frag");

    //Render petlja
    unifiedShader.use();
    //unifiedShader.setVec3("uLightPos", 0, 15, -15);
    //unifiedShader.setVec3("uLightDir", 0.0f, -1.0f, 1.0f);

    unifiedShader.setVec3("dirLight.direction", 0.0f, -1.0f, 1.0f);
    unifiedShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    unifiedShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    unifiedShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    unifiedShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    unifiedShader.setVec3("pointLights[0].diffuse", 0.2f, 0.2f, 0.2f); 
    unifiedShader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
    unifiedShader.setFloat("pointLights[0].constant", 1.0f);
    unifiedShader.setFloat("pointLights[0].linear", 0.7f);
    unifiedShader.setFloat("pointLights[0].quadratic", 1.8f);
    unifiedShader.setVec3("pointLights[0].lightColor", 0.0f, 0.0f, 0.0f);
    
    unifiedShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    unifiedShader.setVec3("pointLights[1].diffuse", 0.2f, 0.2f, 0.2f); 
    unifiedShader.setVec3("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
    unifiedShader.setFloat("pointLights[1].constant", 1.0f);
    unifiedShader.setFloat("pointLights[1].linear", 0.7f);
    unifiedShader.setFloat("pointLights[1].quadratic", 1.8f);
    unifiedShader.setVec3("pointLights[1].lightColor", 0.0f, 0.0f, 0.0f);
    
    unifiedShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    unifiedShader.setVec3("pointLights[2].diffuse", 0.2f, 0.2f, 0.2f); 
    unifiedShader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
    unifiedShader.setFloat("pointLights[2].constant", 1.0f);
    unifiedShader.setFloat("pointLights[2].linear", 0.7f);
    unifiedShader.setFloat("pointLights[2].quadratic", 1.8f);
    unifiedShader.setVec3("pointLights[2].lightColor", 1.0f, 1.0f, 1.0f);
    
    unifiedShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    unifiedShader.setVec3("pointLights[3].diffuse", 0.2f, 0.2f, 0.2f); 
    unifiedShader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
    unifiedShader.setFloat("pointLights[3].constant", 1.0f);
    unifiedShader.setFloat("pointLights[3].linear", 0.7f);
    unifiedShader.setFloat("pointLights[3].quadratic", 1.8f);
    unifiedShader.setVec3("pointLights[3].lightColor", 1.0f, 1.0f, 1.0f);
    
    for (int i = 4; i < 14; i++) {
        unifiedShader.setVec3("pointLights["+to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
        unifiedShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 0.2f, 0.2f, 0.2f);
        unifiedShader.setVec3("pointLights[" + to_string(i) + "].specular", 0.0f, 0.0f, 0.0f);
        unifiedShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
        unifiedShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.7f);
        unifiedShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 1.8f);
        unifiedShader.setVec3("pointLights[" + to_string(i) + "].lightColor", 0.0f, 0.0f, 0.0f);
    }

    unifiedShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
    unifiedShader.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
    unifiedShader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
    unifiedShader.setFloat("spotLight[0].constant", 1.0f);
    unifiedShader.setFloat("spotLight[0].linear", 0.09f);
    unifiedShader.setFloat("spotLight[0].quadratic", 0.032f);
    unifiedShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(15.5f)));
    unifiedShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(25.0f)));
    unifiedShader.setVec3("spotLight[0].lightColor", 0.0f, 0.0f, 0.0f);

    unifiedShader.setVec3("spotLight[1].ambient", 0.0f, 0.0f, 0.0f);
    unifiedShader.setVec3("spotLight[1].diffuse", 1.0f, 1.0f, 1.0f);
    unifiedShader.setVec3("spotLight[1].specular", 1.0f, 1.0f, 1.0f);
    unifiedShader.setFloat("spotLight[1].constant", 1.0f);
    unifiedShader.setFloat("spotLight[1].linear", 0.09f);
    unifiedShader.setFloat("spotLight[1].quadratic", 0.032f);
    unifiedShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(15.5f)));
    unifiedShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(25.0f)));
    unifiedShader.setVec3("spotLight[1].lightColor", 0.0f, 0.0f, 0.0f);

    unifiedShader.setVec3("spotLight[2].ambient", 0.0f, 0.0f, 0.0f);
    unifiedShader.setVec3("spotLight[2].diffuse", 1.0f, 1.0f, 1.0f);
    unifiedShader.setVec3("spotLight[2].specular", 0.0f, 0.0f, 0.0f);
    unifiedShader.setFloat("spotLight[2].constant", 1.0f);
    unifiedShader.setFloat("spotLight[2].linear", 0.09f);
    unifiedShader.setFloat("spotLight[2].quadratic", 0.032f);
    unifiedShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(40.5f)));
    unifiedShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(50.0f)));
    unifiedShader.setVec3("spotLight[2].lightColor", 1.0f, 1.0f, 1.0f);
    
    unifiedShader.setVec3("spotLight[3].ambient", 0.0f, 0.0f, 0.0f);
    unifiedShader.setVec3("spotLight[3].diffuse", 1.0f, 1.0f, 1.0f);
    unifiedShader.setVec3("spotLight[3].specular", 1.0f, 1.0f, 1.0f);
    unifiedShader.setFloat("spotLight[3].constant", 1.0f);
    unifiedShader.setFloat("spotLight[3].linear", 0.09f);
    unifiedShader.setFloat("spotLight[3].quadratic", 0.032f);
    unifiedShader.setFloat("spotLight[3].cutOff", glm::cos(glm::radians(50.5f)));
    unifiedShader.setFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(70.0f)));
    unifiedShader.setVec3("spotLight[3].lightColor", 0.0f, 0.0f, 0.0f);
    unifiedShader.setBool("nightVision", false);

    unifiedShader.setVec3("uViewPos", 0, 0, 5);
    unifiedShader.setVec3("uLightColor", 0.5, 0.5, 0.5);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);
    unifiedShader.setMat4("uP", projection);

    //moonShader.use();
    //moonShader.set

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 moon = glm::mat4(1.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, -25.0f); // Initial camera position
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Initial camera front vector
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Initial camera up vector

    float cameraSpeed = 0.005f;
    float parallelSpeed = 0.001f;
    float yaw = 90.0f;
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool nightVision = false;
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (!nightVision) glClearColor(0.027f, 0.05f, 0.1f, 1.0f);
        else glClearColor(0.0135f, 0.525f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 move;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            move = cameraSpeed * cameraFront * gas * float(gear+1);
            if (!scene.detectCollision(cameraPos + move)) {
                cameraPos += move;
                gas = std::min(gas + 0.01f, 1.1f);
                totalKM += 0.0005;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
            gas = std::max(gas - 0.0005f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            move = cameraSpeed * cameraFront * gas * float(gear + 1);
            if (!scene.detectCollision(cameraPos - move)) {
                cameraPos -= move;
                gas = std::min(gas + 0.005f, 1.1f);
                totalKM += 0.0001;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
            gas = std::max(gas - 0.0005f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            move = glm::normalize(glm::cross(cameraFront, cameraUp)) * parallelSpeed;
            cameraPos -= move;
            glm::normalize(glm::cross(cameraFront, cameraUp)) * parallelSpeed;
            if (scene.detectCollision(cameraPos)) {
                cameraPos += move;
                gas = std::min(gas + 0.001f, 0.5f);

            }

        }
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
            gas = std::max(gas - 0.0005f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            move = glm::normalize(glm::cross(cameraFront, cameraUp)) * parallelSpeed;
            cameraPos += move;
            if (scene.detectCollision(cameraPos)) {
                cameraPos -= move;
                gas = std::min(gas + 0.001f, 0.5f);

            }
        }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
            gas = std::max(gas - 0.0005f, 0.0f);
        }

        rpm = gas * maxRPM;
        if (rpm >= maxRPM && gear != 5) { 
            gear = std::min(gear + 1, 5);
            rpm = 0;
            gas = 0.5;
        }
        else if (rpm <= 1000 && gear != 0) {
            gear = std::max(gear - 1, 0);
            rpm = maxRPM;
           
        }


        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            yaw -= 50.0f * deltaTime; 

        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            yaw += 50.0f * deltaTime; 
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            blending = true;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            blending = false;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            checkEngine = true;
            unifiedShader.setVec3("pointLights[0].lightColor", 1.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            checkBattery = true;
            unifiedShader.setVec3("pointLights[1].lightColor", 1.0f, 0.0f, 0.0f);

        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
            checkEngine = false;
            checkBattery = false;

            unifiedShader.setVec3("pointLights[0].lightColor", 0.0f, 0.0f, 0.0f);
            unifiedShader.setVec3("pointLights[1].lightColor", 0.0f, 0.0f, 0.0f);

        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(0.0f));
        direction.y = sin(glm::radians(0.0f));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(0.0f));
        cameraFront = glm::normalize(direction);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            unifiedShader.setVec3("spotLight[1].lightColor", 1.0f, 1.0f, 1.0f);
            unifiedShader.setVec3("spotLight[0].lightColor", 1.0f, 1.0f, 1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            unifiedShader.setVec3("spotLight[1].lightColor", 0.0f, 0.0f, 0.0f);
            unifiedShader.setVec3("spotLight[0].lightColor", 0.0f, 0.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            nightVision = true;
            unifiedShader.setBool("nightVision", true);
            unifiedShader.setVec3("spotLight[3].lightColor", 1.0f, 1.0f, 1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            nightVision = false;
            unifiedShader.setBool("nightVision", false);
            unifiedShader.setVec3("spotLight[3].lightColor", 0.0f, 0.0f, 0.0f);

        }

        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

        // Adjust these values to control how close the spotlights are to the camera
        float leftOffset = 1.5f; 
        float rightOffset = 1.5f; 

        glm::vec3 leftSpotlightPos = cameraPos + (cameraRight * leftOffset) + glm::vec3(0.0, 0.0, -0.3);
        glm::vec3 rightSpotlightPos = cameraPos - (cameraRight * rightOffset) + glm::vec3(0.0, 0.0, -0.3);

        unifiedShader.setVec3("spotLight[0].position", leftSpotlightPos);
        unifiedShader.setVec3("spotLight[0].direction", cameraFront);

        unifiedShader.setVec3("spotLight[1].position", rightSpotlightPos);
        unifiedShader.setVec3("spotLight[1].direction", cameraFront);
        
        //unifiedShader.setVec3("spotLight[2].position", cameraPos + glm::vec3(0.0, 1.0, 5.0));
        //unifiedShader.setVec3("spotLight[2].direction", cameraFront);

        unifiedShader.setVec3("spotLight[3].position", cameraPos - 5.0f * cameraFront);
        unifiedShader.setVec3("spotLight[3].direction", -cameraFront);

        
        glBindVertexArray(moonVAO);
        moonShader.use();
        moonShader.setMat4("projection", projection);
        moonShader.setMat4("view", view);
        moonShader.setVec4("color", glm::vec4(1.0, 1.0, 1.0, 1.0));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTex);
        moon = glm::mat4(1.0f);
        moon = glm::translate(moon, glm::vec3(0.0f, 10.0f, -25.0f));
        moonShader.setMat4("model", moon);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));
        glBindVertexArray(0);

        unifiedShader.use();
        
        glm::vec3 wheelOffset = glm::vec3(-0.2f, -0.6f, 1.0f); 
        glm::vec3 newWheelPosition = cameraPos + cameraFront * wheelOffset.z + cameraUp * wheelOffset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * wheelOffset.x;
        model = glm::translate(model, newWheelPosition);
        
        model = glm::scale(model, glm::vec3(0.1f));

        float angle = glm::radians(-yaw + 90.0f);
        model = glm::rotate(model, angle, cameraUp);
        

        unifiedShader.setMat4("uM", model);
        unifiedShader.setMat4("uV", view);

        wheel.Draw(unifiedShader);

        model = glm::mat4(1.0f);
        glm::vec3 offset = glm::vec3(0.0f, -0.35f, 1.3f);
        glm::vec3 newdashPosition = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        model = glm::translate(model, newdashPosition);
        model = glm::scale(model, glm::vec3(0.2f));

        angle = glm::radians(-yaw);
        model = glm::rotate(model, angle, cameraUp);
        unifiedShader.setMat4("uM", model);
        dashboard.Draw(unifiedShader);
        unifiedShader.setInt("isMaterial", 0);


        model = glm::mat4(1.0f);
        unifiedShader.setMat4("uM", model);
        scene.Draw(unifiedShader);

        //windshield
        moonShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, windshieldTexture);
        moonShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 0.0));
        glm::mat4 m = glm::mat4(1.0f);
        offset = glm::vec3(0.0f, 0.0f, 1.3f);
        glm::vec3 newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.75f));

        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        moonShader.setMat4("model", m);
        glBindVertexArray(VAO[6]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);


        //display
        unifiedShader.use();
        unifiedShader.setInt("isMaterial", 1);
        unifiedShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        unifiedShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        unifiedShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        unifiedShader.setFloat("material.shininess", 32.0);
        m = glm::mat4(1.0f);
        offset = glm::vec3(0.25f, -0.32f, 0.99f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.25f));
        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        unifiedShader.setMat4("uM", m);
        //dashboardShader.setVec3("uCol", glm::vec3(0.0, 0.0, 0.0));
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(displayVertices) / (6 * sizeof(float)));

        //dashboard
        unifiedShader.setVec3("material.ambient", 0.19225f, 0.19225f, 0.19225f);
        unifiedShader.setVec3("material.diffuse", 0.50754f, 0.50754f, 0.50754f);
        unifiedShader.setVec3("material.specular", 0.508273f, 0.508273f, 0.508273f);
        unifiedShader.setFloat("material.shininess", 32.0);
        m = glm::mat4(1.0f);
        offset = glm::vec3(-0.2f, -0.25f, 1.0f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.9f));

        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        unifiedShader.setMat4("uM", m);
        glBindVertexArray(VAO[8]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(dashboardVertices)/(6 * sizeof(float)));

        //progress bar
        m = glm::mat4(1.0f);

        int km = round(totalKM);
        km = km % 10 + 1;
        offset = glm::vec3(0.25, -0.32 + 0.015, 0.98);

        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.25f, 0.025 * km, 0.25f));

        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        unifiedShader.setMat4("uM", m);
        unifiedShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        unifiedShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        unifiedShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        unifiedShader.setFloat("material.shininess", 32.0);
        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        for (int i = 4; i < 4 + km;i++) {
            unifiedShader.setVec3("pointLights[" + to_string(i) + "].lightColor", glm::vec3(1.0, 1.0, 1.0));
            unifiedShader.setVec3("pointLights[" + to_string(i) + "].position", glm::vec3(m[3][0]+0.16, m[3][1] + (km-1) * 0.02, m[3][2] -0.02));
        }

        m = glm::mat4(1.0f);
        offset = glm::vec3(0.25f, -0.32f, 0.99f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.25f));
        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        unifiedShader.setVec3("material.ambient", 0.19225f, 0.19225f, 0.19225f);
        unifiedShader.setVec3("material.diffuse", 0.50754f, 0.50754f, 0.50754f);
        unifiedShader.setVec3("material.specular", 0.508273f, 0.508273f, 0.508273f);
        unifiedShader.setFloat("material.shininess", 32.0);
        unifiedShader.setMat4("uM", m);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        unifiedShader.setInt("isMaterial", 0);


        //speed
        dashboardShader.use();
        dashboardShader.setMat4("uP", projection);
        dashboardShader.setMat4("uV", view);
        m = glm::mat4(1.0f);
        offset = glm::vec3(0.0f, -0.17f, 0.99f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.2f));
        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        dashboardShader.setMat4("uM", m);
        float R = abs(sin(2 * gear * glfwGetTime()));
        dashboardShader.setVec3("uCol", glm::vec3(R, 0.0, 0.0));
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / stride);

        unifiedShader.use();
        unifiedShader.setVec3("pointLights[3].lightColor", glm::vec3(R, 0.0, 0.0));
        unifiedShader.setVec3("pointLights[3].position", glm::vec3(m[3]));

        dashboardShader.use();
        //tachLine
        m = glm::mat4(1.0f);
        dashboardShader.setInt("one", 1);
        float rotationAngle = (rpm / maxRPM) * 220 * 0.7;
        dashboardShader.setFloat("rotationAngle", rotationAngle);
        offset = glm::vec3(-0.34f, -0.21f, 0.98f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.1));
        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        dashboardShader.setMat4("uM", m);
        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_LINES, 0, 2);
        dashboardShader.setInt("one", 0);


        if (blending) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else {
            glDisable(GL_BLEND);

        }

        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            batPos.y += 0.001;
            if (batPos.y >= -0.28) batPos.y -= 0.001;
         
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            batPos.y -= 0.001;
            if (batPos.y <= -0.367) batPos.y += 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            batPos.x -= 0.001;
            if (batPos.x <= 0.173) batPos.x += 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            batPos.x += 0.001;
            if (batPos.x >= 0.32) batPos.x -= 0.001;
        }

        //bat
        dashboardShader.setVec3("uCol", glm::vec3(0.0));
        m = glm::mat4(1.0f);
        newPos = cameraPos + cameraFront * batPos.z + cameraUp * batPos.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * batPos.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.25f, 0.18f, 0.25f));
        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        dashboardShader.setMat4("uM", m);
        glBindVertexArray(VAO[0]);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
        glEnable(GL_CULL_FACE);
        unifiedShader.use();
        unifiedShader.setVec3("pointLights[2].position", glm::vec3(m[3][0], m[3][1]+0.1, m[3][2]));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        //tachometer
        texShader.use();
        texShader.setMat4("projection", projection);
        texShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tachometerTexture);
        m = glm::mat4(1.0f);
        offset = glm::vec3(-0.4f, -0.21f, 0.99f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.2f));

        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        texShader.setMat4("model", m);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(tachometerVertices) / textureStride);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (checkEngine) {
            m = glm::mat4(1.0f);
            offset = glm::vec3(-0.2f, -0.21f, 0.99f);
            newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
            m = glm::translate(m, newPos);
            m = glm::scale(m, glm::vec3(0.12f));
            angle = glm::radians(-yaw + 90.0f);
            m = glm::rotate(m, angle, cameraUp);
            texShader.setMat4("model", m);
            glBindVertexArray(VAO[7]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, engineTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            unifiedShader.use();
            unifiedShader.setVec3("pointLights[0].position", glm::vec3(m[3][0], m[3][1]-0.02, m[3][2]));
            texShader.use();

        }
        if (checkBattery) {
            m = glm::mat4(1.0f);
            offset = glm::vec3(-0.2f, -0.2f, 0.99f);
            newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
            m = glm::translate(m, newPos);
            m = glm::scale(m, glm::vec3(0.12f));
            angle = glm::radians(-yaw + 90.0f);
            m = glm::rotate(m, angle, cameraUp);
            texShader.setMat4("model", m);
            glBindVertexArray(VAO[7]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, batteryTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            glBindTexture(GL_TEXTURE_2D, 0);
            unifiedShader.use();
            unifiedShader.setVec3("pointLights[1].position", glm::vec3(m[3][0], m[3][1] - 0.06, m[3][2]));
            texShader.use();
            
        }
        texShader.use();
        //name
        m = glm::mat4(1.0f);
        offset = glm::vec3(-0.35f, 0.35f, 0.99f);
        newPos = cameraPos + cameraFront * offset.z + cameraUp * offset.y + glm::normalize(glm::cross(cameraFront, cameraUp)) * offset.x;
        m = glm::translate(m, newPos);
        m = glm::scale(m, glm::vec3(0.15f, 0.02f, 0.0f));
        angle = glm::radians(-yaw + 90.0f);
        m = glm::rotate(m, angle, cameraUp);
        texShader.setMat4("model", m);
        glBindVertexArray(VAO[9]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, nameTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        model = glm::mat4(1.0f);
        unifiedShader.use();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(10, VBO);
    glDeleteVertexArrays(10, VAO);
    glfwTerminate();
    return 0;
}



static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}