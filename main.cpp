#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "main.hpp"
/*
float points[] = {
    -0.95f,  0.95f, 0.0f,
     0.95f,  0.95f, 0.0f,
     0.95f, -0.95f, 0.0f,
    -0.95f, -0.95f, 0.0f
  };
*/
float points[(N_CELLS+1)*8]; // 2*(N+1) number of lines, 2 points / line, 2 floats / point
void defineGrid() {
    float coords[N_CELLS+1][N_CELLS+1][2];
    for(int i = 0; i <= N_CELLS; i++) {
        for(int j = 0; j <= N_CELLS; j++) {
            coords[i][j][0] = DRAW_MIN + i * (DRAW_MAX - DRAW_MIN) / N_CELLS;
            coords[i][j][1] = DRAW_MIN + j * (DRAW_MAX - DRAW_MIN) / N_CELLS;
        }
    }
    int idx = 0;
    for(int i = 0; i <= N_CELLS; i++) {
        points[idx++] = coords[i][0][0]; // x coord 
        points[idx++] = coords[i][0][1]; // y coord 
        points[idx++] = coords[i][N_CELLS][0]; // x coord 
        points[idx++] = coords[i][N_CELLS][1]; // y coord 
    }
    for(int i = 0; i <= N_CELLS; i++) {
        points[idx++] = coords[0][i][0]; // x coord 
        points[idx++] = coords[0][i][1]; // y coord 
        points[idx++] = coords[N_CELLS][i][0]; // x coord 
        points[idx++] = coords[N_CELLS][i][1]; // y coord 
    }
    std::cout << "Last idx = " << idx << "\n";
}

GLuint shaderProgram;
GLuint vbo, vao;

void initShadersGL(void)
{
  std::string vertex_shader_file("simple_vs.glsl");
  std::string fragment_shader_file("simple_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  
}

void initVertexBufferGL(void)
{
  //Ask GL for a Vertex Buffer Object (vbo)
  glGenBuffers (1, &vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  //Copy the points into the current buffer - 9 float values, start pointer and static data
  glBufferData (GL_ARRAY_BUFFER, 8 * (N_CELLS+1) * sizeof (float), points, GL_STATIC_DRAW);

  //Ask GL for a Vertex Attribute Object (vao)
  glGenVertexArrays (1, &vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (vao);
  //Enable the vertex attribute
  glEnableVertexAttribArray (0);
  //This the layout of our first vertex buffer
  //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats 
  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);

  glBindVertexArray (vao);

  // Draw points 0-3 from the currently bound VAO with current in-use shader
  glDrawArrays(GL_LINES, 0, 4*(N_CELLS+1));
}

int main(int argc, char** argv)
{
  defineGrid();
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(640, 480, "CS475/CS675 OpenGL Framework", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initShadersGL();
  initVertexBufferGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

