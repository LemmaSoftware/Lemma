/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/29/2014 07:05:13 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "GroundedElectricDipole.h"


namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const GroundedElectricDipole &ob) {

        stream << *(DipoleSource*)(&ob);

        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  GroundedElectricDipole
    //      Method:  GroundedElectricDipole
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    GroundedElectricDipole::GroundedElectricDipole (const std::string& name) : DipoleSource(name) {

    }  // -----  end of method GroundedElectricDipole::GroundedElectricDipole  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  GroundedElectricDipole
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    GroundedElectricDipole* GroundedElectricDipole::New() {
        GroundedElectricDipole*  Obj = new GroundedElectricDipole("GroundedElectricDipole");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  GroundedElectricDipole
    //      Method:  ~GroundedElectricDipole
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    GroundedElectricDipole::~GroundedElectricDipole () {

    }  // -----  end of method GroundedElectricDipole::~GroundedElectricDipole  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  GroundedElectricDipole
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void GroundedElectricDipole::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  GroundedElectricDipole
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void GroundedElectricDipole::Release() {
        delete this;
    }

}		// -----  end of Lemma  name  -----

