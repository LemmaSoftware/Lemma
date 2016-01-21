// ===========================================================================
//
//       Filename:  uttemsandbox.cpp
//
//        Created:  09/29/2010 07:38:26 AM
//       Compiler:  Tested with g++, icpc, and MSVC 2010
//
//         Author:  Trevor Irons (ti), M. Andy Kass
//
//
//   Organisation:  Colorado School of Mines (CSM)
//                  United States Geological Survey (USGS)
//					Broken Spoke Development, LLC
//
//          Email:  tirons@mines.edu, tirons@usgs.gov
//					mkass@numericalgeo.com
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
  @date     09/29/2010
  @version   0.0
 **/

#include "Lemma"
using namespace Lemma;

#ifdef LEMMAUSEVTK
#include "matplot.h"
using namespace matplot;
#endif

int main() {


    int nfreq  =    6000;  // Number of frequencies

	// Specify receiver times
	int ntimes	=	30;
	VectorXr times(ntimes);
	times <<
	36.0, 45.25, 57.0, 72.25, 92.0,
	117.0, 148.0, 186.5, 234.0, 290.0,
	352.5, 427.5, 525.0, 647.5, 802.5,
	1002.5, 1257.5, 1582.5, 1997.5, 2525.0,
	3197.5, 4055.0, 5147.5, 6542.5, 8322.5,
	10592.0, 13490.0, 17187.0, 21902.0, 27915.0;

	times = times.array()*1.0e-6;

	//CALCULATE ABSCISSA
	//define bounds

	gaussianquadrature *lgqw = gaussianquadrature::New();

	Real lowb;
	Real upb;
	lowb = 1.0;
	upb = 6000000.0;
	lgqw->SetFreqs(nfreq,upb,lowb);
	lgqw->CalcAW();

	VectorXr xu(nfreq);
	xu=lgqw->GetAbscissae();
	DipoleSource* Trans=DipoleSource::New();
	Trans->SetType(MAGNETICDIPOLE);
	Trans->SetPolarisation(ZPOLARISATION);
	Trans->SetLocation(0.0,0.0,-1e-4);


    // Specify Transmitter
    //PolygonalWireAntenna* Trans  = PolygonalWireAntenna::New();
    //Trans->SetNumberOfPoints(5);
    //Trans->SetPoint(0, Vector3r(   0,   0, -1e-3));
    //Trans->SetPoint(1, Vector3r( 100,   0, -1e-3));
    //Trans->SetPoint(2, Vector3r( 100, 100, -1e-3));
    //Trans->SetPoint(3, Vector3r(   0, 100, -1e-3));
    //Trans->SetPoint(4, Vector3r(   0,   0, -1e-3));
    Trans->SetNumberOfFrequencies(nfreq);
    VectorXr  f(nfreq);
    for (int ifreq=0; ifreq<nfreq; ++ifreq) {
    //    Trans->SetFrequency(ifreq, 1e-3 + dfreq*(Real)(ifreq));
		Trans->SetFrequency(ifreq,xu(ifreq));
        f(ifreq) = xu(ifreq);
    }
	//cout << Trans->GetFrequencies();
    //Trans->SetCurrent(1);
    //Trans->SetNumberOfTurns(1);

    // Earth properties, top layer is air layer 0 conductivity is fine
    LayeredEarthEM *Earth = LayeredEarthEM::New();
        Earth->SetNumberOfLayers(4);
        Earth->SetLayerConductivity( (VectorXcr(4) << 0.,1.e-6,1.e-2,1.e-6 ).finished() );
        Earth->SetLayerThickness( (VectorXr(2) << 50, 20).finished() );

    // Receivers, just 1 in the centre for now
   	ReceiverPoints *Receivers = ReceiverPoints::New();
		Vector3r loc;
		Real ox     =    50.;
		Real oy     =    50.;
		Real depth  =  -1e-2;
		Receivers->SetNumberOfReceivers(1);
		loc << ox, oy, depth;
		Receivers->SetLocation(0, loc);

	// EmEarth
	EMEarth1D  *EmEarth = EMEarth1D::New();
		//EmEarth->AttachWireAntenna(Trans);
		EmEarth->AttachDipoleSource(Trans);
		EmEarth->AttachLayeredEarthEM(Earth);
		EmEarth->AttachReceiverPoints(Receivers);
		EmEarth->SetFieldsToCalculate(H);
        // slower but may be more accurate, depending on frequencies
        EmEarth->SetHankelTransformMethod(CHAVE);
        //EmEarth->SetHankelTransformMethod(DIGITALFILTERING);

    // Do calculation
   	//EmEarth->CalculateWireAntennaFields();
	//EmEarth->MakeCalc();
	EmEarth->MakeCalc3();

    // Grab Z component
    // A little painful, I'll try to clean up the API
    VectorXcr Hw(nfreq);
    VectorXr  e(nfreq);
    Hw(0) = 0.; // DC component
    for (int ifreq=0; ifreq<nfreq; ++ifreq) {
        Hw(ifreq) = (Receivers->GetHfield(ifreq, 0))(2);
        e(ifreq) = ifreq;
    }

    VectorXr Ht(ntimes);                   // Time domain H field

	//Sine transform
	VectorXr func(nfreq);
	for (int ii=0;ii<ntimes;++ii) {
		func = (Hw.imag().array()*((f.array()*times(ii)).sin()));
		lgqw->SetFunc(func);
		lgqw->Integrate();
		Ht(ii)=lgqw->GetResult();
	}
	Ht = Ht.array()*(-2.0/PI);

	std::ofstream myfile1;
	myfile1.open("solution.txt");
	for (int ii=0;ii<ntimes;++ii) {
		myfile1 << times(ii) << "   " << Ht(ii) << std::endl;
	}
	myfile1.close();

	std::ofstream myfile2;
	myfile2.open("freq_domain.txt");
	for (int ii=0;ii<nfreq;++ii) {
		myfile2 << f(ii) << "  " << Hw(ii).real() << "  " << Hw(ii).imag()
			<< std::endl;
	}
	myfile2.close();

    //std::cout << Ht << std::endl;
    //std::cout << Hw.real() << std::endl;

    // Quick and dirty plot
    #ifdef  LEMMAUSEVTK
    double colour1[3] = {0.0,0.0,1.0};
    double colour2[3] = {1.0,0.0,0.0};

    Plot2D_VTK p1("Hz", "Re(Hw)", 800, 600);
    p1.plot(f, Hw.real().eval(), colour1, ".-");
    p1.show();

    Plot2D_VTK p2("Hz", "Im(Hw)", 800, 600);
    p2.plot(f, Hw.imag().eval(), colour2, ".-");
    p2.show();

    Plot2D_VTK p3("Time (seconds)", "Ht", 800, 600);
    p3.plot(times, Ht, colour2, ".-");
    p3.show();

	//Plot2D_VTK p4("index", "frequency",800,600);
	//p4.plot(e,f,colour1,".-");
	//p4.show();

    #endif

    // Clean up
    Trans->Delete();
    Earth->Delete();
    Receivers->Delete();
    EmEarth->Delete();

    return EXIT_SUCCESS;
}
