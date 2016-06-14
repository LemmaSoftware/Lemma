/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/29/2014 09:53:49 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include <iostream>
#include <fstream>
#include "dipolesource.h"
#include "layeredearthem.h"
#include "receiverpoints.h"
#include "emearth1d.h"

#ifdef LEMMAUSEOMP
#include "omp.h"
#endif

// For testing purposes disable VTK and run scale.sh
//#undef LEMMAUSEVTK
//#undef KIHALEE_EM1D

#ifdef LEMMAUSEVTK
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDoubleArray.h"
#include "vtkXYPlotActor.h"
#include "vtkXYPlotWidget.h"
#include "vtkProperty2D.h"
#endif

#include "timer.h"

using namespace Lemma;

int main() {

    // Do calculation
    jsw_timer timer;

    DipoleSource *dipole = DipoleSource::New();
    dipole->SetType(MAGNETICDIPOLE);
        dipole->SetPolarisation(0., 0.0,  1.0);
        //dipole->SetPolarisation(XPOLARISATION);
        //dipole->SetPolarisation(YPOLARISATION);
        //dipole->SetPolarisation(ZPOLARISATION);

        //dipole->SetMoment(1);
        //dipole->SetLocation(1,1,-.0100328);
        dipole->SetLocation(0., 0., -0.001);
        //dipole->SetLocation(-2.5,1.25,0);
        int iif=10000;
        dipole->SetNumberOfFrequencies(iif);
        Real w0 = 10;
        for (int iw=0; iw<iif; ++iw) {
            dipole->SetFrequency(iw, w0);
            w0 += 1523.;
        }

    VectorXcr sigma(8);
    sigma << 0., 3e-4, 5e-4, 1e-2, .5, 5e-6, .03, .04;
    VectorXr  thick(6);
        thick << 10 , 10 , 10 , 10 , 10 , 10;

    LayeredEarthEM *earth = LayeredEarthEM::New();
        earth->SetNumberOfLayers(8);
        earth->SetLayerConductivity(sigma);
        earth->SetLayerThickness(thick);

    ReceiverPoints *receivers = ReceiverPoints::New();
        Vector3r loc;
        receivers->SetNumberOfReceivers(1);
        loc << 150, 175, -.01;
        receivers->SetLocation(0, loc);

    // EmEarth
    EMEarth1D  *EmEarth = EMEarth1D::New();
        EmEarth->AttachDipoleSource(dipole);
        EmEarth->AttachLayeredEarthEM(earth);
        EmEarth->AttachReceiverPoints(receivers);
        //EmEarth->SetFieldsToCalculate(BOTH);
        EmEarth->SetFieldsToCalculate(H);

        //EmEarth->SetHankelTransformMethod(GAUSSIANQUADRATURE);
        //EmEarth->SetHankelTransformMethod(DIGITALFILTERING);
        //EmEarth->SetHankelTransformMethod(FHTKEY201);
        EmEarth->SetHankelTransformMethod(QWEKEY);

    timer.begin();
    EmEarth->MakeCalc3();
    //std::cout << "H\n" << receivers->GetHfield(0)
    //          << "End of H" << std::endl;
    Real  lemmaTime = timer.end();
    //std::cout << receivers->GetHfield() << std::endl;
    std::cout << lemmaTime << "\t";

    #ifdef LEMMAUSEVTK
    vtkDataObject *_dataObjectxr = receivers->GetVtkDataObjectFreq(HFIELDREAL, 0,
                    0, iif, XCOMPONENT, dipole->GetFrequencies());

    vtkDataObject *_dataObjectxi = receivers->GetVtkDataObjectFreq(HFIELDIMAG, 0,
                    0, iif, XCOMPONENT, dipole->GetFrequencies());

    vtkDataObject *_dataObjectyr= receivers->GetVtkDataObjectFreq(HFIELDREAL, 0,
                    0, iif, YCOMPONENT, dipole->GetFrequencies());

    vtkDataObject *_dataObjectyi = receivers->GetVtkDataObjectFreq(HFIELDIMAG, 0,
                    0, iif, YCOMPONENT, dipole->GetFrequencies());

    vtkDataObject *_dataObjectzr = receivers->GetVtkDataObjectFreq(HFIELDREAL, 0,
                    0, iif, ZCOMPONENT, dipole->GetFrequencies());

    vtkDataObject *_dataObjectzi = receivers->GetVtkDataObjectFreq(HFIELDIMAG, 0,
                    0, iif, ZCOMPONENT, dipole->GetFrequencies());
    #endif


//     //dipole->SetPolarisation(XPOLARISATION);
//     //std::cout << "Fortran Calc" << std::endl;
//     #ifdef KIHALEE_EM1D
//     timer.begin();
//     EmEarth->MakeCalc();
//     Real fortranTime = timer.end();
//     std::cout << fortranTime << "\t";
//     std::cout << fortranTime/lemmaTime << std::endl;
//     #endif

//     #ifdef LEMMAUSEVTK
//     vtkDataObject *_fdataObjectxr = receivers->GetVtkDataObjectFreq(HFIELDREAL, 0,
//                     0, receivers->GetNumberOfReceivers(), XCOMPONENT, dipole->GetFrequencies());
//
//     vtkDataObject *_fdataObjectxi = receivers->GetVtkDataObjectFreq(HFIELDIMAG, 0,
//                     0, receivers->GetNumberOfReceivers(), XCOMPONENT, dipole->GetFrequencies());
//
//     vtkDataObject *_fdataObjectyr = receivers->GetVtkDataObjectFreq(HFIELDREAL, 0,
//                     0, receivers->GetNumberOfReceivers(), YCOMPONENT, dipole->GetFrequencies());
//
//     vtkDataObject *_fdataObjectyi = receivers->GetVtkDataObjectFreq(HFIELDIMAG, 0,
//                     0, receivers->GetNumberOfReceivers(), YCOMPONENT, dipole->GetFrequencies());
//
//     vtkDataObject *_fdataObjectzr = receivers->GetVtkDataObjectFreq(HFIELDREAL, 0,
//                     0, receivers->GetNumberOfReceivers(), ZCOMPONENT, dipole->GetFrequencies());
//
//     vtkDataObject *_fdataObjectzi = receivers->GetVtkDataObjectFreq(HFIELDIMAG, 0,
//                     0, receivers->GetNumberOfReceivers(), ZCOMPONENT, dipole->GetFrequencies());
//     #endif


    #ifdef LEMMAUSEVTK
    vtkRenderer *_ren = vtkRenderer::New();
    vtkRenderWindow *_renwin = vtkRenderWindow::New();
    _renwin->AddRenderer(_ren);

    ///////////////////////////////////////////////
    // X Component Plot
    vtkXYPlotActor *_xplot = vtkXYPlotActor::New();
    //vtkXYPlotWidget *_wplot = vtkXYPlotWidget::New();
        // Add the datasets
        _xplot->AddDataObjectInput( _dataObjectxr);
        _xplot->AddDataObjectInput( _dataObjectxi);
        //_xplot->AddDataObjectInput(_fdataObjectxr);
        //_xplot->AddDataObjectInput(_fdataObjectxi);
        _xplot->SetTitle("X field");
        _xplot->SetXTitle("Depth");
        _xplot->SetYTitle("E Field");
        _xplot->SetXValuesToValue();
        //_plot->SetXValuesToIndex();

        // set which parts of the data object are to be used for which axis
        _xplot->SetDataObjectXComponent(0,1);
        _xplot->SetDataObjectYComponent(0,0);
        _xplot->SetDataObjectXComponent(1,1);
        _xplot->SetDataObjectYComponent(1,0);
        _xplot->SetDataObjectXComponent(2,1);
        _xplot->SetDataObjectYComponent(2,0);
        _xplot->SetDataObjectXComponent(3,1);
        _xplot->SetDataObjectYComponent(3,0);
        _xplot->SetNumberOfXLabels(3);
        _xplot->SetPlotColor(0,1,1,1);
        _xplot->SetPlotColor(1,1,1,1);
        _xplot->SetPlotColor(2,1,0,0);
        _xplot->SetPlotColor(3,1,0,0);
        _xplot->GetProperty()->SetLineWidth(2);
        _xplot->GetProperty()->SetPointSize(4);

        //  _plot->SetPlotPoints(0,0);
        //  _plot->SetPlotPoints(1,0);
        //  _plot->SetPlotPoints(2,0);
        //  _plot->SetPlotPoints(3,0);

        //  _plot->SetPlotLines(0,0);
        //  _plot->SetPlotLines(1,0);
        //  _plot->SetPlotLines(2,0);
        //  _plot->SetPlotLines(3,0);

        _xplot->PlotCurvePointsOff();
        _xplot->PlotCurveLinesOff();
        _xplot->GetPositionCoordinate()->SetValue(0.0, 0.67, 0);
        _xplot->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0);
        //_plot->SetReverseYAxis(1); // Just flips axis, not data!

    ///////////////////////////////////////////////
    // Y Component Plot
    vtkXYPlotActor *_yplot = vtkXYPlotActor::New();
    //vtkXYPlotWidget *_wplot = vtkXYPlotWidget::New();
        // Add the datasets
        _yplot->AddDataObjectInput( _dataObjectyr);
        _yplot->AddDataObjectInput( _dataObjectyi);
        //_yplot->AddDataObjectInput(_fdataObjectyr);
        //_yplot->AddDataObjectInput(_fdataObjectyi);
        _yplot->SetTitle("Y field");
        _yplot->SetXTitle("Depth");
        _yplot->SetYTitle("E Field");
        _yplot->SetXValuesToValue();
        //_yplot->SetXValuesToIndex();

        // set which parts of the data object are to be used for which axis
        _yplot->SetDataObjectXComponent(0,1);
        _yplot->SetDataObjectYComponent(0,0);
        _yplot->SetDataObjectXComponent(1,1);
        _yplot->SetDataObjectYComponent(1,0);
        _yplot->SetDataObjectXComponent(2,1);
        _yplot->SetDataObjectYComponent(2,0);
        _yplot->SetDataObjectXComponent(3,1);
        _yplot->SetDataObjectYComponent(3,0);
        _yplot->SetNumberOfXLabels(3);
        _yplot->SetPlotColor(0,1,1,1);
        _yplot->SetPlotColor(1,1,1,1);
        _yplot->SetPlotColor(2,1,0,0);
        _yplot->SetPlotColor(3,1,0,0);
        _yplot->GetProperty()->SetLineWidth(2);
        _yplot->GetProperty()->SetPointSize(4);
        _yplot->GetPositionCoordinate()->SetValue(0.00, 0.33, 0);
        _yplot->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0);

        //  _plot->SetPlotPoints(0,0);
        //  _plot->SetPlotPoints(1,0);
        //  _plot->SetPlotPoints(2,0);
        //  _plot->SetPlotPoints(3,0);

        //  _plot->SetPlotLines(0,0);
        //  _plot->SetPlotLines(1,0);
        //  _plot->SetPlotLines(2,0);
        //  _plot->SetPlotLines(3,0);

        _yplot->PlotCurvePointsOff();
        _yplot->PlotCurveLinesOff();
        //_plot->SetReverseYAxis(1)

    ///////////////////////////////////////////////
    // Z Component Plot
    vtkXYPlotActor *_zplot = vtkXYPlotActor::New();
    //vtkXYPlotWidget *_wplot = vtkXYPlotWidget::New();
        // Add the datasets
        _zplot->AddDataObjectInput( _dataObjectzr);
        _zplot->AddDataObjectInput( _dataObjectzi);
        //_zplot->AddDataObjectInput(_fdataObjectzr);
        //_zplot->AddDataObjectInput(_fdataObjectzi);
        _zplot->SetTitle("Z field");
        _zplot->SetXTitle("Depth");
        _zplot->SetYTitle("E Field");
        _zplot->SetXValuesToValue();
        //_plot->SetXValuesToIndex();

        // set which parts of the data object are to be used for which axis
        _zplot->SetDataObjectXComponent(0,1);
        _zplot->SetDataObjectYComponent(0,0);
        _zplot->SetDataObjectXComponent(1,1);
        _zplot->SetDataObjectYComponent(1,0);
        _zplot->SetDataObjectXComponent(2,1);
        _zplot->SetDataObjectYComponent(2,0);
        _zplot->SetDataObjectXComponent(3,1);
        _zplot->SetDataObjectYComponent(3,0);
        _zplot->SetNumberOfXLabels(3);
        _zplot->SetPlotColor(0,1,1,1);
        _zplot->SetPlotColor(1,1,1,1);
        _zplot->SetPlotColor(2,1,0,0);
        _zplot->SetPlotColor(3,1,0,0);
        _zplot->GetProperty()->SetLineWidth(2);
        _zplot->GetProperty()->SetPointSize(4);
        _zplot->GetPositionCoordinate()->SetValue(0.0, 0.0, 0);
        _zplot->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0);
        //  _plot->SetPlotPoints(0,0);
        //  _plot->SetPlotPoints(1,0);
        //  _plot->SetPlotPoints(2,0);
        //  _plot->SetPlotPoints(3,0);

        //  _plot->SetPlotLines(0,0);
        //  _plot->SetPlotLines(1,0);
        //  _plot->SetPlotLines(2,0);
        //  _plot->SetPlotLines(3,0);

        _zplot->PlotCurvePointsOff();
        _zplot->PlotCurveLinesOff();
        //_plot->SetReverseYAxis(1)

    _ren->AddActor2D(_xplot);
    _ren->AddActor2D(_yplot);
    _ren->AddActor2D(_zplot);

    vtkRenderWindowInteractor *_iren = vtkRenderWindowInteractor::New();
    _iren->SetRenderWindow(_renwin);
    _iren->Initialize();
    _renwin->Render();

    //_wplot->SetXYPlotActor(_plot);
    //_wplot->SetInteractor(_iren);
    //_wplot->EnabledOn();

    _iren->Start();

    // TODO clean up you dirty whore
//  _iren->Delete();
//  _plot->Delete();
//  _dataObject->Delete();
//  _fieldData->Delete();
//  _renwin->Delete();
//  _ren->Delete();
//  _xdatareal->Delete();
//  _ydata->Delete();
//  _depth->Delete();
    #endif

    EmEarth->Delete();
    receivers->Delete();
    earth->Delete();
    dipole->Delete();

    return 0;


}
