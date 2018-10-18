#include "clothsystem.h"
#include "camera.h"
#include "vertexrecorder.h"
#include <iostream>
#include <math.h>

 // your system should at least contain 8x8 particles.
const int W = 5;
const int H = 4;
const float g = 5.0;
const float drag = 2.0;

int stateIndex(int i, int j)
{
    return (W*i + j)*2;
}

ClothSystem::ClothSystem()
{
    // TODO 5. Initialize m_vVecState with cloth particles. 
    // You can again use rand_uniform(lo, hi) to make things a bit more interesting

    //populate springs
    float spring_constant = 20.0;
    std::vector<int> offsets = {-1,0 , 1,0 , 0,-1 , 0,1 , -1,1 , -1,-1 , 1,-1 , 1,1, 0,2 , 0,-2 , 2,0 , -2,0};
    for(int i=0; i<H; i++)
    {
        for(int j=0; j<W; j++)
        {
            for(int k=0; k<offsets.size()/2; k++)
            {
                float r = .3;
                if(k >= 4 && k <= 7)
                {
                    r = sqrt(2)*r;
                }else if(k>=8)
                {
                    r = 2*r;
                }
                int y = i + offsets[2*k];
                int x = j + offsets[2*k + 1];
                int stateIndex1 = stateIndex(i,j);
                int stateIndex2 = stateIndex(y,x);
                if(x>=0 && x<W && y>=0 && y<H && stateIndex1<stateIndex2)
                {
                    springs.push_back(Spring(stateIndex1, stateIndex2, spring_constant, r));
                }
            }
        }
    }
    
    m_vVecState = std::vector<Vector3f>(2*W*H);
    for(int i=0; i<H; i++)
    {
        for(int j=0; j<W; j++)
        {
            m_vVecState[stateIndex(i, j)] = Vector3f(-2+i*0.3,
                                                     j*0.3,
                                                     rand_uniform(-0.1f, 0.1f));
            m_vVecState[stateIndex(i, j)+1] = Vector3f(0,0,0);
        }
    }
    
    //state is vector of size 2n where where positions are stored at even indices and velocities at odd indices.
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

std::vector<Vector3f> ClothSystem::evalF(std::vector<Vector3f> state)
{
    std::vector<Vector3f> f(state.size());
    
    for(int i=0; i < (int)state.size()/2; i++)
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
        
        if(i==0 || i==W*(H-1))//0 force on the two top corners of the cloth
        {
            f[2*i] = Vector3f(0,0,0);
            f[2*i+1] = Vector3f(0,0,0);
        }
    }
    
    return f;
}


void ClothSystem::draw(GLProgram& gl)
{
    const Vector3f CLOTH_COLOR(0.9f, 0.9f, 0.9f);
    gl.updateMaterial(CLOTH_COLOR);
    
    for(int i=0; i < (int)m_vVecState.size()/2; i++)
    {
        Vector3f pos = m_vVecState[2*i];
        gl.updateModelMatrix(Matrix4f::translation(pos));
        drawSphere(0.075f, 10, 10);
    }
    
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

