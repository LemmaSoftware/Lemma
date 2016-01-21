// ===========================================================================
// 
//       Filename:  uttemreader.cpp
// 
//    Description:  
// 
//        Version:  0.0
//        Created:  02/19/2011 12:30:59 PM
//       Revision:  none
//       Compiler:  Tested with g++, icpc, and MSVC 2000 
// 
//         Author:  M. Andy Kass (MAK)
//
//   Organisation:  Colorado School of Mines (CSM)
//                  Broken Spoke Brewery, LLC
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

using namespace Lemma;


int main() {

	/*
	ModelReaderTem1DUBC* Reader = ModelReaderTem1DUBC::New();
	PolygonalWireAntenna* Trans = PolygonalWireAntenna::New();
	ReceiverPoints *Receivers = ReceiverPoints::New();
	LayeredEarthEM *Earth = LayeredEarthEM::New();
	Reader->SetEMEarth1D(Earth);
	Reader->SetTransmitter(Trans);
	Reader->SetReceiver(Receivers);
	Reader->ReadParameters("em1dtmfwd.in");

	Reader->Delete();
	Earth->Delete();
	Receivers->Delete();
	Trans->Delete();
	*/

	int rnObs;
	int rnGates;
	std::string inputfile;
	MatrixXr somedata;
	Vector3Xr somepositions;

	DataTEM* TheData = DataTEM::New();
	DataReaderTem* Reader = DataReaderTem::New();

	inputfile = "temreadertest.txt";

	Reader->SetDataTEM(TheData);
	Reader->ReadData(inputfile);

	rnObs = TheData->GetnObs();
	rnGates = TheData->GetnGates();
	somedata = TheData->GetData();
	somepositions = TheData->GetPositions();

	std::cout << rnObs << " " << rnGates << std::endl;
	std::cout << somedata << std::endl;
	std::cout << somepositions << std::endl;

	Reader->Delete();
	TheData->Delete();

	return EXIT_SUCCESS;
}
