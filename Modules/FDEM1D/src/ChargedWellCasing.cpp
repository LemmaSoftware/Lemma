/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      14/04/19 15:39:02
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2019, University of Utah
 * @copyright Copyright (c) 2019, Lemma Software, LLC
 */


#include "ChargedWellCasing.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const ChargedWellCasing &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  ChargedWellCasing
    //      Method:  ChargedWellCasing
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    ChargedWellCasing::ChargedWellCasing (const ctor_key&) : LemmaObject( LemmaObject::ctor_key()  ) {

    }  // -----  end of method ChargedWellCasing::ChargedWellCasing  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ChargedWellCasing
    //      Method:  ChargedWellCasing
    // Description:  DeSerializing constructor (locked)
    //--------------------------------------------------------------------------------------
    ChargedWellCasing::ChargedWellCasing (const YAML::Node& node, const ctor_key&) :
        LemmaObject(node, LemmaObject::ctor_key()) {

    }  // -----  end of method ChargedWellCasing::ChargedWellCasing  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ChargedWellCasing
    //      Method:  NewSP()
    // Description:  public constructor returing a shared_ptr
    //--------------------------------------------------------------------------------------
    std::shared_ptr< ChargedWellCasing >  ChargedWellCasing::NewSP() {
        return std::make_shared< ChargedWellCasing >( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  ChargedWellCasing
    //      Method:  ~ChargedWellCasing
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    ChargedWellCasing::~ChargedWellCasing () {

    }  // -----  end of method ChargedWellCasing::~ChargedWellCasing  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ChargedWellCasing
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  ChargedWellCasing::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method ChargedWellCasing::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ChargedWellCasing
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<ChargedWellCasing> ChargedWellCasing::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() !=  "ChargedWellCasing" ) {
            throw  DeSerializeTypeMismatch( "ChargedWellCasing", node.Tag());
        }
        return std::make_shared< ChargedWellCasing > ( node, ctor_key() );
    }		// -----  end of method ChargedWellCasing::DeSerialize  -----

} // ----  end of namespace Lemma  ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */

