/**
 * \file        IODispatcher.cpp
 * \date        Nov 20, 2010
 * \version     v0.5
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


#include "IODispatcher.h"
#include "../pedestrian/Pedestrian.h"
#include "../routing/NavMesh.h"
#include "../tinyxml/tinyxml.h"
#include "../geometry/SubRoom.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace std;


IODispatcher::IODispatcher()
{
     _outputHandlers = vector<Trajectories*>();
}

IODispatcher::~IODispatcher()
{
     for (int i = 0; i < (int) _outputHandlers.size(); i++)
          delete _outputHandlers[i];
     _outputHandlers.clear();
}


void IODispatcher::AddIO(Trajectories* ioh)
{
     _outputHandlers.push_back(ioh);
}


const vector<Trajectories*>& IODispatcher::GetIOHandlers()
{
     return _outputHandlers;
}

void IODispatcher::WriteHeader(int nPeds, double fps, Building* building, int seed)
{
     for (vector<Trajectories*>::iterator it = _outputHandlers.begin(); it != _outputHandlers.end(); ++it)
     {
          (*it)->WriteHeader(nPeds, fps, building, seed);
     }
}
void IODispatcher::WriteGeometry(Building* building)
{
     for (vector<Trajectories*>::iterator it = _outputHandlers.begin(); it != _outputHandlers.end(); ++it)
     {
          (*it)->WriteGeometry(building);
     }
}
void IODispatcher::WriteFrame(int frameNr, Building* building)
{
     for (vector<Trajectories*>::iterator it = _outputHandlers.begin(); it != _outputHandlers.end(); ++it)
     {
          (*it)->WriteFrame(frameNr, building);
     }
}
void IODispatcher::WriteFooter()
{
     for (vector<Trajectories*>::iterator it = _outputHandlers.begin(); it != _outputHandlers.end(); ++it)
     {
          (*it)->WriteFooter();
     }
}


string TrajectoriesJPSV04::WritePed(Pedestrian* ped)
{
     double RAD2DEG = 180.0 / M_PI;
     char tmp[CLENGTH] = "";

     double v0 = ped->GetV0Norm();
     int color=1; // red= very low velocity

     if (v0 != 0.0) {
          double v = ped->GetV().Norm();
          color = (int) (v / v0 * 255);
     }

     if(ped->GetSpotlight()==false) color=-1;

     double a = ped->GetLargerAxis();
     double b = ped->GetSmallerAxis();
     double phi = atan2(ped->GetEllipse().GetSinPhi(), ped->GetEllipse().GetCosPhi());
     sprintf(tmp, "<agent ID=\"%d\"\t"
               "xPos=\"%.2f\"\tyPos=\"%.2f\"\t"
               "zPos=\"%.2f\"\t"
               "radiusA=\"%.2f\"\tradiusB=\"%.2f\"\t"
               "ellipseOrientation=\"%.2f\" ellipseColor=\"%d\"/>\n",
               ped->GetID(), (ped->GetPos().GetX()) * FAKTOR,
               (ped->GetPos().GetY()) * FAKTOR,(ped->GetElevation()+0.3) * FAKTOR ,a * FAKTOR, b * FAKTOR,
               phi * RAD2DEG, color);
     return tmp;
}

void TrajectoriesJPSV04::WriteHeader(int nPeds, double fps, Building* building, int seed      )
{

     nPeds = building->GetNumberOfPedestrians();
     string tmp;
     tmp =
               "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" "<trajectories>\n";
     char agents[CLENGTH] = "";
     sprintf(agents, "\t<header version = \"%s\">\n", JPS_VERSION);
     tmp.append(agents);
     sprintf(agents, "\t\t<agents>%d</agents>\n", nPeds);
     tmp.append(agents);
     sprintf(agents, "\t\t<seed>%d</seed>\n", seed);
     tmp.append(agents);
     sprintf(agents, "\t\t<frameRate>%0.2f</frameRate>\n", fps);
     tmp.append(agents);
     tmp.append("\t</header>\n");
     Write(tmp);

}

void TrajectoriesJPSV04::WriteGeometry(Building* building)
{
     // just put a link to the geometry file
     string embed_geometry;
     embed_geometry.append("\t<geometry>\n");
     char file_location[CLENGTH] = "";
     sprintf(file_location, "\t<file location= \"%s\"/>\n", building->GetGeometryFilename().c_str());
     embed_geometry.append(file_location);
     embed_geometry.append("\t</geometry>\n");
     //Write(embed_geometry);
     //return;
     //
     string geometry;
     geometry.append("\t<geometry>\n");

     bool plotHlines = true;
     bool plotCrossings = true;
     bool plotTransitions = true;
     bool plotPlayingField=false;
     vector<string> rooms_to_plot;

     //Promenade
     //rooms_to_plot.push_back("outside");
     //rooms_to_plot.push_back("010");

     // first the rooms
     //to avoid writing navigation line twice
     vector<int> navLineWritten;

     for (int i = 0; i < building->GetNumberOfRooms(); i++) {
          Room* r = building->GetRoom(i);
          string caption = r->GetCaption(); //if(r->GetID()!=1) continue;
          if (rooms_to_plot.empty() == false)
               if (IsElementInVector(rooms_to_plot, caption) == false)
                    continue;

          for (int k = 0; k < r->GetNumberOfSubRooms(); k++) {
               SubRoom* s = r->GetSubRoom(k); //if(s->GetSubRoomID()!=0) continue;
               geometry.append(s->WriteSubRoom());

               // the hlines
               if (plotHlines) {
                    const vector<Hline*>& hlines = s->GetAllHlines();
                    for (unsigned int i = 0; i < hlines.size(); i++) {
                         Hline* hline = hlines[i];
                         int uid1 = hline->GetUniqueID();
                         if (!IsElementInVector(navLineWritten, uid1)) {
                              navLineWritten.push_back(uid1);
                              if (rooms_to_plot.empty()
                                        || IsElementInVector(rooms_to_plot, caption)) {
                                   geometry.append(hline->WriteElement());
                              }
                         }
                    }

                    // the crossings
                    if (plotCrossings) {
                         const vector<Crossing*>& crossings = s->GetAllCrossings();
                         for (unsigned int i = 0; i < crossings.size(); i++) {
                              Crossing* crossing = crossings[i];
                              int uid1 = crossing->GetUniqueID();
                              if (!IsElementInVector(navLineWritten, uid1)) {
                                   navLineWritten.push_back(uid1);
                                   if (rooms_to_plot.empty()
                                             || IsElementInVector(rooms_to_plot,
                                                       caption)) {
                                        geometry.append(crossing->WriteElement());
                                   }
                              }
                         }
                    }

                    // the transitions
                    if (plotTransitions) {
                         const vector<Transition*>& transitions =
                                   s->GetAllTransitions();
                         for (unsigned int i = 0; i < transitions.size(); i++) {
                              Transition* transition = transitions[i];
                              int uid1 = transition->GetUniqueID();
                              if (!IsElementInVector(navLineWritten, uid1)) {
                                   navLineWritten.push_back(uid1);

                                   if (rooms_to_plot.empty()) {
                                        geometry.append(transition->WriteElement());

                                   } else {

                                        Room* room1 = transition->GetRoom1();
                                        Room* room2 = transition->GetRoom2();
                                        string caption1 = room1->GetCaption();
                                        if (room2) {
                                             string caption2 = room2->GetCaption();
                                             if (IsElementInVector(rooms_to_plot,
                                                       caption1)
                                                       || IsElementInVector(rooms_to_plot,
                                                                 caption2)) {
                                                  geometry.append(transition->WriteElement());
                                             }

                                        } else {
                                             if (IsElementInVector(rooms_to_plot,
                                                       caption1)) {
                                                  geometry.append(transition->WriteElement());
                                             }
                                        }

                                   }
                              }
                         }
                    }
               }
          }
     }

     //eventually write any goal
     for (map<int, Goal*>::const_iterator itr = building->GetAllGoals().begin();
               itr != building->GetAllGoals().end(); ++itr) {
          geometry.append(itr->second->Write());
     }

     if(plotPlayingField) {
          //add the playing area
          double width=3282;
          double length=5668;
          char tmp[100];
          geometry.append("\t\t<wall>\n");
          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",-length,width);
          geometry.append(tmp);

          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",-length,-width);
          geometry.append(tmp);
          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",length,-width);
          geometry.append(tmp);
          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",length,width);
          geometry.append(tmp);
          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",-length,width);
          geometry.append(tmp);
          geometry.append("\t\t</wall>\n");

          geometry.append("\t\t<wall>\n");
          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",0.0,width);
          geometry.append(tmp);
          sprintf(tmp, "\t\t\t<point xPos=\"%.2f\" yPos=\"%.2f\"/>\n",0.0,-width);
          geometry.append(tmp);
          geometry.append("\t\t</wall>\n");
     }
     geometry.append("\t</geometry>\n");
     Write(geometry);
}

void TrajectoriesJPSV04::WriteFrame(int frameNr, Building* building)
{
     string data;
     char tmp[CLENGTH] = "";
     vector<string> rooms_to_plot;

     if( building->GetAllPedestrians().size() == 0)
          return;
     sprintf(tmp, "<frame ID=\"%d\">\n", frameNr);
     data.append(tmp);

     for (int roomindex = 0; roomindex < building->GetNumberOfRooms(); roomindex++) {
          Room* r = building->GetRoom(roomindex);
          string caption = r->GetCaption();

          if ((rooms_to_plot.empty() == false)
                    && (IsElementInVector(rooms_to_plot, caption) == false)) {
               continue;
          }

          for (int k = 0; k < r->GetNumberOfSubRooms(); k++) {
               SubRoom* s = r->GetSubRoom(k);
               for (int i = 0; i < s->GetNumberOfPedestrians(); ++i) {
                    Pedestrian* ped = s->GetPedestrian(i);
                    data.append(WritePed(ped));
               }
          }
     }
     data.append("</frame>\n");
     Write(data);
}

void TrajectoriesJPSV04::WriteFooter()
{
     Write("</trajectories>\n");
}


/**
 * FLAT format implementation
 */

