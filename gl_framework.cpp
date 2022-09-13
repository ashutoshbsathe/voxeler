#include "gl_framework.hpp"
extern GLfloat xrot, yrot, zrot, rotamount, cursor_x, cursor_y, cursor_z, moveamount, cursor_max, cursor_min;
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST); 
    //Enable Gourard shading
    glShadeModel(GL_SMOOTH);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        yrot -= rotamount;
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        yrot += rotamount;
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
        xrot -= rotamount;
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        xrot += rotamount;
    }
    else if(key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
        zrot -= rotamount;
    }
    else if(key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
        zrot += rotamount;
    }
    else if(key == GLFW_KEY_W && action == GLFW_PRESS) {
        if(cursor_x < cursor_max - moveamount) {
            cursor_x += moveamount;
        }
    }
    else if(key == GLFW_KEY_S && action == GLFW_PRESS) {
        if(cursor_x > cursor_min) {
            cursor_x -= moveamount;
        }
    }
    else if(key == GLFW_KEY_D && action == GLFW_PRESS) {
        if(cursor_y < cursor_max - moveamount) {
            cursor_y += moveamount;
        }
    }
    else if(key == GLFW_KEY_A && action == GLFW_PRESS) {
        if(cursor_y > cursor_min) {
            cursor_y -= moveamount;
        }
    }
    else if(key == GLFW_KEY_E && action == GLFW_PRESS) {
        if(cursor_z < cursor_max - moveamount) {
            cursor_z += moveamount;
        }
    }
    else if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
        if(cursor_z > cursor_min) {
            cursor_z -= moveamount;
        }
    }
  }
};  
  


