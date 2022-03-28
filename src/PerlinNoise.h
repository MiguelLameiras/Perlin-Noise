#ifndef __PerlinNoise__
#define __PerlinNoise__

#include <iostream>
#include <vector>
#include <stdio.h>    
#include <stdlib.h>    
#include <time.h>      

class PerlinNoise
{
public:
    PerlinNoise(int, int, int);
    PerlinNoise(int, int, int, int);
    ~PerlinNoise() = default;

    double Compute_Height(int,int);
    void Generate_Gradient();

private:
    std::vector<std::vector<std::pair<double, double>>> Gradient;

    int freq;
    int seed;
    int h;
    int w;
};

#endif