TrajectoriesFLAT::TrajectoriesFLAT() : Trajectories()
{
}

void TrajectoriesFLAT::WriteHeader(int nPeds, double fps, Building* building, int seed)
{
     char tmp[CLENGTH] = "";
     Write("#description: my super simulation");
     sprintf(tmp, "#framerate: %0.2f",fps);
     Write(tmp);
     sprintf(tmp,"#geometry: %s",building->GetGeometryFilename().c_str());
     Write(tmp);
     Write("#ID: the agent ID");
     Write("#FR: the current frame");
     Write("#X,Y,Z: the agents coordinates (in metres)");
     Write("\n");
     Write("#ID\tFR\tX\tY\tZ");
}

void TrajectoriesFLAT::WriteGeometry(Building* building)
{

}

void TrajectoriesFLAT::WriteFrame(int frameNr, Building* building)
{
     char tmp[CLENGTH] = "";

     for (int roomindex = 0; roomindex < building->GetNumberOfRooms(); roomindex++) {
          Room* r = building->GetRoom(roomindex);
          for (int k = 0; k < r->GetNumberOfSubRooms(); k++) {
               SubRoom* s = r->GetSubRoom(k);
               for (int i = 0; i < s->GetNumberOfPedestrians(); ++i) {
                    Pedestrian* ped = s->GetPedestrian(i);
                    double x = ped->GetPos().GetX();
                    double y = ped->GetPos().GetY();
                    double z = ped->GetElevation();
                    sprintf(tmp, "%d\t%d\t%0.2f\t%0.2f\t%0.2f", ped->GetID(), frameNr, x,
                              y,z);
                    Write(tmp);
               }
          }
     }
}

