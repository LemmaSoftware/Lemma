// ===========================================================================
//
//       Filename:  uthankel2.cpp
//
//    Description:
//
//        Version:  0.0
//        Created:  05/14/2010 10:26:59 AM
//       Revision:  none
//       Compiler:  Tested with g++, icpc, and MSVC 2000
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

#include "hankeltransformhankel2.h"
#include "hankeltransformgaussianquadrature.h"
#include "kernelem1d.h"
#include "layeredearthem.h"

using namespace Lemma;

int main() {

	// Define model
	LayeredEarthEM *Earth = LayeredEarthEM::New();
		VectorXcr sigma(3);
		sigma << 0., .1, 0.01;
		VectorXr  thick(1);
		thick << 10;
		Earth->SetNumberOfLayers(3);
		Earth->SetLayerConductivity(sigma);
		Earth->SetLayerThickness(thick);

	// Source
	// Test with a single dipole
	DipoleSource *dipole = DipoleSource::New();
		dipole->SetMoment(2);
		dipole->SetType(MAGNETICDIPOLE);
		dipole->SetPolarisation(ZPOLARISATION);
		dipole->SetLocation(0,0,0);
		dipole->SetNumberOfFrequencies(1);
		dipole->SetFrequency(0,1000);
		dipole->SetPhase(0);

	KernelEm1D *Kernel = KernelEm1D::New();
		Kernel->Initialise(Earth);
		Kernel->SetUpSource(dipole, 0);
		Kernel->SetUpReceiver(-0.4);
    //std::vector< KernelEm1DBase* > KernelVec;
    //    KernelVec.push_back( )

	std::cout.precision(10);

	Hankel2 *dig = Hankel2::New();
//         dig->ComputeRelated(.23 , Kernel);
//  		std::cout << "  dig->Zgauss "
//  				  << dig->Zgauss(0, TE, 0, 100.,
//  								  1000.*std::sqrt(MU0*EPSILON0), Kernel)
//  			      << std::endl;

// 	HankelTransformGaussianQuadrature *gauss =
// 			HankelTransformGaussianQuadrature::New();
// 		std::cout << "gauss->Zgauss "
// 				  << gauss->Zgauss(0, TE, 0, 100.,
// 								  1000.*std::sqrt(MU0*EPSILON0), Kernel)
// 			      << std::endl;

	Earth->Delete();
	dig->Delete();
	//gauss->Delete();
    dipole->Delete();
	Kernel->Delete();
	return EXIT_SUCCESS;
}
