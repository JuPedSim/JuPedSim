/**
 * \file        Hline.h
 * \date        Aug 1, 2012
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
 

#ifndef HLINE_H_
#define HLINE_H_

#include "Room.h"
#include "NavLine.h"

class SubRoom;
class Room;


class Hline: public NavLine {

private:
     int _id;
     Room* _room1;
     std::string _caption;
     SubRoom* _subRoom1;

public:
     Hline();
     virtual ~Hline();

     /**
      * Set/Get the id of the line
      */
     void SetID(int ID);

     /**
      * Set/Get the Room containing this line
      */
     void SetRoom1(Room* r);

     /**
      * Set/Get the line caption
      */
     void SetCaption(std::string s);

     /**
      * Set/Get the subroom containing this line
      */
     void SetSubRoom1(SubRoom* r);

     /**
      * Set/Get the id of the line
      */
     int GetID() const;

     /**
      * Set/Get the line caption
      */
     virtual std::string GetCaption() const;

     /**
      * Set/Get the Room containing this line
      */
     Room* GetRoom1() const;

     /**
      * Set/Get the subroom containing this line
      */
     SubRoom* GetSubRoom1() const;

     /**
      * @return true if the line is in the given subroom
      */
     virtual bool IsInSubRoom(int subroomID) const;

     /**
      * @return true if the line is in the given room
      */
     virtual bool IsInRoom(int roomID) const;

     /**
      * Debug output
      */
     virtual void WriteToErrorLog() const;

     /**
      * @return a nicely formatted string representation of the object
      */
     virtual std::string GetDescription() const;
};

#endif /* HLINE_H_ */
