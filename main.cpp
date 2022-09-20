#include "main.hpp"
#include <vector>

float points[(N_CELLS+1)*8]; // 2*(N+1) number of lines, 2 points / line, 2 floats / point
float grid_offsets[N_CELLS+2];
float cube_coords[36*3];
float cube_colors[36*3] = {
    // green
    0/255.0, 255/255.0, 0/255.0,
    0/255.0, 255/255.0, 0/255.0,
    0/255.0, 255/255.0, 0/255.0,

    0/255.0, 255/255.0, 0/255.0,
    0/255.0, 255/255.0, 0/255.0,
    0/255.0, 255/255.0, 0/255.0,

    // blue
    0/255.0, 0/255.0, 255/255.0,
    0/255.0, 0/255.0, 255/255.0,
    0/255.0, 0/255.0, 255/255.0,

    0/255.0, 0/255.0, 255/255.0,
    0/255.0, 0/255.0, 255/255.0,
    0/255.0, 0/255.0, 255/255.0,

    // yellow
    255/255.0, 255/255.0, 0/255.0,
    255/255.0, 255/255.0, 0/255.0,
    255/255.0, 255/255.0, 0/255.0,

    255/255.0, 255/255.0, 0/255.0,
    255/255.0, 255/255.0, 0/255.0,
    255/255.0, 255/255.0, 0/255.0,

    // white
    255/255.0, 255/255.0, 255/255.0,
    255/255.0, 255/255.0, 255/255.0,
    255/255.0, 255/255.0, 255/255.0,

    255/255.0, 255/255.0, 255/255.0,
    255/255.0, 255/255.0, 255/255.0,
    255/255.0, 255/255.0, 255/255.0,

    // red
    255/255.0, 0/255.0, 0/255.0,
    255/255.0, 0/255.0, 0/255.0,
    255/255.0, 0/255.0, 0/255.0,

    255/255.0, 0/255.0, 0/255.0,
    255/255.0, 0/255.0, 0/255.0,
    255/255.0, 0/255.0, 0/255.0,

    // magenta
    255/255.0, 0/255.0, 255/255.0,
    255/255.0, 0/255.0, 255/255.0,
    255/255.0, 0/255.0, 255/255.0,

    255/255.0, 0/255.0, 255/255.0,
    255/255.0, 0/255.0, 255/255.0,
    255/255.0, 0/255.0, 255/255.0,
};
bool compare_vec3(glm::vec3 v1, glm::vec3 v2) {
    if(v1.x == v2.x) {
        if(v1.y == v2.y) {
            return v1.z < v2.z;
        }
        else {
            return v1.y < v2.y;
        }
    }
    else {
        return v1.x < v1.x;
    }
}
bool equal_vec3(glm::vec3 v1, glm::vec3 v2) {
    return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
}
bool compare_tri(std::vector<glm::vec3> t1, std::vector<glm::vec3> t2) {
    if(t1.size() != t2.size()) {
        return t1.size() < t2.size();
    }
    if(equal_vec3(t1[0], t2[0])) {
        if(equal_vec3(t1[1], t2[1])) {
            return compare_vec3(t1[2], t2[2]);
        }
        else {
            return compare_vec3(t1[1], t2[1]);
        }
    }
    else {
        return compare_vec3(t1[0], t2[0]);
    }
}
bool (*compare_vec3_ptr)(glm::vec3, glm::vec3) = compare_vec3;
bool (*compare_tri_ptr)(std::vector<glm::vec3>, std::vector<glm::vec3>) = compare_tri;
//std::map<glm::vec3, glm::vec3, bool(*)(glm::vec3, glm::vec3)> model(compare_vec3_ptr);
std::map<Point, Point> model; // TODO: Make this unordered
// TODO: Allocate as arrays with maximum number possible as (N_CELLS)^3 times 2
// Also maintain an unsigned integer `size` variable. Initially `size = 0` but size updates as we add or remove triangles
//std::vector<std::vector<glm::vec3>> model_triangle_list; // TODO: allocate an array on heap instead
#define MODEL_TRIANGLES(i, j, k) (model_triangle_list[3*3*i + 3*j + k])
float *model_triangle_list;
//std::vector<glm::vec3> model_triangle_colors; // TODO: allocate an array on heap instead
#define MODEL_COLORS(i, j, k) (model_triangle_colors[3*3*i + 3*j + k])
float *model_triangle_colors;
bool update_vbo = false;
unsigned long num_triangles = 0, max_num_triangles = 6 * N_CELLS * N_CELLS * 2;

