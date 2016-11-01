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

    void testFieldPoints( void )
    {
        auto Obj = FieldPoints::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("FieldPoints") );
    }

};

