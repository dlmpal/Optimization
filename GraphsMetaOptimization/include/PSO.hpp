
#ifndef PSO_HPP 
#define PSO_HPP 

#include <Particle.hpp>
#include <vector> 


class PSO{

    public: 
        PSO(int NumVars, int NumParticles, int MaxIters);
        void Run();         

    private: 
        int NumVars; 
        int NumParticles; 
        int MaxIters; 
        std::vector<Particle> Particles; 
        



};



#endif