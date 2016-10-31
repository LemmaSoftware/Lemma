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

#include <random>
#include "FDEM1D"

using namespace Lemma;

int main() {

       std::random_device rd;
       std::mt19937 gen(rd());
       std::discrete_distribution<> d({0,0,40, 10, 10, 40});
           int nl = d(gen);

    std::uniform_real_distribution<> dis(0, 1);
    VectorXcr con = VectorXcr(nl);
    con(0) = 0;
    for ( int i=1; i<nl; ++i ) {
        con(i) = Complex(dis(gen), dis(gen));
    }
    std::cout << con << std::endl;

    auto model = LayeredEarthEM::NewSP();
        model->SetNumberOfLayers(nl);
        //model->SetLayerThickness( (VectorXr(4) << 10,10,10,10).finished() );
        //model->SetLayerConductivity( (VectorXcr(6) << .1, .10,.10,.10,.10,.10).finished() );
    auto model2 = LayeredEarthEM::DeSerialize(model->Serialize());

    std::cout << model->GetNumberOfLayers() << std::endl;
    std::cout << model2->GetNumberOfLayers() << std::endl;

}

