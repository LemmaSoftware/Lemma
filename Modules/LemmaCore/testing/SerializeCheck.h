/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      06/16/2016 09:12:46 PM
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Trevor Irons & Lemma Software, LLC
 */

#include <cxxtest/TestSuite.h>
#include <LemmaCore>

using namespace Lemma;

class MyTestSuite : public CxxTest::TestSuite
{
    public:

//     void test_trace(void)
//     {
//         TS_TRACE("This is a test tracing message.");
//     }
//
//     void test_warn(void)
//     {
//         TS_WARN("This is a test warning message.");
//     }

    void testASCIIParser( void )
    {
        auto Obj = ASCIIParser::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = ASCIIParser::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }

    void testCubicSplineInterpolator(void)
    {
        auto Obj = CubicSplineInterpolator::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = CubicSplineInterpolator::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }

    void testRectilinearGrid( void )
    {
        auto Obj = RectilinearGrid::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = RectilinearGrid::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }

    void testRectilinearGridReader( void )
    {
        auto Obj = RectilinearGridReader::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = RectilinearGridReader::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }

    void testWindowFilter( void )
    {
        auto Obj = WindowFilter::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = WindowFilter::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }

// How do we test abstract classes?
//     void testLayeredEarth( void )
//     {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::discrete_distribution<> d({0,40, 10, 10, 40});
//         auto Obj = LayeredEarth::NewSP();
//             int nl = d(gen);
//             Obj->SetNumberOfLayers(nl);
//         YAML::Node node = Obj->Serialize();
//         auto Obj2 = LayeredEarth::DeSerialize(node);
//         TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
//         TS_ASSERT_EQUALS( Obj->GetNumberOfLayers(), Obj2->GetNumberOfLayers() );
//
//     }


//     void testRectilinearGridVTKExporter( void )
//     {
//         auto Obj = RectilinearGridVTKExporter::NewSP();
//         TS_ASSERT_EQUALS( Obj->GetName(), std::string("RectilinearGridVTKExporter") );
//     }

};

