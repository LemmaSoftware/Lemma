/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/15/2016 09:54:13 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Trevor Irons & Lemma Software, LLC
 */

#include <iostream>
#include "LemmaConfig.h"
#include "lemma.h"

int main() {
    std::cout << "Hello Lemma " << LEMMA_VERSION_MAJOR << "\t" << LEMMA_VERSION_MINOR << std::endl;
    std::cout << "thingy()\t" << thingy() << std::endl;
}

