/**
 * \file        RoutingEngine.h
 * \date        Jan 10, 2013
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
 *
 *
 **/


#ifndef ROUTINGENGINE_H_
#define ROUTINGENGINE_H_

#include "Router.h"

#include <string>
#include <vector>

class Pedestrian;


class RoutingEngine {
public:
     /**
      * Constructor
      */
     RoutingEngine();

     /**
      * Destructor
      */
     virtual ~RoutingEngine();

     /**
      * Add a final destination in the system.
      * The destinations are segments (@see Transitions @see Crossings)
      * @param id
      */
     void AddFinalDestinationID(int id);

     /**
      * Add a new trip to the system. Individual pedestrian can be assigned a particular trip.
      * @param trip
      */
     void AddTrip(std::vector<std::string> trip);

     /**
      * Return a trip/route with the particular id
      * @param id, the trip id
      * @return the corresponding trip
      */
     const std::vector<std::string>& GetTrip(int id) const;

     /**
      * @return all available routers
      *
      */
     const std::vector<Router*> GetAvailableRouters() const;

     /**
      * Find the next destination using the appropriate router from
      * the collection for the pedestrian ped.
      */
     void FindRoute(Pedestrian* ped);

     /**
      * Add a new router to the routing system
      *
      */
     void AddRouter(Router* router);

     /**
      * Return the router with the specified  strategy
      */
     Router* GetRouter(RoutingStrategy strategy) const;

     /**
      * Return the router with the specified  id
      * TODO: Remove this method
      * Should prefer etRouter(RoutingStrategy strategy)
      */
     Router* GetRouter(int id) const;

     /**
      * Initialize all routers with the current building object
      * @param building
      * @return the status of the initialisation
      */
     bool Init(Building* building);

private:
     /// collections of all routers used
     std::vector<Router*> _routersCollection;
     /// collection of all trips/routes
     std::vector<std::vector<std::string> >_tripsCollection;
};

#endif /* ROUTINGENGINE_H_ */
