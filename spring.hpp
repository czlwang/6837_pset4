//
//  Spring.hpp
//  a3
//
//  Created by Christopher Wang on 10/14/18.
//
//

#ifndef spring_hpp
#define spring_hpp

#include <stdio.h>
#include <vector>
class Spring
{
public:
    Spring(int part1, int part2, float k, float r);
    int getSpringNeighbor(int i);
    float k;
    float r;
    std::vector<int> particles;
};

#endif /* spring_hpp */
