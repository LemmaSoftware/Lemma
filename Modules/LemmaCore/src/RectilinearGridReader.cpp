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

    std::ostream &operator << (std::ostream &stream, const RectilinearGridReader &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  RectilinearGridReader
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    RectilinearGridReader::RectilinearGridReader ( const ctor_key& key ) : GridReader( key ),
        rGrid( nullptr ), Parser( nullptr ) {

    }  // -----  end of method RectilinearGridReader::RectilinearGridReader  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  RectilinearGridReader
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    RectilinearGridReader::RectilinearGridReader (const YAML::Node& node, const ctor_key&  key) : GridReader(node, key) {
    }  // -----  end of method RectilinearGridReader::RectilinearGridReader  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  NewSP()
    // Description:  constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr< RectilinearGridReader > RectilinearGridReader::NewSP() {
        return std::make_shared< RectilinearGridReader > ( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  ~RectilinearGridReader
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    RectilinearGridReader::~RectilinearGridReader () {
    }  // -----  end of method RectilinearGridReader::~RectilinearGridReader  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  RectilinearGridReader::Serialize (  ) const {
        YAML::Node node = GridReader::Serialize();;
        node.SetTag( GetName() );
        //node["CommentString"] = CommentString;
        //node["BufferSize"] = BufferSize;
        return node;
    }		// -----  end of method GridReader::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<RectilinearGridReader> RectilinearGridReader::DeSerialize ( const YAML::Node& node ) {
        if (node.Tag() != "RectilinearGridReader") {
            throw  DeSerializeTypeMismatch( "RectilinearGridReader", node.Tag());
        }
        return std::make_shared< RectilinearGridReader >( node, ctor_key() ); //, ctor_key() );
    }		// -----  end of method GridReader::DeSerialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGridReader
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string RectilinearGridReader::GetName (  ) const {
        return CName;
    }		// -----  end of method RectilinearGridReader::get_GetName  -----

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
