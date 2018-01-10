/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/27/2013 01:44:46 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2013-2018 Trevor Irons
 */

#include "LayeredEarthEMReader.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const LayeredEarthEMReader &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }


    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  LayeredEarthEMReader
    //      Method:  LayeredEarthEMReader
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    LayeredEarthEMReader::LayeredEarthEMReader ( const ctor_key& key ) : LemmaObject( key ),
        LayEarth(nullptr) {

    }  // -----  end of method LayeredEarthEMReader::LayeredEarthEMReader  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  LayeredEarthEMReader
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr<LayeredEarthEMReader> LayeredEarthEMReader::NewSP() {
        return std::make_shared<LayeredEarthEMReader>( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  LayeredEarthEMReader
    //      Method:  ~LayeredEarthEMReader
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    LayeredEarthEMReader::~LayeredEarthEMReader () {
    }  // -----  end of method LayeredEarthEMReader::~LayeredEarthEMReader  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  LayeredEarthEMReader
    //      Method:  GetLayeredEarth
    //--------------------------------------------------------------------------------------
    std::shared_ptr<LayeredEarthEM> LayeredEarthEMReader::GetLayeredEarth (  ) {
        return LayEarth;
    }		// -----  end of method LayeredEarthEMReader::GetLayeredEarth  -----


    //--------------------------------------------------------------------------------------
    //       Class:  LayeredEarthEMReader
    //      Method:  ReadASCIIInputFile
    //--------------------------------------------------------------------------------------
    void LayeredEarthEMReader::ReadASCIIInputFile ( const std::string& name ) {

        auto Parser = ASCIIParser::NewSP();
        LayEarth = LayeredEarthEM::NewSP();

        Parser->SetCommentString("//");
        Parser->Open(name);

        int nlay = Parser->ReadInts( 1 )[0]; // non-air layers
            LayEarth->SetNumberOfLayers(nlay+1);

        VectorXcr sigma = VectorXcr::Zero(nlay+1);
        VectorXr  thick(nlay-1);
        for (int ilay=1; ilay<nlay; ++ilay) {
            std::vector<Real> rval = Parser->ReadReals(2);
            sigma(ilay) =  1./rval[0];
            thick(ilay-1) =  rval[1]; //CondMod[ilay+1];
        }
        sigma(nlay) = 1./Parser->ReadReals(1)[0];
        LayEarth->SetLayerConductivity(sigma);
        if (thick.size() > 0) LayEarth->SetLayerThickness(thick);
        return ;
    }		// -----  end of method LayeredEarthEMReader::ReadASCIIInputFile  -----


}		// -----  end of Lemma  name  -----
