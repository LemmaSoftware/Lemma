// ===========================================================================
//
//       Filename:  utdipolesource.cpp
//
//    Description:
//
//        Version:  0.0
//        Created:  12/02/2009 11:57:14 AM
//       Revision:  none
//       Compiler:  g++ (c++)
//
//         Author:  Trevor Irons (ti)
//
//   Organisation:  Colorado School of Mines (CSM)
//                  United States Geological Survey (USGS)
//
//          Email:  tirons@mines.edu, tirons@usgs.gov
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ===========================================================================

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
#undef LEMMAUSEVTK
#undef KIHALEE_EM1D

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

    //////////////////
    // TODO List of Dipole settings that aren't working

    // Do calculation
    jsw_timer timer;
    timer.begin();

    // Test with a single dipole
    DipoleSource *dipole = DipoleSource::New();
        //dipole->SetMoment(2);
        ///////////////////////////////////
        // E field Tests
        // Sems to be working!
        //dipole->SetType(GROUNDEDELECTRICDIPOLE);
        //dipole->SetPolarisation(XPOLARISATION);
        //dipole->SetPolarisation(YPOLARISATION);
        //dipole->SetPolarisation(ZPOLARISATION);
        //dipole->SetAzimuth(0);  // X - Northing
        //dipole->SetAzimuth(90); // Y - Easting

        dipole->SetType(GROUNDEDELECTRICDIPOLE);
            //dipole->SetPolarisation(1., 0.0,  1.0);
        //    dipole->SetPolarisation(XPOLARISATION);
        dipole->SetPolarisation(YPOLARISATION);
        //dipole->SetPolarisation(ZPOLARISATION);

        /////////////
        //dipole->SetType(MAGNETICDIPOLE);
            //dipole->SetPolarisation(0., 0.0,  1.0);
            //dipole->SetPolarisation(XPOLARISATION);
        //dipole->SetPolarisation(YPOLARISATION);
        //dipole->SetPolarisation(ZPOLARISATION);

        //dipole->SetMoment(1);
        //dipole->SetLocation(1,1,-.0100328);
        dipole->SetLocation(0., 0., -0.001);
        //dipole->SetLocation(-2.5,1.25,0);
        dipole->SetNumberOfFrequencies(1);
        dipole->SetFrequency(0, 2e7);
        //dipole->SetFrequency(1,2000);
        //dipole->SetPhase(0);

        //////////////////////////////////
        // H field Tests

