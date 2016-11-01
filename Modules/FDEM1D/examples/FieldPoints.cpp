/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      11/01/2016 02:24:11 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Lemma Software, LLC
 */
#include<random>
#include<FDEM1D>
using namespace Lemma;

int main()
{

    std::random_device rd;
    std::mt19937 gen(rd());

    // up to 11 layers
    std::discrete_distribution<> d({0,0,0,10,10,10,10,10,10,10,10,10,10});
    int np = d(gen);

    auto Obj = FieldPoints::NewSP();
    // fill with details
        Obj->SetNumberOfPoints(np);
        //void SetLocation(const int& nrec, const Real& xp, const Real& yp,
        //                 const Real& zp);

    YAML::Node node = Obj->Serialize();
    std::cout << node << std::endl;
    auto Obj2 = FieldPoints::DeSerialize(node);
    //std::cout << Obj2->GetNumberOfPoints() << std::endl;
    //std::cout << Obj->GetNumberOfPoints() << std::endl;
}

