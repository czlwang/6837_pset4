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
    float k = 1.0;
    float r = 0.1;
    springs = {Spring(0, 2, 8*k, r),
               Spring(2, 4, 4*k, 2*r),
               Spring(4, 6, 2*k, 4*r),
               Spring(6, 8, k, 8*r)
              };
    
    //state is vector of size 2n where where positions are stored at even indices and velocities at odd indices.
    m_vVecState = { Vector3f(0,1,0), Vector3f(0,0,0),
                    Vector3f(rand_uniform(-0.5f, 0.5f),0,0), Vector3f(0,0,0),
                    Vector3f(rand_uniform(-0.5f, 0.5f),-1,0), Vector3f(0,0,0),
                    Vector3f(0,-2,0), Vector3f(0,0,0),
                    Vector3f(0,-3,0), Vector3f(0,0,0)
                  };
    
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
    
    for(int i=0; i<(int)m_vVecState.size()/2; i++)
    {
        std::cout << "point " << 2*i << std::endl;
        for(int j=0; j<pointsToSprings[2*i].size(); j++)
        {
            std::cout<< "spring " << pointsToSprings[2*i][j] << std::endl;
        }
    }
    
    // TODO 4.3 Extend to multiple particles
    
    // To add a bit of randomness, use e.g.
    // float f = rand_uniform(-0.5f, 0.5f);
    // in your initial conditions.
}


std::vector<Vector3f> PendulumSystem::evalF(std::vector<Vector3f> state)
{
//    std::cout<<"pendulum evalF"<<std::endl;
    std::vector<Vector3f> f(state.size());
    // TODO 4.1: implement evalF
    //  - gravity
    //  - viscous drag
    //  - springs
    
    //state is vector of size 2n where where positions are stored at even indices and velocities at odd indices.
    
    f.push_back(state[0]);
    f.push_back(Vector3f(0,0,0));//0 force
    
    for(int i=1; i < (int)state.size()/2; i++)
    {
        Vector3f xi = state[2*i];
        Vector3f vel = state[2*i + 1];
        Vector3f force = g*Vector3f(0,-1,0) - drag*vel;
//        std::cout << "particle " << 2*i << std::endl;
        for(int spring_index : pointsToSprings[2*i])
        {
//            std::cout << "made it in the spring loop " << 2*i << std::endl;
            Spring spring = springs[spring_index];
            Vector3f xj = state[spring.getSpringNeighbor(2*i)];
//            std::cout << "neighbor particle " << spring.getSpringNeighbor(2*i) << std::endl;

//            std::cout << "made it in the spring loop " << state.size() << std::endl;
            float k = spring.k;
            float r = spring.r;
            Vector3f d = xi - xj;
//            std::cout << "d " << d[0] << " " << d[1] << " " << d[2] << std::endl;
            Vector3f springForce = -k*(d.abs()-r)*d/d.abs();
//            std::cout << "springForce " << springForce[0] << " " << springForce[1] << " " << springForce[2] << std::endl;
            force += springForce;
        }
        f[2*i] = vel;
        f[2*i+1] = force;
    }
//    std::cout << "size " << state.size() << std::endl;
//    std::cout << "f2 " << f[2][0] << " " << f[2][1] << " " << f[2][2] << std::endl;
    
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw(GLProgram& gl)
{
    const Vector3f PENDULUM_COLOR(0.73f, 0.0f, 0.83f);
    gl.updateMaterial(PENDULUM_COLOR);
    
    // TODO 4.2, 4.3

    // example code. Replace with your own drawing  code
    for(int i=0; i < (int)m_vVecState.size()/2; i++)
    {
        Vector3f pos = m_vVecState[2*i];
        gl.updateModelMatrix(Matrix4f::translation(pos));
        drawSphere(0.075f, 10, 10);
    }
    
    const Vector3f CLOTH_COLOR(0.2f, 0.2f, 0.9f);
    gl.updateMaterial(CLOTH_COLOR);
    
    // EXAMPLE for how to render cloth particles.
    //  - you should replace this code.
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
