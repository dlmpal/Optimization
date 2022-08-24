

#include <Particle.hpp>

#include <limits>
#include <stdio.h>

double Particle::global_best_val = std::numeric_limits<double>::max();

Particle::Particle(int NumVars)
{   this->NumVars = NumVars; 
    this->x = new double[NumVars];
    this->best_x = new double[NumVars];
    this->vel = new double[NumVars];
    this->best_val = std::numeric_limits<double>::max();
    this->Graph = GraphTSP(1000);
    this->InitStatus = 1;
    for (int i = 0; i < this->NumVars; i++)
    {
        this->x[i] = 0;
        this->vel[i] = 0;
        this->best_x[i] = 0;
    }
};

Particle &Particle::operator=(const Particle &rhs)
{   
    this->NumVars = rhs.NumVars;
    this->x = new double[this->NumVars];
    this->best_x = new double[this->NumVars];
    this->vel = new double[this->NumVars];

    for(int i = 0 ; i < rhs.NumVars ; i++){
        this->x[i] = rhs.x[i]; 
        this->best_x[i] = rhs.best_x[i];
        this->vel[i] = rhs.vel[i];
    }
    this->best_val = rhs.best_val;
    this->Graph = rhs.Graph;
    this->InitStatus = 1;
    return *this;
};

Particle::Particle(const Particle &rhs)
{   
    this->NumVars = rhs.NumVars;
    this->x = new double[this->NumVars];
    this->best_x = new double[this->NumVars];
    this->vel = new double[this->NumVars];

    for(int i = 0 ; i < NumVars ; i++){
        this->x[i] = rhs.x[i]; 
        this->best_x[i] = rhs.best_x[i];
        this->vel[i] = rhs.vel[i];
    }
    this->best_val = rhs.best_val;
    this->Graph = rhs.Graph;
    this->InitStatus = 1;
};

void Particle::Update(double r1, double r2)
{
    for (int i = 0; i < NumVars; i++)
    {
        this->vel[i] = 0.6 * this->vel[i] + 2 * r1 * (this->best_x[i] - this->x[i]) + 2 * r2 * (this->global_best_x[i] - this->x[i]);
        this->x[i] += this->vel[i];
    }
}
void Particle::Set(double *y , double *PosX , double *PosY)
{   if(InitStatus == 0){
    x = new double[NumVars];
    best_x = new double[NumVars];
    }
   
    this->Graph.CopyNodeVertices(PosX,PosY);
    for (int i = 0; i < NumVars; i++)
    {       
        this->x[i] = y[i];
        this->best_x[i] = y[i];
    }

}

void Particle::ObjEval()
{
    int MaxOuterIters = 1000;
    int MaxInnerIters = 200;
    double CurVal = Graph.SA(this->x[0], this->x[1], MaxOuterIters , MaxInnerIters, 0, 0);
    if (CurVal <= this->best_val)
    {
        this->best_x = this->x;
        this->best_val = CurVal;
        if (this->best_val <= global_best_val)
        {
            global_best_val = this->best_val;
            for(int i = 0 ; i < NumVars ; i++){
                global_best_x[i] = this->best_x[i];
            }
        }
    }
}

void Particle::Print(){
    printf("NumVars:%d , InitStatus:%d , Best (x):%lf , %lf, Best f(x):%lf\n",this->NumVars,this->InitStatus,this->x[0] ,this->x[1] , this->best_val);


}