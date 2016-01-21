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
#include "layeredearth.h"
#include "receiverpoints.h"
#include "emearth1d.h"
#include "WireAntenna.h"
#include "PolygonalWireAntenna.h"

#if  LEMMAUSEVTK
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderLargeImage.h"
#include "vtkPNGWriter.h"
#include "vtkRectilinearGrid.h"
#include "vtkXMLRectilinearGridWriter.h"
#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include "vtkCellData.h"
#endif     // -----  not LEMMA_USE_VTK  -----

// For testing purposes disable VTK and run scale.sh
//#undef LEMMAUSEVTK

#include "timer.h"

using namespace Lemma;

double randDouble(double low, double high) {
	//srand(time(0));
	double temp;
	/* swap low & high around if the user makes no sense */
	if (low > high)	{
	temp = low;
	low = high;
	high = temp;
	}
	/* calculate the random number & return it */
	temp = (rand() / (static_cast<double>(RAND_MAX) + 1.0))
	* (high - low) + low;
	return temp;
}


int main() {

	// Keep track of time
	jsw_timer timer;


	srand(time(0));

	PolygonalWireAntenna *pa = PolygonalWireAntenna::New();
	pa->SetNumberOfFrequencies(1);
	pa->SetFrequency(0, 540000);
    Real Depth  =   370;  // nominal depth
    Real Width  = 0.14;  // transmitter height
    Real Height = 0.70;//  .14;  //.014;  // transmitter width


	pa->SetNumberOfPoints(5);
    /*
    pa->SetPoint(0, Vector3r(       0,       0, Depth));
	pa->SetPoint(1, Vector3r(       0,       0, Depth+Height));
	pa->SetPoint(2, Vector3r(       0,   Width, Depth+Height));
	pa->SetPoint(3, Vector3r(       0,   Width, Depth));
	pa->SetPoint(4, Vector3r(       0,       0, Depth));
    */

    pa->SetPoint(0, Vector3r(   -Width/2., -Height/2., Depth));
	pa->SetPoint(1, Vector3r(    Width/2., -Height/2., Depth));
	pa->SetPoint(2, Vector3r(    Width/2.,  Height/2., Depth));
	pa->SetPoint(3, Vector3r(   -Width/2.,  Height/2., Depth));
	pa->SetPoint(4, Vector3r(   -Width/2., -Height/2., Depth));

	pa->SetCurrent(1.);
    pa->SetNumberOfTurns(6);
	//Vector3r rp = Vector3r::Random(3);
	//rp << 150., 10, 0.;
	//rp << -27.1456, 15.2350, -1e-3;
	//rp << randDouble(-35,35), randDouble(-35,35), randDouble(-35,35);
	//rp << 3.22806, -13.1548, 14.9695;
	//rp.setRandom(3);
	//std::cout <<  "rp " << rp.transpose() << std::endl;
	//pa->ApproximateWithElectricDipoles(rp);


	// Define model
    Real Sigma = 1.0/20. ; // .05;
	VectorXcr sigma(2);
		sigma << Complex(0.,0), Complex(Sigma,0);
	VectorXr  thick(1);
		thick << 10;
	LayeredEarthEM *earth = LayeredEarthEM::New();
		earth->SetNumberOfLayers(2);
		earth->SetLayerConductivity(sigma);
		//earth->SetLayerThickness(thick);

	// Receivers
	ReceiverPoints *receivers = ReceiverPoints::New();
		Vector3r loc;
		Real ox    =    -5.*Width - .003373;
		Real oy    =    -3.*Height - .003373;
		Real oz    =    Depth - Height/2. - .003373;
		int nx     =   120; // 60;
		int ny     =   180; //  50;
		int nz     =   100; // 40;
		Real hx    =    11.*Width/nx;
		Real hy    =     7.*Height/ny;
		Real hz    =     1.*Height/nz;

        receivers->SetNumberOfReceivers(nx*ny*nz);
		int ir = 0;
		for (int iz=0; iz<nz; ++iz) {
		for (int iy=0; iy<ny; ++iy) {
		for (int ix=0; ix<nx; ++ix) {
			loc << ox+ix*hx, oy+iy*hy, oz+iz*hz;
			receivers->SetLocation(ir, loc);
			++ ir;
		}
        }
        }

	// EmEarth
	EMEarth1D  *EmEarth = EMEarth1D::New();
		//EmEarth->AttachWireAntenna(wire);
		EmEarth->AttachWireAntenna(pa);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachReceiverPoints(receivers);
		EmEarth->SetFieldsToCalculate(E);
        //EmEarth->SetHankelTransformMethod(GAUSSIANQUADRATURE);
        EmEarth->SetHankelTransformMethod(ANDERSON801);

	// Do calculation
	timer.begin();
	EmEarth->CalculateWireAntennaFields();
	Real paTime = timer.end();
	std::cout << "Polygonal wire antennae time: " << paTime << "\n";

	//EmEarth->AttachWireAntenna(wire);
	//timer.begin();
	//EmEarth->CalculateWireAntennaFields();
	//Real waTime = timer.end();
	//std::cout << "Fixed wire antennae time: " << waTime << "\n";
/*
	depth = depth2;
	std::fstream real("reale_lay.dat", std::ios::out);
	std::fstream imag("image_lay.dat", std::ios::out);
	for (int iz=0; iz<nz; ++iz) {
		Vector3cr temp = receivers->GetEfield(0,iz);
		real << ox << "\t" << oy << "\t" << depth << "\t"
				<< temp(0).real() << "\t" << temp(1).real()
				<< "\t" << temp(2).real() << std::endl;
		imag << ox << "\t" << oy << "\t" << depth << "\t"
				<< std::imag(temp(0)) << "\t" << std::imag(temp(1))
				<< "\t" << std::imag(temp(2)) << std::endl;
		depth += dx;
	}
	real.close();
	imag.close();
*/

	//wire->Delete();
#if  LEMMAUSEVTK

    // Set Coordinates
    vtkDoubleArray *xCoords = vtkDoubleArray::New();
    xCoords->InsertNextValue(ox-hx/2.);
    double xm1 = ox-hx/2.;
    for (int ix=0; ix<nx; ix++) {
        xCoords->InsertNextValue(xm1 + hx);
        xm1 += hx;
    }

    vtkDoubleArray *yCoords = vtkDoubleArray::New();
    yCoords->InsertNextValue(oy-hy/2.);
    double ym1 = oy-hy/2.;
    for (int iy=0; iy<ny; iy++) {
        yCoords->InsertNextValue(ym1 + hy);
        ym1 += hy;
    }

    vtkDoubleArray *zCoords = vtkDoubleArray::New();
    zCoords->InsertNextValue(oz-hz/2.);
    double zm1 = oz-hz/2.;
    for (int iz=0; iz<nz; iz++) {
        zCoords->InsertNextValue(zm1 + hz);
        zm1 += hz;
    }

    vtkDoubleArray *EReal = vtkDoubleArray::New();
    vtkDoubleArray *EImag = vtkDoubleArray::New();
    vtkDoubleArray *Watts = vtkDoubleArray::New();
    EReal->SetNumberOfComponents(3);
    EImag->SetNumberOfComponents(3);
    Watts->SetNumberOfComponents(1);

    ir = 0;
    Real WattsTotal(0);
    Real WattsInterior(0);
    for (int iz=0; iz<nz; ++iz) {
    for (int iy=0; iy<ny; ++iy) {
    for (int ix=0; ix<nx; ++ix) {
        //sigmaArray->InsertTuple1(i,  sigma[ix][iy][iz] );
		Vector3cr E = receivers->GetEfield(0, ir);
        EReal-> InsertTuple3(ir, real(E(0)), real(E(1)), real(E(2)));
        EImag-> InsertTuple3(ir, imag(E(0)), imag(E(1)), imag(E(2)));
        //std::cout << std::abs(ox+ix*hx) << "\t"  <<  Width/2. << endl; // && std::abs(oy+iy*hy) > Height/2. ) { // && std::abs(oz+iz*hz - Depth) > Width/2. ) {
        //if ( std::abs( ox+ix*hx ) < Width/2. && std::abs(oy+iy*hy) < Height/2. && std::abs(oz+iz*hz - Depth) < Width/2. ) {
        if ( std::sqrt( std::pow(ox+ix*hx,2)  + std::pow(oz+iz*hz - Depth, 2) ) < .085 && std::abs(oy+iy*hy) < Height/2. ) {
            Watts-> InsertTuple1(ir,  1e-20 );
            WattsInterior += .5*  (( pow((std::abs(E(0)) + std::abs(E(1)) + std::abs(E(2))), 2)*Sigma)*hx*hy*hz );
        } else {
            Watts-> InsertTuple1(ir, .5* (pow( (std::abs(E(0)) + std::abs(E(1)) + std::abs(E(2))), 2)*Sigma)*hx*hy*hz );
            //Watts-> InsertTuple1(ir,  1e-20 );
            WattsTotal += .5*  (( pow((std::abs(E(0)) + std::abs(E(1)) + std::abs(E(2))), 2)*Sigma)*hx*hy*hz );
        }
		++ ir;
    }
    }
    }
    std::cout << "Total Power: " << Sigma << "\t" << WattsTotal << "\t" << WattsInterior << endl;

    EReal->SetName("E_real");
    EImag->SetName("E_imag");
    Watts->SetName("Power");

    vtkRectilinearGrid *rgrid = vtkRectilinearGrid::New();
        rgrid->SetDimensions(nx+1,ny+1,nz+1);
        rgrid->SetXCoordinates(xCoords);
        rgrid->SetYCoordinates(yCoords);
        rgrid->SetZCoordinates(zCoords);

        rgrid->GetCellData()->AddArray(EReal);
        rgrid->GetCellData()->AddArray(EImag);
        rgrid->GetCellData()->AddArray(Watts);
        //rgrid->Update();

    vtkXMLRectilinearGridWriter *gridWrite = vtkXMLRectilinearGridWriter::New();
        gridWrite->SetInputData(rgrid);
        gridWrite->SetFileName("ors.vtr");
        gridWrite->Write();
        //gridWrite->Update();

#endif
	EmEarth->Delete();
	receivers->Delete();
	earth->Delete();

#if  LEMMAUSEVTKX
	// Create the usual rendering stuff.
	vtkRenderer           *renderer = vtkRenderer::New();
	vtkRenderWindow         *renWin = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

	std::cout << "wire antennae approximating " << std::endl;
	loc << 0, .5*Width, Depth+.5*Height;
	pa->ApproximateWithElectricDipoles(loc);
    std::cout <<  "Wire approximated with " << pa->GetNumberOfDipoles() << std::endl;

    vtkActor **pdipActors = new vtkActor*[pa->GetNumberOfDipoles()];
	for (int id=0; id<pa->GetNumberOfDipoles(); ++id) {
		pdipActors[id] = pa->GetVtkActor(id);
		renderer->AddActor(pdipActors[id]);
	}

    /*
    vtkActor **dipActors = new vtkActor*[wire->GetNumberOfDipoles()];
	for (int id=0; id<wire->GetNumberOfDipoles(); ++id) {
		dipActors[id] = wire->GetVtkActor(id);
		renderer->AddActor(dipActors[id]);
	}
    */

	renderer->SetBackground(1,1,1);

	// Render the window
	renWin->AddRenderer(renderer);
	renWin->SetWindowName("Wire antennae");

	iren->SetRenderWindow(renWin);
	iren->Initialize();
	iren->Start();
	iren->Render();

    #if 0
	cout << "Enter File name?: ";
	std::string pngName;
	std::cin >> pngName;
	vtkPNGWriter *pngwrite = vtkPNGWriter::New();
	vtkRenderLargeImage *renlarge = vtkRenderLargeImage::New();
	renlarge->SetInput(renderer);
	renlarge->SetMagnification(2);
	pngwrite->SetInputConnection(renlarge->GetOutputPort());
	pngName.append(".png");
	pngwrite->SetFileName(pngName.c_str());
	pngwrite->Write();
    #endif

#endif     // -----  not LEMMA_USE_VTK  -----


	//std::cout << *pa << std::endl;
	//pa->Delete();

	return 0;
}