void TrajectoriesFLAT::WriteFooter()
{

}


/**
 *  VTK Implementation of the geometry and trajectories
 */


TrajectoriesVTK::TrajectoriesVTK()
{
}

void TrajectoriesVTK::WriteHeader(int nPeds, double fps, Building* building, int seed)
{
     Write("# vtk DataFile Version 4.0");
     Write(building->GetCaption());
     Write("ASCII");
     Write("");
}

void TrajectoriesVTK::WriteGeometry(Building* building)
{
     stringstream tmp;

     NavMesh* nv= new NavMesh(building);
     nv->BuildNavMesh();
     //nv->WriteToFile("../pedunc/examples/stadium/arena.nav");
     Write("DATASET UNSTRUCTURED_GRID");

     //writing the vertices
     const vector<NavMesh::JVertex*>& vertices= nv->GetVertices() ;
     tmp<<"POINTS "<<vertices.size()<<" FLOAT"<<endl;
     for (unsigned int v=0; v<vertices.size(); v++) {
          tmp<<vertices[v]->pPos.GetX()<<" " <<vertices[v]->pPos.GetY() <<" 0.0"<<endl;
     }
     Write(tmp.str());
     tmp.str(std::string());

     //writing the cells data
     const vector<NavMesh::JNode*>& cells= nv->GetNodes();
     int nComponents=cells.size();
     stringstream tmp1;
     for (unsigned int n=0; n<cells.size(); n++) {
          int hSize=cells[n]->pHull.size();

          tmp1<<hSize<<"";
          for(unsigned int i=0; i<cells[n]->pHull.size(); i++) {
               tmp1<<" "<<cells[n]->pHull[i].id;
          }
          tmp1<<endl;
          nComponents+= hSize;
     }
     tmp<<"CELLS "<<cells.size()<<" "<<nComponents<<endl;
     tmp<<tmp1.str();
     Write(tmp.str());
     tmp.str(std::string());

     // writing the cell type
     tmp<<"CELL_TYPES "<<cells.size()<<endl;
     for (unsigned int n=0; n<cells.size(); n++) {
          tmp<<"9"<<endl;
     }

     Write(tmp.str());
     delete nv;
}

