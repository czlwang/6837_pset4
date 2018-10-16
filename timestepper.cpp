#include "timestepper.h"
#include <iostream>
#include <cstdio>

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
//    std::cout<<"take step"<<std::endl;
    std::vector<Vector3f> currentState = particleSystem->getState();
    std::vector<Vector3f> f = particleSystem->evalF(currentState);
    std::vector<Vector3f> new_state;
//    std::cout<<"take step2"<<std::endl;
    for(int i = 0; i<(int)currentState.size(); i++)
    {
//        std::cout << "here " << std::endl;
//        std::cout << currentState[i][0] << " " << currentState[i][1] << " " << currentState[i][2] << std::endl;
//        std::cout << f[i][0] << " " << f[i][1] << " " << f[i][2] << std::endl;
        Vector3f sum = currentState[i] + stepSize*f[i];
//        std::cout << sum[0] << " " << sum[1] << " " << sum[2] << std::endl;
        new_state.push_back(sum);
    }
    particleSystem->setState(new_state);
}

void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    std::vector<Vector3f> currentState = particleSystem->getState();
    std::vector<Vector3f> f0 = particleSystem->evalF(currentState);
    std::vector<Vector3f> timestepped_state;
    for(int i = 0; i<(int)currentState.size(); i++)
    {
        timestepped_state.push_back(currentState[i] + stepSize*f0[i]);
    }
    std::vector<Vector3f> f1 = particleSystem->evalF(timestepped_state);
    
    std::vector<Vector3f> new_state;
    for(int i = 0; i<(int)currentState.size(); i++)
    {
        new_state.push_back( currentState[i] + stepSize/2.0*(f0[i]+f1[i]) );
    }
    particleSystem->setState(new_state);
}


void RK4::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    std::vector<Vector3f> xn = particleSystem->getState();
    std::vector<Vector3f> k1 = particleSystem->evalF(xn);
    std::vector<Vector3f> timestepped_state_k1;
    for(int i = 0; i<(int)xn.size(); i++)
    {
        timestepped_state_k1.push_back(xn[i] + stepSize/2.0*k1[i]);
    }
    std::vector<Vector3f> k2 = particleSystem->evalF(timestepped_state_k1);
    
    std::vector<Vector3f> timestepped_state_k2;
    for(int i = 0; i<(int)xn.size(); i++)
    {
        timestepped_state_k2.push_back(xn[i] + stepSize/2.0*k2[i]);
    }
    std::vector<Vector3f> k3 = particleSystem->evalF(timestepped_state_k2);
    
    std::vector<Vector3f> timestepped_state_k3;
    for(int i = 0; i<(int)xn.size(); i++)
    {
        timestepped_state_k3.push_back(xn[i] + stepSize*k3[i]);
    }
    std::vector<Vector3f> k4 = particleSystem->evalF(timestepped_state_k3);
    
    std::vector<Vector3f> x_next;
    for(int i = 0; i<(int)xn.size(); i++)
    {
        x_next.push_back(xn[i] + stepSize/6.0*(k1[i] + 2*k2[i] + 2*k3[i] + k4[i]));
    }
    particleSystem->setState(x_next);
}

