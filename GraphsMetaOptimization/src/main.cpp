// Meta-optimization using PSO
// Optimizing a simulated annealing optimizer
// which finds a solution to a TSP problem 


#include <vector> 
#include <iostream>
#include <random>
#include "PSO.hpp"
#include "Particle.hpp"



std::vector<double> Particle::global_best_x = {100,0.99,1000,200};

int main(){
   // PSO arguments: Number of varibales to optimize , Number of particles , Maximum iterations 
   // Here , the two variables are the starting annealing temperature the the factor by which it is reduced 
   PSO pso1(2 , 7 , 60);
   pso1.Run();
   return 0; 
}
