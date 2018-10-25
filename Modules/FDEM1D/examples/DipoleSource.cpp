/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/25/2018 12:41:37 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#include "FDEM1D"

using namespace Lemma;

int main() {
    auto dip = DipoleSource::NewSP();
    dip->SetLocation( Vector3r(1.,2.,3.) );
    std::cout << "Dipole 1: " << dip << "\n" << *dip << std::endl;

    auto dip2 = dip->Clone(); // literal copy of pointer
    dip2->SetLocation( Vector3r(1.,2.,3.) );
    std::cout << "Dipole 2: " << dip2 << "\n" << *dip2 << std::endl;

//    if (*dip == *dip2) {
//        std::cout << "equal" << std::endl;
//    } else {
//        std::cout << "not equal" << std::endl;
//    }
}

