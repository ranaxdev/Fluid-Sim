#ifndef _H_FLUID
#define _H_FLUID

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Globals.h"
#include "Model.h"


struct Particle{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float pressure;
    float density;
    bool bounded = true;
    float shrink = 1.0f;
};

class Fluid{
public:
    Fluid();

    void initParticles(int amount);
    void update();
    void reset();
    void tune(float t_mass, float t_gas, float t_dens, float t_visc);
    std::vector<Particle*> particles;


    Model* ball;

    std::vector<float> render_data;

private:
    float r = 52.0f/255.0f;
    float g = 204.0f/255.0f;
    float b = 235.0f/255.0f;

    float delta = 1/120.0f; // time step
    float mass = 0.05f; // global mass for all particles

    float radius = 0.70f;
    float k = 0.03f; // gas constant
    float rest_density = 200.5f;
    float mew =0.025f; // viscosity constant

    // Kernel functions
    double W_poly6(float r, float h);
    double W_spiky(float r, float h);
    double W_visc(float r, float h);

};

#endif