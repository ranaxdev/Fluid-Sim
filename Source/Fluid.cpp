#include "Fluid.h"
#include <iostream>
#include <cmath>
Fluid::Fluid() {
    initParticles(125);
    ball = new Model(std::string(SRC+"Res/sphere.obj").c_str(), "vert_ball", "frag_ball", std::make_pair(8,9), glm::vec3(0.8f, 0.8f, 1.0f));
}

// Initializes n particles and stores them in a cube shape
void Fluid::initParticles(int amount) {
    float x_val = -0.75f;
    float z_val = -0.75f;
    float y_val = 1.0f;

    // Randomness
    float LO = -0.05f;
    float HI = 0.05f;

    for(int i=0; i < amount; i++){
        // Initialize new particle at calibrated x,y,z position
        auto* p = new Particle;
        p->position = glm::vec3(x_val, y_val, z_val);
        p->velocity = glm::vec3(0.0f);
        p->force = glm::vec3(0.0f);
        p->density = rest_density;
        p->pressure = 0.0f;
        particles.push_back(p);

        // Calibrate x,y,z positions by spacing them in a cube and adding some randomness
        x_val += radius + (LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
        if(x_val >= 0.75f){
            x_val = -0.75f;
            z_val += radius + (LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
        }
        if(z_val >= 0.75f){
            z_val = -0.75f;
            y_val += (radius) + (LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
        }


    }
}


// Compute shallow wave equations to get forces on particles
// Pressure, density, viscosity calculated using different kernel functions
// For particle i, the neighbourhood particles j will be the ones within the spherical radius h of i
// Integration scheme to update positions and velocity of particles (leap-frog)
void Fluid::update() {

    /* ATTRIBUTE CALCULATION */
    for(auto& p : particles){
        p->density = 0.0f;
        p->pressure = 0.0f;
        // Density
        // Search neighbourhood particles
        for(auto& n : particles){
            if(n == p)
                continue;

            glm::vec3 diff = p->position - n->position;
            float dist_sq = pow(diff.x, 2) + pow(diff.y, 2) + pow(diff.z, 2);

            if(dist_sq < pow(radius,2)){
                // Using W_poly kernel function for density
                p->density += mass * (float)W_poly6(dist_sq, radius);
            }
        }
        // Density of the particle itself
        p->density += (mass * (float) W_poly6(0, radius));

        // Pressure
        p->pressure = k * (p->density - rest_density);

    }


    /* FORCE COMPUTATION */
    for(auto& p: particles){
        // Reset forces
        p->force = glm::vec3(0.0f);
        auto pressure_force = glm::vec3(0.0f);
        auto viscosity_force = glm::vec3(0.0f);

        // Calculate pressure & viscosity of particle using neighbourhood particles
        for(auto& n : particles){
            if(n == p)
                continue;

            glm::vec3 diff = p->position - n->position;
            float dist = sqrt(pow(diff.x,2) + pow(diff.y, 2) + pow(diff.z,2));

            if(dist <= radius){
                // Using W_spiky kernel function for pressure force
                pressure_force += -glm::normalize(diff) * mass * ((p->pressure + n->pressure)/ (2 * n->density)) * (float)W_spiky(dist, radius);
                // Using W_visc kernel function for viscosity
                viscosity_force += mew * mass * ((n->velocity - p->velocity) / (n->density)) * (float) W_visc(dist, radius);
            }
        }

        // Calculate gravitational force
        glm::vec3 gravity_force = (mass/p->density) * glm::vec3(0.0f, -9.81f, 0.0f);

        // Compute total force
        p->force = pressure_force + viscosity_force + gravity_force;
    }



    /* INTEGRATION */
    // Leap-frog integration
    for(auto& p : particles){
            glm::vec3 half_velocity = p->velocity + ((p->force/Fluid::mass) * delta/2.0f);
            p->position = p->position + (half_velocity * delta);
            p->velocity = half_velocity + ((p->force/Fluid::mass) * delta/2.0f);


        // Handle boundary conditions (water-tank collisions)
        // Base
        if(p->position.y < -2.0f){
            p->velocity.y *= -0.5f; // damping
            p->position.y = -2.0f;
        }
        // Handle spillage
        if(p->position.y > 0.0f && (p->position.z < -1.0f || p->position.z > 1.0f || p->position.x > 1.0f || p->position.x < -1.0f)){
            p->bounded = false;
        }
        // Slow down spillage
        if(!p->bounded && p->position.y <= -2.0f){
            p->velocity *= 0.0f;
        }

        // Left pane
        if(p->position.z < -1.0f && p->bounded){
            p->velocity.z *= -0.5f;
            p->position.z = -1.0f;
        }
        // Right pane
        if(p->position.z > 1.0f && p->bounded){
            p->velocity.z *= -0.5f;
            p->position.z = 1.0f;
        }
        // Front pane
        if(p->position.x > 1.0f && p->bounded){
            p->velocity.x *= -0.5f;
            p->position.x = 1.0f;
        }
        // Back pane
        if(p->position.x < -1.0f && p->bounded){
            p->velocity.x *= -0.5f;
            p->position.x = -1.0f;
        }


    }
}


// Kernel functions
double Fluid::W_poly6(float dist, float rad) {
    if(dist >= 0 && dist <= rad){
        double c = 315.0/(64.0 * 3.14159 * pow(rad,9));
        return c * pow(pow(rad,2)-pow(dist,2),3);
    }
    return 0.0;
}

double Fluid::W_spiky(float dist, float rad) {
    if(dist >= 0 && dist <= rad){
        double c = 15.0/(3.14159 * pow(rad,6));
        return c * pow(rad-dist,3);
    }
    return 0.0;
}

double Fluid::W_visc(float dist, float rad) {
    if(dist >=0 && dist <= rad){
        double c = 45.0/(3.14159 * pow(rad,6));
        return c * (rad - dist);
    }
    return 0.0;
}

void Fluid::reset() {
    for(auto& p: particles){
        delete p;
    }
    particles.clear();
    initParticles(125);
}

void Fluid::tune(float t_mass, float t_gas, float t_dens, float t_visc) {
    mass = t_mass;
    k = t_gas;
    rest_density = t_dens;
    mew = t_visc;
}


