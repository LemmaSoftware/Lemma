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
	pa->SetFrequency(0, 1000);
	pa->SetNumberOfPoints(5);
	pa->SetPoint(0, Vector3r(   0,   0, -.001));
	pa->SetPoint(1, Vector3r( 100,   0, -.001));
	pa->SetPoint(2, Vector3r( 100, 100, -.001));
	pa->SetPoint(3, Vector3r(   0, 100, -.001));
	pa->SetPoint(4, Vector3r(   0,   0, -.001));
	pa->SetCurrent(1.);
    pa->SetNumberOfTurns(1);
	//Vector3r rp = Vector3r::Random(3);
	//rp << 150., 10, 0.;
	//rp << -27.1456, 15.2350, -1e-3;
	//rp << randDouble(-35,35), randDouble(-35,35), randDouble(-35,35);
	//rp << 3.22806, -13.1548, 14.9695;
	//rp.setRandom(3);
	//std::cout <<  "rp " << rp.transpose() << std::endl;
	//pa->ApproximateWithElectricDipoles(rp);

	WireAntenna *wire = WireAntenna::New();
		wire->SetNumberOfPoints(5);
		wire->SetPoint(0, Vector3r(   0,   0, -1e-3));
		wire->SetPoint(1, Vector3r( 10,   0, -1e-3));
		wire->SetPoint(2, Vector3r( 10, 10, -1e-3));
		wire->SetPoint(3, Vector3r(   0, 10, -1e-3));
		wire->SetPoint(4, Vector3r(   0,   0, -1e-3));
		// TODO change wire antennae to use my class
		//wire->SetNumberOfFrequencies(1);
		wire->SetCurrent(1.);
		wire->SetNumberOfFrequencies(1);
		wire->SetFrequency(0, 1000);
        wire->SetNumberOfTurns(1);
		//wire->ApproximateWithElectricDipoles(5);

	// Define model
	VectorXcr sigma(2);
		sigma << Complex(0.,0), Complex(.1,0);
	VectorXr  thick(1);
		thick << 10;
	LayeredEarthEM *earth = LayeredEarthEM::New();
		earth->SetNumberOfLayers(2);
		earth->SetLayerConductivity(sigma);
		//earth->SetLayerThickness(thick);

	// Receivers
	ReceiverPoints *receivers = ReceiverPoints::New();
		Vector3r loc;
		Real ox    =    50.561 ;
		Real oy    =   105.235 ;
		Real depth =    -3.75e1;
		Real depth2 =   depth;
		Real dx    =    1.;
		int nz     =    1;
		receivers->SetNumberOfReceivers(nz);
		int ir = 0;
		for (int iz=0; iz<nz; ++iz) {
			loc << ox, oy, depth;
			receivers->SetLocation(ir, loc);
			depth += dx;
			++ ir;
		}

	// EmEarth
	EMEarth1D  *EmEarth = EMEarth1D::New();
		//EmEarth->AttachWireAntenna(wire);
		EmEarth->AttachWireAntenna(pa);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachReceiverPoints(receivers);
		EmEarth->SetFieldsToCalculate(H);
        //EmEarth->SetHankelTransformMethod(GAUSSIANQUADRATURE);

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

	depth = depth2;
	std::fstream real("reale_lay.dat", std::ios::out);
	std::fstream imag("image_lay.dat", std::ios::out);
	std::fstream hreal("real_lay.dat", std::ios::out);
	std::fstream himag("imag_lay.dat", std::ios::out);
	for (int iz=0; iz<nz; ++iz) {
		Vector3cr temp = receivers->GetEfield(0,iz);
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
		depth += dx;
	}
	real.close();
	imag.close();
	hreal.close();
	himag.close();
	EmEarth->Delete();
	receivers->Delete();
	earth->Delete();
	//wire->Delete();

#if  LEMMAUSEVTK
	// Create the usual rendering stuff.
	vtkRenderer           *renderer = vtkRenderer::New();
	vtkRenderWindow         *renWin = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

	// Line of tx
	vtkLineSource       *vline = vtkLineSource::New();
	vtkTubeFilter       *vTube = vtkTubeFilter::New();
 	vtkPolyDataMapper   *vMapper = vtkPolyDataMapper::New();
	vtkActor             *vActor = vtkActor::New();
	vline->SetPoint1(0,0,0);
	vline->SetPoint2(10,0,0);

	vTube->SetInputConnection(vline->GetOutputPort());
	vTube->SetRadius(.2);
	vTube->SetNumberOfSides(6);
	vMapper->SetInputConnection(vTube->GetOutputPort());
	vActor->SetMapper(vMapper);
	vActor->GetProperty()->SetColor(0.0, .0, 1.0);
	vActor->GetProperty()->SetOpacity(.15);
	renderer->AddActor(vActor);

	vtkLineSource       *vline2 = vtkLineSource::New();
	vtkTubeFilter       *vTube2 = vtkTubeFilter::New();
 	vtkPolyDataMapper   *vMapper2 = vtkPolyDataMapper::New();
	vtkActor             *vActor2 = vtkActor::New();
	vline2->SetPoint1(10,0,0);
	vline2->SetPoint2(10,10,0);

	vTube2->SetInputConnection(vline2->GetOutputPort());
	vTube2->SetRadius(.2);
	vTube2->SetNumberOfSides(6);
	vMapper2->SetInputConnection(vTube2->GetOutputPort());
	vActor2->SetMapper(vMapper2);
	vActor2->GetProperty()->SetColor(0.0, .0, 1.0);
	vActor2->GetProperty()->SetOpacity(.15);
	renderer->AddActor(vActor2);

	vtkLineSource       *vline3 = vtkLineSource::New();
	vtkTubeFilter       *vTube3 = vtkTubeFilter::New();
 	vtkPolyDataMapper   *vMapper3 = vtkPolyDataMapper::New();
	vtkActor             *vActor3 = vtkActor::New();
	vline3->SetPoint1(10,10,0);
	vline3->SetPoint2(0,10,0);

	vTube3->SetInputConnection(vline3->GetOutputPort());
	vTube3->SetRadius(.2);
	vTube3->SetNumberOfSides(6);
	vMapper3->SetInputConnection(vTube3->GetOutputPort());
	vActor3->SetMapper(vMapper3);
	vActor3->GetProperty()->SetColor(0.0, .0, 1.0);
	vActor3->GetProperty()->SetOpacity(.15);
	renderer->AddActor(vActor3);

	vtkLineSource       *vline4 = vtkLineSource::New();
	vtkTubeFilter       *vTube4 = vtkTubeFilter::New();
 	vtkPolyDataMapper   *vMapper4 = vtkPolyDataMapper::New();
	vtkActor             *vActor4 = vtkActor::New();
	vline4->SetPoint1(0,10,0);
	vline4->SetPoint2(0,0,0);

	vTube4->SetInputConnection(vline4->GetOutputPort());
	vTube4->SetRadius(.2);
	vTube4->SetNumberOfSides(6);
	vMapper4->SetInputConnection(vTube4->GetOutputPort());
	vActor4->SetMapper(vMapper4);
	vActor4->GetProperty()->SetColor(0.0, .0, 1.0);
	vActor4->GetProperty()->SetOpacity(.15);
	renderer->AddActor(vActor4);

    loc << 50, 50, -1e-3;
	pa->ApproximateWithElectricDipoles(loc);

    vtkActor **pdipActors = new vtkActor*[pa->GetNumberOfDipoles()];
    std::cout <<  "Wire approximated with " << pa->GetNumberOfDipoles() << std::endl;
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
