/**
 * \file        OperationalModel.h
 * \date        Nov. 11, 2014
 * \version     v0.6
 * \author      Ulrich Kemloh
 * \copyright   <2009-2014> Forschungszentrum Jülich GmbH. All rights reserved.
 *
 * \section License
 * This file is part of JuPedSim.
 *
 * JuPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * JuPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JuPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * \section Description
 *
 * This class defines the interface for operational models, which aim is to compute the next positions of
 * the pedestrians.
 *
 **/

#ifndef OPERATIONALMODEL_H_
#define OPERATIONALMODEL_H_

#include <string>

class Building;

class OperationalModel
{
public:
     /**
      * Constructor
      */
     OperationalModel();

     /**
      * Destructor
      */
     virtual ~OperationalModel();

     /**
      * Performs whatever initialization is needed/required.
      * This function is called at the beginning the simulation once.
      * @param building, the building object
      * return the status of the initialisation
      */
     virtual bool Init (Building* building) const = 0;

     /**
      * @return a description of the model possibly with all model parameters in a nicely formatted string
      */
     virtual std::string GetDescription() const = 0;

     /**
      * Computes and update the positions/velocities /... of the pedestrians for the next time steps.
      * The pedestrians are stored in the Building object.
      *
      * @param current, the elapsed time since the begin of the simulation
      * @param deltaT, the timestep
      * @param building, the representation of the building
      */
     virtual void ComputeNextTimeStep(double current, double deltaT, Building* building) const = 0 ;
};

#endif /* OPERATIONALMODEL_H_ */
