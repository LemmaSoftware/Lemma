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

using namespace Lemma;

class MyTestSuite : public CxxTest::TestSuite
{
    public:

    void testLayeredEarthEM( void )
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        // up to 11 layers
        std::discrete_distribution<> d({0,0,0,10,10,10,10,10,10,10,10,10,10});
        int nl = d(gen);

        // for Reals
        std::uniform_real_distribution<> dis(1e-2, 1);

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
        TS_ASSERT_LESS_THAN( std::abs((Obj->GetLayerConductivity() - Obj2->GetLayerConductivity()).array().sum()), 1e-16 );
        TS_ASSERT_LESS_THAN( std::abs((Obj->GetLayerThickness() - Obj2->GetLayerThickness()).array().sum()), 1e-16 );
    }

};