void TrajectoriesVTK::WriteFrame(int frameNr, Building* building)
{
}

void TrajectoriesVTK::WriteFooter()
{
}


void TrajectoriesJPSV06::WriteHeader(int nPeds, double fps, Building* building, int seed)
{
     nPeds = building->GetNumberOfPedestrians();
     string tmp;
     tmp =
               "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" "<trajectories>\n";
     char agents[CLENGTH] = "";
     sprintf(agents, "\t<header version = \"0.6\">\n");
     tmp.append(agents);
     sprintf(agents, "\t\t<agents>%d</agents>\n", nPeds);
     tmp.append(agents);
     sprintf(agents, "\t\t<seed>%d</seed>\n", seed);
     tmp.append(agents);
     sprintf(agents, "\t\t<frameRate>%0.2f</frameRate>\n", fps);
     tmp.append(agents);
     tmp.append("\t\t<!-- Frame count HACK\n");
     tmp.append("replace me\n");
     tmp.append("\t\tFrame count HACK -->\n");
     //tmp.append("<frameCount>xxxxxxx</frameCount>\n");
     tmp.append("\t</header>\n");
     Write(tmp);

}

void TrajectoriesJPSV06::WriteGeometry(Building* building)
{
     // just put a link to the geometry file
//     string embed_geometry;
//     embed_geometry.append("\t<geometry>\n");
//     char file_location[CLENGTH] = "";
//     sprintf(file_location, "\t<file location= \"%s\"/>\n", building->GetGeometryFilename().c_str());
//     embed_geometry.append(file_location);
//     //embed_geometry.append("\t</geometry>\n");
//
//     const map<int, Hline*>& hlines=building->GetAllHlines();
//     if(hlines.size()>0){
//          //embed_geometry.append("\t<geometry>\n");
//          for (std::map<int, Hline*>::const_iterator it=hlines.begin(); it!=hlines.end(); ++it)
//          {
//               embed_geometry.append(it->second->WriteElement());
//          }
//          //embed_geometry.append("\t</geometry>\n");
//     }
//     embed_geometry.append("\t</geometry>\n");
//     Write(embed_geometry);

     //set the content of the file
     string fileName=building->GetProjectRootDir()+"/"+building->GetGeometryFilename().c_str();
     string embed_geometry;
     string tmp; //lines to drop
     std::ifstream t(fileName.c_str());
     std::getline(t,tmp); //drop the first line <?xml version="1.0" encoding="UTF-8"?>
     std::stringstream buffer;
     buffer << t.rdbuf();
     embed_geometry=buffer.str();
     Write(embed_geometry);
//
//
//     //collecting the hlines
//     std::stringstream hlines_buffer;
//     // add the header
//     hlines_buffer<<" <routing version=\"0.5\" "
//               <<"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
//               <<"xsi:noNamespaceSchemaLocation=\"http://134.94.2.137/jps_routing.xsd\" >"<<endl
//               <<"<Hlines> "<<endl;
//
//     const map<int, Hline*>& hlines=building->GetAllHlines();
//     for (std::map<int, Hline*>::const_iterator it=hlines.begin(); it!=hlines.end(); ++it)
//     {
//          Hline* hl=it->second;
//          hlines_buffer <<"\t<Hline id=\""<< hl->GetID()<<"\" room_id=\""<<hl->GetRoom1()->GetID()
//                                        <<"\" subroom_id=\""<< hl->GetSubRoom1()->GetSubRoomID()<<"\">"<<endl;
//          hlines_buffer <<"\t\t<vertex px=\""<< hl->GetPoint1()._x<<"\" py=\""<< hl->GetPoint1()._y<<"\" />"<<endl;
//          hlines_buffer <<"\t\t<vertex px=\""<< hl->GetPoint2()._x<<"\" py=\""<< hl->GetPoint2()._y<<"\" />"<<endl;
//          hlines_buffer <<"\t</Hline>"<<endl;
//     }
//     hlines_buffer<<"</Hlines> "<<endl;
//     hlines_buffer<<"</routing> "<<endl;
//
//     string hline_string=hlines_buffer.str();
//     string to_replace="</geometry>";
//     hline_string.append(to_replace);
//
//     size_t start_pos = embed_geometry.find(to_replace);
//     if(start_pos == std::string::npos)
//     {
//          Log->Write("WARNING:\t missing %s tag while writing the geometry in the trajectory file.",to_replace.c_str());
//     }
//
//     embed_geometry.replace(start_pos, to_replace.length(), hline_string);
//     Write(embed_geometry);

//     Write("\t<AttributeDescription>");
//     Write("\t\t<property tag=\"x\" description=\"xPosition\"/>");
//     Write("\t\t<property tag=\"y\" description=\"yPosition\"/>");
//     Write("\t\t<property tag=\"z\" description=\"zPosition\"/>");
//     Write("\t\t<property tag=\"rA\" description=\"radiusA\"/>");
//     Write("\t\t<property tag=\"rB\" description=\"radiusB\"/>");
//     Write("\t\t<property tag=\"eC\" description=\"ellipseColor\"/>");
//     Write("\t\t<property tag=\"eO\" description=\"ellipseOrientation\"/>");
//     Write("\t</AttributeDescription>\n");
}

void TrajectoriesJPSV06::WriteFrame(int frameNr, Building* building)
{
     string data;
     char tmp[CLENGTH] = "";
     double RAD2DEG = 180.0 / M_PI;
     vector<string> rooms_to_plot;

     sprintf(tmp, "<frame ID=\"%d\">\n", frameNr);
     data.append(tmp);

     for (int roomindex = 0; roomindex < building->GetNumberOfRooms(); roomindex++) {
          Room* r = building->GetRoom(roomindex);
          string caption = r->GetCaption();

          if ((rooms_to_plot.empty() == false)
                    && (IsElementInVector(rooms_to_plot, caption) == false)) {
               continue;
          }

          for (int k = 0; k < r->GetNumberOfSubRooms(); k++) {
               SubRoom* s = r->GetSubRoom(k);
               for (int i = 0; i < s->GetNumberOfPedestrians(); ++i)
               {
                    char tmp[CLENGTH] = "";
                    Pedestrian* ped = s->GetPedestrian(i);
                    double v0 = ped->GetV0Norm();
                    int color=1; // red= very low velocity

                    if (v0 != 0.0) {
                         double v = ped->GetV().Norm();
                         color = (int) (v / v0 * 255);
                    }
                    if(ped->GetSpotlight()==false) color=-1;


                    double a = ped->GetLargerAxis();
                    double b = ped->GetSmallerAxis();
                    double phi = atan2(ped->GetEllipse().GetSinPhi(), ped->GetEllipse().GetCosPhi());
                    sprintf(tmp, "<agent ID=\"%d\"\t"
                              "x=\"%.2f\"\ty=\"%.2f\"\t"
                              "z=\"%.2f\"\t"
                              "rA=\"%.2f\"\trB=\"%.2f\"\t"
                              "eO=\"%.2f\" eC=\"%d\"/>\n",
                              ped->GetID(), (ped->GetPos().GetX()) * FAKTOR,
                              (ped->GetPos().GetY()) * FAKTOR,(ped->GetElevation()+0.3) * FAKTOR ,a * FAKTOR, b * FAKTOR,
                              phi * RAD2DEG, color);
                    data.append(tmp);
               }
          }
     }
     data.append("</frame>\n");
     Write(data);
}

