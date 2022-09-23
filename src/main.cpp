#include "main.hpp"
#include <GLFW/glfw3.h>

float points[(N_CELLS+1)*8]; // 2*(N+1) number of lines, 2 points / line, 2 floats / point
float grid_offsets[N_CELLS+2];

float default_cursor_colors[36*3], padded_cursor_colors[36*3], *cursor_colors = default_cursor_colors;
bool update_model_vbo = false, update_cursor_vbo = false;

float default_cursor_triangle_list[12][3][3], padded_cursor_triangle_list[12][3][3], *cursor_triangle_list = (float *)default_cursor_triangle_list; // 12 tri, 3 pt/tri, 3 coords/pt

GLuint grid_shader_program, cursor_shader_program, model_shader_program;
GLuint grid_vbo, grid_vao, cursor_vbo, cursor_vao, model_vbo, model_vao;

GLuint grid_offset_id, cursor_position_id, cursor_offset_id, cursor_color_id, model_position_id, model_color_id;

glm::mat4 rotation_matrix;
glm::mat4 view_matrix;
glm::mat4 ortho_matrix;
glm::mat4 projection_matrix;
glm::mat4 modelviewproject_matrix;
GLuint grid_uModelViewProjectMatrix_id, cursor_uModelViewProjectMatrix_id, model_uModelViewProjectMatrix_id;

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
    // prepare `grid_offsets` which will be passed as a uniform
    grid_offsets[0] = N_CELLS;
    for(int i = 1; i <= N_CELLS+1; i++) {
        grid_offsets[i] = DRAW_MIN + (i-1) * (DRAW_MAX - DRAW_MIN) / N_CELLS;
    }
}
void insertAtCursor() {
    if(model.find(Point(cursor_x, cursor_y, cursor_z)) == model.end()) {
        insertAt(cursor_x, cursor_y, cursor_z, cursor_color);
    } else {
        deleteAt(cursor_x, cursor_y, cursor_z);
        insertAt(cursor_x, cursor_y, cursor_z, cursor_color);
    }
    update_model_vbo = true;
    updateCursor();
}

void deleteAtCursor() {
    if(model.find(Point(cursor_x, cursor_y, cursor_z)) != model.end()) {
        deleteAt(cursor_x, cursor_y, cursor_z);
        update_model_vbo = true;
        updateCursor();
    }
}

void updateCursor() {
    if(model.find(Point(cursor_x, cursor_y, cursor_z)) != model.end()) {
        Point color = model[Point(cursor_x, cursor_y, cursor_z)];
        for(int i = 0; i < 36; i++) {
            padded_cursor_colors[3*i] = color.x;
            padded_cursor_colors[3*i+1] = color.y;
            padded_cursor_colors[3*i+2] = color.z;
        }
        cursor_triangle_list = (float *)padded_cursor_triangle_list;
        cursor_colors = padded_cursor_colors;
        update_cursor_vbo = true;
    } else {
        cursor_triangle_list = (float *)default_cursor_triangle_list;
        cursor_colors = default_cursor_colors;
        update_cursor_vbo = true;
    }
}

void resetModel() {
    num_triangles = 0;
    model.clear();
    update_model_vbo = true;
    updateCursor();
}

