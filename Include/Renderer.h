#ifndef _H_RENDERER
#define _H_RENDERER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Globals.h"
#include "Shader.h"
#include "Fluid.h"
#include "Menu.h"

class Renderer{
public:
    Renderer(GLuint& VAO, GLuint buf[]);
    ~Renderer() = default;

    void enableAxis();
    void enableTank();
    void enableModel(Model& m);

    void enableFluid(std::vector<float>& data);
    Fluid* fluid;


    void renderGUI(Menu& g);
    void renderAxis();
    void renderFluid();
    void renderTank();
    void renderModel(Model& m);
    void renderModelRegular(Model& m);

    unsigned int prepBuf(GLfloat data[], GLuint size);
    unsigned int prepBuf(GLushort data[], GLuint size);
    unsigned int prepBuf(std::vector<GLfloat>& data, bool big);
    unsigned int editBuf(std::vector<GLfloat>& data, GLuint i);

    void formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs, Shader& s);

private:
    // Memory mgmt
    static int free_buf;
    static int free_bindpoint;
    GLuint VAO;
    GLuint* buf;
    GLuint fluid_buffer;

    // Shaders
    // REMINDER: change environment variable SRC in globals to setup shaders correctly
    Shader shader_axis = Shader(SRC+"Shaders/vert.glsl", SRC+"Shaders/frag.glsl");
    Shader shader_fluid = Shader(SRC+"Shaders/vert2.glsl", SRC+"Shaders/frag2.glsl");
    Shader shader_tank = Shader(SRC+"Shaders/vert3.glsl", SRC+"Shaders/frag3.glsl");
    Shader shader_tank_outlines = Shader(SRC+"Shaders/vert4.glsl", SRC+"Shaders/frag3.glsl");

    // Colors
    glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

    // Other
    glm::mat4 tank_transform;
    Model* platform;


};
#endif