/**
 * \file        ForceModel.cpp
 * \date        Dec 13, 2010
 * \version     v0.6
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
 * Implementation of the GCFM and the Gompertz model.
 *
 **/


#include "ForceModel.h"
#include "../routing/DirectionStrategy.h"
#include "../mpi/LCGrid.h"
#include "../pedestrian/Pedestrian.h"


#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#define omp_get_max_threads()  1
#endif

using namespace std;

ForceModel::ForceModel()
{
}

ForceModel::~ForceModel()
{
}



/************************************************************
 Gompertz ForceModel
 ************************************************************/