float cube_triangle_list[12][3][3]; // 12 tri, 3 pt/tri, 3 coords/pt
std::vector<glm::vec3> cube_triangle_colors;

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
    /*
    for(int i = 0; i <= N_CELLS; i++) {
        points[idx++] = coords[0][i][0]; // x coord 
        points[idx++] = coords[0][i][1]; // y coord 
        points[idx++] = coords[N_CELLS][i][0]; // x coord 
        points[idx++] = coords[N_CELLS][i][1]; // y coord 
    }
    */
    // prepare `grid_offsets` which will be passed as a uniform 
    grid_offsets[0] = N_CELLS;
    for(int i = 1; i <= N_CELLS+1; i++) {
        grid_offsets[i] = DRAW_MIN + (i-1) * (DRAW_MAX - DRAW_MIN) / N_CELLS;
    }
}

void printTriangleList() {
    std::cout << "Triangle List: (Length = " << num_triangles << "): {\n";
    for(unsigned long i = 0; i < num_triangles; i++) {
        std::cout << "\t[ ";
        for(int j = 0; j < 3; j++) {
            std::cout << "(" << MODEL_TRIANGLES(i,j,0) << ", " << MODEL_TRIANGLES(i,j,1) << ", " << MODEL_TRIANGLES(i,j,2) << ") ";
        }
        std::cout << "]\n";
    }
    std::cout << "}\n";
}

void printModel() {
    std::cout << "Model: (Length = " << model.size() << "): {\n";
    for(auto pair : model) {
        std::cout << "\t(" << pair.first.x << ", " << pair.first.y << ", " << pair.first.z << ") -> (" << pair.second.x << ", " << pair.second.y << ", " << pair.second.z << ")\n";
    }
    std::cout << "}\n";
}

void cubeAt(float x, float y, float z) {
    // 6 faces, 12 triangles, 36 vertices, 36 * 3 coords
    // cube's left bottom back is at (x, y, z)
    float temp[36*3] = {
        x, y, z,
        x+N_UNITS, y+N_UNITS, z,
        x+N_UNITS, y, z,

        x, y, z,
        x, y+N_UNITS, z,
        x+N_UNITS, y+N_UNITS, z,

        x, y, z,
        x, y, z+N_UNITS,
        x, y+N_UNITS, z+N_UNITS,

        x, y, z,
        x, y+N_UNITS, z+N_UNITS,
        x, y+N_UNITS, z,

        x, y, z,
        x+N_UNITS, y, z,
        x+N_UNITS, y, z+N_UNITS,

        x, y, z,
        x+N_UNITS, y, z+N_UNITS,
        x, y, z+N_UNITS,

        x, y, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
        x+N_UNITS, y, z+N_UNITS,

        x, y, z+N_UNITS,
        x, y+N_UNITS, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,

        x+N_UNITS, y, z,
        x+N_UNITS, y, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,

        x+N_UNITS, y, z,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
        x+N_UNITS, y+N_UNITS, z,

        x, y+N_UNITS, z,
        x+N_UNITS, y+N_UNITS, z,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,

        x, y+N_UNITS, z,
        x+N_UNITS, y+N_UNITS, z+N_UNITS,
        x, y+N_UNITS, z+N_UNITS,
    };
    // I might be hating C++
    for(int i = 0; i < 36*3; i++) {
        cube_coords[i] = temp[i];
    }
}

