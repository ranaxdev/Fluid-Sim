#include <string>
#include <iostream>
#include "Logger.h"
#include "Harness.h"
#include "Renderer.h"

// Static initializations
int Renderer::free_buf          = -1;
int Renderer::free_bindpoint    = -1;

Renderer::Renderer(GLuint &VAO, GLuint *buf)
: VAO(VAO), buf(buf) {
    fluid = new Fluid();
    enableFluid(fluid->render_data);

    enableModel(*fluid->ball);

    platform = new Model(std::string(SRC+"Res/ground.obj").c_str(), "vert5", "frag3", std::make_pair(11,12), glm::vec3(0.5f, 0.5f, 0.5f));
    platform->transformation = glm::translate(platform->transformation, glm::vec3(0.0f, -2.11f ,0.0f));
    platform->transformation = glm::scale(platform->transformation, glm::vec3(0.35f));

    enableModel(*platform);

}

/* ****************************************************************
 *                  RENDER-ABLES INITIALIZATIONS                  *
 ******************************************************************/
/*
 * Initialize axes data
 */
void Renderer::enableAxis() {
    const GLfloat axis_data[] = {
            // Axis 1 (red)
            0.0f, 0.0f, 2.5f,     1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -2.5f,    1.0f, 0.0f, 0.0f,
            // Axis 2 (green)
            0.0f, 2.5f, 0.0f,    0.0f, 1.0f, 0.0f,
            0.0f, -2.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            // Axis 3 (blue)
            2.5f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            -2.5f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    // Prepare buffer
    GLuint loc = prepBuf((GLfloat*)axis_data, sizeof(axis_data));

    // Format data
    formatBuf(loc, 3, {0, 1}, Renderer::shader_axis);

}

/*
 * Initialize axes data
 */
void Renderer::enableTank() {
    float dim = 1.1f;
    float h = 2.1f;
    const GLfloat tank_data[] = {
            // Base
            -dim, -h, -dim,    0.0f, 0.0f, 1.0f,
            dim, -h, -dim,     0.0f, 0.0f, 1.0f,
            -dim, -h, dim,     0.0f, 0.0f, 1.0f,
            dim, -h, dim,      0.0f, 0.0f, 1.0f,
            dim, -h, -dim,     0.0f, 0.0f, 1.0f,
            -dim, -h, dim,     0.0f, 0.0f, 1.0f,
            // Left pane
            -dim, -h, -dim,    0.0f, 0.0f, 1.0f,
            -dim, 0.0f, -dim,     0.0f, 0.0f, 1.0f,
            -dim, 0.0f, dim,      0.0f, 0.0f, 1.0f,
            -dim, -h, -dim,    0.0f, 0.0f, 1.0f,
            -dim, -h, dim,     0.0f, 0.0f, 1.0f,
            -dim, 0.0f, dim,      0.0f, 0.0f, 1.0f,
            // Right pane
            dim, -h, -dim,    0.0f, 0.0f, 1.0f,
            dim, 0.0f, -dim,     0.0f, 0.0f, 1.0f,
            dim, 0.0f, dim,      0.0f, 0.0f, 1.0f,
            dim, -h, -dim,    0.0f, 0.0f, 1.0f,
            dim, -h, dim,     0.0f, 0.0f, 1.0f,
            dim, 0.0f, dim,      0.0f, 0.0f, 1.0f,
            // Back pane
            -dim, -h, -dim,   0.0f, 0.0f, 1.0f,
            dim, -h, -dim,    0.0f, 0.0f, 1.0f,
            dim, 0.0f, -dim,     0.0f, 0.0f, 1.0f,
            -dim, -h, -dim,   0.0f, 0.0f, 1.0f,
            -dim, 0.0f, -dim,    0.0f, 0.0f, 1.0f,
            dim, 0.0f, -dim,     0.0f, 0.0f, 1.0f,
            // Front pane
            -dim, -h, dim,   0.0f, 0.0f, 1.0f,
            dim, -h, dim,    0.0f, 0.0f, 1.0f,
            dim, 0.0f, dim,     0.0f, 0.0f, 1.0f,
            -dim, -h, dim,   0.0f, 0.0f, 1.0f,
            -dim, 0.0f, dim,    0.0f, 0.0f, 1.0f,
            dim, 0.0f, dim,     0.0f, 0.0f, 1.0f,
    };
    const GLfloat tank_outlines[] = {
            // Base
            -dim, -h, -dim,  0.0f, 0.0f, 0.0f,
            dim, -h, -dim,   0.0f, 0.0f, 0.0f,
            dim, -h, -dim,   0.0f, 0.0f, 0.0f,
            dim, -h, dim,    0.0f, 0.0f, 0.0f,
            dim, -h, dim,    0.0f, 0.0f, 0.0f,
            -dim, -h, dim,   0.0f, 0.0f, 0.0f,
            -dim, -h, dim,   0.0f, 0.0f, 0.0f,
            -dim, -h, -dim,  0.0f, 0.0f, 0.0f,
            // Left pane
            -dim, -h, -dim,  0.0f, 0.0f, 0.0f,
            -dim, -h, dim,   0.0f, 0.0f, 0.0f,
            -dim, -h, dim,   0.0f, 0.0f, 0.0f,
            -dim, 0.0f, dim,    0.0f, 0.0f, 0.0f,
            -dim, 0.0f, dim,    0.0f, 0.0f, 0.0f,
            -dim, 0.0f, -dim,    0.0f, 0.0f, 0.0f,
            -dim, 0.0f, -dim,    0.0f, 0.0f, 0.0f,
            -dim, -h, -dim,    0.0f, 0.0f, 0.0f,
            // Right pane
            dim, -h, -dim,  0.0f, 0.0f, 0.0f,
            dim, -h, dim,   0.0f, 0.0f, 0.0f,
            dim, -h, dim,   0.0f, 0.0f, 0.0f,
            dim, 0.0f, dim,    0.0f, 0.0f, 0.0f,
            dim, 0.0f, dim,    0.0f, 0.0f, 0.0f,
            dim, 0.0f, -dim,    0.0f, 0.0f, 0.0f,
            dim, 0.0f, -dim,    0.0f, 0.0f, 0.0f,
            dim, -h, -dim,    0.0f, 0.0f, 0.0f,
            // Back pane
            -dim, -h, -dim,  0.0f, 0.0f, 0.0f,
            dim, -h, -dim,   0.0f, 0.0f, 0.0f,
            dim, -h, -dim,   0.0f, 0.0f, 0.0f,
            dim, 0.0f, -dim,   0.0f, 0.0f, 0.0f,
            dim, 0.0f, -dim,   0.0f, 0.0f, 0.0f,
            -dim, 0.0f, -dim,   0.0f, 0.0f, 0.0f,
            -dim, 0.0f, -dim,   0.0f, 0.0f, 0.0f,
            -dim, -h, -dim,   0.0f, 0.0f, 0.0f,
            // Front pane
            -dim, -h, dim,  0.0f, 0.0f, 0.0f,
            dim, -h, dim,   0.0f, 0.0f, 0.0f,
            dim, -h, dim,   0.0f, 0.0f, 0.0f,
            dim, 0.0f, dim,   0.0f, 0.0f, 0.0f,
            dim, 0.0f, dim,   0.0f, 0.0f, 0.0f,
            -dim, 0.0f, dim,   0.0f, 0.0f, 0.0f,
            -dim, 0.0f, dim,   0.0f, 0.0f, 0.0f,
            -dim, -h, dim,   0.0f, 0.0f, 0.0f,


    };
    tank_transform = glm::mat4(1.0f);
    tank_transform = glm::translate(tank_transform, glm::vec3(0.0f, 0.0f, 0.0f));
    // Prepare buffer
    GLuint loc = prepBuf((GLfloat*)tank_data, sizeof(tank_data));
    GLuint loc2 = prepBuf((GLfloat*)tank_outlines, sizeof(tank_outlines));

    // Format data
    formatBuf(loc, 3, {4,5}, Renderer::shader_tank);
    formatBuf(loc2, 3, {6,7}, Renderer::shader_tank_outlines);

}

/*
 * Initialize fluid data
 */
void Renderer::enableFluid(std::vector<float> &data) {
    // Prepare buffer
    GLuint loc = prepBuf(data, true);
    fluid_buffer = loc;

    // Format data
    formatBuf(loc, 3, {2,3}, Renderer::shader_fluid);
}

/*
 * Initialize model data
 */
void Renderer::enableModel(Model &m) {
    // Prepare buffer
    GLuint loc = prepBuf(m.model_data, true);

    // Format data
    formatBuf(loc, 3, {m.attribs.first, m.attribs.second, 10}, *m.shader);
}




/* ****************************************************************
 *                        RENDERING ROUTINES                      *
 ******************************************************************/
// temp - just to get oriented in the world real quick
void Renderer::renderAxis() {
    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);
    glLineWidth(3.0f);

    glDrawArrays(GL_LINES , 0, 6);
}
void Renderer::renderTank(){
    shader_tank_outlines.bind();
    shader_tank_outlines.setMat4(20, qaiser::Harness::VP);
    shader_tank_outlines.setMat4(22, tank_transform);
    glDrawArrays(GL_LINES, 0, 40);

    shader_tank.bind();
    shader_tank.setMat4(20, qaiser::Harness::VP);
    shader_tank.setFloat(21, 0.02f);
    shader_tank.setMat4(22, tank_transform);
    glDrawArrays(GL_TRIANGLES, 0, 30);

}

