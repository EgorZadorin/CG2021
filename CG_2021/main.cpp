#include <iostream> 
#include <fstream>
#include <vector>
#include <map>
#include "common.h"
#include "Image.h"
#include "Player.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

//window size
constexpr GLsizei WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 1280;

constexpr int MAP_WIDTH = WINDOW_WIDTH / tileSize, MAP_HEIGHT = WINDOW_WIDTH / tileSize;

constexpr int LEVELS_NUM = 2;

struct InputState
{
  bool keys[1024] {}; 
  GLfloat lastX = 400, lastY = 300; 
  bool firstMouse = true;
  bool captureMouse = true;  
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  switch (key)
  {
  case GLFW_KEY_ESCAPE:
    if (action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    break;
  //case GLFW_KEY_1:
  //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  //  break;
  //case GLFW_KEY_2:
  //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  //  break;
  default:
    if (action == GLFW_PRESS)
      Input.keys[key] = true;
    else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
    }
}

void processPlayerMovement(Player &player, std::vector<std::vector<int>> &tileMap, int &level)
{
  if (Input.keys[GLFW_KEY_W])
    player.ProcessInput(MovementDir::UP, tileMap, level);
  else if (Input.keys[GLFW_KEY_S])
    player.ProcessInput(MovementDir::DOWN, tileMap, level);
  if (Input.keys[GLFW_KEY_A])
    player.ProcessInput(MovementDir::LEFT, tileMap, level);
  else if (Input.keys[GLFW_KEY_D])
    player.ProcessInput(MovementDir::RIGHT, tileMap, level);
  if (Input.keys[GLFW_KEY_E]) 
    player.ProcessInput(MovementDir::OPEN, tileMap, level);
}

//void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
//{
//  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
//    Input.captureMouse = !Input.captureMouse;
//
//  if (Input.captureMouse)
//  {
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    Input.capturedMouseJustNow = true;
//  }
//  else
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

//}

//void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
//{
//  if (Input.firstMouse)
//  {
//    Input.lastX = float(xpos);
//    Input.lastY = float(ypos);
//    Input.firstMouse = false;
//  }

//  GLfloat xoffset = float(xpos) - Input.lastX;
//  GLfloat yoffset = Input.lastY - float(ypos);

//  Input.lastX = float(xpos);
//  Input.lastY = float(ypos);
//}


//void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
//{
//  // ...
//}

void createLevel(Image &level, const std::string &levelpath, std::vector<std::vector<int>> &tileMap, 
    std::map <char, Image> &tiles, Point &starting_pos, int cur_lvl) 
{
  std::fstream f;
  f.open(levelpath, std::ios::in);
  for (int i = 0; i < WINDOW_WIDTH; ++i) 
   for (int j = 0; j < WINDOW_HEIGHT; ++j) 
    level.PutPixel(i, j, backgroundColor); 
  
  int x = 0, y = 0;
  
  char c;

  while (f.get(c)) {
    switch (c) {
      case ' ':
        tileMap[y / tileSize][x / tileSize] = 1;
        for (int i = 0; i < tileSize; ++i)
          for (int j = 0; j < tileSize; ++j)
            level.DrawPixel(x + i, y + j, tiles[c].GetPixel(i, j));
        x += tileSize;
        break;
      case '#':
        tileMap[y / tileSize][x / tileSize] = 1;
        for (int i = 0; i < tileSize; ++i)
          for (int j = 0; j < tileSize; ++j)
            level.DrawPixel(x + i, y + j, tiles[c].GetPixel(i, j));
        x += tileSize;
        break;
      case '.':
        tileMap[y / tileSize][x / tileSize] = 0;
          for (int i = 0; i < tileSize; ++i)
            for (int j = 0; j < tileSize; ++j) 
              level.DrawPixel(x + i, y + j, tiles[c].GetPixel(i, j));
        x += tileSize;
        break;
      case '@':
        starting_pos.x = x;
        starting_pos.y = y;
        tileMap[y / tileSize][x / tileSize] = 0;
        for (int i = 0; i < tileSize; ++i)
          for (int j = 0; j < tileSize; ++j)
            level.DrawPixel(x + i, y + j, tiles['.'].GetPixel(i, j));
        x += tileSize;
        break;
      case 'x':
        tileMap[y / tileSize][x / tileSize] = cur_lvl + 1;
        for (int i = 0; i < tileSize; ++i)
          for (int j = 0; j < tileSize; ++j)
            level.DrawPixel(x + i, y + j, tiles[c].GetPixel(i, j));
        x += tileSize;
        break;
      case 'T': 
        tileMap[y / tileSize][x / tileSize] = -1;
          for (int i = 0; i < tileSize; ++i)
            for (int j = 0; j < tileSize; ++j)
              level.DrawPixel(x + i, y + j, tiles[c].GetPixel(i, j));
        x += tileSize;
        break;
      case '\n':
        y += tileSize;
        x = 0;
        break;
      default:
        break;
      }
    }
    f.close();
}

int initGL()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor" << std::endl;
  std::cout << "W, A, S, D - movement" << std::endl;
  std::cout << "E - go to the next level" << std::endl; 
  std::cout << "press ESC to exit" << std::endl;