void TrajectoriesJPSV06::WriteFooter()
{
     Write("</trajectories>\n");
}


void TrajectoriesXML_MESH::WriteGeometry(Building* building)
{
     //Navigation mesh implementation
     NavMesh* nv= new NavMesh(building);
     nv->BuildNavMesh();
     string geometry;
     nv->WriteToString(geometry);
     Write("<geometry>");
     Write(geometry);
     Write("</geometry>");
     nv->WriteToFile(building->GetProjectFilename()+".full.nav");
     delete nv;
}


void TrajectoriesJPSV05::WriteHeader(int nPeds, double fps, Building* building, int seed)
{
     nPeds = building->GetNumberOfPedestrians();
     string tmp;
     tmp = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n" "<trajectories>\n";
     char agents[CLENGTH] = "";
     sprintf(agents, "\t<header version = \"0.5.1\">\n");
     tmp.append(agents);
     sprintf(agents, "\t\t<agents>%d</agents>\n", nPeds);
     tmp.append(agents);
     sprintf(agents, "\t\t<seed>%d</seed>\n", seed);
     tmp.append(agents);
     sprintf(agents, "\t\t<frameRate>%0.2f</frameRate>\n", fps);
     tmp.append(agents);
     //tmp.append("\t\t<!-- Frame count HACK\n");
     //tmp.append("replace me\n");
     //tmp.append("\t\tFrame count HACK -->\n");
     //tmp.append("<frameCount>xxxxxxx</frameCount>\n");
     tmp.append("\t</header>\n");
     _outputHandler->Write(tmp);
}

