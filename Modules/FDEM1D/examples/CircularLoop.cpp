/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      12/01/2017 09:38:57 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2017, University of Utah
 * @copyright Copyright (c) 2017, Trevor Irons & Lemma Software, LLC
 */


#include <FDEM1D>
using namespace Lemma;

int main(int argc, char** argv) {

    if (argc < 6) {
        std::cout << "Usage:\n";
        std::cout << "CircularLoop <radius> <centreNorthing>  <centreEasting>"
                  <<  "  <numTurns>  <npoints>\n"  ;
        exit(EXIT_SUCCESS);
    }

    Real rad = atof(argv[1]);
    Real cx = atof(argv[2]);
    Real cy = atof(argv[3]);
    int nt = atoi(argv[4]);
    int np = atoi(argv[5]);

    auto wire = PolygonalWireAntenna::NewSP();
        wire->SetNumberOfPoints(np);
        wire->SetNumberOfTurns(nt);
        Real da = 2.*PI / (Real)(np-1);
        for (int ip=0; ip<np-1; ++ip) {
            wire->SetPoint(ip, cx+rad*std::sin(da*static_cast<Real>(ip)),
                               cy+rad*std::cos(da*static_cast<Real>(ip)), -1e-3);
        }
        wire->SetPoint(np-1, cx, cy+rad, -1e-3);

    wire->SetNumberOfFrequencies(1);
        wire->SetFrequency(0, 1);

    std::cout << *wire << std::endl;

    //auto wire2 = wire->Clone();
    //std::cout << "copy\n" << *wire2 << std::endl;
}

