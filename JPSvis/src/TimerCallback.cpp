/**
 * @file TimerCallback.cpp
 * @author   Ulrich Kemloh <kemlohulrich@gmail.com>
 * @version 0.1
 * Copyright (C) <2009-2010>
 *
 * @section LICENSE
 * This file is part of OpenPedSim.
 *
 * OpenPedSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * OpenPedSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenPedSim. If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 * This class is called by the timer and update all elements on the screen
 *
 * \brief Callback class for the visualisation thread. Triggered by the timer
 *
 *
 *
 *  Created on: 11.05.2009
 *
 */


#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>


#ifdef WIN32
#include <vtkAVIWriter.h>
#include <windows.h>
#define TRAVISTO_FFMPEG
#endif


#ifdef __linux__
#ifdef __vtkFFMPEGWriter_h
#include <vtkFFMPEGWriter.h>
#define TRAVISTO_FFMPEG
#endif
#endif

#include <QObject>
#include <QString>
#include <QTime>
#include <QDir>
#include <qwaitcondition.h>

#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkWindowToImageFilter.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPNGWriter.h>
#include <vtkPostScriptWriter.h>
#include <vtkActor2DCollection.h>
#include <vtkTextActor.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkSphereSource.h>
#include <vtkMapper2D.h>
#include <vtkMapper.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkLabeledDataMapper.h>




#include "geometry/FacilityGeometry.h"
#include "geometry/Point.h"

#include "Pedestrian.h"
#include "Frame.h"
#include "FrameElement.h"
#include "TrajectoryPoint.h"
#include "SyncData.h"
#include "SystemSettings.h"
#include "TrailPlotter.h"
#include "geometry/PointPlotter.h"
#include "TimerCallback.h"

#define VTK_CREATE(type, name) \
    vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

using namespace std;

TimerCallback* TimerCallback::New()
{
    TimerCallback *cb = new TimerCallback;
    cb->RenderTimerId = 0;
    cb->windowToImageFilter=NULL;
    cb->runningTime=NULL;
    return cb;
}


void TimerCallback::Execute(vtkObject *caller, unsigned long eventId,
                            void *callData){
    if (vtkCommand::TimerEvent == eventId)
    {
        int  frameNumber=0;
        int nPeds=0;
        static bool isRecording =false;
        int tid = * static_cast<int *>(callData);

        if (tid == this->RenderTimerId)
        {
            //dont update anything if the system is actually paused
            //if(extern_is_pause) return;

            vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
            vtkRenderWindow  *renderWindow = iren->GetRenderWindow();
            vtkRenderer *renderer =renderWindow->GetRenderers()->GetFirstRenderer();

            if (iren && renderWindow && renderer)
            {

                //first pedestrian group
                if(extern_first_dataset_loaded) {
                    Frame * frame=NULL;

                    // return the same frame if the system is paused
                    // in fact you could just return, but in this case no update will be made
                    // e.g showing captions/trails...

                    if(extern_is_pause)
                        frame=extern_trajectories_firstSet.getFrame(extern_trajectories_firstSet.getFrameCursor());
                    else
                        frame = extern_trajectories_firstSet.getNextFrame();

                    if(frame==NULL)
                    {

                    }
                    else
                    {
                        frameNumber=extern_trajectories_firstSet.getFrameCursor();
                        nPeds= frame->getSize();

                        if(SystemSettings::get2D()==true)
                        {
                            vtkPolyData* pData=frame->GetPolyData2D();
#if VTK_MAJOR_VERSION <= 5
                            extern_glyphs_pedestrians->SetInput(pData);
                            ((vtkLabeledDataMapper*)extern_pedestrians_labels->GetMapper())->SetInput(pData);
#else
                            extern_glyphs_pedestrians->SetInputData(pData);
                            extern_pedestrians_labels->GetMapper()->SetInputDataObject(pData);
#endif
                            extern_glyphs_pedestrians->Update();
                        }
                        else
                        {
                            vtkPolyData* pData=frame->GetPolyData3D();
#if VTK_MAJOR_VERSION <= 5
                            extern_glyphs_pedestrians_3D->SetInput(pData);
                            ((vtkLabeledDataMapper*)extern_pedestrians_labels->GetMapper())->SetInput(pData);
#else
                            extern_glyphs_pedestrians_3D->SetInputData(pData);
                            extern_pedestrians_labels->GetMapper()->SetInputDataObject(pData);
#endif
                            extern_glyphs_pedestrians_3D->Update();
                        }

                        if(extern_tracking_enable)
                        {
                            const std::vector<FrameElement *> &elements=frame->GetFrameElements();

                            for(unsigned int i=0;i<elements.size();i++)
                            {
                                FrameElement* el = elements[i];
                                double pos[3];
                                double color;
                                el->GetPos(pos);
                                el->GetColor(&color);
                                extern_trail_plotter->PlotPoint(pos,color);
                            }
                        }
                    }
                }


                int* winSize=renderWindow->GetSize();
                static int  lastWinX=winSize[0]+1; // +1 to trigger a first change
                static int lastWinY=winSize[1];

                sprintf(runningTimeText,"Pedestrians: %d      Time: %ld Sec",nPeds,frameNumber*iren->GetTimerDuration(tid)/1000);
                runningTime->SetInput(runningTimeText);
                runningTime->Modified();

                if((lastWinX!=winSize[0]) || (lastWinY!=winSize[1]) || (frameNumber<10))
                {
                    static std::string winBaseName(renderWindow->GetWindowName());
                    std::string winName=winBaseName;
                    std::string s;
                    winName.append(" [ ");
                    s=QString::number(winSize[0]).toStdString();
                    winName.append(s);
                    winName.append("X");
                    s=QString::number(winSize[1]).toStdString();
                    winName.append(s);
                    winName.append(" ] ");

                    int posY=winSize[1]*(1.0-30.0/536.0);
                    int posX=winSize[0]*(1.0-450.0/720.0);
                    runningTime->SetPosition(posX,posY);
                    renderWindow->SetWindowName(winName.c_str());

                    lastWinX=winSize[0];
                    lastWinY=winSize[1];
                }

                iren->Render();

                if(extern_force_system_update){
                    updateSettings(renderWindow);
                }
                if(extern_take_screenshot){
                    takeScreenshot(renderWindow);
                }
                if(SystemSettings::getRecordPNGsequence()){
                    takeScreenshotSequence(renderWindow);
                }

                if (frameNumber!=0) {
                    int desiredfps=1000.0/iren->GetTimerDuration(tid);
                    int effectivefps=1/(renderer->GetLastRenderTimeInSeconds());

                    effectivefps = (effectivefps>desiredfps)?desiredfps:effectivefps;

                    emit signalFrameNumber(frameNumber);
                    emit signalRunningTime(frameNumber*iren->GetTimerDuration(tid));
                    emit signalRenderingTime(effectivefps);
                }

#ifdef TRAVISTO_FFMPEG

                if(extern_launch_recording){
                    extern_launch_recording=false; //reset

                    windowToImageFilter=vtkWindowToImageFilter::New();
#ifdef WIN32
                    pAVIWriter=vtkAVIWriter::New();
#endif

#ifdef __linux__
                    pAVIWriter=vtkFFMPEGWriter::New();
#endif

                    pAVIWriter->SetQuality(2);
                    pAVIWriter->SetRate(1000.0/iren->GetTimerDuration(tid));

                    QString videoName;
                    SystemSettings::getOutputDirectory(videoName);
                    //create directory if not exits
                    if(!QDir(videoName).exists()){
                        QDir dir;
                        if(!dir.mkpath (videoName )){
                            cerr<<"could not create directory: "<< videoName.toStdString();
                            videoName=""; // current
                        }
                    }

                    videoName += "/tvtvid_"+QDateTime::currentDateTime().toString("yyMMdd_hh_mm_").append(SystemSettings::getFilenamePrefix()).append(".avi");

                    pAVIWriter->SetFileName(videoName.toStdString().c_str());

                    if(windowToImageFilter!=NULL)
                        if(windowToImageFilter->GetInput()==NULL){ //should be the case by first call
                            windowToImageFilter->SetInput(renderWindow);
#if VTK_MAJOR_VERSION <= 5
                            pAVIWriter->SetInput(windowToImageFilter->GetOutput());
#else
                            pAVIWriter->SetInputConnection(windowToImageFilter->GetOutputPort());
#endif
                            pAVIWriter->Start();
                        }
                    extern_recording_enable=true;
                    isRecording=true;
                }

                if(isRecording){
                    windowToImageFilter->Modified();
                    // only write when not paused
                    if(!extern_is_pause) pAVIWriter->Write();

                    if(extern_recording_enable==false){ //stop the recording
                        pAVIWriter->End();
                        windowToImageFilter->Delete();
                        pAVIWriter->Delete();
                        isRecording=false;
                    }
                }
#endif //TRAVISTO_FFMPEG

                if(extern_shutdown_visual_thread){
                    emit signalFrameNumber(0);

                    // this will force an update of the windows
                    lastWinX=0;
                    lastWinY=0;
                    //exit if and only if the recording process is terminated
                    if(isRecording) extern_recording_enable=false;
                    else iren->ExitCallback();
                }
            }
        }
    }
}

void TimerCallback::updateSettings(vtkRenderWindow* renderWindow) {

    static bool fullscreen=false;

    extern_glyphs_pedestrians_actor_2D->SetVisibility(SystemSettings::getShowAgents()&& SystemSettings::get2D());
    extern_glyphs_pedestrians_actor_3D->SetVisibility(SystemSettings::getShowAgents()&& !SystemSettings::get2D());
    extern_trail_plotter->SetVisibility(extern_tracking_enable);

    //agents captions
    extern_pedestrians_labels->SetVisibility(SystemSettings::getShowAgentsCaptions());

    //geometry captions

    //enable / disable full screen
    if(fullscreen!=extern_fullscreen_enable){
        renderWindow->SetFullScreen(extern_fullscreen_enable);
        //renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
        fullscreen=extern_fullscreen_enable;
    }

    // take
    extern_force_system_update=false;
}