std::pair<std::vector<Triangle>, std::vector<Triangle>> trianglesAt(Point c, Point cubeColor) {
    /* Ref for cube ascii : https://codegolf.stackexchange.com/q/189
     *         (+Y)
     *         |
     *         g--------f
     *        /|       /|
     *       / |      / |
     *      h--------e  |
     *      |  |     |  |
     *      |  c-----|--b----(+X)
     *      | /      | /
     *      |/       |/
     *      d--------a
     *     /
     *    /
     * (+Z)
     * Takes the position of c in WCS as an input
     * Checks if any cubes are adjacent to this cube :
     *  If any face has a cube adjacent to it, triangles corr to it will be added to (deleteList)
     *  Else, triangles corr to that face will be added to (drawList)
     * Returns the pair (drawList, deleteList)
     */
    Point a, b, d, e, f, g, h;
    std::vector<Triangle> drawList, deleteList;

    a = c + Point(N_UNITS, 0, N_UNITS);
    b = c + Point(N_UNITS, 0, 0);
    d = c + Point(0, 0, N_UNITS);

    e = c + Point(N_UNITS, N_UNITS, N_UNITS);
    f = c + Point(N_UNITS, N_UNITS, 0);
    g = c + Point(0, N_UNITS, 0);
    h = c + Point(0, N_UNITS, N_UNITS);
   
	// Need to define these triangles with the passed cubeColor. 
    Triangle left_1(c, h, g, cubeColor), left_2(c, d, h, cubeColor);
    Triangle right_1(b, a, e, cubeColor), right_2(b, e, f, cubeColor);
    Triangle bottom_1(c, a, d, cubeColor), bottom_2(c, b, a, cubeColor);
    Triangle top_1(g, e, h, cubeColor), top_2(g, f, e, cubeColor);
    Triangle front_1(d, e, a, cubeColor), front_2(d, h, e, cubeColor);
    Triangle back_1(c, f, b, cubeColor), back_2(c, g, f, cubeColor);
    
	/*
	 While checking, if we found a triangle already at some position, then delete that triangle along with the found color of the traingle from the model.
	 i.e: if(model.find(c+Point(0, -N_UNITS, 0) != model.end()))
	 => then deleteList.push_back(bottom_1.changeColor(model(c+Point(0,-N_UNITS,0)))), and do the same for the other triangle.

	 */
    // Bottom face
    if(model.find(c + Point(0, -N_UNITS, 0)) != model.end()) {
        deleteList.push_back(bottom_1.changeColor(model[c + Point(0, -N_UNITS, 0)]));
        deleteList.push_back(bottom_2.changeColor(model[c + Point(0, -N_UNITS, 0)]));
    }
    else {
        drawList.push_back(bottom_1);
        drawList.push_back(bottom_2);
    }
    // Top face
    if(model.find(c + Point(0, N_UNITS, 0)) != model.end()) {
        deleteList.push_back(top_1.changeColor(model[c + Point(0, N_UNITS, 0)]));
        deleteList.push_back(top_2.changeColor(model[c + Point(0, N_UNITS, 0)]));
    }
    else {
        drawList.push_back(top_1);
        drawList.push_back(top_2);
    }
    // Right face
    if(model.find(c + Point(N_UNITS, 0, 0)) != model.end()) {
        deleteList.push_back(right_1.changeColor(model[c + Point(N_UNITS, 0, 0)]));
        deleteList.push_back(right_2.changeColor(model[c + Point(N_UNITS, 0, 0)]));
    }
    else {
        drawList.push_back(right_1);
        drawList.push_back(right_2);
    }
    // Left face
    if(model.find(c + Point(-N_UNITS, 0, 0)) != model.end()) {
        deleteList.push_back(left_1.changeColor(model[c + Point(-N_UNITS, 0, 0)]));
        deleteList.push_back(left_2.changeColor(model[c + Point(-N_UNITS, 0, 0)]));
    }
    else {
        drawList.push_back(left_1);
        drawList.push_back(left_2);
    }
    // Front face
    if(model.find(c + Point(0, 0, N_UNITS)) != model.end()) {
        deleteList.push_back(front_1.changeColor(model[c + Point(0, 0, N_UNITS)]));
        deleteList.push_back(front_2.changeColor(model[c + Point(0, 0, N_UNITS)]));
    }
    else {
        drawList.push_back(front_1);
        drawList.push_back(front_2);
    }
    // Back face
    if(model.find(c + Point(0, 0, -N_UNITS)) != model.end()) {
        deleteList.push_back(back_1.changeColor(model[c + Point(0, 0, -N_UNITS)]));
        deleteList.push_back(back_2.changeColor(model[c + Point(0, 0, -N_UNITS)]));
    }
    else {
        drawList.push_back(back_1);
        drawList.push_back(back_2);
    }

    return std::pair<std::vector<Triangle>, std::vector<Triangle>>(drawList, deleteList);
}

