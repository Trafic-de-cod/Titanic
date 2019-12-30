// Proiectare Titanic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Shader.h"

unsigned int texture1Location;
void CreateTextures(const std::string& strExePath)
{
    // load and create a texture 
    // -------------------------
    // texture 1
    // ---------
    glGenTextures(1, &texture1Location);
    glBindTexture(GL_TEXTURE_2D, texture1Location);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load((strExePath + "\\MetalTex.jpeg").c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void CreateVBOs()
{
    float vertices[] = {
        0.0f, 1.0f, 0.0f,       1.0f, 0.0f, 0.0f,   0.5f, 1.0f,     //0
        -0.25f, 0.75f, 0.0f,    0.0f, 1.0f, 0.0f,   0.25f, 0.75f,   //1
        0.25f, 0.75f, 0.0f,     0.0f, 0.0f, 1.0f,   0.25f, 0.75f,   //2
        -0.25f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f,   0.25f, 0.75f,   //3
        0.25f,-0.75f,0.0f,      0.0f, 1.0f, 0.0f,   0.0f, 0.0f,     //4
        0.0f, -1.0f, 0.0f,      0.0f, 0.0f, 1.0f,   0.5f, 0.0f,     //5
        0.0f, 0.0f, -0.25f,      0.0f, 0.0f, 1.0f,   0.5f, 0.0f,     //6 varf jos
        -0.25f, 0.75f, -0.25f,    0.0f, 1.0f, 0.0f,   0.25f, 0.75f,   //7
        0.25f, 0.75f, -0.25f,     0.0f, 0.0f, 1.0f,   0.25f, 0.75f,   //8
        -0.25f, -0.75f, -0.25f,   1.0f, 0.0f, 0.0f,   0.25f, 0.75f,   //9
        0.25f,-0.75f,-0.25f,      0.0f, 1.0f, 0.0f,   0.0f, 0.0f,     //10
    };
    unsigned int indices[] = {
        //surface
        0,1,2,
        1,2,3,
        2,3,4,
        3,4,5, 
        //bottom
        7,8,9,
        8,9,10,
        //front
        0,7,8,
        0,1,7,
        0,2,8,
        //back
        5,9,10,
        3,5,9,
        4,5,10,
        //left
        1,3,7,
        3,7,9,
        //right
        2,8,10,
        2,4,10
    };

    unsigned int VBO, VAO, IndBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IndBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}


int main(int argc, char** argv)
{
    std::string strFullExeFileName = argv[0];
    std::string strExePath;
    const size_t last_slash_idx = strFullExeFileName.rfind('\\');
    if (std::string::npos != last_slash_idx) {
        strExePath = strFullExeFileName.substr(0, last_slash_idx);
    }
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* win = glfwCreateWindow(800, 600, "Proiect Titanic", nullptr, nullptr);
    if (win == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    Shader triangleShader("vertexShader.vs", "fragShader.fs");
    
    CreateVBOs();
    CreateTextures(strExePath);

    while (!glfwWindowShouldClose(win))
    {
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::translate(transform, glm::vec3(0.0f, 0.f, -0.5f));
        triangleShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1Location);
        unsigned int transformLoc = glGetUniformLocation(triangleShader.GetID(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 3*17, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
}

