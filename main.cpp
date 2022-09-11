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
float grid_offsets[N_CELLS+2];
float cursor_points[] {
    -0.95f,  0.95f, 0.0f,
     0.95f,  0.95f, 0.0f,
     0.95f, -0.95f, 0.0f,
    -0.95f, -0.95f, 0.0f
};
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
    /*
    for(int i = 0; i <= N_CELLS; i++) {
        points[idx++] = coords[0][i][0]; // x coord 
        points[idx++] = coords[0][i][1]; // y coord 
        points[idx++] = coords[N_CELLS][i][0]; // x coord 
        points[idx++] = coords[N_CELLS][i][1]; // y coord 
    }
    */
    std::cout << "Last idx = " << idx << "\n";
    // prepare `grid_offsets` which will be passed as a uniform 
    grid_offsets[0] = N_CELLS;
    for(int i = 1; i <= N_CELLS+1; i++) {
        grid_offsets[i] = DRAW_MIN + (i-1) * (DRAW_MAX - DRAW_MIN) / N_CELLS;
    }
}

GLuint grid_shader_program, cursor_shader_program;
GLuint grid_vbo, grid_vao, cursor_vbo, cursor_vao;

GLuint grid_offset_id, cursor_position_id, cursor_color_id;

glm::mat4 rotation_matrix;
glm::mat4 view_matrix;
glm::mat4 ortho_matrix;
glm::mat4 projection_matrix;
glm::mat4 modelviewproject_matrix;
GLuint grid_uModelViewProjectMatrix_id, cursor_uModelViewProjectMatrix_id;

float cube_coords[36*3];
float cube_colors[12*3] = {
    183/255.0, 28/255.0, 28/255.0,
    136/255.0, 14/255.0, 79/255.0,
    179/255.0, 136/255.0, 255/255.0,
    74/255.0, 20/255.0, 140/255.0,
    140/255.0, 158/255.0, 255/255.0,
    83/255.0, 109/255.0, 254/255.0,
    100/255.0, 255/255.0, 218/255.0,
    29/255.0, 233/255.0, 182/255.0,
    238/255.0, 255/255.0, 65/255.0,
    198/255.0, 255/255.0, 0/255.0,
    158/255.0, 158/255.0, 158/255.0,
    238/255.0, 238/255.0, 238/255.0,
};
void cubeAt(float x, float y, float z) {
    // 6 faces, 12 triangles, 36 vertices, 36 * 3 coords
    // cube's left bottom back is at (x, y, z)
    float temp[36*3] = {
        x, y, z,
        x+N_UNITS, y, z,
        x+N_UNITS, y+N_UNITS, z,

        x, y, z,
        x+N_UNITS, y+N_UNITS, z,
        x, y+N_UNITS, z,

        x, y, z,
        x, y+N_UNITS, z+N_UNITS,
        x, y, z+N_UNITS,

        x, y, z,
        x, y+N_UNITS, z,
        x, y+N_UNITS, z+N_UNITS,

        x, y, z,
        x+N_UNITS, y, z+N_UNITS,
        x+N_UNITS, y, z,

        x, y, z,
        x, y, z+N_UNITS,
        x+N_UNITS, y, z+N_UNITS,

        x, y, z+N_UNITS,
        x+N_UNITS, y, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,

        x, y, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
        x, y+N_UNITS, z+N_UNITS,

        x+N_UNITS, y, z,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
        x+N_UNITS, y, z+N_UNITS,

        x+N_UNITS, y, z,
        x+N_UNITS, y+N_UNITS, z,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,

        x, y+N_UNITS, z,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z,

        x, y+N_UNITS, z,
        x, y+N_UNITS, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
    };
    // I might be hating C++
    for(int i = 0; i < 36*3; i++) {
        cube_coords[i] = temp[i];
    }
}

void gridInitShadersGL(void)
{
  std::string vertex_shader_file("grid_vs.glsl");
  std::string fragment_shader_file("grid_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  grid_shader_program = csX75::CreateProgramGL(shaderList);
  grid_offset_id = glGetUniformLocation(grid_shader_program, "grid_offsets");
  grid_uModelViewProjectMatrix_id = glGetUniformLocation(grid_shader_program, "uModelViewProjectMatrix");
}

void cursorInitShadersGL(void)
{
  std::string vertex_shader_file("cursor_vs.glsl");
  std::string fragment_shader_file("cursor_fs.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  cursor_shader_program = csX75::CreateProgramGL(shaderList);
  cursor_position_id = glGetAttribLocation(cursor_shader_program, "vPosition");
  cursor_color_id = glGetAttribLocation(cursor_shader_program, "vColor");
  cursor_uModelViewProjectMatrix_id = glGetUniformLocation(cursor_shader_program, "uModelViewProjectMatrix");
}

void gridInitVertexBufferGL(void)
{
  //Ask GL for a Vertex Attribute Object (grid_vao)
  glGenVertexArrays (1, &grid_vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (grid_vao);
  //Ask GL for a Vertex Buffer Object (grid_vbo)
  glGenBuffers (1, &grid_vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, grid_vbo);
  //Copy the points into the current buffer - 9 float values, start pointer and static data
  glBufferData (GL_ARRAY_BUFFER, 8 * (N_CELLS+1) * sizeof (float), points, GL_STATIC_DRAW);

  //Enable the vertex attribute
  glEnableVertexAttribArray (0);
  //This the layout of our first vertex buffer
  //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats 
  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void cursorInitVertexBufferGL(void)
{
  //Ask GL for a Vertex Attribute Object (cursor_vao)
  glGenVertexArrays (1, &cursor_vao);
  //Set it as the current array to be used by binding it
  glBindVertexArray (cursor_vao);
  //Ask GL for a Vertex Buffer Object (cursor_vbo)
  glGenBuffers (1, &cursor_vbo);
  //Set it as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, cursor_vbo);
  //Copy the points into the current buffer - 9 float values, start pointer and static data
  glBufferData (GL_ARRAY_BUFFER, 36 * 3 * sizeof (float) + 12 * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cube_coords);
  glBufferSubData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), 12 * 3 * sizeof(float), cube_colors);

  //Enable the vertex attribute
  glEnableVertexAttribArray (cursor_position_id);
  //This the layout of our first vertex buffer
  //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats 
  glVertexAttribPointer (cursor_position_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(cursor_color_id);
  glVertexAttribPointer(cursor_color_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(36 * 3 * sizeof(float)));
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
  
  view_matrix = glm::lookAt(glm::vec3(0.0,0.0,VIEW_PADDING*DRAW_MIN),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
  
  // ortho_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
  projection_matrix = glm::frustum(VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX, VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX, VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX);

  modelviewproject_matrix = projection_matrix * view_matrix * rotation_matrix;

  // Drawing the grid first 
  glUseProgram(grid_shader_program);
  glBindVertexArray(grid_vao);
  glUniform1fv(grid_offset_id, N_CELLS + 2, grid_offsets);
  glUniformMatrix4fv(grid_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
  glDrawArraysInstanced(GL_LINES, 0, 2*(N_CELLS+1), 3*(N_CELLS+1));
  
  // Draw the cursor cube
  glUseProgram(cursor_shader_program);
  glBindVertexArray(cursor_vao);
  glUniformMatrix4fv(cursor_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Finally draw the model
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
  cubeAt(0, 0, 0);
  gridInitShadersGL();
  gridInitVertexBufferGL();
  cursorInitShadersGL();
  cursorInitVertexBufferGL();

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

