#ifndef ENERGYMINIMIZERFIRE_H
#define ENERGYMINIMIZERFIRE_H

#include "functions.h"
#include "gpuarray.h"
#include "simpleModel.h"
#include "baseUpdater.h"


/*! \file energyMinimizerFIRE.h */
//!Implement energy minimization via the FIRE algorithm
/*!
This class uses the "FIRE" algorithm to perform an energy minimization.
The class is in the same framework as the simpleEquationOfMotion class, so it is called by calling
performTimestep on a Simulation object that has been given the FIRE minimizer and the configuration
to minimize. Each timestep, though, is a complete minimization (i.e. will run for the maximum number
of iterations, or until a target tolerance has been acheived, or whatever stopping condition the user
sets.
*/
class energyMinimizerFIRE : public updater
    {
    public:
        //!The basic constructor
        energyMinimizerFIRE(){initializeParameters();};
        //!The basic constructor that feeds in a target system to minimize
        energyMinimizerFIRE(shared_ptr<simpleModel> system);
        //!Sets a bunch of default parameters that do not depend on the number of degrees of freedom
        void initializeParameters();
        //!Set a bunch of default initialization parameters (if the State is available to determine the size of vectors)
        void initializeFromModel();

        //!Set the maximum number of iterations before terminating (or set to -1 to ignore)
        void setMaximumIterations(int maxIt){maxIterations = maxIt;};
        //!Set the force cutoff
        void setForceCutoff(scalar fc){forceCutoff = fc;};
        //!set the initial value of deltaT
        void setDeltaT(scalar dt){deltaT = dt;deltaTMin=dt*.01;};
        //!set the initial value of alpha and alphaStart
        void setAlphaStart(scalar as){alphaStart = as;alpha = as;};
        //!Set the maximum deltaT
        void setDeltaTMax(scalar tmax){deltaTMax = tmax;};
        //!Set the fraction by which delta increments
        void setDeltaTInc(scalar dti){deltaTInc = dti;};
        //!Set the fraction by which delta decrements
        void setDeltaTDec(scalar dtc){deltaTDec = dtc;};
        //!Set the fraction by which alpha decrements
        void setAlphaDec(scalar ad){alphaDec = ad;};
        //!Set the number of consecutive steps P must be non-negative before increasing delatT
        void setNMin(int nm){NMin = nm;};

        //!an interface to call either the CPU or GPU velocity Verlet algorithm
        void velocityVerlet();
        //!Perform a velocity Verlet step on the CPU
        void velocityVerletCPU();
        //!Perform a velocity Verlet step on the GPU
        void velocityVerletGPU();

        //!an interface to call either the CPU or GPU FIRE algorithm
        void fireStep();
        //!Perform a velocity Verlet step on the CPU
        void fireStepCPU();
        //!Perform a velocity Verlet step on the GPU
        void fireStepGPU();

        //!Minimize to either the force tolerance or the maximum number of iterations
        void minimize();
        //!The "intergate equatios of motion just calls minimize
        virtual void performUpdate(){minimize();};

        //!Return the maximum force
        scalar getMaxForce(){return forceMax;};

    protected:
        //!The number of iterations performed
        int iterations;
        //!The maximum number of iterations allowed
        int maxIterations;
        //!The cutoff value of the maximum force
        scalar forceMax;
        //!The cutoff value of the maximum force
        scalar forceCutoff;
        //!The number of points, or cells, or particles
        int N;
        //!The numer of consecutive time steps the power must be positive before increasing deltaT
        int NMin;
        //!The numer of consecutive time since the power has be negative
        int NSinceNegativePower;
        //!The internal time step size
        scalar deltaT;
        //!The minimum time step size
        scalar deltaTMin;
        //!The maximum time step size
        scalar deltaTMax;
        //!The fraction by which deltaT can get bigger
        scalar deltaTInc;
        //!The fraction by which deltaT can get smaller
        scalar deltaTDec;
        //!The internal value of the "power"
        scalar Power;
        //!The alpha parameter of the minimization routine
        scalar alpha;
        //!The initial value of the alpha parameter
        scalar alphaStart;
        //!The fraction by which alpha can decrease
        scalar alphaDec;
        //!an array of displacements
        GPUArray<dVec> displacement;

        //!Utility array for computing force.velocity
        GPUArray<scalar> forceDotVelocity;
        //!Utility array for computing force.force
        GPUArray<scalar> forceDotForce;
        //!Utility array for computing velocity.velocity
        GPUArray<scalar> velocityDotVelocity;

        //!Utility array for simple reductions
        GPUArray<scalar> sumReductionIntermediate;
        //!Utility array for simple reductions
        GPUArray<scalar> sumReductions;
    };
#endif