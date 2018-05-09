// ===========================================================================
//
//       Filename:  hantenna.cpp
//
//        Created:  10/07/2010 08:57:04 AM
//       Modified:  11 April 2018
//       Compiler:  Tested with g++, icpc, and MSVC 2017
//
//         Author:  Trevor Irons (ti)
//
//       Copyright (C) 2012,2018    Trevor Irons
//
//   Organisation:  Lemma Software
//
//          Email:  Trevor.Irons@lemmasoftware.org
//
// ===========================================================================

/**
  @file
  @author   Trevor Irons
  @date     10/07/2010
  $Id$
 **/

#include "LemmaCore"
#include "FDEM1D"
#include "timer.h"

#if defined(__clang__)
	/* Clang/LLVM. ---------------------------------------------- */
    const char* compiler = "clang";
    const char* ver = __VERSION__;
#elif defined(__ICC) || defined(__INTEL_COMPILER)
	/* Intel ICC/ICPC. ------------------------------------------ */
    const char* compiler = "icpc";

#elif defined(__GNUC__) || defined(__GNUG__)
	/* GNU GCC/G++. --------------------------------------------- */
    const char* compiler = "gcc (GCC) ";//  __VERSION__;
    const char* ver = __VERSION__;
#elif defined(_MSC_VER)
	/* Microsoft Visual Studio. --------------------------------- */
    const char* compiler = "msvc ";
    const int ver = _MSC_FULL_VER;

#elif defined(__PGI)
	/* Portland Group PGCC/PGCPP. ------------------------------- */
    const char* compiler = "pgc";
#endif

using namespace Lemma;

std::vector<Real>  readinpfile(const std::string& fname);

std::vector<std::string>  readinpfile2(const std::string& fname);