void Renderer::renderFluid(){
    if(fluid != nullptr){
        editBuf(fluid->render_data, fluid_buffer);
    }


    shader_fluid.bind();
    shader_fluid.setMat4(20, qaiser::Harness::VP);

    renderModel(*fluid->ball);
}


void Renderer::renderModel(Model &m) {
    m.shader->bind();
    m.shader->setMat4(22, m.transformation);
    m.shader->setMat4(20, qaiser::Harness::VP);
    m.shader->setVec3(24, qaiser::Harness::campos);


    int tris = (int)(m.model_data.size()/2);

    for(int i=0; i<fluid->particles.size(); i++){

        glm::mat4 trans = m.transformation;
        trans = glm::translate(trans, fluid->particles[i]->position);
        trans = glm::scale(trans, glm::vec3(0.13f));
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(qaiser::Harness::V * m.transformation));

        m.shader->setFloat(25, 0.5f);
        // Fell out of tank
        if(!fluid->particles[i]->bounded){
            // Make opaque
            m.shader->setFloat(25, 1.0f);
            // Shrink until it goes away
            if(fluid->particles[i]->position.y <= -2.0f){
                if(fluid->particles[i]->shrink >= 0.0f){
                    trans = glm::scale(trans, glm::vec3(fluid->particles[i]->shrink));
                    fluid->particles[i]->shrink -= 0.005f;
                }
                else{
                    trans = glm::scale(trans, glm::vec3(0.0f));
                }
            }
        }
        m.shader->setMat4(22, trans);
        m.shader->setMat4(23, normalMatrix);
        glDrawArrays(GL_TRIANGLES , 0, tris);
    }

}

