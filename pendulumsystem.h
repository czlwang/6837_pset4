#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vector>

#include "particlesystem.h"

class PendulumSystem : public ParticleSystem
{
public:
    PendulumSystem();

    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;
    void draw(GLProgram&);
    std::vector<int> getSpringNeighbors(int i); //get the points which are connected to this point by a spring
    // inherits 
    // std::vector<Vector3f> m_vVecState;
private:
    std::vector<float> springs;
    std::map<int, std::vector<int>> springsToPoints;//map from spring indices to point indices. The point indices are indices into the state vector
    std::map<int, std::vector<int>> pointsToSprings;//map from point indices to spring indices
};

#endif
