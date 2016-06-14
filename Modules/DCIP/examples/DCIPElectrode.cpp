/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      11/10/2014 12:31:01 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "DCIPElectrode.h"
#include "DCSurvey.h"

using namespace Lemma;

int main() {


    //DCIPElectrode* Electrode1 = DCIPElectrode::New();
    DCIPElectrode* Electrode2 = DCIPElectrode::New();
    DCIPElectrode* Electrode3 = DCIPElectrode::New();
    DCIPElectrode* Electrode4 = DCIPElectrode::New();
    DCIPElectrode* Electrode5 = DCIPElectrode::New();
    DCIPElectrode* Electrode6 = DCIPElectrode::New();
    DCIPElectrode* Electrode7 = DCIPElectrode::New();
    DCIPElectrode* Electrode8 = DCIPElectrode::New();
    DCIPElectrode* Electrode9 = DCIPElectrode::New();

        //Electrode1->SetLocation( Vector3r( 23.23, 18.1, -45 )  );
        Electrode2->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode3->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode4->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode4->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode5->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode6->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode7->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode8->SetLocation( Vector3r( 13.23, 18.1, -25 ) );
        Electrode9->SetLocation( Vector3r( 13.23, 18.1, -25 ) );

//:        std::cout << *Electrode1 << std::endl;
//     std::ofstream ofstr("DCIPElectrode.yaml");
//         ofstr << *Electrode1;
//         ofstr.close();

    /*
    //std::ifstream ifstr("DCIPElectrode.yaml");
    std::ifstream ifstr("test.yaml");
    YAML::Node node = YAML::Load(ifstr);
    DCIPElectrode* Ex = DCIPElectrode::DeSerialize(node);
    std::cout << *Ex << std::endl;
    */
    //exit(1);

    //std::cout << *Electrode << std::endl;
    //std::cout << *Electrode2 << std::endl;

    // Set up a Survey
    DCSurvey* Survey = DCSurvey::New();
        // Instead we should just put location in, and then survey acts as factory?
        DCIPElectrode* Electrode1 = Survey->PoundElectrode( Vector3r( 13.23, 18.1, -25 ), "L0-E0" );
        Survey->PoundElectrode( Electrode2 , "L0-E1" );
        Survey->PoundElectrode( Electrode3 , "L0-E2" );
        Survey->PoundElectrode( Electrode4 , "L0-E3" );
        Survey->PoundElectrode( Electrode5 , "L0-E4" );
        Survey->PoundElectrode( Electrode6 , "L0-E5" );
        Survey->PoundElectrode( Electrode7 , "L0-E6" );
        Survey->PoundElectrode( Electrode8 , "L0-E7" );
        Survey->PoundElectrode( Electrode9 , "L0-E8" );

        int J0 = Survey->AddInjection( Electrode1, Electrode2, 1.0 );
            Survey->AddMeasurement(J0, Electrode3, Electrode4 );
            Survey->AddMeasurement(J0, Electrode5, Electrode6 );
            Survey->AddMeasurement(J0, Electrode6, Electrode7 );
            Survey->AddMeasurement(J0, "L0-E7", "L0-E8" );

        int J1 = Survey->AddInjection( Electrode2, Electrode3, 1.0 );
            Survey->AddMeasurement(J1, Electrode3, Electrode4 );
            Survey->AddMeasurement(J1, Electrode5, Electrode6 );
            Survey->AddMeasurement(J1, Electrode6, Electrode7 );

        int J2 = Survey->AddInjection( Electrode3, Electrode4, 1.0 );
            Survey->AddMeasurement(J2, Electrode3, Electrode4 );
            Survey->AddMeasurement(J2, Electrode5, Electrode6 );
            Survey->AddMeasurement(J2, Electrode6, Electrode7 );

        //Survey->AddMeasurement(L1, L2)
        std::cout << *Survey << std::endl;

        std::ofstream outstr("DC.yaml");
            outstr << *Survey << std::endl;
            outstr.close();

        Survey->Delete();

        //TODO recheck DeSerialize make sure we are filling in all the Tag vectors

        std::ifstream ifstr2("DC.yaml");
        YAML::Node node2 = YAML::Load(ifstr2);
        DCSurvey* Survey2 = DCSurvey::DeSerialize(node2);

        std::cout << "==============================================================\n";
        std::cout << *Survey2 << std::endl;

    return(EXIT_SUCCESS);

}