void updateTrianglesList(std::vector<Triangle> to_add, std::vector<Triangle> to_remove) {
    // convert `to_remove` to a set so that searching that becomes logarithmic, will need to define 
    // a total ordering over triangles 
    // TODO: Make sure opposite face triangles are in same order and define a triangle compare function
    // Then mantain 2 indices (i) and (j) such that initially i = j = 0
    // For every outermost element i.e. a triangle (3 vertices = 9 floats) in the `model_triangle_list`,
    // if i'th triangle belongs to the set `to_remove` (possible via our custom function), increment j+1 
    // do `model_triangle_list[i] = model_triangle_list[j]` (move all 9 floats correctly)
    // This works since it skips over those which we want to remove. TODO: verify on pen paper
	
	unsigned long i=0, j=0;
	// converting the `to_remove` vector to a set
    std::set<Triangle> to_remove_set(to_remove.begin(), to_remove.end()); // TODO: make unordered set
    std::cout << "|to_add| = " << to_add.size() << ", |to_remove| = " << to_remove.size() << ", |to_remove_set| = " << to_remove_set.size() << "\n";
    for(j = 0; j < num_triangles; j++) {
        // is j'th triangle to be removed ?
        Triangle tri_j(
            Point(
                MODEL_TRIANGLES(j,0,0),
                MODEL_TRIANGLES(j,0,1),
                MODEL_TRIANGLES(j,0,2)
            ),
            Point(
                MODEL_TRIANGLES(j,1,0),
                MODEL_TRIANGLES(j,1,1),
                MODEL_TRIANGLES(j,1,2)
            ),
            Point(
                MODEL_TRIANGLES(j,2,0),
                MODEL_TRIANGLES(j,2,1),
                MODEL_TRIANGLES(j,2,2)
            ),
			Point(
				MODEL_COLORS(j, 0, 0),
				MODEL_COLORS(j, 0, 1),
				MODEL_COLORS(j, 0, 2)
			)
        );
        if(to_remove_set.find(tri_j) == to_remove_set.end()) {
            // tri_j not to be removed
            // copy tri_j to tri_i
            MODEL_TRIANGLES(i,0,0) = MODEL_TRIANGLES(j,0,0);
            MODEL_TRIANGLES(i,0,1) = MODEL_TRIANGLES(j,0,1);
            MODEL_TRIANGLES(i,0,2) = MODEL_TRIANGLES(j,0,2);
            MODEL_TRIANGLES(i,1,0) = MODEL_TRIANGLES(j,1,0);
            MODEL_TRIANGLES(i,1,1) = MODEL_TRIANGLES(j,1,1);
            MODEL_TRIANGLES(i,1,2) = MODEL_TRIANGLES(j,1,2);
            MODEL_TRIANGLES(i,2,0) = MODEL_TRIANGLES(j,2,0);
            MODEL_TRIANGLES(i,2,1) = MODEL_TRIANGLES(j,2,1);
            MODEL_TRIANGLES(i,2,2) = MODEL_TRIANGLES(j,2,2);

            MODEL_COLORS(i,0,0) = MODEL_COLORS(j,0,0);
            MODEL_COLORS(i,0,1) = MODEL_COLORS(j,0,1);
            MODEL_COLORS(i,0,2) = MODEL_COLORS(j,0,2);
            MODEL_COLORS(i,1,0) = MODEL_COLORS(j,1,0);
            MODEL_COLORS(i,1,1) = MODEL_COLORS(j,1,1);
            MODEL_COLORS(i,1,2) = MODEL_COLORS(j,1,2);
            MODEL_COLORS(i,2,0) = MODEL_COLORS(j,2,0);
            MODEL_COLORS(i,2,1) = MODEL_COLORS(j,2,1);
            MODEL_COLORS(i,2,2) = MODEL_COLORS(j,2,2);

            i++;
        }
        else {
            std::cout << "Skipping j = " << j << ": [ ";
            for(int k = 0; k < 3; k++) {
                std::cout << "(" << MODEL_TRIANGLES(j,k,0);
                std::cout << "," << MODEL_TRIANGLES(j,k,1);
                std::cout << "," << MODEL_TRIANGLES(j,k,2);
                std::cout << ") ";
            }
            std::cout << "]\n";
        }
    }
    for(auto tri_add : to_add) {
        MODEL_TRIANGLES(i,0,0) = tri_add.p1.x;
        MODEL_TRIANGLES(i,0,1) = tri_add.p1.y;
        MODEL_TRIANGLES(i,0,2) = tri_add.p1.z;
        MODEL_TRIANGLES(i,1,0) = tri_add.p2.x;
        MODEL_TRIANGLES(i,1,1) = tri_add.p2.y;
        MODEL_TRIANGLES(i,1,2) = tri_add.p2.z;
        MODEL_TRIANGLES(i,2,0) = tri_add.p3.x;
        MODEL_TRIANGLES(i,2,1) = tri_add.p3.y;
        MODEL_TRIANGLES(i,2,2) = tri_add.p3.z;

		MODEL_COLORS(i,0,0) = tri_add.c.x;
        MODEL_COLORS(i,0,1) = tri_add.c.y;
        MODEL_COLORS(i,0,2) = tri_add.c.z;
        MODEL_COLORS(i,1,0) = tri_add.c.x;
        MODEL_COLORS(i,1,1) = tri_add.c.y;
        MODEL_COLORS(i,1,2) = tri_add.c.z;
        MODEL_COLORS(i,2,0) = tri_add.c.x;
        MODEL_COLORS(i,2,1) = tri_add.c.y;
        MODEL_COLORS(i,2,2) = tri_add.c.z;       
        i++;
    }
    num_triangles = i;
}