int main(int argc, char** argv) {

const char *buildString = __DATE__ ", " __TIME__;
    std::cout
    << "===========================================================================\n"
    << "Lemma " << LEMMA_VERSION << "\n"
    << "[" << compiler << " " << ver << " " <<  buildString << "]\n"
    << "This program is part of Lemma, a geophysical modelling and inversion API. \n"
    << "     This Source Code Form is subject to the terms of the Mozilla Public\n"
    << "     License, v. 2.0. If a copy of the MPL was not distributed with this\n"
    << "     file, You can obtain one at http://mozilla.org/MPL/2.0/. \n"
    << "Copyright (C) 2018 Lemma Software \n"
    << "More information may be found at: https://lemmasoftware.org\n"
    << "                                     info@lemmasoftware.org\n"
    << "===========================================================================\n\n"
    << "Hantenna calculates the harmonic H field from polygonal wire loop sources\n";

    if (argc < 5) {
        std::cout << "usage: hantenna.exe  trans.inp cond.inp points.inp config.inp \n";
        exit(0);
    }

    #ifdef LEMMAUSEOMP
    std::cout << "OpenMP is using " << omp_get_max_threads() << " threads" << std::endl;
    #endif

    std::vector<Real> Trans   = readinpfile(std::string(argv[1]));
    std::vector<Real> CondMod = readinpfile(std::string(argv[2]));
    std::vector<Real> Points  = readinpfile(std::string(argv[3]));
    std::vector<std::string> config  = readinpfile2(std::string(argv[4]));

    //////////////////////////////////////
    // Define transmitter
    auto trans = PolygonalWireAntenna::NewSP();
        trans->SetNumberOfPoints((int)(Trans[0]));
        int ip=1;
        for ( ; ip<=(int)(Trans[0])*2; ip+=2) {
            trans->SetPoint(ip/2, Vector3r (Trans[ip], Trans[ip+1], -1e-3));
        }
 	    trans->SetNumberOfFrequencies(1);
 	    trans->SetFrequency(0, Trans[ip]);
        trans->SetCurrent(Trans[ip+1]);
        trans->SetMinDipoleRatio(atof(config[1].c_str()));
        trans->SetMinDipoleMoment(atof(config[2].c_str()));
        trans->SetMaxDipoleMoment(atof(config[3].c_str()));

    /////////////////////////////////////
	// Field calculations
 	auto receivers = FieldPoints::NewSP();
        int nx = (int)Points[0];
        int ny = (int)Points[1];
        int nz = (int)Points[2];
        Real ox = Points[3];
        Real oy = Points[4];
        Real oz = Points[5];
     	Vector3r loc;
        VectorXr dx(nx-1);
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
 		receivers->SetNumberOfPoints(nx*ny*nz);
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
    auto earth = LayeredEarthEM::NewSP();
    VectorXcr sigma;
    VectorXr  thick;
 	earth->SetNumberOfLayers(static_cast<int>(CondMod[0])+1);
 	sigma.resize(static_cast<int>(CondMod[0])+1); sigma(0) = 0; // airlayer
    thick.resize(static_cast<int>(CondMod[0])-1);
    int ilay=1;
    for ( ; ilay/2<CondMod[0]-1; ilay+=2) {
        sigma(ilay/2+1) =  1./CondMod[ilay];
        thick(ilay/2) =  CondMod[ilay+1];
    }
    sigma(ilay/2+1) = 1./ CondMod[ilay];
	earth->SetLayerConductivity(sigma);
    if (thick.size() > 0) earth->SetLayerThickness(thick);

	auto EmEarth = EMEarth1D::NewSP();
		EmEarth->AttachWireAntenna(trans);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachFieldPoints(receivers);
		EmEarth->SetFieldsToCalculate(H);
        EmEarth->SetHankelTransformMethod(string2Enum<HANKELTRANSFORMTYPE>(config[0]));

    ///////////////////////////////////////////////
	// Keep track of time
	jsw_timer timer;
  	timer.begin();
    clock_t launch = clock();
    EmEarth->CalculateWireAntennaFields(true);    // true=status bar
	Real paTime = timer.end();

    std::cout << "\n\n===========================================\ncalc. real time: " << paTime/60. << "\t[m]\n";

    std::cout << "calc. user time: " <<  (clock()-launch)/CLOCKS_PER_SEC/60.   << "\t[CPU m]"
		 << std::endl;

    ////////////////////////////////////
    // Report
 	std::fstream hrep("hfield.yaml", std::ios::out);
    std::fstream hreal("hfield.dat", std::ios::out);

    hrep << *EmEarth << std::endl;
    hrep.close();
    //hreal << *trans << std::endl;
    //hreal << *earth << std::endl;

    hreal << "// Right hand coordinate system, z is positive down\n";
    hreal << "// x[m]\ty[m]\tz[m]\tHx[A/m]\tHy[A/m]\tHz[A/m]\n";
    hreal.precision(8);
    int i=0;
	for (int iz=0; iz<nz; ++iz) {
	for (int iy=0; iy<ny; ++iy) {
	for (int ix=0; ix<nx; ++ix) {
        hreal << receivers->GetLocation(i).transpose() << "\t";
 		//hreal << receivers->GetHfield(0, i).transpose() << "\n"; ( complex, notation )
 		hreal << receivers->GetHfield(0, i).transpose().real() << "\t";
 		hreal << receivers->GetHfield(0, i).transpose().imag() << "\n";
        ++i;
    }
    }
    }
    hreal.close();
    // Clean up
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

std::vector<std::string>  readinpfile2(const std::string& fname) {
    std::string buf;
    char dump[255];
    std::vector<std::string> vals;
    std::fstream input(fname.c_str(), std::ios::in);
    if (input.fail()) {
        std::cerr << "Input file " << fname << " failed to open\n";
        exit(EXIT_FAILURE);
    }
    while (input >> buf) {
        if (buf.substr(0,2) == "//") {
            input.getline(dump, 255);
        } else {
            vals.push_back( std::string(buf.c_str() ));
        }
    }
    return vals;
}
