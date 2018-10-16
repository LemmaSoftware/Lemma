/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/16/2018 12:43:19 PM
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

//     void test_trace(void)
//     {
//         TS_TRACE("This is a test tracing message.");
//     }
//
//     void test_warn(void)
//     {
//         TS_WARN("This is a test warning message.");
//     }

    void testTEMTransmitter( void )
    {
        auto Obj = TEMTransmitter::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = TEMTransmitter::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }

    void testTEMReceiver( void )
    {
        auto Obj = TEMReceiver::NewSP();
        YAML::Node node = Obj->Serialize();
        auto Obj2 = TEMReceiver::DeSerialize(node);
        TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );
    }


};
