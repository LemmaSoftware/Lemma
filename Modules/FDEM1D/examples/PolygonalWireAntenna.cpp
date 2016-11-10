/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      11/07/2016 12:01:34 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Trevor Irons & Lemma Software, LLC
 */

#include <FDEM1D>
using namespace Lemma;

int main() {

    auto wire = PolygonalWireAntenna::NewSP();
        wire->SetNumberOfPoints(5);
        wire->SetPoint(0, 0, 0, 0);
        wire->SetPoint(1, 1, 0, 0);
        wire->SetPoint(2, 1, 1, 0);
        wire->SetPoint(3, 0, 1, 0);
        wire->SetPoint(4, 0, 0, 0);
    std::cout << *wire << std::endl;

    auto wire2 = wire->Clone();
    std::cout << "copy\n" << *wire2 << std::endl;
}
