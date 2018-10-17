#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vector>
#include "spring.hpp"
#include "particlesystem.h"

class ClothSystem : public ParticleSystem
{
    ///ADD MORE FUNCTION AND FIELDS HERE
public:
    ClothSystem();

    // evalF is called by the integrator at least once per time step
    std::vector<Vector3f> evalF(std::vector<Vector3f> state) override;

    // draw is called once per frame
    void draw(GLProgram& ctx);

    // inherits
    // std::vector<Vector3f> m_vVecState;
    std::vector<int> getSpringNeighbors(int i); //get the points which are connected to this point by a
private:
    std::vector<Spring> springs;
    std::map<int, std::vector<int>> pointsToSprings;//map from point indices to spring indices
};


#endif
