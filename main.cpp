#include <iostream>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include "Settings.h"
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"
#include "InputController.h"
#include "TextureLoader.h"
#include "TerrainTile.h"

GLFWwindow* initGLFW();
void generateTerrainData(std::vector<std::vector<float>>& map, int cycles, std::vector<TerrainTile>& tiles);
void generateTerrainTestData(std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles);
void createTiles(int x, int y, int width, int depth, std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles);
template <typename T> void initializeMap(std::vector<std::vector<T>>& map)
{
  map.clear();
  map.reserve(LANDS_HEIGHT + 1);
  for (size_t row = 0; row < LANDS_HEIGHT + 1; row++)
    {
      std::vector<T> rowN;
      rowN.reserve(LANDS_WIDTH + 1);
      map.emplace_back(rowN);
    }
  for (auto& row : map)
    for (size_t elem = 0; elem < LANDS_WIDTH + 1; elem++)
      row.emplace_back(0);
}

GLFWwindow* window;
Timer timer;
Camera cam(glm::vec3(0.0f, 3.0f, 0.0f));
InputController input;
TextureLoader textureLoader;

int main()
{
  //GLOBAL SETUP
  window = initGLFW();
  glewExperimental = GL_TRUE;
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  //SHADER STUFF
  Shader scene(PROJ_PATH + "/shaders/scene.vs", PROJ_PATH + "/shaders/scene.fs");
  scene.use();

  //TEXTURE LOADING
  glActiveTexture(GL_TEXTURE0);
  GLuint flatTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/grass.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE1);
  GLuint hillTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/grassHill.jpg", GL_REPEAT);
  scene.setInt("grassTexture", 0);
  scene.setInt("hillTexture", 1);

  //BASE TILE VERTEX DATA
  GLuint VAO, VBO, EBO, EBOx;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &EBOx);

  GLuint indices[] = {
    0,1,2,
    2,3,0
  };
  GLuint indicesCrossed[] = {
    3,0,1,
    1,2,3
  };
  //height set slightly higher than base 0.0 level to prevent z-fighting
  GLfloat flatSquareVertices[] = {
     -0.5f * LANDS_WIDTH, -0.01f,  0.5f * LANDS_WIDTH,  0.0f, 0.0f,
      0.5f * LANDS_WIDTH, -0.01f,  0.5f * LANDS_WIDTH,  1.0f * LANDS_WIDTH, 0.0f,
      0.5f * LANDS_WIDTH, -0.01f, -0.5f * LANDS_WIDTH,  1.0f * LANDS_WIDTH, 1.0f * LANDS_WIDTH,
     -0.5f * LANDS_WIDTH, -0.01f, -0.5f * LANDS_WIDTH,  0.0f, 1.0f * LANDS_WIDTH
    };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOx);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCrossed), indicesCrossed, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(flatSquareVertices), flatSquareVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //INSTANCE TILES VERTEX DATA
  GLuint iVAO, iVBO, iEBO;
  glGenVertexArrays(1, &iVAO);
  glBindVertexArray(iVAO);
  glGenBuffers(1, &iVBO);
  glGenBuffers(1, &iEBO);

  GLfloat vertices[] = {
    -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 1.0f,
    -0.5f, 0.0f, -0.5f,  0.0f, 1.0f
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, iVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  //generating land height map
  std::vector<std::vector<float>> map;
  std::vector<TerrainTile> tiles;
  tiles.reserve(NUM_LANDS);
//  generateTerrainTestData(map, tiles);
  generateTerrainData(map, 8, tiles);
  generateTerrainData(map, 8, tiles);
  generateTerrainData(map, 8, tiles);
  tiles.shrink_to_fit();

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(cam.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float delta = timer.tick();
      input.processKeyboard(delta);

      //scene update
      scene.use();
      scene.setMat4("projection", projection);
      scene.setMat4("view", cam.getViewMatrix());

      //height tiles
      glBindVertexArray(iVAO);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, hillTexture);
      for (auto& tile : tiles)
        {
          /* method 1: using glMapBuffer = slow*/
//          glBindBuffer(GL_ARRAY_BUFFER, iVBO);
//          float* vertex_pointer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//          *vertex_pointer = -0.5 * (tile.width * (tile.width - 1) / tile.width);
//          *(vertex_pointer+1) = tile.lowLeft;
//          *(vertex_pointer+2) =  0.5 * (tile.depth * (tile.depth - 1) / tile.depth);

//          *(vertex_pointer+5) =  0.5 * (tile.width * (tile.width - 1) / tile.width);
//          *(vertex_pointer+6) = tile.lowRight;
//          *(vertex_pointer+7) =  0.5 * (tile.depth * (tile.depth - 1) / tile.depth);
//          *(vertex_pointer+8) =  2.0f * tile.width - 1;

//          *(vertex_pointer+10) =  0.5 * (tile.width * (tile.width - 1) / tile.width);
//          *(vertex_pointer+11) = tile.upperRight;
//          *(vertex_pointer+12) = -0.5 * (tile.depth * (tile.depth - 1) / tile.depth);
//          *(vertex_pointer+13) =  2.0f * tile.width - 1;
//          *(vertex_pointer+14) =  2.0f * tile.depth - 1;

//          *(vertex_pointer+15) = -0.5 * (tile.width * (tile.width - 1) / tile.width);
//          *(vertex_pointer+16) = tile.upperLeft;
//          *(vertex_pointer+17) = -0.5 * (tile.depth * (tile.depth - 1) / tile.depth);
//          *(vertex_pointer+19) =  2.0f * tile.depth - 1;
//          glUnmapBuffer(GL_ARRAY_BUFFER);

          /*
            Method 2: Brute force = fast enough
          */
          vertices[0] = -0.5 * (tile.width * (tile.width - 1) / tile.width);
          vertices[1] = tile.lowLeft;
          vertices[2] =  0.5 * (tile.depth * (tile.depth - 1) / tile.depth);

          vertices[5] =  0.5 * (tile.width * (tile.width - 1) / tile.width);
          vertices[6] = tile.lowRight;
          vertices[7] =  0.5 * (tile.depth * (tile.depth - 1) / tile.depth);
          vertices[8] =  2.0f * tile.width - 1;

          vertices[10] =  0.5 * (tile.width * (tile.width - 1) / tile.width);
          vertices[11] = tile.upperRight;
          vertices[12] = -0.5 * (tile.depth * (tile.depth - 1) / tile.depth);
          vertices[13] =  2.0f * tile.width - 1;
          vertices[14] =  2.0f * tile.depth - 1;

          vertices[15] = -0.5 * (tile.width * (tile.width - 1) / tile.width);
          vertices[16] = tile.upperLeft;
          vertices[17] = -0.5 * (tile.depth * (tile.depth - 1) / tile.depth);
          vertices[19] =  2.0f * tile.depth - 1;

          glBindBuffer(GL_ARRAY_BUFFER, iVBO);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
          glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
          if (tile.orderCrossed)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOx);
          else
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          scene.setMat4("model", tile.model);
          scene.setFloat("HeightOffset", (rand()/RAND_MAX) / 10);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

      //base tile
      glBindVertexArray(VAO);
      glm::mat4 model;
      scene.setMat4("model", model);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
    }

  //CLEANUP
  glDeleteTextures(1, &flatTexture);
  glDeleteTextures(1, &hillTexture);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &EBOx);
  glDeleteVertexArrays(1, &iVAO);
  glDeleteBuffers(1, &iVBO);
  glDeleteBuffers(1, &iEBO);
  scene.cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow* initGLFW()
{
  glfwSetErrorCallback([](int, const char* msg){printf("Error while loading GLFW: %s", msg);});
  if (!glfwInit())
    {
      std::cerr << "Error while loading GLFW\n";
      glfwTerminate();
    }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain Test", glfwGetPrimaryMonitor(), 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void createTiles(int x, int y, int width, int depth, std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles)
{
  glm::vec3 translation(-1.0f * (LANDS_WIDTH / 2 - x - width / 2 + (width+1) % 2),
                         0.0f,
                        -1.0f * (LANDS_HEIGHT / 2 - y - depth / 2 - (depth+1) % 2));
  //flat rect
  glm::mat4 model;
  model = glm::translate(model, translation);
  model = glm::translate(model, glm::vec3((width+1)%2/2.0f, 0.0f, -(depth+1)%2/2.0*3));
  tiles.emplace_back(model, x, y, map[y+depth-1][x], map[y+depth-1][x+width-1], map[y][x+width-1], map[y][x], width, depth);

  //lower oblique
  glm::mat4 low;
  low = glm::translate(low, translation);
  low = glm::translate(low, glm::vec3((width % 2 == 0 ? 0.5f : 0), 0.0f, depth / 2.0f - (depth+1)%2/2.0f*3));
  tiles.emplace_back(low, x, y, map[y+depth][x], map[y+depth][x+width-1], map[y+depth-1][x+width-1], map[y+depth-1][x], width, 2);

  //upper oblique
  glm::mat4 up;
  up = glm::translate(up, translation);
  up = glm::translate(up, glm::vec3((width % 2 == 0 ? 0.5f : 0), 0.0f, -depth / 2 - 0.5f - (depth+1) % 2));
  tiles.emplace_back(up, x, y, map[y][x], map[y][x+width-1], map[y-1][x+width-1], map[y-1][x], width, 2);

  //left oblique
  glm::mat4 left;
  left = glm::translate(left, translation);
  left = glm::translate(left, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0), 0.0f, -(depth+1)%2/2.0*3));
  tiles.emplace_back(left, x, y, map[y+depth-1][x-1], map[y+depth-1][x], map[y][x], map[y][x-1], 2, depth);

  //right oblique
  glm::mat4 right;
  right = glm::translate(right, translation);
  right = glm::translate(right, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0) + width, 0.0f, -(depth+1)%2/2.0*3));
  tiles.emplace_back(right, x, y, map[y+depth-1][x+width-1], map[y+depth-1][x+width], map[y][x+width], map[y][x+width-1], 2, depth);

  //lower-left corner
  glm::mat4 ll;
  ll = glm::translate(ll, translation);
  ll = glm::translate(ll, glm::vec3(-width / 2.0f + (width % 2 == 0 ? 0.5f : 0), 0.0f, depth / 2.0f - (depth+1)%2/2.0f*3));
  tiles.emplace_back(ll, x, y, map[y+depth][x-1], map[y+depth][x], map[y+depth-1][x], map[y+depth-1][x-1], 2, 2);

  //lower-right corner
  glm::mat4 lr;
  lr = glm::translate(lr, translation);
  lr = glm::translate(lr, glm::vec3(width / 2.0f + (width % 2 == 0 ? 0.5f : 0), 0.0f, depth / 2.0f - (depth+1)%2/2.0f*3));
  tiles.emplace_back(lr, x, y, map[y+depth][x+width-1], map[y+depth][x+width], map[y+depth-1][x+width], map[y+depth-1][x+width-1], 2, 2, true);

  //upper-right corner
  glm::mat4 ur;
  ur = glm::translate(ur, translation);
  ur = glm::translate(ur, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0) + width, 0.0f, -depth / 2 - 0.5f - (depth+1) % 2));
  tiles.emplace_back(ur, x, y, map[y][x+width-1], map[y][x+width], map[y-1][x+width], map[y-1][x+width-1], 2, 2);

  //upper-left corner
  glm::mat4 ul;
  ul = glm::translate(ul, translation);
  ul = glm::translate(ul, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0), 0.0f, -depth / 2 - 0.5f - (depth+1) % 2));
  tiles.emplace_back(ul, x, y, map[y][x-1], map[y][x], map[y-1][x], map[y-1][x-1], 2, 2, true);
}