void insertAt(float x, float y, float z, Point cubeColor) {
    auto triangles = trianglesAt(Point(x, y, z), cubeColor);
    auto to_add = triangles.first, to_remove = triangles.second;
    updateTrianglesList(to_add, to_remove);
    model[Point(x, y, z)] = cubeColor;
};

void deleteAt(float x, float y, float z) {
    /* Make sure that model[x, y, z] exists */
    auto triangles = trianglesAt(Point(x, y, z), Point(0, 1, 0));
    auto to_add = triangles.first, to_remove = triangles.second;
    updateTrianglesList(to_remove, to_add);
    model.erase(Point(x, y, z));
}

void insertAtCursor() {
    if(model.find(Point(cursor_x, cursor_y, cursor_z)) == model.end()) {        
        insertAt(cursor_x, cursor_y, cursor_z, current_color);
    }
    else {
        deleteAt(cursor_x, cursor_y, cursor_z);
        insertAt(cursor_x, cursor_y, cursor_z, current_color);
    }
    update_vbo = true;
}

void deleteAtCursor() {
    if(model.find(Point(cursor_x, cursor_y, cursor_z)) != model.end()) {
        deleteAt(cursor_x, cursor_y, cursor_z);
        update_vbo = true;
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
  cursor_offset_id = glGetUniformLocation(cursor_shader_program, "cursor_offset");
  cursor_uModelViewProjectMatrix_id = glGetUniformLocation(cursor_shader_program, "uModelViewProjectMatrix");
}

void modelInitShadersGL(void)
{
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
  glBufferData (GL_ARRAY_BUFFER, 36 * 3 * sizeof (float) + 36 * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
  //glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cube_coords);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 36 * 3 * sizeof(float), cube_triangle_list);
  glBufferSubData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), 36 * 3 * sizeof(float), cube_colors);

  //Enable the vertex attribute
  glEnableVertexAttribArray (cursor_position_id);
  //This the layout of our first vertex buffer
  //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats 
  glVertexAttribPointer (cursor_position_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(cursor_color_id);
  glVertexAttribPointer(cursor_color_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(36 * 3 * sizeof(float)));
}

void modelInitVertexBufferGL(void)
{
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

void renderGL(void)
{
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

  if(persp){
	modelviewproject_matrix = projection_matrix * view_matrix * rotation_matrix;
  }
  else 
	modelviewproject_matrix = ortho_matrix * view_matrix * rotation_matrix;

  // Drawing the grid first
  
  glUseProgram(grid_shader_program);
  glBindVertexArray(grid_vao);
  glUniform1fv(grid_offset_id, N_CELLS + 2, grid_offsets);
  glUniformMatrix4fv(grid_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
  glDrawArraysInstanced(GL_LINES, 0, 2*(N_CELLS+1), 3*(N_CELLS+1));
  
  // Draw the cursor cube
  glUseProgram(cursor_shader_program);
  glBindVertexArray(cursor_vao);
  glUniform3f(cursor_offset_id, cursor_x, cursor_y, cursor_z);
  glUniformMatrix4fv(cursor_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Finally draw the model
  glUseProgram(model_shader_program);
  glBindVertexArray(model_vao);
  if(update_vbo) {
      glBindBuffer (GL_ARRAY_BUFFER, model_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, max_num_triangles * 3 * 3 * sizeof(float), model_triangle_list);
      glBufferSubData(GL_ARRAY_BUFFER, max_num_triangles * 3 * 3 * sizeof(float), max_num_triangles * 3 * 3 * sizeof(float), model_triangle_colors);
      update_vbo = false;
      std::cout << "num_triangles = " << num_triangles << "\n";
  }
  glUniformMatrix4fv(model_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
  glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);
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

  model.clear();

  model_triangle_list = (float *)calloc(max_num_triangles * 3 * 3, sizeof(float));
  model_triangle_colors = (float *)calloc(max_num_triangles * 3 * 3, sizeof(float));

  //model_triangle_list.clear();
  //model_triangle_colors.clear();

  //cube_triangle_list.clear();
  //cube_triangle_colors.clear();
  
  // since model is empty it should return all triangles
  auto tmp = trianglesAt(Point(0, 0, 0), current_color).first;
  for(int i = 0; i < tmp.size(); i++) {
        cube_triangle_list[i][0][0] = tmp[i].p1.x;
        cube_triangle_list[i][0][1] = tmp[i].p1.y;
        cube_triangle_list[i][0][2] = tmp[i].p1.z;
        cube_triangle_list[i][1][0] = tmp[i].p2.x;
        cube_triangle_list[i][1][1] = tmp[i].p2.y;
        cube_triangle_list[i][1][2] = tmp[i].p2.z;
        cube_triangle_list[i][2][0] = tmp[i].p3.x;
        cube_triangle_list[i][2][1] = tmp[i].p3.y;
        cube_triangle_list[i][2][2] = tmp[i].p3.z;
  }
  /*
  for(int i = 0; i < 12; i++) {
      std::cout << "Triangle #" << i+1 << ": [ ";
      for(int j = 0; j < 3; j++) {
          std::cout << "(";
          std::cout << *((float *)cube_triangle_list + i * 9 + j * 3);
          std::cout << ", ";
          std::cout << *((float *)cube_triangle_list + i * 9 + j * 3 + 1);
          std::cout << ", ";
          std::cout << *((float *)cube_triangle_list + i * 9 + j * 3 + 2);
          std::cout << ") ";
      }
      std::cout << "]\n";
  }
  std::cout << "------------------------\n";
  cubeAt(0, 0, 0);
  // 12 triangles
  for(int i = 0; i < 12; i++) {
      // each triangle has 3 points
      for(int j = 0; j < 3; j++) {
        std::cout << "(" << cube_coords[i*9+j*3] << ", " << cube_coords[i*9+j*3+1] << ", " << cube_coords[i*9+j*3+2] << ")\n";
      }
      std::cout << "\n";
  }
  */
  /**/
  printModel();
  printTriangleList();
  insertAt(0, 0, 0, current_color);
  
  printModel();
  printTriangleList();
  insertAt(0, 0, N_UNITS, current_color);
  
  printModel();
  printTriangleList();
  insertAt(N_UNITS, 0, 0, current_color);
  
  printModel();
  printTriangleList();
  /**/
  /**/
  gridInitShadersGL();
  gridInitVertexBufferGL();
  cursorInitShadersGL();
  cursorInitVertexBufferGL();
  modelInitShadersGL();
  modelInitVertexBufferGL();
  /**/
  for(int i = 0; i < num_triangles * 3 * 3; i++) {
      std::cout << *((float *)model_triangle_list + i) << ", " << *((float *)model_triangle_colors + i) << "\n";
  }
  std::cout << "-------\nnum_triangles = " << num_triangles << "\n";
  /*
  for(int i = 0; i < 12 * 3 * 3; i++) {
      std::cout << *((float *)cube_triangle_list + i) << ", " << *((float *)cube_colors + i) << "\n";
  }
  */
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
  free(model_triangle_list);
  free(model_triangle_colors);
  return 0;
}

