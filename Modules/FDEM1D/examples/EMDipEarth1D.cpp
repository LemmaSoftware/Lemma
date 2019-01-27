// ===========================================================================
//
//       Filename:  utemdipearth1d.cpp
//
//    Description:
//
//        Version:  0.0
//        Created:  01/06/2010 04:27:56 PM
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
#include <FDEM1D>

using namespace Lemma;

int main() {

	// Test with a single dipole
	auto dipole = DipoleSource::NewSP();
		dipole->SetType(GROUNDEDELECTRICDIPOLE);
		dipole->SetPolarisation(XPOLARISATION);
		dipole->SetNumberOfFrequencies(1);
		dipole->SetMoment(1);
		dipole->SetFrequency(0, 4400.1000);
		//dipole->SetPhase(0);
		//dipole->SetLocation( (VectorXr(3) << 49, 49, -1e-4).finished() );
		dipole->SetLocation( 49, 49, -1e-4  );


	// Define model
	VectorXcr sigma(2);
		sigma << 0., 1e-3;//, .1;//, .01, .001;
	VectorXr  thick(1);
		thick << 10;//, 10, 10;

	auto earth = LayeredEarthEM::NewSP();
        earth->SetNumberOfLayers(2);
		earth->SetLayerConductivity(sigma);
		//earth->SetLayerThickness(thick);

	// Receivers
	auto receivers = FieldPoints::NewSP();
		Vector3r loc;
		//Real ox = -5.1456;
		//Real oy =  2.2350;
		Real ox =    50.;
		Real oy =    20.;
		Real depth = 18.10;
		Real dz = 2.6;
		int  nz = 1;

		receivers->SetNumberOfPoints(nz);
		int ir = 0;
		for (int iz=0; iz<nz; ++iz) {
			loc << ox, oy, depth;
            //std::cout << "Receiver location " << loc.transpose() << std::endl;
			receivers->SetLocation(ir, loc);
			depth += dz;
			++ ir;
		}
	//receivers->SetLocation(1, ox, oy, depth);

    auto EmEarth = EMEarth1D::NewSP();
        //EmEarth->SetHankelTransformMethod(DIGITALFILTERING);
        EmEarth->SetFieldsToCalculate(BOTH); // Fortran needs this
		EmEarth->AttachDipoleSource(dipole);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachFieldPoints(receivers);

        //dipole->SetType(ELECTRICDIPOLE);
//         receivers->SetNumberOfReceivers(1);
//         //for
// 	    receivers->SetLocation(0, ox, oy, depth);

    std::cout << "Dipole location " << dipole->GetLocation( ).transpose() << "\n";
    std::cout << "Receiver location " << receivers->GetLocation(0).transpose() << "\n";

	std::cout << "C++\n";
	EmEarth->MakeCalc3();
	std::cout << receivers->GetEfield(0,0) << std::endl;
	//std::cout << receivers->GetEfield(0) << std::endl;
	//std::cout << receivers->GetHfield(1) << std::endl;
	//std::cout << receivers->GetEfield(1) << std::endl;
	//receivers->ClearFields();

    // swap tx rx posigion
    /*
    receivers->SetLocation(0, dipole->GetLocation());
    dipole->SetLocation(loc);
	EmEarth->MakeCalc3();
	std::cout << receivers->GetEfield(0,0) << std::endl;
	receivers->ClearFields();
    */
    auto lc = receivers->GetEfield(0,0);

 #ifdef KIHALEE_EM1D
	receivers->ClearFields();
    std::cout << "\nFORTRAN KiHa\n";
 	EmEarth->MakeCalc();
    auto fc = receivers->GetEfield(0,0);
// 	std::cout << receivers->GetHfield(0,0) << std::endl;
 	std::cout << receivers->GetEfield(0,0) << std::endl;

    std::cout << "Difference norm |" << (lc - fc).norm() << "|" << std::endl;
 #endif

}
