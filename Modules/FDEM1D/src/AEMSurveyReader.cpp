/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/24/2013 04:36:14 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#include "AEMSurveyReader.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const AEMSurveyReader &ob) {
        stream << ob.Serialize() << "\n";
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  AEMSurveyReader
    //      Method:  AEMSurveyReader
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    AEMSurveyReader::AEMSurveyReader (const ctor_key& key) : LemmaObject(key), Survey(nullptr) {

    }  // -----  end of method AEMSurveyReader::AEMSurveyReader  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  AEMSurveyReader
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr<AEMSurveyReader> AEMSurveyReader::NewSP() {
        return std::make_shared<AEMSurveyReader>(ctor_key());
    }

    //--------------------------------------------------------------------------------------
    //       Class:  AEMSurveyReader
    //      Method:  ~AEMSurveyReader
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    AEMSurveyReader::~AEMSurveyReader () {
    }  // -----  end of method AEMSurveyReader::~AEMSurveyReader  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  AEMSurveyReader
    //      Method:  GetSurvey
    //--------------------------------------------------------------------------------------
    std::shared_ptr<AEMSurvey> AEMSurveyReader::GetSurvey (  ) {
        return Survey;
    }		// -----  end of method AEMSurveyReader::GetSurvey  -----


    //--------------------------------------------------------------------------------------
    //       Class:  AEMSurveyReader
    //      Method:  ReadASCIIAEMFile
    //--------------------------------------------------------------------------------------
    void AEMSurveyReader::ReadASCIIAEMFile (  const std::string& fname ) {

        Survey = AEMSurvey::NewSP();
        auto Parser = ASCIIParser::NewSP();
        Parser->SetCommentString("//");
        Parser->Open(fname);

        // Sanity check
        std::vector<int> ivals = Parser->ReadInts(1);
        std::vector<Real> freqs;
        for (int isc=0; isc<ivals[0]; ++isc) {
            Real df = Parser->ReadReals(1)[0]; // frequency Hz
            bool unique = true;
            for (unsigned int ifreq=0; ifreq<freqs.size(); ++ifreq) {
                if (df - freqs[ifreq] < 1e-2) {
                    unique = false;
                }
            }
            if (unique) freqs.push_back(df);
            std::string DT = Parser->ReadStrings(1)[0];
            if (DT == "MD" || DT == "ED") {
            } else {
                std::cerr << "In AEMSurveyReader::ReadASCIIAEMFile. The source type: "
                          << DT << " is not supported.\n";
                std::exit(EXIT_FAILURE);
            }
            Parser->ReadReals(4); // position and moment
        }

        Survey->Freqs = VectorXr::Map(&freqs[0], freqs.size());

        // OK, now get cracking, those sources at every location
        int nb = Parser->ReadInts(1)[0];  // number of locations
        for (int ib=0; ib<nb; ++ib) {
            std::vector<Real> rvals = Parser->ReadReals(6); // position and moment
            int bp = Parser->GetFileLocation(  );
            Parser->JumpToLocation( 0 );
            //std::vector<int> ivals = Parser->ReadInts(1);   // number of frequencies
            int nf = Parser->ReadInts(1)[0];   // number of frequencies
            for (int isc=0; isc<nf; ++isc) {
                Survey->Sources.push_back(DipoleSource::NewSP());
                int cnt = Survey->Sources.size() - 1; //
                // and now set it
                Survey->Sources[cnt]->SetNumberOfFrequencies(1);
                Survey->Sources[cnt]->SetFrequency(0, Parser->ReadReals(1)[0]);
                std::string DT = Parser->ReadStrings(1)[0];
                if (DT == "MD") {
                     Survey->Sources[cnt]->SetType(MAGNETICDIPOLE);
                } else if (DT == "ED") {
                     Survey->Sources[cnt]->SetType(UNGROUNDEDELECTRICDIPOLE);
                } else {
                    std::cerr << "In AEMSurveyReader::ReadASCIIAEMFile. The source type: "
                              << DT << " is not supported.\n";
                    std::exit(EXIT_FAILURE);
                }
                std::vector<Real> irvals = Parser->ReadReals(4); // position and moment
                Survey->Sources[cnt]->SetMoment(irvals[3]);
                Survey->Sources[cnt]->SetLocation(rvals[0] + irvals[0], rvals[1] + irvals[1],
                                                     rvals[2] + irvals[2]);
                Survey->Sources[cnt]->SetPolarisation(rvals[3], rvals[4], rvals[5]);
            }
            Parser->JumpToLocation(bp);
        }
        return ;
    }		// -----  end of method AEMSurveyReader::ReadASCIIAEMFile  -----


}		// -----  end of Lemma  name  -----