//     // Define model, this is a difficult case
//     VectorXcr sigma(23);
//         sigma << 0., 0.0625, 0.0166666666666667, 0.04, 0.0181818181818182,
//                 0.05, 0.0222222222222222, 0.2, 0.025, 0.05, 0.025,
//             0.05, 0.00833333333333333, 0.025, 0.05, 0.0222222222222222,
//             0.0277777777777778, 0.0333333333333333, 0.0208333333333333,
//             0.0181818181818182, 0.025, 0.04;
//
//     VectorXr  thick(21);
//         //thick << 10, 10, 10, 10, 10, 10;
//         thick << 11, 5, 5, 4, 3.5, 4.5, 4.5,
//                  5.5, 3, 36, 15, 3, 6, 3, 8,
//                  9, 10, 7, 8, 11, 68;

    //VectorXcr sigma(8);
    //    sigma << 0., Complex(.01,0.01), 0.001, .005, .0034, .0023, .0024, .0001;

    VectorXcr sigma(8);
        sigma << 0., 3e-4, 5e-4, 1e-2, .5, 5e-6, .03, .04;
    //VectorXcr sigma(2);
    //    sigma << 0., .0;
    //VectorXr susl(3);
    //    susl << 1., 1., 1.5;//, 5e-6, 5e-6, 5e-6;
    //VectorXr sush(3);
    //    sush << 1., 1.01;//, 5e-6, 5e-6, 5e-6;
    //VectorXr sustau(2);
    //    sustau << 1., 1.0;//, 5e-6, 5e-6, 5e-6;
    //VectorXr susb(2);
    //    susb << 0., .0;//, 5e-6, 5e-6, 5e-6;
    VectorXr  thick(6);
        thick << 10 , 10 , 10 , 10 , 10 , 10;

    LayeredEarthEM *earth = LayeredEarthEM::New();
        earth->SetNumberOfLayers(8);
        //earth->SetNumberOfLayers(23);
        earth->SetLayerConductivity(sigma);
        //earth->SetLayerLowFreqSusceptibility(susl);
        //earth->SetLayerHighFreqSusceptibility(sush);
        //earth->SetLayerTauSusceptibility(sustau);
        //earth->SetLayerBreathSusceptibility(susb);
        //earth->SetLayerHighFreqSusceptibility(susl);
        //earth->SetLayerHighFreqSusceptibility(susl);
        earth->SetLayerThickness(thick);

    // just a test
    timer.end();

    // Receivers
    ReceiverPoints *receivers = ReceiverPoints::New();
        Vector3r loc;
        Real ox     =  50.;
        Real oy     =  70. ;
        Real depth  =  -40.13423;
        Real depth2 =  depth;
        Real dz     =  .25;
        int  nz     =  3000;
        Real dx     = .0;
        receivers->SetNumberOfReceivers(nz);
        int ir = 0;
        for (int iz=0; iz<nz; ++iz) {
            loc << ox, oy, depth;
            receivers->SetLocation(ir, loc);
            depth += dz;
            ox += dx;
            ++ ir;
        }

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
        //EmEarth->SetHankelTransformMethod(QWEKEY);
        EmEarth->SetHankelTransformMethod(FHTKEY51);

    timer.begin();
    EmEarth->MakeCalc3();
    //std::cout << "H\n" << receivers->GetHfield(0)
    //          << "End of H" << std::endl;
    Real  lemmaTime = timer.end();
    std::cout << lemmaTime << "\t";

    //std::cout << "Make Calc3\n";
    //receivers->ClearFields();

    //EmEarth->MakeCalc3();
    //std::cout << "Make Calc3 finished\n";

    #ifdef LEMMAUSEVTK
    int iif=0;
    vtkDataObject *_dataObjectxr = receivers->GetVtkDataObject(HFIELDREAL, iif,
                    0, receivers->GetNumberOfReceivers(), XCOMPONENT, ZCOORD);

    vtkDataObject *_dataObjectxi = receivers->GetVtkDataObject(HFIELDIMAG, iif,
                    0, receivers->GetNumberOfReceivers(), XCOMPONENT, ZCOORD);

    vtkDataObject *_dataObjectyr= receivers->GetVtkDataObject(HFIELDREAL, iif,
                    0, receivers->GetNumberOfReceivers(), YCOMPONENT, ZCOORD);

    vtkDataObject *_dataObjectyi = receivers->GetVtkDataObject(HFIELDIMAG, iif,
                    0, receivers->GetNumberOfReceivers(), YCOMPONENT, ZCOORD);

    vtkDataObject *_dataObjectzr = receivers->GetVtkDataObject(HFIELDREAL, iif,
                    0, receivers->GetNumberOfReceivers(), ZCOMPONENT, ZCOORD);

    vtkDataObject *_dataObjectzi = receivers->GetVtkDataObject(HFIELDIMAG, iif,
                    0, receivers->GetNumberOfReceivers(), ZCOMPONENT, ZCOORD);
    #endif

    /*
    // Write out to file
    depth = depth2;
    std::fstream real("reale_lay.dat", std::ios::out);
    std::fstream imag("image_lay.dat", std::ios::out);
    std::fstream hreal("real_lay.dat", std::ios::out);
    std::fstream himag("imag_lay.dat", std::ios::out);
    for (int iz=0; iz<nz; ++iz) {
        Vector3cr temp = receivers->GetEfield(0,iz);

        //std::cout << temp << endl;
        real << ox << "\t" << oy << "\t" << depth << "\t"
                << temp(0).real() << "\t" << temp(1).real()
                << "\t" << temp(2).real() << std::endl;

        imag << ox << "\t" << oy << "\t" << depth << "\t"
                << std::imag(temp(0)) << "\t" << std::imag(temp(1))
                << "\t" << std::imag(temp(2)) << std::endl;

        temp = receivers->GetHfield(0, iz);

        hreal << ox << "\t" << oy << "\t" << depth << "\t"
                << std::real(temp(0)) << "\t" << std::real(temp(1))
                << "\t" << std::real(temp(2)) << std::endl;
        himag << ox << "\t" << oy << "\t" << depth << "\t"
                << std::imag(temp(0)) << "\t" << std::imag(temp(1))
                << "\t" << std::imag(temp(2)) << std::endl;
        depth += dz;
    }
    real.close();
    imag.close();
    */

    // TODO check all field components for both E and H,
    // diplay in some sort of reasonable manner.
    // Then, and only then, optimise.

    receivers->ClearFields();

    //dipole->SetPolarisation(XPOLARISATION);
    //std::cout << "Fortran Calc" << std::endl;
    #ifdef KIHALEE_EM1D
    timer.begin();
    EmEarth->MakeCalc();
    Real fortranTime = timer.end();
    std::cout << fortranTime << "\t";
    std::cout << fortranTime/lemmaTime << std::endl;
    #endif

    #ifdef LEMMAUSEVTK
    vtkDataObject *_fdataObjectxr = receivers->GetVtkDataObject(HFIELDREAL, 0,
                    0, receivers->GetNumberOfReceivers(), XCOMPONENT, ZCOORD);

    vtkDataObject *_fdataObjectxi = receivers->GetVtkDataObject(HFIELDIMAG, 0,
                    0, receivers->GetNumberOfReceivers(), XCOMPONENT, ZCOORD);

    vtkDataObject *_fdataObjectyr = receivers->GetVtkDataObject(HFIELDREAL, 0,
                    0, receivers->GetNumberOfReceivers(), YCOMPONENT, ZCOORD);

    vtkDataObject *_fdataObjectyi = receivers->GetVtkDataObject(HFIELDIMAG, 0,
                    0, receivers->GetNumberOfReceivers(), YCOMPONENT, ZCOORD);

    vtkDataObject *_fdataObjectzr = receivers->GetVtkDataObject(HFIELDREAL, 0,
                    0, receivers->GetNumberOfReceivers(), ZCOMPONENT, ZCOORD);

    vtkDataObject *_fdataObjectzi = receivers->GetVtkDataObject(HFIELDIMAG, 0,
                    0, receivers->GetNumberOfReceivers(), ZCOMPONENT, ZCOORD);
    #endif

    // Simple 2D plot in vtk to compare algorithms
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
        _xplot->AddDataObjectInput(_fdataObjectxr);
        _xplot->AddDataObjectInput(_fdataObjectxi);
        _xplot->SetTitle("H_x");
        _xplot->SetXTitle("");
        _xplot->SetYTitle("");
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
        _xplot->SetPlotColor(1,0,0,1);

        _xplot->SetPlotColor(2,1,0,0);
        _xplot->SetPlotColor(3,0,1,0);

        _xplot->SetPlotLabel(0, "Lemma real" );
        _xplot->SetPlotLabel(1, "Lemma imag" );
        _xplot->SetPlotLabel(2, "KiHa real" );
        _xplot->SetPlotLabel(3, "KiHa imag" );

        _xplot->GetProperty()->SetLineWidth(1);
        _xplot->GetProperty()->SetPointSize(22);

        _xplot->LegendOn();
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
        _yplot->AddDataObjectInput(_fdataObjectyr);
        _yplot->AddDataObjectInput(_fdataObjectyi);
        _yplot->SetTitle("H_y");
        _yplot->SetXTitle("");
        _yplot->SetYTitle("");
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
        _yplot->SetPlotColor(1,0,0,1);

        _yplot->SetPlotColor(2,1,0,0);
        _yplot->SetPlotColor(3,0,1,0);

        _yplot->SetPlotLabel(0, "Lemma real" );
        _yplot->SetPlotLabel(1, "Lemma imag" );
        _yplot->SetPlotLabel(2, "KiHa real" );
        _yplot->SetPlotLabel(3, "KiHa imag" );
        _yplot->LegendOn();

        _yplot->GetProperty()->SetLineWidth(1);
        _yplot->GetProperty()->SetPointSize(12);
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
        _zplot->AddDataObjectInput(_fdataObjectzr);
        _zplot->AddDataObjectInput(_fdataObjectzi);
        _zplot->SetTitle("H_z");
        _zplot->SetXTitle("Depth");
        _zplot->SetYTitle("");
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
        _zplot->SetPlotColor(1,0,0,1);

        _zplot->SetPlotColor(2,1,0,0);
        _zplot->SetPlotColor(3,0,1,0);

        _zplot->LegendOn();

        _zplot->SetPlotLabel(0, "Lemma real" );
        _zplot->SetPlotLabel(1, "Lemma imag" );
        _zplot->SetPlotLabel(2, "KiHa real" );
        _zplot->SetPlotLabel(3, "KiHa imag" );

        _zplot->GetProperty()->SetLineWidth(1);
        _zplot->GetProperty()->SetPointSize(14);
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

        _zplot->PlotCurvePointsOn();
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
