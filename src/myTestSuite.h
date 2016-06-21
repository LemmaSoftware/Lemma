/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      06/14/2016 10:06:34 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Trevor Irons & Lemma Software, LLC
 */

// MyTestSuite.h
#include <cxxtest/TestSuite.h>

class MyTestSuite : public CxxTest::TestSuite
{
    public:
    void testAddition( void )
    {
        TS_ASSERT( 1 + 1 > 1 );
        TS_ASSERT( 3 - 1 > 1 );
        TS_ASSERT_EQUALS( 1 + 1, 2 );
    }
};

