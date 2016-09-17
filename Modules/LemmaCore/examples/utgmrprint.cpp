// ===========================================================================
//   This file is distributed with Lemma,
//
//       Filename:  utsnmrinversion1d.cpp
//
//        Created:  10/07/2010 08:57:04 AM
//       Compiler:  Tested with g++, icpc, and MSVC 2010
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

/**
  @file
  @author   Trevor Irons
  @date     10/07/2010
  @version   0.0
 **/

#ifdef LEMMAUSEVTK
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPNGWriter.h"
#include "vtkRenderLargeImage.h"
#endif

#include "receiverpoints.h"
#include "emearth1d.h"
#include "PolygonalWireAntenna.h"

using namespace Lemma;

std::vector<Real>  readinpfile(const std::string& fname);

int main(int argc, char** argv) {

    std::cout <<
    "\n"
    << "hantenna - a programme for computing the h field from polygonal wire\n"
    << "loop sources \n\n"
    << "The following copyrights apply to this application:\n"
    << "Copyright (C) 2009, 2010, 2011, 2012 Colorado School of Mines\n"
    << "Copyright (C) 2009, 2010, 2011, 2012 Trevor Irons\n"
    << "Copyright (C) 2011 Broken Spoke Development, LLC\n\n"
    << "hantenna was built using Lemma (Lemma is an Electromagnetics Modelling API)\n"
    << "More information may be found at https://lemmasoftware.org\n\n"
    << "This program is free software: you can redistribute it and/or modify\n"
    << "it under the terms of the GNU General Public License as published by\n"
    << "the Free Software Foundation, either version 3 of the License, or\n"
    << "(at your option) any later version.\n\n"

    << "This program is distributed in the hope that it will be useful,\n"
    << "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    << "GNU General Public License for more details.\n\n"

    << "You should have received a copy of the GNU General Public License\n"
    << "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n";

    if (argc < 4) {
        std::cout << "usage: hantenna.exe  trans.inp cond.inp points.inp \n";
        exit(0);
    }

    std::vector<Real> Trans   = readinpfile(std::string(argv[1]));
    std::vector<Real> CondMod = readinpfile(std::string(argv[2]));
    std::vector<Real> Points  = readinpfile(std::string(argv[3]));

    //////////////////////////////////////
    // Define transmitter
    PolygonalWireAntenna* trans = PolygonalWireAntenna::New();
        trans->SetNumberOfPoints((int)(Trans[0]));
        int ip=1;
        for ( ; ip<=(int)(Trans[0])*2; ip+=2) {
            trans->SetPoint(ip/2, Vector3r (Trans[ip], Trans[ip+1], -1e-3));
        }
 	    trans->SetNumberOfFrequencies(1);
 	    trans->SetFrequency(0, Trans[ip]);
        trans->SetCurrent(Trans[ip+1]);

	// Receivers
 	ReceiverPoints *receivers = ReceiverPoints::New();
        int nx = (int)Points[0];
        int ny = (int)Points[1];
        int nz = (int)Points[2];
        Real ox = Points[3];
        Real oy = Points[4];
        Real oz = Points[5];
     	Vector3r loc;
        VectorXr dx(nx-1); // TODO map the dx, dy, dz vectors
        VectorXr dy(ny-1);
        VectorXr dz(nz-1);
        ip = 6;
        int ir = 0;
        for ( ; ip <6+nx-1; ++ip) {
            dx[ir] = Points[ip];
            ++ir;
        }
        ir = 0;
        for ( ; ip <6+ny-1+nx-1; ++ip) {
            dy[ir] = Points[ip];
            ++ir;
        }
        ir = 0;
        for ( ; ip <6+nz-1+ny-1+nx-1; ++ip) {
            dz[ir] = Points[ip];
            ++ir;
        }
 		receivers->SetNumberOfReceivers(nx*ny*nz);
 		ir = 0;
        Real pz  = oz;
 		for (int iz=0; iz<nz; ++iz) {
 		    Real py    =  oy;
 		    for (int iy=0; iy<ny; ++iy) {
 		        Real px    =  ox;
 		        for (int ix=0; ix<nx; ++ix) {
 			        loc << px, py, pz;
 			        receivers->SetLocation(ir, loc);
 			        if (ix < nx-1) px += dx[ix];
 			        ++ ir;
     	        }
                if (iy<ny-1) py += dy[iy];
            }
            if (iz<nz-1) pz += dz[iz];
        }

    ////////////////////////////////////
    // Define model
    LayeredEarthEM *earth = LayeredEarthEM::New();
    VectorXcr sigma;
    VectorXr  thick;
 	earth->SetNumberOfLayers(CondMod[0]+1);
 	sigma.resize(CondMod[0]+1); sigma(0) = 0; // airlayer
    thick.resize(CondMod[0]-1);
    int ilay=1;
    for ( ; ilay/2<CondMod[0]-1; ilay+=2) {
        sigma(ilay/2+1) =  1./CondMod[ilay];
        thick(ilay/2) =  CondMod[ilay+1];
    }
    sigma(ilay/2+1) = 1./ CondMod[ilay];
	earth->SetLayerConductivity(sigma);
    if (thick.size() > 0) earth->SetLayerThickness(thick);

	EMEarth1D  *EmEarth = EMEarth1D::New();
		EmEarth->AttachWireAntenna(trans);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachReceiverPoints(receivers);
		EmEarth->SetFieldsToCalculate(H);
        EmEarth->CalculateWireAntennaFields();

    #ifdef LEMMAUSEVTK
	vtkRenderer           *renderer = vtkRenderer::New();
	vtkRenderWindow         *renWin = vtkRenderWindow::New();
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    trans->ApproximateWithElectricDipoles( (VectorXr (3) << 0,0,0).finished() );
	vtkActor **dipActors = new vtkActor*[trans->GetNumberOfDipoles()];
	for (int id=0; id<trans->GetNumberOfDipoles(); ++id) {
		dipActors[id] = trans->GetVtkActor(id);
		renderer->AddActor(dipActors[id]);
	}
    vtkActor *hfield  = receivers->GetVtkGlyphActor (HFIELDREAL, 3, 1e2, 0);
 	vtkActor *hfieldi = receivers->GetVtkGlyphActor (HFIELDIMAG, 3, 1e2, 0);
    hfieldi->GetProperty()->SetColor(1,0,1);
 	hfield->GetProperty()->SetColor(0,1,0);
 	renderer->AddActor(hfield);
 	renderer->AddActor(hfieldi);
	renderer->SetBackground(0,0,0);
	// Render the window
	renWin->AddRenderer(renderer);
	iren->SetRenderWindow(renWin);
	iren->Initialize();
	iren->Start();
	iren->Render();
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
	iren->Delete();
	renWin->Delete();
	renderer->Delete();
    #endif

    ////////////////////////////////////
    // Report
 	std::fstream hreal("hfield.dat", std::ios::out);
    hreal << *trans << std::endl;
    hreal << *earth << std::endl;
    hreal << "Right hand coordinate system, z is positive down\n";
    hreal << "x[m]\ty[m]\tz[m]\tHx[A/m]\tHy[A/m]\tHz[A/m]\n";
    hreal.precision(8);
    int i=0;
	for (int iz=0; iz<nz; ++iz) {
	for (int iy=0; iy<ny; ++iy) {
	for (int ix=0; ix<nx; ++ix) {
        hreal << receivers->GetLocation(i).transpose() << "\t";
 		hreal << std::real(receivers->GetHfield(0, i).transpose()[0]) << "\t";
 		hreal << std::imag(receivers->GetHfield(0, i).transpose()[0]) << "\t";
 		hreal << std::real(receivers->GetHfield(0, i).transpose()[1]) << "\t";
 		hreal << std::imag(receivers->GetHfield(0, i).transpose()[1]) << "\t";
 		hreal << std::real(receivers->GetHfield(0, i).transpose()[2]) << "\t";
 		hreal << std::imag(receivers->GetHfield(0, i).transpose()[2]) << "\n";
        ++i;
    }
    }
    }
    hreal.close();

    // Clean up
    EmEarth->Delete();
    earth->Delete();
    receivers->Delete();
    trans->Delete();
}

std::vector<Real>  readinpfile(const std::string& fname) {
    std::string buf;
    char dump[255];
    std::vector<Real> vals;
    std::fstream input(fname.c_str(), std::ios::in);
    if (input.fail()) {
        std::cerr << "Input file " << fname << " failed to open\n";
        exit(EXIT_FAILURE);
    }
    while (input >> buf) {
        if (buf.substr(0,2) == "//") {
            input.getline(dump, 255);
        } else {
            vals.push_back( atof(buf.c_str() ));
        }
    }
    return vals;
}
