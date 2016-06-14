// ===========================================================================
// 
//       Filename:  utfemforward.cpp
// 
//    Description:  
// 
//        Version:  0.0
//        Created:  01/15/2013 12:11:34 PM
//       Revision:  none
//       Compiler:  Tested with g++ 
// 
//         Author:  M. Andy Kass (MAK)
//
//   Organisation:  Broken Spoke Development, LLC
//                  
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

#include <time.h>
#include "Lemma"
#include "banner.h"

using namespace Lemma;

#ifdef LEMMAUSEVTK
#include "matplot.h"
using namespace matplot;
#endif

int main() {

	clock_t launch = clock();

	// Banner display
	std::string name;
	std::string version;
	std::string usage;
	name = "FEM Forward Modeller - 1D ";
	version = "1.0alpha";
	usage = "utfemforward [inputfile]";
	banner(name,version,usage);

	time_t curr = time(0);
	std::cout << std::endl << "  Start time: " << ctime(&curr) << std::endl;

	// Define some objects
	DipoleSource* Trans = DipoleSource::New();
	ReceiverPoints* Receivers = ReceiverPoints::New();
	LayeredEarthEM* Earth = LayeredEarthEM::New();



	// SINGLE SOUNDING

	// Create new data object to receive forward model
	DataFEM* modelledData = DataFEM::New();

	// Create instrument object
	InstrumentFem* theinstrument = InstrumentFem::New();	

	// Set up EMEarthModel
	Earth->SetNumberOfLayers(5);
	Earth->SetLayerConductivity((VectorXcr(5) << 0.,1.e-4,1.e-2,
		1.e-4,1.e-6).finished());
	Earth->SetLayerThickness((VectorXr(3) << 20.,5.,50.).finished());
	
	// Set up transmitter
	Real momtemp;
	momtemp = 1;
	Real freq;
	freq = 391;
	Trans->SetType(MAGNETICDIPOLE);
	Trans->SetLocation(0,0,-1.e-3);
	Trans->SetPolarisation(ZPOLARISATION);
	// as a test
	Trans->SetNumberOfFrequencies(1);
	Trans->SetFrequency(0,freq);
	Trans->SetMoment(momtemp);

	// Set up receivers
	Vector3r loc1;
	Vector3r loc2;
	Receivers->SetNumberOfReceivers(1);
	loc1 << 9,0,-1.e-3;
	Receivers->SetLocation(0,loc1);


	// Attach to instrument
	theinstrument->SetReceiverPoints(Receivers);
	theinstrument->SetDipoleSource(Trans);
	theinstrument->EMEarthModel(Earth);
	theinstrument->SetOutputData(modelledData);

	// Calculate
	theinstrument->MakeCalculation();

	// See what comes back...
	std::cout << Receivers->GetHfield(0,0)(2) << std::endl;
	
	// Need to convert H field to ppm


	theinstrument->Delete();
	modelledData->Delete();

	// MULTIPLE SOUNDING MODEL USING READ-IN DATA


	InstrumentFem* theinstrument2 = InstrumentFem::New();
	DataFEM* modelledData2 = DataFEM::New();

	// Using the UBC data reader to read in the survey params
	// this will be used for the multiple sounding testing
	std::string datfile;
	datfile = "126.obs";

	DataReaderFemUBC* Reader = DataReaderFemUBC::New();
	DataFEM* inpdata = DataFEM::New();
	Reader->SetDataFEM(inpdata);
	try {
		Reader->ReadData(datfile,1);
	} catch(std::exception& e) {
		exit(EXIT_FAILURE);
	}
	std::cout << *inpdata << std::endl;
	theinstrument2->SetOutputData(modelledData2);
	theinstrument2->AlignWithData(inpdata);
	// Now, how do I want to pass it an earth model for each sounding?
	//
	inpdata->Delete();




	// Need to loop over transmitters
	// The relative positions of the transmitter and receiver is fixed
	// For testing, do some kind of random realisation of Earth model?
//	int nObs;
//	int nFreqs;
//	nObs = inpdata->GetnObs();
//	nFreqs = inpdata->GetnFreq();

//	Trans->SetType(MAGNETICDIPOLE);
//	Trans->SetLocation(0,0,-1e-3);

//	Vector3r loc;
//	Receivers->SetNumberOfReceivers(1);
//	loc << 0,0,-1e-3;
//	Receivers->SetLocation(0,loc);

//	Earth->SetNumberOfLayers(5); //Including the two halfspaces
//	Earth->SetLayerConductivity((VectorXcr(5) << 0.,1.e-4,1.e-2,
//		1.e-4,1.e-6).finished());
//	Earth->SetLayerThickness((VectorXr(3) << 20.,5.,50.).finished());

//	for (int ii=0;ii<nObs;ii++) {
//		for (int jj=0;jj<nFreqs;jj++) {
//			Trans->SetMoment(inpdata->GetTxMom().coeff(jj));
//			
//		}
//	}
	
	//This is a bunch of testing stuff
	//theinstrument->AlignWithData(inpdata);
	//theinstrument->MakeCalculation();
	//inpdata->Delete();
	//theinstrument->MakeCalculation();


	theinstrument2->Delete();
	Reader->Delete();
	Earth->Delete();
	Receivers->Delete();
	Trans->Delete();

	// Timey-wimey stuff
	Real time;
	clock_t done = clock();
	time = (done-launch)/CLOCKS_PER_SEC;
	std::cout << "  Execution time: " << time << " [CPU] seconds."
		<< std::endl;

	return EXIT_SUCCESS;
}
