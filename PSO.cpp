
#include <vector>
#include <random>
#include <math.h>
#include <thread>
#include <iostream>

#include <PSO.hpp>
#include <Particle.hpp>
#include <Objective.hpp>

PSO::PSO(int NumVars, int NumParticles, int MaxIters)
    : NumVars(NumVars), NumParticles(NumParticles), MaxIters(MaxIters)
{

    Particles.reserve(NumParticles);
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> dist_StartTemp(90, 150);
    std::uniform_real_distribution<double> dist_Factor(0.95, 0.999);
    std::uniform_int_distribution<int> dist_MaxOuterIters(300, 1000);
    std::uniform_int_distribution<int> dist_MaxInnerIters(100, 500);

    std::uniform_real_distribution<double> dist_XY(0, 1e4);
    int NumGraphNodes = 10000;

    double PosX[NumGraphNodes], PosY[NumGraphNodes];

    for (int i = 0; i < NumGraphNodes; i++)
    {
        PosX[i] = dist_XY(generator);
        PosY[i] = dist_XY(generator);
    }
    for (int i = 0; i < NumParticles; i++)
    {

        double StartTemp = dist_StartTemp(generator);
        double Factor = dist_Factor(generator);
        int MaxOuterIters = dist_MaxOuterIters(generator); 
        int MaxInnerIters = dist_MaxInnerIters(generator);
        double x[] = {StartTemp, Factor , (double) MaxOuterIters , (double) MaxInnerIters};
        //Particle P1(NumVars);
        Particles.push_back(Particle(NumVars));
        Particles[i].Set(x, PosX, PosY);
        Particles[i].ObjEval();
    }
};

void PSO::Run()
{
    printf("Running...\n");

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> dist_r(0, 1);
    double r1, r2;

    int IterCount = 0;
    while (IterCount < MaxIters)
    {
        std::vector<std::thread> ParticleThreads;

        for (int i = 0; i < NumParticles; i++)
        {
            r1 = dist_r(generator);
            r2 = dist_r(generator);
            Particles[i].Update(r1, r2);
            ParticleThreads.push_back(std::thread(Objective, std::ref(Particles[i])));
        }

        for (int i = 0; i < NumParticles; i++)
        {
            ParticleThreads[i].join();
        }
        printf("Iter: %d , f(x): %lf, x: %lf %lf %lf %lf\n", IterCount, Particle::global_best_val, Particle::global_best_x[0], Particle::global_best_x[1],
                                                                        Particle::global_best_x[2] , Particle::global_best_x[3]);
        IterCount++;
    }
};