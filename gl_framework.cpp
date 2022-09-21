#include "gl_framework.hpp"
#include <GLFW/glfw3.h>
extern GLfloat xrot, yrot, zrot, rotamount, cursor_x, cursor_y, cursor_z, moveamount, cursor_max, cursor_min;
extern bool persp, draw_grid;
extern void insertAtCursor(), deleteAtCursor(), updateCursor(), resetModel();
extern void saveModelToFile(std::string), readModelFromFile(std::string);
extern float cursor_r, cursor_g, cursor_b;
extern Point cursor_color;
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
            updateCursor();
        }
    }
    else if(key == GLFW_KEY_S && action == GLFW_PRESS) {
        if(cursor_x > cursor_min) {
            cursor_x -= moveamount;
            updateCursor();
        }
    }
    else if(key == GLFW_KEY_D && action == GLFW_PRESS) {
        if(cursor_y < cursor_max - moveamount) {
            cursor_y += moveamount;
            updateCursor();
        }
    }
    else if(key == GLFW_KEY_A && action == GLFW_PRESS) {
        if(cursor_y > cursor_min) {
            cursor_y -= moveamount;
            updateCursor();
        }
    }
    else if(key == GLFW_KEY_E && action == GLFW_PRESS) {
        if(cursor_z < cursor_max - moveamount) {
            cursor_z += moveamount;
            updateCursor();
        }
    }
    else if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
        if(cursor_z > cursor_min) {
            cursor_z -= moveamount;
            updateCursor();
        }
    }
    else if(key == GLFW_KEY_V && action == GLFW_PRESS) {
		if(persp)
			persp = false;
		else
			persp = true;
    }
    else if(key == GLFW_KEY_P && action == GLFW_PRESS) {
        insertAtCursor();
    }
    else if(key == GLFW_KEY_X && action == GLFW_PRESS) {
        deleteAtCursor();
    }
	else if(key == GLFW_KEY_C && action == GLFW_PRESS){
		float cursor_r, cursor_g, cursor_b;
		std::cout<<"Changing the cursor color:\n";
		std::cout<<"Enter 'r' component:";
		std::cin>>cursor_r;
		std::cout<<"Enter 'g' component:";
		std::cin>>cursor_g;
		std::cout<<"Enter 'b' component:";
		std::cin>>cursor_b;
		Point updated_color(cursor_r, cursor_g, cursor_b);
		cursor_color = updated_color;
		std::cout<<"Cursor color changed successfully.\n";
	}
	else if(key == GLFW_KEY_G && action == GLFW_PRESS){
        std::string fname;
        std::cout << "Enter filename:";
        std::cin >> fname;
        saveModelToFile(fname);
        std::cout << "Done!\n";
	}
	else if(key == GLFW_KEY_R && action == GLFW_PRESS){
        resetModel();
	}
	else if(key == GLFW_KEY_K && action == GLFW_PRESS){ 
        resetModel();
        std::string fname;
        std::cout << "Enter filename:";
        std::cin >> fname;
        readModelFromFile(fname);
        std::cout << "Done!\n";
	}
    else if(key == GLFW_KEY_T && action == GLFW_PRESS) {
        draw_grid = !draw_grid;
    }
  }
};  
  