void generateTerrainData(std::vector<std::vector<float>>& map, int cycles, std::vector<TerrainTile>& tiles)
{
  srand(time(NULL));
  const int MIN_DISTANCE = cycles;
  for (int c = 0; c < cycles; c++)
    {
      initializeMap(map);
      int width = rand() / (RAND_MAX / 12) + 6;
      int depth = rand() / (RAND_MAX / 12) + 6;
      int cycle = rand() / (RAND_MAX / LANDS_WIDTH / MIN_DISTANCE);
      while (cycle <= MIN_DISTANCE)
        cycle = rand() / (RAND_MAX / LANDS_WIDTH / MIN_DISTANCE);
      for (int y = 1; y < LANDS_HEIGHT; y++)
        {
          for (int x = 1; x < LANDS_WIDTH; x++)
            {
              if ((y * LANDS_WIDTH + x) % cycle == 0)
                {
                  for (int w = 0; w < width; w++)
                    {
                      if (x + width >= LANDS_WIDTH)
                        width -= x+width - LANDS_WIDTH;
                      for (int d = 0; d < depth; d++)
                        {
                          if (y + depth >= LANDS_HEIGHT)
                            depth -= y+depth - LANDS_HEIGHT;
                          map[y+d][x+w] += (c+1) / (3.0f + c / 2.0f) + rand()/(float)RAND_MAX/(depth+width)*(depth*width/2.2f);
                        }
                    }
                  createTiles(x, y, width, depth, map, tiles);

                  x += width;
                  y += depth;
                  width = rand() / (RAND_MAX / (12 - c)) + 6;
                  depth = rand() / (RAND_MAX / (12 - c)) + 6;
                }
              cycle = rand() / (RAND_MAX / LANDS_WIDTH / MIN_DISTANCE);
              while (cycle <= MIN_DISTANCE)
                cycle = rand() / (RAND_MAX / LANDS_WIDTH / MIN_DISTANCE);
            }
        }
    }
}

