// Proiectare Titanic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Camera.h"
#include"Shader.h"
#include "Model.h"


unsigned int texture1Location;

unsigned int VBO, VAO, IndBO;
GLuint skyboxVAO, skyboxVBO, skyboxIBO;
unsigned int waterVBO, waterVAO;
unsigned int cubeVBO, cubeVAO;
unsigned int lightVAO;

// settings
const unsigned int SCR_WIDTH = 1900;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float skyboxVertices[] = {
    -1.f, -1.f, -1.f, //0
    1.f,-1.f,-1.f,    //1
    1.f,-1.f,1.f,     //2
    -1.f, -1.f, 1.f,  //3
                      
    -1.f, 1.f, -1.f,  //4
    1.f,1.f,-1.f,     //5
    1.f,1.f,1.f,      //6
    -1.f,1.f,1.f,     //7
};

float waterVertices[] =
{   //position              //color             //normals
   15.0f, -0.5f,  15.0f, 0.0f, 0.0f, 1.0f,      0.0f, 1.0f, -1.0f,
   -15.0f, -0.5f,  15.0f,  0.0f, 0.0f, 1.0f,    0.0f, 1.0f, -1.0f,
   -15.0f, -0.5f, -15.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, -1.0f,
                                                            
   15.0f, -0.5f,  15.0f,  0.0f, 0.0f, 1.0f,     0.0f, 1.0f, -1.0f,
  -15.0f, -0.5f, -15.0f,  0.0f, 0.0f, 1.0f,     0.0f, 1.0f, -1.0f,
   15.0f, -0.5f, -15.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    //BOTTOM
    0,1,3,
    1,2,3,
    //LEFT
    0,3,4,
    3,4,7,
    //TOP
    4,5,7,
    5,6,7,
    //RIGHT
    1,2,5,
    2,5,6,
    //FRONT
    0,1,4,
    1,4,5,
    //BACK
    2,3,6,
    3,6,7
};

std::vector<std::string> faces
{
	"sea_rt.JPG",
	"sea_lf.JPG",
	"sea_up.JPG",
	"sea_dn.JPG",
	"sea_bk.JPG",
	"sea_ft.JPG"
};

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
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
        0.0f, 1.0f, 0.05f,       0.0f, 1.0f, 0.0f,   0.0f, 0.0f,     //0
        -0.25f, 0.75f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   //1
        0.25f, 0.75f, 0.0f,     0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   //2
        -0.25f, -0.75f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   //3
        0.25f,-0.75f,0.0f,      0.0f, 1.0f, 0.0f,   0.0f, 1.0f,     //4
        0.0f, -1.0f, 0.05f,      0.0f, 1.0f, 0.0f,   0.5f, 0.0f,     //5
        0.0f, 0.0f, -0.25f,      0.0f, 1.0f, 0.0f,   0.5f, 0.0f,     //6 varf jos
        -0.25f, 0.75f, -0.25f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   //7
        0.25f, 0.75f, -0.25f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   //8
        -0.25f, -0.75f, -0.25f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   //9
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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.Reset();
    }
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
    camera.ProcessMouseScroll(yOffset);
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
    GLFWwindow* win = glfwCreateWindow(1900, 960, "Proiect Titanic", nullptr, nullptr);
    if (win == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetScrollCallback(win, scroll_callback);
    glEnable(GL_DEPTH_TEST);
    glewInit();

	// skybox VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxIBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

     //second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	// load textures
	// -------------
	
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------

    Shader lightingShader("light.vs", "light.fs");
    Shader triangleShader("vertexShader.vs", "fragShader.fs");
	Shader skyBoxShader("skybox.vs", "skybox.fs");
    Shader waterShader("water.vs", "water.fs");
	Shader titanicShader("titanic.vs", "titanic.fs");
    Shader icebergShader("Iceberg.vs", "Iceberg.fs");
	Shader whaleShader("whale.vs", "whale.fs");

	Model titanicModel("Cruisership 2012/NewScaleCruiser.obj");
    Model waterModel("Ocean obj/Ocean.obj");
    Model icebergModel("Iceberg/iceberg.obj");
	Model whaleModel("Whale/Whale.obj");
	skyBoxShader.SetInt("skybox", 0);
	
    //light Position
    glm::vec3 lightPos(17.0f, 5.0f, 0.0f);
    
    
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 waterMat = glm::mat4(1.0f);
    glm::mat4 icebergMat = glm::mat4(1.0f);

    CreateVBOs();
    CreateTextures(strExePath);

    while (!glfwWindowShouldClose(win))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(win);
        glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture1Location);
        //Boat
        triangleShader.Use();
       //
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
       
        triangleShader.SetMat4("projection", projection);
        triangleShader.SetMat4("view", view);
        triangleShader.SetMat4("transform", transform);
       
       // glBindVertexArray(VAO);
       // glActiveTexture(GL_TEXTURE0);

       // glDrawElements(GL_TRIANGLES, 3*17, GL_UNSIGNED_INT, 0);
       ////LIGHT
       // lightingShader.Use();
       // lightingShader.SetVec3("objectColor", 0.0f, 0.0f, 1.0f);
       // lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
       // lightingShader.SetVec3("lightPos", lightPos);
       // lightingShader.SetVec3("viewPos", camera.Position);
       // lightingShader.SetMat4("projection", projection);
       // lightingShader.SetMat4("view", view);
       // transform = glm::rotate(glm::mat4(1.0), glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
       // lightingShader.SetMat4("model", transform);
       // glBindVertexArray(waterVAO);
       // 
       //

		//Model shader
		titanicShader.SetMat4("projection", projection);
		titanicShader.SetMat4("view", view);
        modelMat = glm::rotate(transform, glm::radians(60.f), glm::vec3(0.0f, 1.0f, 0.0f));
		titanicShader.SetMat4("model", modelMat);
		titanicShader.Use();
		titanicModel.Draw(titanicShader);
        //draw water
        waterShader.Use();
        waterShader.SetMat4("projection", projection);
        waterShader.SetMat4("view", view);
        waterShader.SetMat4("transform", waterMat);
        waterModel.Draw(waterShader);
        //draw iceberg
        icebergShader.Use();
        icebergShader.SetMat4("projection", projection);
        icebergShader.SetMat4("view", view);
        icebergShader.SetMat4("transform", icebergMat);
        icebergModel.Draw(icebergShader);
		//draw whale
		whaleShader.Use();
		whaleShader.SetMat4("projection", projection);
		whaleShader.SetMat4("view", view);
		whaleShader.SetMat4("transform", transform);
		whaleModel.Draw(whaleShader);


		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        glEnable(GL_DEPTH_CLAMP);
		skyBoxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyBoxShader.SetMat4("view", view);
		skyBoxShader.SetMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &waterVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IndBO);
    glDeleteBuffers(1, &waterVBO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxIBO);
    glfwTerminate();
}

