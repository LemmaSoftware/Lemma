/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/06/2014 10:37:30 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "Lemma"
using namespace Lemma;

int main() {

    // Each pulse sequence functions as an autonomous transmitter. A record may contain multiple transmitters (moments).
    // There is some small overhead for wire loop locations, but generality is retained doing this, as well as monitoring of
    // slight changes in geometry for both pulses.
    TEMTransmitter* TxHM = TEMTransmitter::New();
        TxHM->SetRepFrequency( 20, KHZ );
        VectorXr Times (18);
        VectorXr Amps (18);
        Times << 0.0, 0.03051, 0.10267, 0.19408, 0.19889, 0.21332, 0.74249, 1.3775, 1.83452, 2.52245, \
                3.191132, 3.9031135, 4.0, 4.00484486, 4.123904, 4.200182, 4.20732, 4.212946;
        Amps << 0.0, 14.71872, 62.34372, 114.84372, 117.84372, 118.96872, 118.96872, 118.96872, 118.96872,\
                118.59372, 119.34372, 120.0, 120.0, 117.94176, 47.60364, 0.8905848, 0.1203888, 0.0;
        TxHM->SetWaveform( Times, Amps, MILLISEC );
        // Define wire loop
        TxHM->SetNumberOfPoints(8);
        TxHM->SetPoint(0, Vector3r( -16.10,    2.13,  -34));
        TxHM->SetPoint(1, Vector3r(  -7.51,   10.72,  -34));
        TxHM->SetPoint(2, Vector3r(   7.51,   10.72,  -34));
        TxHM->SetPoint(3, Vector3r(  14.92,    3.31,  -34));
        TxHM->SetPoint(4, Vector3r(  14.92,   -3.31,  -34));
        TxHM->SetPoint(5, Vector3r(   7.51,  -10.72,  -34));
        TxHM->SetPoint(6, Vector3r(  -7.51,  -10.72,  -34));
        TxHM->SetPoint(7, Vector3r( -16.10,   -2.13,  -34));
        TxHM->SetNumberOfTurns(8);

    // Each Transmitter needs its own `Receiver(s)', the only difference may be the time gates, but that's OK.
    //     It's a minor amount of overhead for greatly improved genearality. As sometimes different gates are
    //     masked.
    TEMInductiveReceiver* RxHM = TEMInductiveReceiver::New();
        RxHM->SetComponent( ZCOMPONENT );      // What about Overloaded to take (X), (X,Y), or (X,Y,Z) or what about tilt?
        RxHM->SetMoment( 1 );                  // Normalized
        RxHM->SetReferenceTime( 4., MILLISEC );
        RxHM->SetRxLocation( (Vector3r() << -16.80, 0, -36.00).finished() );

        // Gate Centres  ms
        VectorXr centres (37);
        centres << 7.15000000e-04,   2.21500000e-03,   4.21500000e-03,   6.21500000e-03,
                   8.21500000e-03,   1.02150000e-02,   1.22150000e-02,   1.47150000e-02,
                   1.82150000e-02,   2.27150000e-02,   2.82150000e-02,   3.52150000e-02,
                   4.42150000e-02,   5.57150000e-02,   7.02150000e-02,   8.82150000e-02,
                   1.10715000e-01,   1.38715000e-01,   1.74215000e-01,   2.19715000e-01,
                   2.76715000e-01,   3.48715000e-01,   4.39715000e-01,   5.53715000e-01,
                   6.97715000e-01,   8.79215000e-01,   1.10771500e+00,   1.39621500e+00,
                   1.76021500e+00,   2.21871500e+00,   2.79671500e+00,   3.52571500e+00,
                   4.44471500e+00,   5.60321500e+00,   7.06321500e+00,   8.90421500e+00,
                   1.10667200e+01;
        //centres.array() += 4.;

        // Gate Widths  ms
        VectorXr widths (37);
        widths <<   4.30000000e-04,   1.43000000e-03,   3.43000000e-03,   5.43000000e-03,
                    7.43000000e-03,   9.43000000e-03,   1.14300000e-02,   1.34300000e-02,
                    1.64300000e-02,   2.04300000e-02,   2.54300000e-02,   3.14300000e-02,
                    3.94300000e-02,   4.94300000e-02,   6.24300000e-02,   7.84300000e-02,
                    9.84300000e-02,   1.23430000e-01,   1.54430000e-01,   1.94430000e-01,
                    2.45430000e-01,   3.08430000e-01,   3.89430000e-01,   4.90430000e-01,
                    6.17430000e-01,   7.78430000e-01,   9.80430000e-01,   1.23543000e+00,
                    1.55743000e+00,   1.96343000e+00,   2.47443000e+00,   3.11943000e+00,
                    3.93243000e+00,   4.95743000e+00,   6.24943000e+00,   7.87743000e+00,
                    9.93143000e+00;

        RxHM->SetWindows(centres, widths, MILLISEC);

    TEMTransmitter* TxLM = TEMTransmitter::New();
        TxLM->SetRepFrequency( 20, KHZ );
        VectorXr TimesLM (18);
        VectorXr AmpsLM (18);

        TimesLM << -8.00000E-004, -7.86965E-004, -7.66493E-004, -7.23688E-004,
                   -6.39938E-004, -5.16174E-004, -3.93340E-004, -2.63993E-004,
                   -1.43952E-004, -7.15990E-006, -2.50712E-006,  0.00000E+000,
                    2.19597E-007,  1.47193E-006,  3.34398E-006,  4.68669E-006,
                    5.96484E-006,  7.04934E-006;
        TimesLM.array() = TimesLM.array() + 8e-4;  // Valgrind Hack += yields error. Correct for SkyTEM convention


        AmpsLM <<  0.00000E+000,  3.67188E-002,  6.17188E-002,  1.17969E-001,
                   2.14844E-001,  3.28906E-001,	 4.75781E-001,  6.30469E-001,
                   7.82031E-001,  9.92969E-001,	 1.00000E+000,  1.00000E+000,
                   9.63459E-001,  6.01030E-001,  2.29652E-001,  8.64702E-002,
                   2.53196E-002,  0.00000E+000;
        AmpsLM.array() = AmpsLM.array() * 8.;   // Also correct for SkyTEM convention in .geo file

        TxLM->SetWaveform( TimesLM, AmpsLM, SEC );
        // Define wire loop
        TxLM->SetNumberOfPoints(8);
        TxLM->SetPoint(0, Vector3r( -16.10,    2.13,  -34));
        TxLM->SetPoint(1, Vector3r(  -7.51,   10.72,  -34));
        TxLM->SetPoint(2, Vector3r(   7.51,   10.72,  -34));
        TxLM->SetPoint(3, Vector3r(  14.92,    3.31,  -34));
        TxLM->SetPoint(4, Vector3r(  14.92,   -3.31,  -34));
        TxLM->SetPoint(5, Vector3r(   7.51,  -10.72,  -34));
        TxLM->SetPoint(6, Vector3r(  -7.51,  -10.72,  -34));
        TxLM->SetPoint(7, Vector3r( -16.10,   -2.13,  -34));
        TxLM->SetNumberOfTurns(8);


    TEMInductiveReceiver* RxLM = TEMInductiveReceiver::New();
        RxLM->SetComponent( ZCOMPONENT );      // What about Overloaded to take (X), (X,Y), or (X,Y,Z) or what about tilt?
        RxLM->SetMoment( 1 );                  // Normalized
        RxLM->SetReferenceTime( 8e-4, SEC );
        RxLM->SetRxLocation( (Vector3r() << -16.80, 0, -36.00).finished() );

        // Gate Centres  ms
        VectorXr centresLM (26);
            centresLM << 7.150000E-07, 2.215000E-06, 4.215000E-06, 6.215000E-06, 8.215000E-06,
                         1.021500E-05, 1.221500E-05, 1.471500E-05, 1.821500E-05, 2.271500E-05,
                         2.821500E-05, 3.521500E-05, 4.421500E-05, 5.571500E-05, 7.021500E-05,
                         8.821500E-05, 1.107150E-04, 1.387150E-04, 1.742150E-04, 2.197150E-04,
                         2.767150E-04, 3.487150E-04, 4.397150E-04, 5.537150E-04, 6.977150E-04,
                         8.792150E-04;

        VectorXr widthsLM (26);
            widthsLM << 5.700000E-07, 1.570000E-06, 1.570000E-06, 1.570000E-06, 1.570000E-06,
                        1.570000E-06, 1.570000E-06, 2.570000E-06, 3.570000E-06, 4.570000E-06,
                        5.570000E-06, 7.570000E-06, 9.570000E-06, 1.257000E-05, 1.557000E-05,
                        1.957000E-05, 2.457000E-05, 3.057000E-05, 3.957000E-05, 5.057000E-05,
                        6.257000E-05, 8.057000E-05, 1.005700E-04, 1.265700E-04, 1.605700E-04,
                        2.015700E-04;

        RxLM->SetWindows(centresLM, widthsLM, SEC);



    // Specifies survey, this is the Glue Class, the top of the structure, etc.
    TEMSurvey* Survey = TEMSurvey::New();
        Survey->SetNumberOfLines(1);  // Flight lines or
                                      // Internally each line is a class? But that's sort of hidden to the
                                      // end user. Having each line seperate is nice for constrained inversion, where
                                      // each line is a nice thing to deal with.
        Survey->GetLine(0)->SetNumberOfRecords(1);    // Each Record then contains everything needed for modelling response curve(s)
        Survey->GetLine(0)->GetRecord(0)->SetNumberOfPulseSequences( 1 );
        Survey->GetLine(0)->GetRecord(0)->SetTransmitterReceiverPair( 0, TxHM, RxHM );
        //Survey->GetLine(0)->GetRecord(0)->SetTransmitterReceiverPair( 1, TxHM, RxHM );


    LayeredEarthEM* Earth = LayeredEarthEM::New();
        Earth->SetNumberOfLayers(31);
        Earth->SetLayerThickness( (VectorXr(29) <<  5.00E+00,  5.40E+00,
             5.80E+00, 6.30E+00, 6.80E+00, 7.30E+00, 7.80E+00, 8.50E+00,
             9.10E+00, 9.80E+00, 1.06E+01, 1.14E+01, 1.23E+01, 1.33E+01,
             1.43E+01, 1.54E+01, 1.66E+01, 1.79E+01, 1.93E+01, 2.08E+01,
             2.25E+01, 2.42E+01, 2.61E+01, 2.81E+01, 3.03E+01, 3.27E+01,
             3.52E+01, 3.80E+01, 4.10E+01).finished() );

        VectorXcr rho = ( (VectorXcr(31) << 0, 1.54E+01, 4.21E+01,
            9.25E+01, 1.26E+02, 1.17E+02, 7.57E+01, 3.21E+01, 1.40E+01, 1.96E+01,
            2.67E+01, 2.56E+01, 2.00E+01, 1.86E+01, 2.31E+01, 2.97E+01, 3.50E+01,
            3.79E+01, 3.75E+01, 3.32E+01, 2.52E+01, 1.57E+01, 8.38E+00, 5.38E+00,
            5.49E+00, 6.34E+00, 7.07E+00, 7.81E+00, 8.67E+00, 9.59E+00, 1.05E+01
        ).finished() ); // 200 Ohm-m half space

        Earth->SetLayerConductivity( 1./rho.array() );
        // ALL SET UP
        std::cout << *Survey << std::endl;

        // OK do your thinc
        //DataTEM* data =
        Survey->ForwardModel( Earth );


    // Clean up



//     //Survey->Delete();
//     Earth->Delete();
//
//     TxHM->Delete();
//     RxHM->Delete();
//     TxLM->Delete();
//     RxLM->Delete();

}
