/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/21/2016 10:24:57 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2016, University of Utah
 * @copyright Copyright (c) 2016, Trevor Irons & Lemma Software, LLC
 */

#include "CubicSplineInterpolator.h"
#include "ASCIIParser.h"
using namespace Lemma;

int main() {
    //auto Spline = CubicSplineInterpolator::NewSP();
    //auto Spline2 = CubicSplineInterpolator();
    auto Parser = ASCIIParser::NewSP();
    //ASCIIParser Parser2 = ASCIIParser();
    //    Parser2.SetCommentString( "#");
    //auto Parser3 = ASCIIParser(Parser2);
//    auto Parser3 = *Parser;

    //std::cout << Parser3;

    //std::cout << *Parser << std::endl;
    //YAML::Node node = Parser->Serialize();
    //std::cout << node << std::endl;
    //auto Obj2 = ASCIIParser::DeSerialize(Parser->Serialize());
    //std::cout << *Obj2 << std::endl;


//    std::cout << "string " << Parser->GetName() << "\t" << Obj2->GetName() << "\n";
//         TS_ASSERT_EQUALS( Obj->GetName(), Obj2->GetName() );

    //std::cout << Parser2 << std::endl;
    //std::cout << Parser3 << std::endl;
}

