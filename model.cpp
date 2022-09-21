#include "model.hpp"

// Sorted by lexicographic order of <Point> by default !
std::map<Point, Point> model;

#define MODEL_TRIANGLES(i, j, k) (model_triangle_list[3*3*i + 3*j + k])
float *model_triangle_list;
#define MODEL_COLORS(i, j, k) (model_triangle_colors[3*3*i + 3*j + k])
float *model_triangle_colors;

unsigned long num_triangles = 0;

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

    a = c + Point(moveamount, 0, moveamount);
    b = c + Point(moveamount, 0, 0);
    d = c + Point(0, 0, moveamount);

    e = c + Point(moveamount, moveamount, moveamount);
    f = c + Point(moveamount, moveamount, 0);
    g = c + Point(0, moveamount, 0);
    h = c + Point(0, moveamount, moveamount);
   
	// Need to define these triangles with the passed cubeColor. 
    Triangle left_1(c, h, g, cubeColor), left_2(c, d, h, cubeColor);
    Triangle right_1(b, a, e, cubeColor), right_2(b, e, f, cubeColor);
    Triangle bottom_1(c, a, d, cubeColor), bottom_2(c, b, a, cubeColor);
    Triangle top_1(g, e, h, cubeColor), top_2(g, f, e, cubeColor);
    Triangle front_1(d, e, a, cubeColor), front_2(d, h, e, cubeColor);
    Triangle back_1(c, f, b, cubeColor), back_2(c, g, f, cubeColor);
    
	/*
	 * While checking, if we found a triangle already at some position, then delete that triangle along with the found color of the traingle from the model.
	 * i.e: if(model.find(c+Point(0, -moveamount, 0) != model.end()))
	 * => then deleteList.push_back(bottom_1.withColor(model(c+Point(0,-moveamount,0)))), and do the same for the other triangle.
	 */

    // Bottom face
    if(model.find(c + Point(0, -moveamount, 0)) != model.end()) {
        deleteList.push_back(bottom_1.withColor(model[c + Point(0, -moveamount, 0)]));
        deleteList.push_back(bottom_2.withColor(model[c + Point(0, -moveamount, 0)]));
    }
    else {
        drawList.push_back(bottom_1);
        drawList.push_back(bottom_2);
    }
    // Top face
    if(model.find(c + Point(0, moveamount, 0)) != model.end()) {
        deleteList.push_back(top_1.withColor(model[c + Point(0, moveamount, 0)]));
        deleteList.push_back(top_2.withColor(model[c + Point(0, moveamount, 0)]));
    }
    else {
        drawList.push_back(top_1);
        drawList.push_back(top_2);
    }
    // Right face
    if(model.find(c + Point(moveamount, 0, 0)) != model.end()) {
        deleteList.push_back(right_1.withColor(model[c + Point(moveamount, 0, 0)]));
        deleteList.push_back(right_2.withColor(model[c + Point(moveamount, 0, 0)]));
    }
    else {
        drawList.push_back(right_1);
        drawList.push_back(right_2);
    }
    // Left face
    if(model.find(c + Point(-moveamount, 0, 0)) != model.end()) {
        deleteList.push_back(left_1.withColor(model[c + Point(-moveamount, 0, 0)]));
        deleteList.push_back(left_2.withColor(model[c + Point(-moveamount, 0, 0)]));
    }
    else {
        drawList.push_back(left_1);
        drawList.push_back(left_2);
    }
    // Front face
    if(model.find(c + Point(0, 0, moveamount)) != model.end()) {
        deleteList.push_back(front_1.withColor(model[c + Point(0, 0, moveamount)]));
        deleteList.push_back(front_2.withColor(model[c + Point(0, 0, moveamount)]));
    }
    else {
        drawList.push_back(front_1);
        drawList.push_back(front_2);
    }
    // Back face
    if(model.find(c + Point(0, 0, -moveamount)) != model.end()) {
        deleteList.push_back(back_1.withColor(model[c + Point(0, 0, -moveamount)]));
        deleteList.push_back(back_2.withColor(model[c + Point(0, 0, -moveamount)]));
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
	
	unsigned long i = 0, j = 0;
	// converting the `to_remove` vector to a set
    std::set<Triangle> to_remove_set(to_remove.begin(), to_remove.end());
    if(DEBUG_MODEL) {
        std::cout << "|to_add| = " << to_add.size() << ", |to_remove| = " << to_remove.size() << ", |to_remove_set| = " << to_remove_set.size() << "\n";
    }
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
            if(DEBUG_MODEL) {
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
}

void deleteAt(float x, float y, float z) {
    /* Make sure that model[x, y, z] exists */
    auto triangles = trianglesAt(Point(x, y, z), Point(0, 1, 0));
    auto to_add = triangles.first, to_remove = triangles.second;
    updateTrianglesList(to_remove, to_add);
    model.erase(Point(x, y, z));
}

void saveModelToFile(std::string fname) {
    std::ofstream out;
    out.open(fname);
    out << max_num_cubes << "\n";
    for(auto it : model) {
        out << it.first.x - cursor_min << " " << it.first.y - cursor_min << " " << it.first.z - cursor_min << " ";
        out << it.second.x << " " << it.second.y << " " << it.second.z << "\n";
    }
    out.close();
}

void readModelFromFile(std::string fname) {
    std::ifstream in;
    std::string line;
    std::stringstream ss;
    in.open(fname);
    unsigned int n;
    float x, y, z, r, g, b;
    if(in.is_open()) {
        try {
            getline(in, line);
            n = std::stoi(line);
            if(n > max_num_cubes) {
                std::cout << "Loading model larger than current grid size (" << max_num_cubes << "), will ignore everything outside current bounds.\n";
            }
            while(getline(in, line)) {
                ss << line;
                ss >> x >> y >> z >> r >> g >> b;
                x += cursor_min;
                y += cursor_min;
                z += cursor_min;
                if(
                        (x > cursor_min && x < cursor_max - moveamount) &&
                        (y > cursor_min && x < cursor_max - moveamount) &&
                        (z > cursor_min && x < cursor_max - moveamount)
                ) {
                    cursor_x = x;
                    cursor_y = y;
                    cursor_z = z;
                    cursor_r = r;
                    cursor_g = g;
                    cursor_b = b;
                    cursor_color = Point(cursor_r, cursor_g, cursor_b);
                    std::cout << "Inserting at (" << cursor_x << ", " << cursor_y << ", " << cursor_z << ")\n";
                    insertAtCursor();
                }
                ss.clear();
            }
        }
        catch(std::exception const &e) {
            std::cout << "error : " << e.what() << "\n";
        }
    }
    else {
        std::cout << "Unable to open file\n";
    }
    in.close();
}