void generateTerrainTestData(std::vector<std::vector<float>>& map, std::vector<TerrainTile>& tiles)
{
  initializeMap(map);
  int width = 5;
  int depth = 6;
  int y = 2, x = 2;
  for (int y1 = y; y1 < y + depth; y1++)
    {
      for (int x1 = x; x1 < x + width; x1++)
        {
          map[y1][x1] += 1.0f;
        }
    }
  glm::vec3 translation(-1.0f * (LANDS_WIDTH / 2 - x - width / 2 + (width+1) % 2),
                         0.0f,
                        -1.0f * (LANDS_HEIGHT / 2 - y - depth / 2 - (depth+1) % 2));
  //flat rect
  glm::mat4 model;
  model = glm::translate(model, translation);
  model = glm::translate(model, glm::vec3((width+1)%2/2.0f, 0.0f, -(depth+1)%2/2.0*3));
  TerrainTile flatTile(model, x, y, map[y+depth-1][x], map[y+depth-1][x+width-1], map[y][x+width-1], map[y][x], width, depth);
  tiles.push_back(flatTile);

  //lower oblique
  glm::mat4 low;
  low = glm::translate(low, translation);
  low = glm::translate(low, glm::vec3((width % 2 == 0 ? 0.5f : 0), 0.0f, depth / 2.0f - (depth+1)%2/2.0f*3));
  TerrainTile lowTile(low, x, y, map[y+depth][x], map[y+depth][x+width-1], map[y+depth-1][x+width-1], map[y+depth-1][x], width, 2);
  tiles.push_back(lowTile);

  //upper oblique
  glm::mat4 up;
  up = glm::translate(up, translation);
  up = glm::translate(up, glm::vec3((width % 2 == 0 ? 0.5f : 0), 0.0f, -depth / 2 - 0.5f - (depth+1) % 2));
  TerrainTile upTile(up, x, y, map[y][x], map[y][x+width-1], map[y-1][x+width-1], map[y-1][x], width, 2);
  tiles.push_back(upTile);

  //left oblique
  glm::mat4 left;
  left = glm::translate(left, translation);
  left = glm::translate(left, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0), 0.0f, -(depth+1)%2/2.0*3));
  TerrainTile leftTile(left, x, y, map[y+depth-1][x-1], map[y+depth-1][x], map[y][x], map[y][x-1], 2, depth);
  tiles.push_back(leftTile);

  //right oblique
  glm::mat4 right;
  right = glm::translate(right, translation);
  right = glm::translate(right, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0) + width, 0.0f, -(depth+1)%2/2.0*3));
  TerrainTile rightTile(right, x, y, map[y+depth-1][x+width-1], map[y+depth-1][x+width], map[y][x+width], map[y][x+width-1], 2, depth);
  tiles.push_back(rightTile);

  //lower-left corner
  glm::mat4 ll;
  ll = glm::translate(ll, translation);
  ll = glm::translate(ll, glm::vec3(-width / 2.0f + (width % 2 == 0 ? 0.5f : 0), 0.0f, depth / 2.0f - (depth+1)%2/2.0f*3));
  TerrainTile llTile(ll, x, y, map[y+depth][x-1], map[y+depth][x], map[y+depth-1][x], map[y+depth-1][x-1], 2, 2);
  tiles.push_back(llTile);

  //lower-right corner
  glm::mat4 lr;
  lr = glm::translate(lr, translation);
  lr = glm::translate(lr, glm::vec3(width / 2.0f + (width % 2 == 0 ? 0.5f : 0), 0.0f, depth / 2.0f - (depth+1)%2/2.0f*3));
  TerrainTile lrTile(lr, x, y, map[y+depth][x+width-1], map[y+depth][x+width], map[y+depth-1][x+width], map[y+depth-1][x+width-1], 2, 2, true);
  tiles.push_back(lrTile);

  //upper-right corner
  glm::mat4 ur;
  ur = glm::translate(ur, translation);
  ur = glm::translate(ur, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0) + width, 0.0f, -depth / 2 - 0.5f - (depth+1) % 2));
  TerrainTile urTile(ur, x, y, map[y][x+width-1], map[y][x+width], map[y-1][x+width], map[y-1][x+width-1], 2, 2);
  tiles.push_back(urTile);

  //upper-left corner
  glm::mat4 ul;
  ul = glm::translate(ul, translation);
  ul = glm::translate(ul, glm::vec3(-width / 2 - 0.5f + (width % 2 == 0 ? 1.0f : 0), 0.0f, -depth / 2 - 0.5f - (depth+1) % 2));
  TerrainTile ulTile(ul, x, y, map[y][x-1], map[y][x], map[y-1][x], map[y-1][x-1], 2, 2, true);
  tiles.push_back(ulTile);
}
