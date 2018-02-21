#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

// System Headers.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "ShaderProgram.hpp"
#include <exception>
#include <math.h>       /* sin */
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <stb_image.h>

#include "BOX.h"


GLFWwindow* window;
unsigned int VAO, VBO, VBO2, VBO3, EBO, textureID;

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
      std::cout << scancode << " " << mods << std::endl;

      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
      }
}

void
init()
{
    // Initialize GLFW. Most GLFW functions will not work before doing this.
    if (!glfwInit()) {
        throw;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    // Create the window
    window = glfwCreateWindow(800, 600, "Cube!", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create the GLFW window" << std::endl;
        throw;
    }

    // Setup a key callback. It will be called every time a key is pressed, repeated or released.
    glfwSetKeyCallback(window, key_callback);

    // Make the OpenGL context current.
    glfwMakeContextCurrent(window);

    // Enable v-sync.
    glfwSwapInterval(1);

    // Make the window visible.
    glfwShowWindow(window);

    gladLoadGL();
}


void load_texture(const char * filename) {
      int width, heigth, channels;
      unsigned char* pixels = stbi_load (filename, &width, &heigth, &channels, 0);

      glGenTextures(1, &textureID);
      glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have

      // set the texture wrapping parameters.
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glGenerateMipmap(GL_TEXTURE_2D);

      free(pixels);
}

void
buffers_cube() {

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);  // Create VertexArrayObject.
      glGenBuffers(1, &VBO2); // Create VertexArrayObject.
      glGenBuffers(1, &VBO3); // Create VertexArrayObject.
      glGenBuffers(1, &EBO);  // Create VertexArrayObject.

      glBindVertexArray(VAO);

      // Position attribute...
      glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind Vertex VAO...
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPos) * 3, cubeVertexPos, GL_STATIC_DRAW); // Assign buffer to VBO
      glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
      glEnableVertexAttribArray(0);

      // Normal attribute...
      glBindBuffer(GL_ARRAY_BUFFER, VBO2);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexNormal) * 3, cubeVertexNormal, GL_STATIC_DRAW);
      glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
      glEnableVertexAttribArray(1);

      // Text Coord attribute...
      glBindBuffer(GL_ARRAY_BUFFER, VBO3);
      glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexTexCoord) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
      glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);
      glEnableVertexAttribArray(2);

      // Buffer indexes...
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(GL_UNSIGNED_INT) * 3, cubeTriangleIndex, GL_STATIC_DRAW);


      glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO.
      glBindVertexArray(0); // Unbind the VAO.
}

std::string
getDataFile(const std::string& filename)
{
      std::ifstream file(filename);

      file.open("r");
      std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
      file.close();
      return data;
}


std::string
getVertexShaderSource()
{
      return getDataFile("/home/javi/MasterInformatica/2Q/AG/GlitterTag/Glitter/Sources/vertex.glsl");
}


std::string
getFragmentShaderSource()
{
      return getDataFile("/home/javi/MasterInformatica/2Q/AG/GlitterTag/Glitter/Sources/fragments.glsl");
}

void
loop() {
      ShaderProgram* shaderProgram = new ShaderProgram();

      shaderProgram->createVertexShader(getVertexShaderSource());
      shaderProgram->createFragmentShader(getFragmentShaderSource());
      shaderProgram->link();

      shaderProgram->createUniform("projection");
      shaderProgram->createUniform("view");
      shaderProgram->createUniform("model");
      shaderProgram->createUniform("diffuseTexture");

      load_texture("/home/javi/MasterInformatica/2Q/AG/GlitterTag/textura.png");
      buffers_cube();

      float aspectRatio = 4.0f / 3.0f;
      glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.001f, 1000.0f);

      glm::mat4 view(1.0f);
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));


      glEnable(GL_DEPTH_TEST);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      GLint64 timer;
      while (!glfwWindowShouldClose(window)) {

            glGetInteger64v(GL_TIMESTAMP, &timer);

            float angle = std::fmod((timer / 1000000000.0), (2.0f * M_PI));
            model = glm::rotate(angle, glm::vec3(1.0f, -1.0f, 0.0f));

            // Poll for window events. The key callback above will only be
            // invoked during this call.
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0); // Activate first texture unit

            // Render...
            shaderProgram->bind();

            shaderProgram->setUniform("projection", projection);
            shaderProgram->setUniform("view", view);
            shaderProgram->setUniform("diffuseTexture", 0); // El índice es el mismo que en glActiveTexture()

            glBindVertexArray(VAO); // Activa la geometría que se va a pintar.
            glBindTexture(GL_TEXTURE_2D, textureID);

            shaderProgram->setUniform("model", model);

            glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(window); // swap the color buffers
      }
}

void
run()
{
      try {
            init();
            loop();
      } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
      }
      glfwTerminate();
}

int
main() {
      run();
      return 0;
}
