/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/01/19 22:34:11
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2019, University of Utah
 * @copyright Copyright (c) 2019, Lemma Software, LLC
 */

#include <cxxtest/TestSuite.h>
#include <FDEM1D>
#include <random>
#include "timer.h"

#define EPSILON 1e-10
using namespace Lemma;

class MyAlgorithmTest : public CxxTest::TestSuite {
   // Put in your usual, quick unit tests here
};

class MyAlgorithmTestPerformance : public CxxTest::TestSuite {

public:

    std::shared_ptr< DipoleSource > dipole;
    std::shared_ptr< LayeredEarthEM > earth;
    std::shared_ptr< FieldPoints > receivers;
    std::shared_ptr< EMEarth1D > EmEarth;

	jsw_timer timer;

    Real Delta;

    void setUp() {
        // Put in any code needed to set up your test,
        // but that should NOT be counted in the execution time.
	    dipole = DipoleSource::NewSP();

        //dipole->SetType(GROUNDEDELECTRICDIPOLE);
		//dipole->SetPolarisation(XPOLARISATION);


		dipole->SetNumberOfFrequencies(1);
		dipole->SetMoment(1);
		dipole->SetFrequency(0, 4400.1000);
		//dipole->SetPhase(0);
		//dipole->SetLocation( (VectorXr(3) << 49, 49, -1e-4).finished() );
		dipole->SetLocation( 0, 0, -1e-4  );

	    // Define model
	    VectorXcr sigma(2);
		sigma << 0., 1e-3;//, .1;//, .01, .001;
	    VectorXr  thick(1);
		thick << 10;//, 10, 10;

	    earth = LayeredEarthEM::NewSP();
        earth->SetNumberOfLayers(2);
		earth->SetLayerConductivity(sigma);
		//earth->SetLayerThickness(thick);

	    receivers = FieldPoints::NewSP();
		Vector3r loc;

        Real ox = 2.;
		Real oy = 2.;
		Real oz = 2;

		Real dx = 0.62;
		Real dy = 0.62;
		Real dz = 0.62;

		int  nx = 20;
		int  ny = 20;
		int  nz = 20;
        Delta = nx*ny*nz*1e-10;

		receivers->SetNumberOfPoints(nx*ny*nz);
		int ir = 0;
		for (int ix=0; ix<nx; ++ix) {
		for (int iy=0; iy<ny; ++iy) {
		for (int iz=0; iz<nz; ++iz) {
			loc << ox+ix*dx, oy+iy*dy, oz+iz*dz;
            //std::cout << "Receiver location " << loc.transpose() << std::endl;
			receivers->SetLocation(ir, loc);
			//oz += dz;
			++ ir;
		}
        }
        }

        EmEarth = EMEarth1D::NewSP();
        EmEarth->SetHankelTransformMethod(CHAVE);
        EmEarth->SetFieldsToCalculate(BOTH); // Fortran needs this
		EmEarth->AttachDipoleSource(dipole);
		EmEarth->AttachLayeredEarthEM(earth);
		EmEarth->AttachFieldPoints(receivers);

   }

   void tearDown() {
      // Clean-up code, also NOT counted in execution time.
   }

   void test_Hz() {

        dipole->SetType(MAGNETICDIPOLE);
		dipole->SetPolarisation(ZPOLARISATION);

        // Put in a unit test that will be slow.
	    std::cout << "C++\n";

  	    timer.begin();
	    EmEarth->MakeCalc3();
	    Real lemmaTime = timer.end();

        auto lc = receivers->GetEfield( 0 );

        #ifdef KIHALEE_EM1D
	    receivers->ClearFields();
        std::cout << "\nFORTRAN KiHa\n";
  	    timer.begin();
 	    EmEarth->MakeCalc();
	    Real kihaTime = timer.end();

        auto fc = receivers->GetEfield( 0 ); //0,0);

        std::cout << "Lemma time:" << lemmaTime << "\tKiHa time:" << kihaTime << std::endl;

        std::cout.precision(16);
        std::cout << "Lemma norm |" << (lc).norm() << "|" << std::endl;
        std::cout << "KiHa norm  |" << (fc).norm() << "|" << std::endl;
        std::cout << "Difference norm |" << (lc - fc).norm() << "|" << std::endl;

        TS_ASSERT_DELTA((lc-fc).norm(), 0.0, Delta);
        #endif
   }

