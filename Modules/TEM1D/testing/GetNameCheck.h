/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/16/2018 12:46:43 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#include <cxxtest/TestSuite.h>
#include <TEM1D>

using namespace Lemma;

class MyTestSuite : public CxxTest::TestSuite
{
    public:

    void testTEMTransmitter( void )
    {
        auto Obj = TEMTransmitter::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("TEMTransmitter") );
    }

    void testTEMReceiver( void )
    {
        auto Obj = TEMReceiver::NewSP();
        TS_ASSERT_EQUALS( Obj->GetName(), std::string("TEMReceiver") );
    }

};

