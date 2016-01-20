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
#include "dipolesource.h"

using namespace Lemma;

int main() {
    std::cout << "Hello Lemma " << LEMMA_VERSION_MAJOR << "\t" << LEMMA_VERSION_MINOR << std::endl;
//    std::cout << "thingy()\t" << thingy() << std::endl;

    // Test with a single dipole
    DipoleSource *dipole = DipoleSource::New();
        dipole->SetType(GROUNDEDELECTRICDIPOLE);
            //dipole->SetPolarisation(1., 0.0,  1.0);
        //    dipole->SetPolarisation(XPOLARISATION);
        dipole->SetPolarisation(YPOLARISATION);
        //dipole->SetPolarisation(ZPOLARISATION);

        /////////////
        //dipole->SetType(MAGNETICDIPOLE);
            //dipole->SetPolarisation(0., 0.0,  1.0);
            //dipole->SetPolarisation(XPOLARISATION);
        //dipole->SetPolarisation(YPOLARISATION);
        //dipole->SetPolarisation(ZPOLARISATION);

        //dipole->SetMoment(1);
        //dipole->SetLocation(1,1,-.0100328);
        dipole->SetLocation(0., 0., -0.001);
        //dipole->SetLocation(-2.5,1.25,0);
        dipole->SetNumberOfFrequencies(1);
        dipole->SetFrequency(0, 2e7);

    std::cout << *dipole << std::endl;
}