   void test_Hx() {

        dipole->SetType(MAGNETICDIPOLE);
		dipole->SetPolarisation(XPOLARISATION);

        // Put in a unit test that will be slow.
	    std::cout << "C++\n";

  	    timer.begin();
	    EmEarth->MakeCalc3();
	    Real lemmaTime = timer.end();

        auto lc = receivers->GetEfield( 0 );

        #ifdef KIHALEE_EM1D
	    receivers->ClearFields();
        std::cout << "\nFORTRAN KiHa\n";
  	    timer.begin();
 	    EmEarth->MakeCalc();
	    Real kihaTime = timer.end();

        auto fc = receivers->GetEfield( 0 ); //0,0);

        std::cout << "Lemma time:" << lemmaTime << "\tKiHa time:" << kihaTime << std::endl;

        std::cout.precision(16);
        std::cout << "Lemma norm |" << (lc).norm() << "|" << std::endl;
        std::cout << "KiHa norm  |" << (fc).norm() << "|" << std::endl;
        std::cout << "Difference norm |" << (lc - fc).norm() << "|" << std::endl;

        TS_ASSERT_DELTA((lc-fc).norm(), 0.0, Delta);
        #endif
   }

   void test_Hy() {

        dipole->SetType(MAGNETICDIPOLE);
		dipole->SetPolarisation(YPOLARISATION);

        // Put in a unit test that will be slow.
	    std::cout << "C++\n";

  	    timer.begin();
	    EmEarth->MakeCalc3();
	    Real lemmaTime = timer.end();

        auto lc = receivers->GetEfield( 0 );

        #ifdef KIHALEE_EM1D
	    receivers->ClearFields();
        std::cout << "\nFORTRAN KiHa\n";
  	    timer.begin();
 	    EmEarth->MakeCalc();
	    Real kihaTime = timer.end();

        auto fc = receivers->GetEfield( 0 ); //0,0);

        std::cout << "Lemma time:" << lemmaTime << "\tKiHa time:" << kihaTime << std::endl;

        std::cout.precision(16);
        std::cout << "Lemma norm |" << (lc).norm() << "|" << std::endl;
        std::cout << "KiHa norm  |" << (fc).norm() << "|" << std::endl;
        std::cout << "Difference norm |" << (lc - fc).norm() << "|" << std::endl;

        TS_ASSERT_DELTA((lc-fc).norm(), 0.0, Delta);
        #endif
   }

   void test_Ex() {

        dipole->SetType(GROUNDEDELECTRICDIPOLE);
		dipole->SetPolarisation(XPOLARISATION);

        // Put in a unit test that will be slow.
	    std::cout << "C++\n";

  	    timer.begin();
	    EmEarth->MakeCalc3();
	    Real lemmaTime = timer.end();

        auto lc = receivers->GetEfield( 0 );

        #ifdef KIHALEE_EM1D
	    receivers->ClearFields();
        std::cout << "\nFORTRAN KiHa\n";
  	    timer.begin();
 	    EmEarth->MakeCalc();
	    Real kihaTime = timer.end();

        auto fc = receivers->GetEfield( 0 ); //0,0);

        std::cout << "Lemma time:" << lemmaTime << "\tKiHa time:" << kihaTime << std::endl;

        std::cout.precision(16);
        std::cout << "Lemma norm |" << (lc).norm() << "|" << std::endl;
        std::cout << "KiHa norm  |" << (fc).norm() << "|" << std::endl;
        std::cout << "Difference norm |" << (lc - fc).norm() << "|" << std::endl;

        TS_ASSERT_DELTA((lc-fc).norm(), 0.0, Delta);
        #endif
   }

};
