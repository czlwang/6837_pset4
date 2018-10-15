#include "pendulumsystem.h"

#include <cassert>
#include <map>
#include "camera.h"
#include "vertexrecorder.h"

// TODO adjust to number of particles.
const int NUM_PARTICLES = 4;
const float g = 9.8;

PendulumSystem::PendulumSystem()
{

    // TODO 4.2 Add particles for simple pendulum
    springs = {1.0};
    
    m_vVecState = { Vector3f(0,1,0), Vector3f(0,0,0),
                    Vector3f(0,0.5,0), Vector3f(0,0,0)};
    
    pointsToSprings.insert({0, {0}});
    springsToPoints.insert({0, {0,2}});
    
    // TODO 4.3 Extend to multiple particles
    
    
    // To add a bit of randomness, use e.g.
    // float f = rand_uniform(-0.5f, 0.5f);
    // in your initial conditions.
}

std::vector<int> PendulumSystem::getSpringNeighbors(int i)
{
    std::vector<int> res;
    for(int spring_index : pointsToSprings[i])
    {
        std::vector<int> spring = springsToPoints[spring_index];
        if(spring[0] != i)
        {
            res.push_back(spring[0]);
        }else
        {
            res.push_back(spring[1]);
        }
    }
    return res;
}


std::vector<Vector3f> PendulumSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    // TODO 4.1: implement evalF
    //  - gravity
    //  - viscous drag
    //  - springs
    
    //state is vector of size 2n where where positions are stored at even indices and velocities at odd indices.
    
    for(int i=0; i < (int)state.size()/2; i++)
    {
        Vector3f xi = state[i];
        Vector3f pos = state[i];
        Vector3f vel = state[2*i + 1];
        Vector3f force = g*Vector3f(0,-1,0);
        std::vector<int> springNeighbors = getSpringNeighbors(i);
        for(int j : springNeighbors)
        {
            Vector3f xj = state[j];
            Vector3f d = xi - xj;
            
        }
        f.push_back(force);
    }
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw(GLProgram& gl)
{
    const Vector3f PENDULUM_COLOR(0.73f, 0.0f, 0.83f);
    gl.updateMaterial(PENDULUM_COLOR);

    // TODO 4.2, 4.3

    // example code. Replace with your own drawing  code
    gl.updateModelMatrix(Matrix4f::translation(Vector3f(-0.5, 1.0, 0)));
    drawSphere(0.075f, 10, 10);
}
