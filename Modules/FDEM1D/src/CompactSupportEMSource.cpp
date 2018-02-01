/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/01/2018 08:33:23 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */


#include "CompactSupportEMSource.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const CompactSupportEMSource &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  CompactSupportEMSource
    //      Method:  CompactSupportEMSource
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    CompactSupportEMSource::CompactSupportEMSource (const ctor_key&) : LemmaObject( LemmaObject::ctor_key() ) {

    }  // -----  end of method CompactSupportEMSource::CompactSupportEMSource  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CompactSupportEMSource
    //      Method:  CompactSupportEMSource
    // Description:  DeSerializing constructor (locked)
    //--------------------------------------------------------------------------------------
    CompactSupportEMSource::CompactSupportEMSource (const YAML::Node& node, const ctor_key&) : LemmaObject(node, LemmaObject::ctor_key()) {

    }  // -----  end of method CompactSupportEMSource::CompactSupportEMSource  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CompactSupportEMSource
    //      Method:  NewSP()
    // Description:  public constructor returing a shared_ptr
    //--------------------------------------------------------------------------------------
    std::shared_ptr< CompactSupportEMSource >  CompactSupportEMSource::NewSP() {
        return std::make_shared< CompactSupportEMSource >( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  CompactSupportEMSource
    //      Method:  ~CompactSupportEMSource
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    CompactSupportEMSource::~CompactSupportEMSource () {

    }  // -----  end of method CompactSupportEMSource::~CompactSupportEMSource  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CompactSupportEMSource
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  CompactSupportEMSource::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method CompactSupportEMSource::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CompactSupportEMSource
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<CompactSupportEMSource> CompactSupportEMSource::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() !=  "CompactSupportEMSource" ) {
            throw  DeSerializeTypeMismatch( "CompactSupportEMSource", node.Tag());
        }
        return std::make_shared< CompactSupportEMSource > ( node, ctor_key() );
    }		// -----  end of method CompactSupportEMSource::DeSerialize  -----

} // ----  end of namespace Lemma  ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */

