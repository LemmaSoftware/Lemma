/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
  @file
  @date      09/23/2013 11:05:48 AM
  @version   $Id$
  @author    Trevor Irons (ti)
  @email     Trevor.Irons@xri-geo.com
  @copyright Copyright (c) 2013, XRI Geophysics, LLC
  @copyright Copyright (c) 2013, Trevor Irons
 **/

#include "RectilinearGridReader.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const RectilinearGridReader &ob) {

        stream << *(GridReader*)(&ob);

        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  RectilinearGridReader
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    RectilinearGridReader::RectilinearGridReader (const std::string& name) : GridReader(name),
        rGrid( nullptr ), Parser( nullptr ) {

    }  // -----  end of method RectilinearGridReader::RectilinearGridReader  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  NewSP()
    // Description:  constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr< RectilinearGridReader > RectilinearGridReader::NewSP() {
        std::shared_ptr<RectilinearGridReader> sp(new  RectilinearGridReader("RectilinearGridReader"), LemmaObjectDeleter() );
        return sp;
    }



    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  ~RectilinearGridReader
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    RectilinearGridReader::~RectilinearGridReader () {
    }  // -----  end of method RectilinearGridReader::~RectilinearGridReader  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  ReadASCIIGridFile
    //--------------------------------------------------------------------------------------
    void RectilinearGridReader::ReadASCIIGridFile ( const std::string& name  ) {

        rGrid = RectilinearGrid::NewSP();
        Parser = ASCIIParser::NewSP();

        Parser->SetCommentString("//");

        Parser->Open(name);

        std::vector<int> vals = Parser->ReadInts(3);
        rGrid->SetDimensions(vals[0], vals[1], vals[2]);

        std::vector<Real> rvals = Parser->ReadReals(3);
        rGrid->SetOffset(rvals[0], rvals[1], rvals[2]);
        rvals.clear();

        rvals = Parser->ReadReals( vals[0] );
        VectorXr temp = VectorXr::Map(&rvals[0], vals[0]);
        VectorXr hx = temp;

        rvals.clear();
        rvals = Parser->ReadReals( vals[1] );
        VectorXr hy = VectorXr::Map(&rvals[0], vals[1]);

        rvals.clear();
        rvals = Parser->ReadReals( vals[2] );
        VectorXr hz = VectorXr::Map(&rvals[0], vals[2]);

        rGrid->SetSpacing(hx, hy, hz);

        // Read in model(s)/data? Where should this be done?
        Parser->Close();

        return;

    }		// -----  end of method RectilinearGridReader::ReadSCIIGridFile  -----


    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  GetGrid
    //--------------------------------------------------------------------------------------
    std::shared_ptr<Grid> RectilinearGridReader::GetGrid (  ) {
        return std::static_pointer_cast<Grid> (rGrid);
    }		// -----  end of method RectilinearGridReader::GetGrid  -----


}		// -----  end of Lemma  name  -----
