// ===========================================================================
//
//       Filename:  utteminv1d.cpp
//
//    Description:  Invert for the difference in water table
//
//        Version:  0.0
//        Created:  03/23/2011 02:07:19 PM
//       Revision:  none
//       Compiler:  Tested with g++, icpc, and MSVC 2000
//
//         Author:  M. Andy Kass (MAK)
//
//   Organisation:  Colorado School of Mines (CSM)
//                  Broken Spoke Development, LLC
//
//          Email:  mkass@numericalgeo.com
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

#include "Lemma"
#include "banner.h"

using namespace Lemma;

int main () {

	VectorXr maintimes;
	int ntimes;

	//Banner
	std::string name;
	std::string version;
	std::string usage;
	name = "TEM Inversion - 1D";
	version = "1.0beta";
	usage = "teminv1d [inputfile]";
	banner(name,version,usage);

	PolygonalWireAntenna* Trans = PolygonalWireAntenna::New();
	ReceiverPoints* Receivers = ReceiverPoints::New();
	LayeredEarthEM* StartingMod = LayeredEarthEM::New();
	//LayeredEarthEM* RecoveredMod = LayeredEarthEM::New();
	InverseSolverTEM1D* Inverse = InverseSolverTEM1D::New();


	Trans->SetNumberOfPoints(5);
    Trans->SetPoint(0, Vector3r(   0,   0, -1e-3));
    Trans->SetPoint(1, Vector3r( 100,   0, -1e-3));
    Trans->SetPoint(2, Vector3r( 100, 100, -1e-3));
    Trans->SetPoint(3, Vector3r(   0, 100, -1e-3));
    Trans->SetPoint(4, Vector3r(   0,   0, -1e-3));
	Trans->SetCurrent(1);
	Trans->SetNumberOfTurns(1);
    Trans->SetMinDipoleRatio(1/2.);

	Vector3r loc;
	Real ox = 50.;
	Real oy = 50.;
	Real depth = -1.e-3;
	Receivers->SetNumberOfReceivers(1);
	loc << ox,oy,depth;
	Receivers->SetLocation(0,loc);

	StartingMod->SetNumberOfLayers(22);
	StartingMod->SetLayerConductivity( (VectorXcr(22) << 0.,1.e-3,
		1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,
		1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3,1.e-3).finished() );
	StartingMod->SetLayerThickness((VectorXr(20)<<4,4,4,4,4,4,4,4,4,4,4,4,4,
		4,4,4,4,4,4,4).finished());
	//EM-47 gate times
	ntimes = 30;
	maintimes.resize(ntimes);
	maintimes << 36.,45.25,57.,72.25,92.,117.,148.,186.5,234.,290.,
		352.5,427.5,525.,647.5,802.5,1002.5,1257.5,1582.5,1997.5,2525.,
		3197.5,4055.,5147.5,6542.5,8322.5,10592.,13490.,17187.,21902.,
		27915.;
	maintimes = maintimes.array() /1000000;

	Inverse->AttachStartMod(StartingMod);
	Inverse->AttachWireAntenna(Trans);
	Inverse->AttachReceiver(Receivers);
	Inverse->SetTimes(maintimes);
	LayeredEarthEM* RecoveredMod = StartingMod->Clone();
	Inverse->AttachRecMod(RecoveredMod);

	//Read input data - temp for SEG!
	/// TODO write a TEM data reader class.  and a functional TEM data class
	std::fstream infile("rotateddat.dat",std::ios::in);
	if (infile.fail()) {
		std::cout << "Shit!" << std::endl;
	}
	MatrixXr rotinpdata;
	rotinpdata.resize(30,1);
	for (int ii=0;ii<30;ii++) {
		infile >> rotinpdata(ii);
	}
	infile.close();


	Inverse->AttachMeasuredData(rotinpdata);
	//Inverse->SetFreeParams( (VectorXi(2)<<0,1).finished(),
	//	(VectorXi(3)<<1,2,3).finished());

	std::cout << *Inverse << std::endl;

	Inverse->Calculate();
	//Inverse->ShowSoln();
	//Inverse->WriteSoln("temp.dat");

	Inverse->Delete();
	RecoveredMod->Delete();
	StartingMod->Delete();
	Receivers->Delete();
	Trans->Delete();

	return EXIT_SUCCESS;
}