void Renderer::renderModelRegular(Model &m) {
    m.shader->bind();
    m.shader->setMat4(22, m.transformation);
    m.shader->setMat4(20, qaiser::Harness::VP);
    int tris = (int)(m.model_data.size()/2);
    glDrawArrays(GL_TRIANGLES , 0, tris);
}


void Renderer::renderGUI(Menu &g) {
    g.update();
    if(g.start){
        fluid->tune(g.mass, g.gas, g.density, g.viscosity);
        fluid->update();
    }
    if(g.reset){
        g.start = false;
        fluid->reset();
        g.reset = false;
    }
    renderModelRegular(*platform);
}


/* ****************************************************************
 *                      BUFFER SETUP ROUTINES                     *
 ******************************************************************/
/*
 * @data - Array of float data
 * @size - Size of the array in bytes
 * Inits a new buffer and returns its index
 */
unsigned int Renderer::prepBuf(GLfloat *data, GLuint size) {
    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], size, data, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);

    return free_buf;
}
unsigned int Renderer::prepBuf(GLushort *data, GLuint size) {
    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], size, data, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);

    return free_buf;
}


/*
 * @data - List of float data
 * Inits a new buffer and returns its index
 */
unsigned int Renderer::prepBuf(std::vector<GLfloat>& data, bool big) {
    int size = (int) data.size();
    int dat_size = 4*size;

    // Either allocate a big buffer (for large meshes and such)
    // Or just as large as your data
    int to_allocate;
    if(big)
        to_allocate = ONE_MB;
    else
        to_allocate = dat_size;


    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], to_allocate, nullptr, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT); // 1mb buffer

    float* ptr = (float*) glMapNamedBufferRange(buf[free_buf], 0, dat_size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);
    for(int i=0; i<size; i++){
        ptr[i] = data[i];
    }
    glUnmapNamedBuffer(buf[free_buf]);

    return free_buf;
}

/*
 * @buf - Buffer to edit
 * Edits buffer data
 */
unsigned int Renderer::editBuf(std::vector<GLfloat>& data, GLuint i) {
    int size = (int) data.size();
    int dat_size = 4*size;

    // Buffer overflowed
    if(dat_size >= ONE_MB){
        Logger::log(ERROR, "Buffer overflowed, buffer ID: "+ std::to_string(i), __FILENAME__);
    }

    float* ptr = (float*) glMapNamedBufferRange(buf[i], 0, dat_size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);
    for(int x=0; x<size; x++){
        ptr[x] = data[x];
    }
    glUnmapNamedBuffer(buf[i]);

    return i;
}


/*
 * @loc                     - Active buffer location
 * @comps_per_elem          - Number of components in an element
 * @names                   - List of attribute names used in GLSL
 * @s                       - Shader to get the attribute names from
 *
 * Formats the buffer for the VAO
 */

void Renderer::formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs, Shader& s) {
    free_bindpoint++;
    auto num_attribs = attribs.size();

    for(int i=0; i < num_attribs; i++){

        glVertexArrayAttribFormat(VAO, attribs[i], comps_per_elem, GL_FLOAT, GL_FALSE, (i*comps_per_elem)*sizeof(float));
        glVertexArrayAttribBinding(VAO, attribs[i], free_bindpoint);
        glEnableVertexArrayAttrib(VAO, attribs[i]);
    }

    glVertexArrayVertexBuffer(VAO, free_bindpoint, buf[loc], 0, (num_attribs*comps_per_elem)*sizeof(float));
}