void TrajectoriesJPSV05::WriteGeometry(Building* building)
{
     // just put a link to the geometry file
     string embed_geometry;
     embed_geometry.append("\t<geometry>\n");
     char file_location[CLENGTH] = "";
     sprintf(file_location, "\t<file location= \"%s\"/>\n", building->GetGeometryFilename().c_str());
     embed_geometry.append(file_location);
     //embed_geometry.append("\t</geometry>\n");

     const map<int, Hline*>& hlines=building->GetAllHlines();
     if(hlines.size()>0){
          //embed_geometry.append("\t<geometry>\n");
          for (std::map<int, Hline*>::const_iterator it=hlines.begin(); it!=hlines.end(); ++it)
          {
               embed_geometry.append(it->second->WriteElement());
          }
          //embed_geometry.append("\t</geometry>\n");
     }
     embed_geometry.append("\t</geometry>\n");
     _outputHandler->Write(embed_geometry);


     _outputHandler->Write("\t<AttributeDescription>");
     _outputHandler->Write("\t\t<property tag=\"x\" description=\"xPosition\"/>");
     _outputHandler->Write("\t\t<property tag=\"y\" description=\"yPosition\"/>");
     _outputHandler->Write("\t\t<property tag=\"z\" description=\"zPosition\"/>");
     _outputHandler->Write("\t\t<property tag=\"rA\" description=\"radiusA\"/>");
     _outputHandler->Write("\t\t<property tag=\"rB\" description=\"radiusB\"/>");
     _outputHandler->Write("\t\t<property tag=\"eC\" description=\"ellipseColor\"/>");
     _outputHandler->Write("\t\t<property tag=\"eO\" description=\"ellipseOrientation\"/>");
     _outputHandler->Write("\t</AttributeDescription>\n");
}

void TrajectoriesJPSV05::WriteFrame(int frameNr, Building* building)
{
     string data;
     char tmp[CLENGTH] = "";
     double RAD2DEG = 180.0 / M_PI;

     sprintf(tmp, "<frame ID=\"%d\">\n", frameNr);
     data.append(tmp);

     for (int roomindex = 0; roomindex < building->GetNumberOfRooms(); roomindex++) {
          Room* r = building->GetRoom(roomindex);
          string caption = r->GetCaption();


          for (int k = 0; k < r->GetNumberOfSubRooms(); k++) {
               SubRoom* s = r->GetSubRoom(k);
               for (int i = 0; i < s->GetNumberOfPedestrians(); ++i)
               {
                    char tmp[CLENGTH] = "";
                    Pedestrian* ped = s->GetPedestrian(i);
                    double v0 = ped->GetV0Norm();
                    int color=1; // red= very low velocity

                    if (v0 != 0.0) {
                         double v = ped->GetV().Norm();
                         color = (int) (v / v0 * 255);
                    }
                    if(ped->GetSpotlight()==false) color=-1;


                    double a = ped->GetLargerAxis();
                    double b = ped->GetSmallerAxis();
                    double phi = atan2(ped->GetEllipse().GetSinPhi(), ped->GetEllipse().GetCosPhi());
                    sprintf(tmp, "<agent ID=\"%d\"\t"
                              "x=\"%.2f\"\ty=\"%.2f\"\t"
                              "z=\"%.2f\"\t"
                              "rA=\"%.2f\"\trB=\"%.2f\"\t"
                              "eO=\"%.2f\" eC=\"%d\"/>\n",
                              ped->GetID(), (ped->GetPos().GetX()) * FAKTOR,
                              (ped->GetPos().GetY()) * FAKTOR,(ped->GetElevation()+0.3) * FAKTOR ,a * FAKTOR, b * FAKTOR,
                              phi * RAD2DEG, color);
                    data.append(tmp);
               }
          }
     }
     data.append("</frame>\n");
     _outputHandler->Write(data);
}

void TrajectoriesJPSV05::WriteFooter()
{
     _outputHandler->Write("</trajectories>\n");
}