void gridInitShadersGL(void) {
    std::string vertex_shader_file("grid_vs.glsl");
    std::string fragment_shader_file("grid_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    grid_shader_program = csX75::CreateProgramGL(shaderList);
    grid_offset_id = glGetUniformLocation(grid_shader_program, "grid_offsets");
    grid_uModelViewProjectMatrix_id = glGetUniformLocation(grid_shader_program, "uModelViewProjectMatrix");
}

void cursorInitShadersGL(void) {
    std::string vertex_shader_file("cursor_vs.glsl");
    std::string fragment_shader_file("cursor_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    cursor_shader_program = csX75::CreateProgramGL(shaderList);
    cursor_position_id = glGetAttribLocation(cursor_shader_program, "vPosition");
    cursor_color_id = glGetAttribLocation(cursor_shader_program, "vColor");
    cursor_offset_id = glGetUniformLocation(cursor_shader_program, "cursor_offset");
    cursor_uModelViewProjectMatrix_id = glGetUniformLocation(cursor_shader_program, "uModelViewProjectMatrix");
}

void modelInitShadersGL(void) {
    std::string vertex_shader_file("cube_vs.glsl");
    std::string fragment_shader_file("cube_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    model_shader_program = csX75::CreateProgramGL(shaderList);
    model_position_id = glGetAttribLocation(model_shader_program, "vPosition");
    model_color_id = glGetAttribLocation(model_shader_program, "vColor");
    model_uModelViewProjectMatrix_id = glGetUniformLocation(model_shader_program, "uModelViewProjectMatrix");
}

void gridInitVertexBufferGL(void) {
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

void cursorInitVertexBufferGL(void) {
    //Ask GL for a Vertex Attribute Object (cursor_vao)
    glGenVertexArrays (1, &cursor_vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (cursor_vao);
    //Ask GL for a Vertex Buffer Object (cursor_vbo)
    glGenBuffers (1, &cursor_vbo);
    //Set it as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, cursor_vbo);
    //Copy the points into the current buffer - 9 float values, start pointer and static data
    glBufferData (GL_ARRAY_BUFFER, 36 * 3 * sizeof (float) + 36 * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cube_coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cursor_triangle_list);
    glBufferSubData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), 36 * 3 * sizeof(float), cursor_colors);

    //Enable the vertex attribute
    glEnableVertexAttribArray (cursor_position_id);
    //This the layout of our first vertex buffer
    //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats
    glVertexAttribPointer (cursor_position_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(cursor_color_id);
    glVertexAttribPointer(cursor_color_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(36 * 3 * sizeof(float)));
}

void modelInitVertexBufferGL(void) {
    //Ask GL for a Vertex Attribute Object (model_vao)
    glGenVertexArrays (1, &model_vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (model_vao);
    //Ask GL for a Vertex Buffer Object (model_vbo)
    glGenBuffers (1, &model_vbo);
    //Set it as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, model_vbo);
    //Copy the points into the current buffer - 9 float values, start pointer and static data
    glBufferData (GL_ARRAY_BUFFER, max_num_triangles * 3 * 3 * sizeof (float) + max_num_triangles * 3 * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cube_coords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, max_num_triangles * 3 * 3 * sizeof(float), model_triangle_list);
    glBufferSubData(GL_ARRAY_BUFFER, max_num_triangles * 3 * 3 * sizeof(float), max_num_triangles * 3 * 3 * sizeof(float), model_triangle_colors);

    //Enable the vertex attribute
    glEnableVertexAttribArray (model_position_id);
    //This the layout of our first vertex buffer
    //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats
    glVertexAttribPointer (model_position_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(model_color_id);
    glVertexAttribPointer(model_color_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(max_num_triangles * 3 * 3 * sizeof(float)));
}

void renderGL(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
    rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));

    view_matrix = glm::lookAt(glm::vec3(0.0,0.0,VIEW_PADDING*DRAW_MIN),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));

    ortho_matrix = glm::ortho(
                       VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX,
                       VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX,
                       10 * VIEW_PADDING * DRAW_MIN, 10 * VIEW_PADDING * DRAW_MAX
                   );
    // projection_matrix = glm::frustum(VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX, VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX, 10 * VIEW_PADDING * DRAW_MIN, 10 * VIEW_PADDING * DRAW_MAX);
    projection_matrix = glm::frustum(-1,1,-1,1,1,10);

    if(persp) {
        modelviewproject_matrix = projection_matrix * view_matrix * rotation_matrix;
    } else
        modelviewproject_matrix = ortho_matrix * view_matrix * rotation_matrix;

    // Draw the cursor cube
    glUseProgram(cursor_shader_program);
    glBindVertexArray(cursor_vao);
    if(update_cursor_vbo) {
        glBindBuffer (GL_ARRAY_BUFFER, cursor_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cursor_triangle_list);
        glBufferSubData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), 36 * 3 * sizeof(float), cursor_colors);
        update_cursor_vbo = false;
    }
    glUniform3f(cursor_offset_id, cursor_x, cursor_y, cursor_z);
    glUniformMatrix4fv(cursor_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw the model
    glUseProgram(model_shader_program);
    glBindVertexArray(model_vao);
    if(update_model_vbo) {
        glBindBuffer (GL_ARRAY_BUFFER, model_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, num_triangles * 3 * 3 * sizeof(float), model_triangle_list);
        glBufferSubData(GL_ARRAY_BUFFER, max_num_triangles * 3 * 3 * sizeof(float), num_triangles * 3 * 3 * sizeof(float), model_triangle_colors);
        update_model_vbo = false;
    }
    glUniformMatrix4fv(model_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
    glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);

    if(draw_grid) {
        // Drawing the grid last since it has transparency
        glUseProgram(grid_shader_program);
        glBindVertexArray(grid_vao);
        glUniform1fv(grid_offset_id, N_CELLS + 2, grid_offsets);
        glUniformMatrix4fv(grid_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
        glDrawArraysInstanced(GL_LINES, 0, 2*(N_CELLS+1), 3*(N_CELLS+1));
    }
}

int main(int argc, char** argv) {
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
    if (!window) {
        glfwTerminate();
        return -1;
    }

    //! Make the window's context current
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    //Turn this on to get Shader based OpenGL
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    defineGrid();
    model.clear();

    model_triangle_list = (float *)calloc(max_num_triangles * 3 * 3, sizeof(float));
    model_triangle_colors = (float *)calloc(max_num_triangles * 3 * 3, sizeof(float));

    // since model is empty it should return all triangles
    auto tmp = trianglesAt(Point(0, 0, 0), cursor_color).first;
    for(int i = 0; i < tmp.size(); i++) {
        default_cursor_triangle_list[i][0][0] = tmp[i].p1.x;
        default_cursor_triangle_list[i][0][1] = tmp[i].p1.y;
        default_cursor_triangle_list[i][0][2] = tmp[i].p1.z;
        default_cursor_triangle_list[i][1][0] = tmp[i].p2.x;
        default_cursor_triangle_list[i][1][1] = tmp[i].p2.y;
        default_cursor_triangle_list[i][1][2] = tmp[i].p2.z;
        default_cursor_triangle_list[i][2][0] = tmp[i].p3.x;
        default_cursor_triangle_list[i][2][1] = tmp[i].p3.y;
        default_cursor_triangle_list[i][2][2] = tmp[i].p3.z;

        float offset = (1. - CURSOR_PADDING) * N_UNITS / 2.;

        padded_cursor_triangle_list[i][0][0] = CURSOR_PADDING * tmp[i].p1.x + offset;
        padded_cursor_triangle_list[i][0][1] = CURSOR_PADDING * tmp[i].p1.y + offset;
        padded_cursor_triangle_list[i][0][2] = CURSOR_PADDING * tmp[i].p1.z + offset;
        padded_cursor_triangle_list[i][1][0] = CURSOR_PADDING * tmp[i].p2.x + offset;
        padded_cursor_triangle_list[i][1][1] = CURSOR_PADDING * tmp[i].p2.y + offset;
        padded_cursor_triangle_list[i][1][2] = CURSOR_PADDING * tmp[i].p2.z + offset;
        padded_cursor_triangle_list[i][2][0] = CURSOR_PADDING * tmp[i].p3.x + offset;
        padded_cursor_triangle_list[i][2][1] = CURSOR_PADDING * tmp[i].p3.y + offset;
        padded_cursor_triangle_list[i][2][2] = CURSOR_PADDING * tmp[i].p3.z + offset;
	}
   
	for(int i = 0; i < 36; i++) {
        default_cursor_colors[3*i] = 0;
        default_cursor_colors[3*i+1] = 1;
        default_cursor_colors[3*i+2] = 0;
    }

    gridInitShadersGL();
    gridInitVertexBufferGL();
    cursorInitShadersGL();
    cursorInitVertexBufferGL();
    modelInitShadersGL();
    modelInitVertexBufferGL();
    
    std::cout<<"------Modelling Mode------\n";

    // Loop until the user closes the window
    while (glfwWindowShouldClose(window) == 0) {

        // Render here
        renderGL();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    free(model_triangle_list);
    free(model_triangle_colors);
    return 0;
}

