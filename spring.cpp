//
//  Spring.cpp
//  a3
//
//  Created by Christopher Wang on 10/14/18.
//
//

#include "spring.hpp"
Spring::Spring(int part1, int part2, float k, float r)
{
    particles.push_back(part1);
    particles.push_back(part2);
    this->k = k;
    this->r = r;
}

int Spring::getSpringNeighbor(int i)
{
    if(particles[0] == i)
    {
        return particles[1];
    }
    return particles[0];
}