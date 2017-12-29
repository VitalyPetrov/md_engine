/*
* Copyright (c) 2013, Daniel Guterding <guterding@itp.uni-frankfurt.de>
*
* This file is part of vsmd.
*
* vsmd is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* vsmd is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with vsmd. If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <omp.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>

#include "init.hpp"
#include "verletNVE.hpp"
#include "verletNVTscaling.hpp"
#include "lf.hpp"
#include "lfNVTberendsen.hpp"
#include "lfNPTberendsen.hpp"
using namespace std;

#pragma omp parallel;

int main(int argc, char* argv[]){
  
  cout << "Molecular Dynamics Simulator with OpenMP parallelization" << endl;
  cout << "----------------------------------------" << endl << endl;
  
  int numThreads;
  uint i=0;
  fptype start,end;

  numThreads = atoi(argv[1]);
  omp_set_num_threads(numThreads);
  GlobalSettings settings;
  
  if(argc != 13) 
  {
		cout << "Using precompiled settings." << endl;
		settings.processid = 1;
		settings.mode = 1; // Velocity Verlet NVE
		settings.potential = 1; // Lennard-Jones potential
		settings.gridsize = 7;
		settings.n = 4 * pow(settings.gridsize, 3); // Number of particles
		settings.h << 25,  0,  0,
					   0, 25,  0,	// Simulation box dimensions
					   0,  0, 25;
		settings.rcutoff = 2.1;
		settings.deltat = 3E-4; // Timestep
		settings.t_thermo = 100000 * settings.deltat; // t_damp for Berendsen thermostat
		settings.t_baro = 150*settings.deltat;
		settings.T_des = 10.;
		settings.P_des << 35,  0,  0,
						   0, 35,  0,
						   0,  0, 35;
		settings.nsteps = 30000; // Number of steps
  }

  else {
	cout << "Using command line settings." << endl;
	settings.processid = atoi(argv[1]);
	settings.mode = atoi(argv[2]);
	settings.potential = atoi(argv[3]);
	settings.gridsize = atoi(argv[4]);
	settings.n = 4*pow(settings.gridsize,3);
	settings.nsteps = atoi(argv[5]);
	settings.h << atof(argv[6]),  0,  0,

				   0, atof(argv[6]),  0,
				   0,  0, atof(argv[6]);
	settings.rcutoff = atof(argv[7]);
	settings.deltat = atof(argv[8]);
	settings.t_thermo = atof(argv[9])*settings.deltat;
	settings.t_baro = atof(argv[10])*settings.deltat;
	settings.T_des = atof(argv[11]);
	settings.P_des << atof(argv[12]),  0,  0,
					   0, atof(argv[12]),  0,
					   0,  0, atof(argv[12]);
  }
  

  cout << "Simulation mode: ";
  switch(settings.mode){
	case 1: 
	  cout << "Velocity Verlet NVE";
	  break;
	case 2:
	  cout << "Velocity Verlet NVT with Scaling";
	  break;
	case 3:
	  cout << "Leapfrog NVT with Berendsen Temperature Coupling";
	  break;
	case 4:
	  cout << "Leapfrog NPT with Berendsen Temperature and Pressure Coupling";
	  break;
	case 5:
	  cout << "Energy per Particle in NPT Berendsen Algorithm";
	  break;
  }
  cout << endl;
 
  switch(settings.potential){
	case 1: 
	  cout << "Using Lennard-Jones 12-6 Potential." << endl;
	  break;
	case 2:
	  cout << "Using Buckingham Potential." << endl;
	  break;
  }
  

  cout << "Number of Particles: " << settings.n << endl;
  
  cout << "Number of Timesteps: " << settings.nsteps << endl;
  
  cout << "Initial Box Dimensions: " << endl << settings.h << endl;
  
  cout << "Cutoff radius: " << settings.rcutoff << endl;
  
  cout << "Timestep width: " << settings.deltat << endl;
  
  cout << "Thermostat coupling time: " << settings.t_thermo/settings.deltat << endl;
  
  if(4 == settings.mode || 5 == settings.mode){
	cout << "Barostat coupling time: " << settings.t_baro/settings.deltat << endl; 
  }
  
  cout << "Desired Temperature: " << settings.T_des << endl;
  
  if(4 == settings.mode || 5 == settings.mode){
	cout << "Desired Pressure: " << endl << settings.P_des << endl; 
  }
  
  
  double TotalTime = 0., currentPotentialEnergy = 0., currentKineticEnergy = 0., currentTotalEnrgy = 0.;
  Results res(settings);
  initialize(settings,res);
  cout << "Simulation started." << endl;
  switch(settings.mode){
	case 1: //velocity verlet NVE integrator
	  for(i = 0; i < settings.nsteps; i++)
	  {
			start = omp_get_wtime();
			verletNVE(settings, res);
			end = omp_get_wtime();
  			TotalTime += end - start;
  			currentKineticEnergy = res.get_kinetic_energy();
  			currentPotentialEnergy = res.get_potential_energy();
  			currentTotalEnrgy = res.get_total_energy();
  			res.print_snapshot(res.get_positions(), res.get_velocities(), settings.n, i);
			//cout << "Time elapsed for step " << i << " in milliseconds: " << (int)((end-start)*1000) << endl;
  			cout << i << "\t" << "Kinetic = " << currentKineticEnergy << "\t" << "Potential =  " << currentPotentialEnergy << "\t" << "Total = " << currentTotalEnrgy << endl;
	  }
	  break;

	case 2: //velocity verlet NVT integrator with simple temperature scaling
	  for(i=0;i<settings.nsteps;i++) {
	  				start = omp_get_wtime();
			verletNVTscaling(settings, res);
			end = omp_get_wtime();
  			TotalTime += end - start;
  			currentKineticEnergy = res.get_kinetic_energy();
  			currentPotentialEnergy = res.get_potential_energy();
  			currentTotalEnrgy = res.get_total_energy();
  			res.print_snapshot(res.get_positions(), res.get_velocities(), settings.n, i);
			//cout << "Time elapsed for step " << i << " in milliseconds: " << (int)((end-start)*1000) << endl;
  			cout << i << "\t" << "Kinetic = " << currentKineticEnergy << "\t" << "Potential =  " << currentPotentialEnergy << "\t" << "Total = " << currentTotalEnrgy << endl;
	  }
	  break;
	case 3: //leapfrog NVT integrator with berendsen temperature coupling
	  leapfrog_start(settings, res);
	  for(i=0;i<settings.nsteps;i++){
	start = omp_get_wtime();
	leapfrogNVTberendsen(settings, res);
	end = omp_get_wtime();
	cout << "Time elapsed for step " << i << " in milliseconds: " << (int)((end-start)*1000) << endl;
	  }
	  break;
	case 4: //leapfrog NPT integrator with berendsen temperature and pressure coupling
	  leapfrog_start(settings, res);
	  for(i=0;i<settings.nsteps;i++){
		start = omp_get_wtime();
	leapfrogNPTberendsen(settings, res);
	end = omp_get_wtime();
	cout << "Time elapsed for step " << i << " in milliseconds: " << (int)((end-start)*1000) << endl;
	  }
	  break;
	case 5: //averages total energy per particle in berendsen NPT method, first 1E4 time steps are discarded for averaging
	  fptype energy_per_particle = 0;
	  leapfrog_start(settings, res);
	  for(i=0;i<settings.nsteps;i++){
	leapfrogNPTberendsen(settings, res);
	if(i>1E4){
	  energy_per_particle += res.get_total_energy();
	}
	  }
	  energy_per_particle /= (fptype)settings.n * (settings.nsteps-1E4);
	  
	  boost::filesystem::path datadir("data");
	  if(!(boost::filesystem::exists(datadir))){
	boost::filesystem::create_directory(datadir);
	  }
	  
	  string fstr;
	  fstr = "data/P=" + boost::lexical_cast<string>(settings.P_des(0,0)) + "." + boost::lexical_cast<string>(settings.processid) + ".dat";
	  boost::filesystem::path filename(fstr);
	  boost::filesystem::path filename_bak(fstr + ".bak");
	  if(boost::filesystem::exists(filename)){
	if(boost::filesystem::exists(filename_bak)){
	  boost::filesystem::remove(filename_bak);
	}
	boost::filesystem::rename(filename, filename_bak);
	  }
	  boost::filesystem::ofstream file(filename);
	  file << setprecision(8) << settings.T_des << ";" << settings.P_des(0,0) << ";" << energy_per_particle << endl;
	  break;
  }
  
  cout << "Total wall time = " << TotalTime << " seconds" << endl;
  cout << "Simulation complete." << endl;

  return 0;
}
