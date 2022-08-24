#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <GraphTSP.hpp>

class Particle
{

public:

    static std::vector<double> global_best_x;
    static double global_best_val;
    Particle(){};
    Particle(int NumVars);
    ~Particle(){
        delete[] x; 
        delete[] best_x;
        delete[] vel;
    };
    Particle &operator=(const Particle &rhs);
    Particle(const Particle& rhs);
    void Update(double r1, double r2);
    void Set(double *x , double *PosX , double *PosY);
    void ObjEval();
    void Print();

public:
    int NumVars;
    int InitStatus = 0;
    double *x;
    double *vel;
    double *best_x;
    double best_val;
    GraphTSP Graph;

};

#endif