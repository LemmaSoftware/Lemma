/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      07/14/2016 03:14:50 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Lemma Software, LLC
 */

#include "FEM1D"

using namespace Lemma;

int main() {

    auto model = LayeredEarthEM::NewSP();
        model->SetNumberOfLayers(6);
        model->SetLayerThickness( (VectorXr(4) << 10,10,10,10).finished() );
        model->SetLayerConductivity( (VectorXcr(6) << .1, .10,.10,.10,.10,.10).finished() );
    std::cout << *model << std::endl;
}

