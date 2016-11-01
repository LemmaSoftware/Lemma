/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/30/2016 10:07:46 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Trevor Irons & Lemma Software, LLC
 */

#include <cxxtest/TestSuite.h>
#include <FDEM1D>
#include <random>
#define EPSILON 1e-10
using namespace Lemma;

class MyTestSuite : public CxxTest::TestSuite
{
    public:

    void testLayeredEarthEM( void )
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        // up to 11 layers
        std::discrete_distribution<> d({0,0,0,0,10,10,10,10,10,10,10,10,10,10});
        int nl = d(gen);

        // for Reals
        std::uniform_real_distribution<> dis(1e-1, 1);

        // conductivity
        VectorXcr con = VectorXcr(nl);
        VectorXr thick = VectorXr(nl-2);
        con(0) = 0;
        for ( int i=1; i<nl; ++i ) {
            con(i) = Complex(dis(gen), dis(gen));
        }

        for ( int i=0; i<nl-2; ++i ) {
            thick(i) = dis(gen);
        }

        auto Obj = LayeredEarthEM::NewSP();
           Obj->SetNumberOfLayers(nl);
           Obj->SetLayerConductivity(con);
           Obj->SetLayerThickness(thick);

        YAML::Node node = Obj->Serialize();
        auto Obj2 = LayeredEarthEM::DeSerialize(node);

        // TODO assert if two layers throw error with set thickness

        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
        TS_ASSERT_EQUALS( Obj->GetNumberOfLayers(), Obj2->GetNumberOfLayers() );
        for (int ilay=0; ilay<nl; ++ilay) {
            TS_ASSERT_DELTA( std::real(Obj->GetLayerConductivity(ilay)), std::real(Obj2->GetLayerConductivity(ilay)), EPSILON );
        }
        for (int ilay=1; ilay<nl-1; ++ilay) {
            TS_ASSERT_DELTA( Obj->GetLayerThickness(ilay), Obj2->GetLayerThickness(ilay), EPSILON );
        }
    }

    void testFieldPoints(void)
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        // up to 11 layers
        std::discrete_distribution<> d({0,0,0,10,10,10,10,10,10,10,10,10,10});
        std::uniform_real_distribution<> dis(1e-1, 10);

        int np = d(gen);
        auto Obj = FieldPoints::NewSP();
            Obj->SetNumberOfPoints(np);

            for (int ip=0; ip<np; ++ip) {
                Obj->SetLocation( ip, dis(gen), dis(gen), dis(gen) );
            }

        YAML::Node node = Obj->Serialize();
        auto Obj2 = FieldPoints::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
        for (int ip=0; ip<np; ++ip) {
            TS_ASSERT_DELTA( Obj->GetLocationX(ip), Obj2->GetLocationX(ip), EPSILON );
            TS_ASSERT_DELTA( Obj->GetLocationY(ip), Obj2->GetLocationY(ip), EPSILON );
            TS_ASSERT_DELTA( Obj->GetLocationZ(ip), Obj2->GetLocationZ(ip), EPSILON );
            TS_ASSERT_EQUALS( Obj->GetLocationX(ip), Obj->GetLocation(ip)(0) );
            TS_ASSERT_EQUALS( Obj->GetLocationY(ip), Obj->GetLocation(ip)(1) );
            TS_ASSERT_EQUALS( Obj->GetLocationZ(ip), Obj->GetLocation(ip)(2) );
        }
    }
};