void TimerCallback::getTrail(int datasetID, int frameNumber){

    int trailCount=0;
    int trailType=0;
    int trailForm=0;
    int tcMin=0;
    int tcMax=0;

    SystemSettings::getTrailsInfo(&trailCount,&trailType,&trailForm);

    switch(trailType){
    case 0://backward
        tcMin=frameNumber-trailCount;
        tcMax=frameNumber;
        break;

    case 1://symetric
        tcMin=frameNumber-trailCount/2;
        tcMax=frameNumber+trailCount/2;

        break;

    case 2://forward
        tcMin=frameNumber;
        tcMax=frameNumber+trailCount;
        break;

    }


    for (int i=tcMin;i<tcMax;i++){
        Frame* frame = extern_trajectories_firstSet.getFrame(i);
        if(frame==NULL){
            //		cerr<<"Trajectory not available in getTrail(), first data set"<<endl;
        }else {
            FrameElement* point=NULL;
            while(NULL!=(point=frame->getNextElement())){
                //extern_pedestrians_firstSet[point->getIndex()]->plotTrail(point->getX(),point->getY(),point->getZ());
                //extern_pedestrians_firstSet[point->getIndex()]->setTrailGeometry(trailForm);
            }
            frame->resetCursor();
        }
    }
}


void TimerCallback::takeScreenshot(vtkRenderWindow *renderWindow){
    static int imageID=0;
    vtkWindowToImageFilter * winToImFilter  = vtkWindowToImageFilter::New();
    winToImFilter->SetInput( renderWindow );
    //winToImFilter->SetMagnification(4);
    //renderWindow->Delete();
    //vtkPostScriptWriter * image  = vtkPostScriptWriter::New();
    vtkPNGWriter * image  = vtkPNGWriter::New();
    image->SetInputConnection(winToImFilter->GetOutputPort());
    winToImFilter->Delete();

    QString screenshots;
    SystemSettings::getOutputDirectory(screenshots);
    //create directory if not exits
    if(!QDir(screenshots).exists()){
        QDir dir;
        if(!dir.mkpath (screenshots )){
            //Debug::Error("could not create directory: %s",screenshots.toStdString().c_str());
            //try with the current directory
            screenshots="";
        }
    }


    char filename[256]={0};
    //	sprintf(filename,"travisto_video_%d.png",imageID++);
    std::string date= QString(QDateTime::currentDateTime().toString("yyMMdd_hh")).toStdString();

    sprintf(filename,"travisto_snap_%sh_%d.png",date.c_str(),imageID++);

    //append the prefix
    screenshots+=SystemSettings::getFilenamePrefix();
    screenshots+=QString(filename);
    image->SetFileName(screenshots.toStdString().c_str());
    winToImFilter->Modified();

    image->Write ();
    image->Delete();
    extern_take_screenshot=false;
}

/// take png screenshot sequence
void TimerCallback::takeScreenshotSequence(vtkRenderWindow* renderWindow){
    static int imageID=0;
    vtkWindowToImageFilter * winToImFilter  = vtkWindowToImageFilter::New();
    winToImFilter->SetInput( renderWindow );
    //renderWindow->Delete();
    vtkPNGWriter * image  = vtkPNGWriter::New();
    //vtkPostScriptWriter * image  = vtkPostScriptWriter::New();
    image->SetInputConnection( winToImFilter->GetOutputPort());
    winToImFilter->Delete();

    QString screenshots;
    SystemSettings::getOutputDirectory(screenshots);

    screenshots.append("./png_seq_"+QDateTime::currentDateTime().toString("yyMMddhh")+"_"+SystemSettings::getFilenamePrefix());
    screenshots.truncate(screenshots.size()-1);

    //create directory if not exits
    if(!QDir(screenshots).exists()){
        QDir dir;
        if(!dir.mkpath (screenshots )){
            cerr<<"could not create directory: "<< screenshots.toStdString();
            //try with the current directory
            screenshots="./png_seq_"+QDateTime::currentDateTime().toString("yyMMdd")+"_"+SystemSettings::getFilenamePrefix();
            screenshots.truncate(screenshots.size()-1);
        }
    }


    char filename[30]={0};
    sprintf(filename,"/tmp_%07d.png",imageID++);
    screenshots.append(filename);
    image->SetFileName(screenshots.toStdString().c_str());
    winToImFilter->Modified();

    image->Write ();
    image->Delete();
}


void TimerCallback::SetRenderTimerId(int tid)
{
    this->RenderTimerId = tid;
}

void TimerCallback::setTextActor(vtkTextActor* ra){
    runningTime=ra;
}