  return 0;
}



int main(int argc, char** argv)
{
  if (!glfwInit())
    return -1;

  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Heroes of the forgotten dungeons", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }	
    
  //running current context
  glfwMakeContextCurrent(window);

  //initialysing callback functions
  glfwSetKeyCallback(window, OnKeyboardPressed);  
  //glfwSetCursorPosCallback(window, OnMouseMove); 
  //glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
  //glfwSetScrollCallback(window, OnMouseScroll);

  if (initGL() != 0) 
    return -1;
	
  //reset any OpenGL errors which could be present for some reason
  GLenum gl_error = glGetError();
  while (gl_error != GL_NO_ERROR)
    gl_error = glGetError();

  //starting from the middle of the screen
  Point starting_pos;

  //formalizing the map array 
  std::vector<std::vector<int>> tileMap(MAP_WIDTH, std::vector<int>(MAP_HEIGHT));

  Image currentLevel(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
  Image gameOver("./resources/game_over.png");
  Image gameWon("./resources/game_won.png");
  Image nextLevel("./resources/lvl_up.png");

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS; 
 
  int level_num = 1;
  bool level_changed = false;

  //mapping tiles with char
  auto tiles = std::map <char, Image>();

  tiles[' '] = Image("./resources/empty.png");
  tiles['.'] = Image("./resources/floor.png");
  tiles['#'] = Image("./resources/wall.png");
  tiles['x'] = Image("./resources/level.png");
  tiles['T'] = Image("./resources/spike.png");

  //creating level 1
  createLevel(currentLevel, "./resources/level1.txt", tileMap, tiles, starting_pos, level_num);
 
  //creating player
  Player player(starting_pos, "./resources/hero.png");
   
  //game loop
  while (!glfwWindowShouldClose(window) && level_num >= 0 && level_num <= LEVELS_NUM)
  {
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();
    
    //moving player
    processPlayerMovement(player, tileMap, level_num);
 
    //drawing movement 
    player.Draw(screenBuffer, currentLevel);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    
    //game lost
    if (level_num == -1) 
    {
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, gameOver.Data()); GL_CHECK_ERRORS; 
    }

    //moving to the next level
    else if (level_num == LEVELS_NUM && !level_changed) 
    {
      createLevel(currentLevel, "./resources/level2.txt", tileMap, tiles, starting_pos, level_num);
      player.NewPos(starting_pos);
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, nextLevel.Data()); GL_CHECK_ERRORS;
      level_changed = true;
    }

    //end of the game reached 
    else if (level_num > LEVELS_NUM) 
    {
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, gameWon.Data()); GL_CHECK_ERRORS;
    }

    //continue
    else
    { 
      glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
    }

    glfwSwapBuffers(window);
  }
  
  //waiting for "esc" command
  while (!glfwWindowShouldClose(window)) 
  {
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
