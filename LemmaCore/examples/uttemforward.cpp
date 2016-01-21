// ===========================================================================
//
//       Filename:  uttemforward.cpp
//
//    Description:  TEM Forward Modeller
//
//        Version:  0.0
//        Created:  02/10/2011 03:32:18 PM
//       Revision:  none
//       Compiler:  Tested with g++, icpc, and MSVC 2000
//
//         Author:  M. Andy Kass (MAK)
//
//		Copyright:  2011 Trevor Irons and M. Andy Kass
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

#include <time.h>
#include "Lemma"
#include "banner.h"

using namespace Lemma;

#ifdef LEMMAUSEVTK
#include "matplot.h"
using namespace matplot;
#endif

int main(int argv, char** argc) {

	// Just for timing
	clock_t launch =clock();

	// Banner display
	std::string name;
	std::string version;
	std::string usage;
	name = "TEM Forward Modeller - 1D";
	version = "1.0beta";
	usage = "temforward1d [inputfile]";
	banner(name,version,usage);
    if ( argv < 2 ) {
        std::cout << "No input file specified, should be of format\n";
        std::cout <<
"origparabk.obsy        ! Observations-style file.\n\
origpara.con           ! Starting model.\n\
100                    ! Number of Hankel kernel evaluations.\n\
100 1. 1.0E+10         ! Number of Fourier kernel evaluations, & min & max freq.\n\
N                      ! Is noise to be added?\n\
5.  1.0E-08  30        ! Noise to be added: percentage, threshold, seed.\n\
" << std::endl;

        exit(0);
    }

	time_t curr=time(0);
	std::cout << std::endl;
	std::cout << "  Start time: " << ctime(&curr) << std::endl;

	// Define objects
	PolygonalWireAntenna* Trans = PolygonalWireAntenna::New();
//     PolygonalWireAntenna* pa = PolygonalWireAntenna::New();
//         pa->SetNumberOfPoints(5);
// 	    pa->SetPoint(0, Vector3r(   0,   0, -30.001));
// 	    pa->SetPoint(1, Vector3r(   0,  20, -35.001));
// 	    pa->SetPoint(2, Vector3r(  20 , 20, -35.001));
// 	    pa->SetPoint(3, Vector3r(  20,   0, -30.001));
// 	    pa->SetPoint(4, Vector3r(   0,   0, -30.001));
// 	    pa->SetCurrent(100.);
//         pa->SetNumberOfTurns(1);


	//DipoleSource* Trans = DipoleSource::New();
	ReceiverPoints *Receivers = ReceiverPoints::New();
	LayeredEarthEM *Earth = LayeredEarthEM::New();
	VectorXr maintimes;

    /////////////////////////////////////
	// Using model reader
	ModelReaderTem1DUBC* Reader = ModelReaderTem1DUBC::New();
	    Reader->SetEMEarth1D(Earth);
	    Reader->SetTransmitter(Trans);
	    Reader->SetReceiver(Receivers);
	    Reader->ReadParameters( argc[1] );
	    maintimes = Reader->GetTimes();

	// Attach it to instrumentTEM
	InstrumentTem *instrument = InstrumentTem::New();
	    instrument->EMEarthModel(Earth);
	    instrument->SetTransmitLoop(Trans);
	    //instrument->SetTransmitLoop(pa);
	    //instrument->SetDipoleSource(Trans);
	    instrument->SetReceiver(Receivers);



        // TODO need to input these
        VectorXr widths = VectorXr::Ones(maintimes.size()); // * 1e-5;
        /*
        widths.segment(0,5).array()  *= 5e-6;
        widths.segment(5,5).array()  *= 1e-5;
        widths.segment(10,5).array() *= 5e-5;
        widths.segment(15,5).array() *= 2e-4;
        widths.segment(20,5).array() *= 4e-4;
        //widths.segment(25,5).array() *= 4e-4;
        */
        /*
        widths << .48,4.,8.,8.,8.,8.,8.,12.,12.,12.,16.,16.,16.,20.,24.,24.,28.,32.,32.,36.,40.,44.,52.,
  				56.,64.,68.,76.,88.,96.,104.,120.,132.,144.,160.,180.,200.,220.,248.,272.,304.,336.,376.,
  				416.,464.,516.,572.,636.,708.,784.,872.,968.,1076.,1196.,1328.,1476.,1640.,1824.,2024.,
  				2248.,2500. ;
        */
        widths *= 1e-6;
        std::cout << widths.transpose() << std::endl;

        instrument->SetReferenceTime(.025);
        //instrument->SetReferenceTime(.025);
	    instrument->SetTimeGates(maintimes, widths);

        //instrument->SetReceiverType(MAGNETOMETER);
        instrument->SetReceiverType(INDUCTIVE);
        // Set the pulse
        VectorXr Amp(4); Amp << 0,1,1,0;
        VectorXr Times(4); Times << 0., .0005, .02497, .025;

        //VectorXr Amp(4); Amp << 0,1,1,0;
        //VectorXr Times(4); Times << -0.025, -0.0245, -0.00003, 0.0;
        //instrument->SetPulse(Amp, Times);

        //VectorXr Amp(3); Amp << 0,1,0;
        //VectorXr Times(3); Times << 0, .015, .030;
        //instrument->SetPulse(Amp, Times);

        //VectorXr Amp(3); Amp << 0,1,0;
        //VectorXr Times(3); Times << 0, .01, .020;
        instrument->SetPulse(Amp, Times);

    std::cout << *Earth << "\n" << *Trans << "\n" << *Receivers << std::endl;

	// Perform the forward model
	//instrument->MakeDirectCalculation( FHTKEY201  );
	//instrument->MakeLaggedCalculation( ANDERSON801 );
	//instrument->MakeLaggedCalculation( QWEKEY );
	//instrument->MakeLaggedCalculation( CHAVE );
	instrument->MakeLaggedCalculation( FHTKEY201 );
	//instrument->MakeLaggedCalculation( FHTKEY101 );

    int nlag = instrument->GetMeasurements().rows();
	// Output results to screen
	for (int ii=0; ii<nlag; ii++) {
		std::cout<<"  "<<instrument->GetMeasurements()(ii,0)<<"  "<<instrument->GetMeasurements()(ii,1)<<std::endl;
	}

	// Output results to file
	std::ofstream outfile1;
	outfile1.open("solution.out");
    //outfile1 << "//timegate [ms]\t dB/dt [nT/s]" << std::endl;
    outfile1 << "//timegate [s]\t dB/dt [T/s]" << std::endl;
	for (int ii=0; ii<nlag; ii++) {
		//outfile1 << 1e3*instrument->GetMeasurements()(ii,0)<< "\t" << (MU0*1e9)*instrument->GetMeasurements()(ii,1)<<std::endl;
		outfile1 << instrument->GetMeasurements()(ii,0)<< "\t" << MU0*instrument->GetMeasurements()(ii,1) << std::endl;
		//outfile1 << instrument->GetMeasurements()(ii,0)<< "\t" << instrument->GetMeasurements()(ii,1)<<std::endl;
	}
	outfile1.close();


	// Timing stuff
	std::cout << "  Time for execution: " <<  (clock()-launch)/CLOCKS_PER_SEC   << " [CPU] seconds."
		 << std::endl;


	instrument->Delete();
	Trans->Delete();
	Earth->Delete();
	Receivers->Delete();
	Reader->Delete();

	return EXIT_SUCCESS;
}


