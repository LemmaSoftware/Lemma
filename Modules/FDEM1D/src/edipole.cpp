// ===========================================================================
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

#include "receiverpoints.h"
#include "emearth1d.h"
#include "dipolesource.h"

using namespace Lemma;

std::vector<Real>  readinpfile(const std::string& fname);

int main(int argc, char** argv) {

    std::cout <<
    "\n"
    << "ediple.exe Copyright (C) 2011 Broken Spoke Development, LLC\n\n"
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
        std::cout << "usage: edipole.exe  dipole.inp cond.inp points.inp \n";
        exit(0);
    }

    std::vector<Real> Trans   = readinpfile(std::string(argv[1]));
    std::vector<Real> CondMod = readinpfile(std::string(argv[2]));
    std::vector<Real> Points  = readinpfile(std::string(argv[3]));

    //////////////////////////////////////
    // Define dipole source
    DipoleSource* dipole = DipoleSource::New();
        switch ( (int)(Trans[0]) ) {
            case 0:
                dipole->SetType(MAGNETICDIPOLE);
                break;
            case 1:
                dipole->SetType(GROUNDEDELECTRICDIPOLE);
                break;
            default:
                std::cerr << "Undefined dipole type, must be:\n"
                     << "\t0 -> Magnetic\n"
                     << "\t1 -> Grounded Electric\n";
                exit(EXIT_FAILURE);
        }

        switch ( (int)(Trans[1]) ) {
            case 0:
                dipole->SetPolarisation(XPOLARISATION);
                break;
            case 1:
                dipole->SetPolarisation(YPOLARISATION);
                break;
            case 2:
                dipole->SetPolarisation(ZPOLARISATION);
                break;
            default:
                std::cerr << "Undefined orientation, must be:\n"
                          << "\t0 -> x\n"
                          << "\t1 -> y\n"
                          << "\t2 -> z\n";
                exit(EXIT_FAILURE);
        }
        dipole->SetNumberOfFrequencies(1);
        dipole->SetFrequency(0, Trans[2]);
        dipole->SetLocation(Trans[3], Trans[4], Trans[5]);
        dipole->SetMoment(Trans[6]);

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
        int ip = 6;
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
		EmEarth->AttachDipoleSource(dipole);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachReceiverPoints(receivers);
		EmEarth->SetFieldsToCalculate(E);
        EmEarth->MakeCalc3();

    ////////////////////////////////////
    // Report
 	std::fstream hreal("efield.dat", std::ios::out);
    hreal << *dipole << std::endl;
    hreal << *earth << std::endl;
    hreal << "Right hand coordinate system, z is positive down\n\n";
    hreal << "x[m]\ty[m]\tz[m]\tRe(Ex) [V/m]\t Im(Ex) [V/m]\tRe(Ey) [V/m]\tIm(Ey) [V/m]\tRe(Ez) [V/m] Im(Ez) [V/m]\n";
    hreal << "\\\\ ====================================================================================================\n";
    hreal.precision(8);
    int i=0;
	for (int iz=0; iz<nz; ++iz) {
	for (int iy=0; iy<ny; ++iy) {
	for (int ix=0; ix<nx; ++ix) {
        hreal << receivers->GetLocation(i).transpose() << "\t";
 		hreal << std::real(receivers->GetEfield(0, i).transpose()[0]) << "\t";
 		hreal << std::imag(receivers->GetEfield(0, i).transpose()[0]) << "\t";
 		hreal << std::real(receivers->GetEfield(0, i).transpose()[1]) << "\t";
 		hreal << std::imag(receivers->GetEfield(0, i).transpose()[1]) << "\t";
 		hreal << std::real(receivers->GetEfield(0, i).transpose()[2]) << "\t";
 		hreal << std::imag(receivers->GetEfield(0, i).transpose()[2]) << "\n";
        ++i;
    }
    }
    }
    hreal.close();

    // Clean up
    EmEarth->Delete();
    earth->Delete();
    receivers->Delete();
    dipole->Delete();
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
