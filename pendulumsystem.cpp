#include "pendulumsystem.h"

#include <cassert>
#include <map>
#include <iostream>
#include "camera.h"
#include "vertexrecorder.h"
#include "spring.hpp"

// TODO adjust to number of particles.
const int NUM_PARTICLES = 4;
const float g = 1.0;
const float drag = 0.1;

PendulumSystem::PendulumSystem()
{
    // TODO 4.2 Add particles for simple pendulum
    float k = 3.0;
    float r = 0.1;

    //populate springs
    for(int i=0; i<NUM_PARTICLES; i++)
    {
        springs.push_back(Spring(2*i, 2*(i+1), k, r));
    }
    
    //state is vector of size 2n where where positions are stored at even indices and velocities at odd indices.
    //populate state with initial conditions
    m_vVecState = std::vector<Vector3f>(2*(NUM_PARTICLES+1));
    m_vVecState[0] = Vector3f(1, 2, rand_uniform(-0.1f, 0.1f));
    m_vVecState[2] = Vector3f(0,0,0);
    for(int i=1; i<NUM_PARTICLES+1; i++)
    {
            m_vVecState[2*i] = Vector3f(1, 1-i, rand_uniform(-0.1f, 0.1f));
            m_vVecState[2*i+1] = Vector3f(0,0,0);
    }
    
    //populate pointsToSprings, which maps point indices to a list of spring indices
    for(int i=0; i<(int)springs.size(); i++)
    {
        Spring s = springs[i];
        if(pointsToSprings.find(s.particles[0]) == pointsToSprings.end())
        {
            std::vector<int> list;
            pointsToSprings[s.particles[0]] = list;
        }
        pointsToSprings[s.particles[0]].push_back(i);
        if(pointsToSprings.find(s.particles[1]) == pointsToSprings.end())
        {
            std::vector<int> list;
            pointsToSprings[s.particles[1]] = list;
        }
        pointsToSprings[s.particles[1]].push_back(i);
    }
}


std::vector<Vector3f> PendulumSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    
    f.push_back(state[0]);
    f.push_back(Vector3f(0,0,0));//0 force on the first node
    
    for(int i=1; i < (int)state.size()/2; i++)
    {
        Vector3f xi = state[2*i];
        Vector3f vel = state[2*i + 1];
        Vector3f force = g*Vector3f(0,-1,0) - drag*vel;
        for(int spring_index : pointsToSprings[2*i])
        {
            Spring spring = springs[spring_index];
            Vector3f xj = state[spring.getSpringNeighbor(2*i)];
            float k = spring.k;
            float r = spring.r;
            Vector3f d = xi - xj;
            Vector3f springForce = -k*(d.abs()-r)*d/d.abs();
            force += springForce;
        }
        f[2*i] = vel;
        f[2*i+1] = force;
    }
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw(GLProgram& gl)
{
    const Vector3f PENDULUM_COLOR(0.73f, 0.0f, 0.83f);
    gl.updateMaterial(PENDULUM_COLOR);
    for(int i=0; i < (int)m_vVecState.size()/2; i++)
    {
        Vector3f pos = m_vVecState[2*i];
        gl.updateModelMatrix(Matrix4f::translation(pos));
        drawSphere(0.075f, 10, 10);
    }
    
    const Vector3f CLOTH_COLOR(0.2f, 0.2f, 0.9f);
    gl.updateMaterial(CLOTH_COLOR);
    
    float w = 0.2f;
    gl.disableLighting();
    for(int i=0; i<(int)springs.size(); i++)
    {
        Vector3f part1 = m_vVecState[springs[i].particles[0]];
        Vector3f part2 = m_vVecState[springs[i].particles[1]];
        gl.updateModelMatrix(Matrix4f::identity()); // update uniforms after mode change
        VertexRecorder rec;
        rec.record(part1, CLOTH_COLOR);
        rec.record(part2, CLOTH_COLOR);
        glLineWidth(3.0f);
        rec.draw(GL_LINES);
    }
    gl.enableLighting();

}
