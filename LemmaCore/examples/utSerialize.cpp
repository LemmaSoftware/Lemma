/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/29/2014 01:31:26 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "Lemma"

using namespace Lemma;

#ifdef  HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"

int main() {

    std::ofstream ofstr("output.yaml");
    //ofstr << "%YAML 1.2\n" << "---\n";
    //ofstr << "LEMMA_CLASSES:\n";

    // This works for individual classes or even groups of unrelated classes. But
    // How should references work? One option would be to hack YAML and allow for references
    // across 'Docs'. This is maybe undesireable, as then it's not valid YAML anymore.
    // OR we could have some other kind of IDENTIFIER that could be used and Connections could be
    // made AFTER the whole file is processed?

    // Alternative is each class is written to it's own file. And then implicit on the Serialize function, the file tags are written
    // into the YAML instead of the pointer. Alternatively, tags could do this too.

    // Or file could be saved as pointer.yaml

    // *
    // * Or a file contains all the subclasses it point to. So that it's self-enclosed. This is nice as it's quite
    // * flexible. The only downwside is possible repeat of classes. Basically if the API is careful though, only 1
    // * instance will need to be serialized.
    // *


    TEMTransmitter* Seq = TEMTransmitter::New();
        Seq->SetRepFrequency( 20, KHZ );
        VectorXr Times (18);
        VectorXr Amps (18);
        Times << 0.0, 0.03051, 0.10267, 0.19408, 0.19889, 0.21332, 0.74249, 1.3775, 1.83452, 2.52245, \
                3.191132, 3.9031135, 4.0, 4.00484486, 4.123904, 4.200182, 4.20732, 4.212946;
        Amps << 0.0, 14.71872, 62.34372, 114.84372, 117.84372, 118.96872, 118.96872, 118.96872, 118.96872,\
                118.59372, 119.34372, 120.0, 120.0, 117.94176, 47.60364, 0.8905848, 0.1203888, 0.0;
        Seq->SetWaveform(Times, Amps, MILLISEC);
    // OK dump to disk
    ofstr << *Seq ;
    ofstr.close();
    Seq->Delete();


    // OK now load
    std::ifstream ifstr("output.yaml");
    std::vector<YAML::Node> nodes = YAML::LoadAll(ifstr);

    TEMTransmitter* Seq2 = TEMTransmitter::DeSerialize( nodes[0] );
    std::cout << "Loaded\n" << *Seq2;
    Seq2->Delete();
    ifstr.close();

    //YAML::Node nodes = YAML::LoadFile("output.yaml");
    //std::cout << nodes[0] << std::endl;
    //std::cout << nodes[1] << std::endl;

    exit(EXIT_SUCCESS);
}

#else
int main() {

}
#endif
