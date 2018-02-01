/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/01/2018 10:45:39 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     tirons@egi.utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Lemma Software, LLC
 */

#include "CircularLoop.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const CircularLoop &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  CircularLoop
    //      Method:  CircularLoop
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    CircularLoop::CircularLoop (const ctor_key&) : CompactSupportEMSource( CompactSupportEMSource::ctor_key()  ) {

    }  // -----  end of method CircularLoop::CircularLoop  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CircularLoop
    //      Method:  CircularLoop
    // Description:  DeSerializing constructor (locked)
    //--------------------------------------------------------------------------------------
    CircularLoop::CircularLoop (const YAML::Node& node, const ctor_key&) : CompactSupportEMSource(node, CompactSupportEMSource::ctor_key()) {

    }  // -----  end of method CircularLoop::CircularLoop  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CircularLoop
    //      Method:  NewSP()
    // Description:  public constructor returing a shared_ptr
    //--------------------------------------------------------------------------------------
    std::shared_ptr< CircularLoop >  CircularLoop::NewSP() {
        return std::make_shared< CircularLoop >( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  CircularLoop
    //      Method:  ~CircularLoop
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    CircularLoop::~CircularLoop () {

    }  // -----  end of method CircularLoop::~CircularLoop  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CircularLoop
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  CircularLoop::Serialize (  ) const {
        YAML::Node node = CompactSupportEMSource::Serialize();
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method CircularLoop::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  CircularLoop
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<CircularLoop> CircularLoop::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() !=  "CircularLoop" ) {
            throw  DeSerializeTypeMismatch( "CircularLoop", node.Tag());
        }
        return std::make_shared< CircularLoop > ( node, ctor_key() );
    }		// -----  end of method CircularLoop::DeSerialize  -----

} // ----  end of namespace Lemma  ----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */


