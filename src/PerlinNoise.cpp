#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(int w_, int h_, int freq_)
{
    w = w_;
    h = h_;
    freq = freq_;
    seed = NULL;
    Gradient = std::vector<std::vector<std::pair<double, double>>> {w, std::vector<std::pair<double, double>>(h)};
}

PerlinNoise::PerlinNoise(int w_, int h_, int freq_, int seed_)
{
    w = w_;
    h = h_;
    freq = freq_;
    seed = seed_;
    Gradient = std::vector<std::vector<std::pair<double, double>>> {w, std::vector<std::pair<double, double>>(h)};
}

void PerlinNoise::Generate_Gradient()
{
    seed ? srand(seed) : srand (time(NULL));

    // Generate grid of vectors
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Gradient[x][y] = std::make_pair(rand() % 2 - 1, rand() % 2 - 1);
            //std::cout << Gradient[x][y].first << " " << Gradient[x][y].second << std::endl;
        }
    }

}

double PerlinNoise::Compute_Height(int x, int y)
{
    // Variable to hold final result
    double z = 0;

    // Center of the cell and not on corners
    double real_x = x + 0.5;
    double real_y = y + 0.5;

    // Compute dot product between distance vector and gradient vectors
    std::vector<double> dot_product;
    std::vector<std::pair<int, int>> corners;
    dot_product.clear();
    corners.clear();

    // Closest corners to point
    corners.push_back(std::make_pair((int)real_x / freq, (int)real_y / freq));
    corners.push_back(std::make_pair((int)real_x / freq + 1, (int)real_y / freq));
    corners.push_back(std::make_pair((int)real_x / freq, (int)real_y / freq + 1));
    corners.push_back(std::make_pair((int)real_x / freq + 1, (int)real_y / freq + 1));

    // Calculate dot product between de distance vector and gradient vector
    for (int i = 0; i < 4; i++)
    {
        dot_product.push_back(Gradient[corners[i].first][corners[i].second].first * (real_x - corners[i].first * freq) + Gradient[corners[i].first][corners[i].second].second * (real_y - corners[i].second * freq));
        // std::cout << i << " " << dot_product[i] << " " << corners[i].first << " " << corners[i].second << std::endl;
    }

    // Interpolate dot product results -> Bilinear Interpolation

    // Interpolate in x direction
    std::vector<double> f;
    f.clear();
    f.push_back(((corners[1].first * freq - real_x) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[0] + ((real_x - corners[0].first * freq) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[1]);
    f.push_back(((corners[1].first * freq - real_x) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[2] + ((real_x - corners[0].first * freq) / (corners[1].first * freq - corners[0].first * freq)) * dot_product[3]);

    // Interpolate in the y direction
    z = ((corners[0].second * freq - real_y) / (corners[2].second *freq - corners[0].second * freq)) * f[1] + ((real_y - corners[2].second * freq) / (corners[2].second * freq - corners[0].second * freq)) * f[0];

    return z;
}
