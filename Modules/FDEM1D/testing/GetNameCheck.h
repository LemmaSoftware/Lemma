/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      06/23/2016 01:26:46 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Lemma Software, LLC
 */

#include <cxxtest/TestSuite.h>
#include <LemmaCore>
#include <FDEM1D>

using namespace Lemma;

class MyTestSuite : public CxxTest::TestSuite
{
    public:

    void testLayeredEarthEM( void )
    {
        auto Obj = LayeredEarthEM::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("LayeredEarthEM") );
    }

    void testLayeredEarthEMReader( void )
    {
        auto Obj = LayeredEarthEMReader::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("LayeredEarthEMReader") );
    }

    void testFieldPoints( void )
    {
        auto Obj = FieldPoints::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("FieldPoints") );
    }

    void testWireAntenna( void )
    {
        auto Obj = WireAntenna::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("WireAntenna") );
    }

    void testCircularLoop( void )
    {
        auto Obj = CircularLoop::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("CircularLoop") );
    }

    void testPolygonalWireAntenna( void )
    {
        auto Obj = PolygonalWireAntenna::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("PolygonalWireAntenna") );
    }

    void testDipoleSource( void )
    {
        auto Obj = DipoleSource::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("DipoleSource") );
    }

// MSVC fails on this    
//    void testEMEarth1D( void )
//    {
//        auto Obj = EMEarth1D::NewSP();
//        TS_ASSERT_EQUALS( Obj->GetName(), std::string("EMEarth1D") );
//    }

//    void testAEMSurvey( void )
//    {
//        auto Obj = AEMSurvey::NewSP();
//        TS_ASSERT_EQUALS( Obj->GetName(), std::string("AEMSurvey") );
//    }

//    void testAEMSurveyReader( void )
//    {
//        auto Obj = AEMSurveyReader::NewSP();
//        TS_ASSERT_EQUALS( Obj->GetName(), std::string("AEMSurveyReader") );
//    }

//    void testChargedWellCasing( void )
//    {
//        auto Obj = ChargedWellCasing::NewSP();
//        TS_ASSERT_EQUALS( Obj->GetName(), std::string("ChargedWellCasing") );
//    }

//     void testKernelEM1DManager( void )
//     {
//         auto Obj = KernelEM1DManager::NewSP();
//         TS_ASSERT_EQUALS( Obj->GetName(), std::string("KernelEM1DManager") );
//     }
//
//     void testKernelEM1DSpec( void )
//     {
//         auto Obj = KernelEM1DSpec::NewSP();
//         TS_ASSERT_EQUALS( Obj->GetName(), std::string("KernelEM1DSpec") );
//     }
//
//     void testKernelEM1DReflSpec( void )
//     {
//         auto Obj = KernelEM1DReflSpec::NewSP();
//         TS_ASSERT_EQUALS( Obj->GetName(), std::string("KernelEM1DReflSpec") );
//     }

};

