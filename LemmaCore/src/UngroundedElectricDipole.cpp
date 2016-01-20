/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/29/2014 08:55:18 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "UngroundedElectricDipole.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const UngroundedElectricDipole &ob) {

        stream << *(DipoleSource*)(&ob);

        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  UngroundedElectricDipole
    //      Method:  UngroundedElectricDipole
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    UngroundedElectricDipole::UngroundedElectricDipole (const std::string& name) : DipoleSource(name) {

    }  // -----  end of method UngroundedElectricDipole::UngroundedElectricDipole  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  UngroundedElectricDipole
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    UngroundedElectricDipole* UngroundedElectricDipole::New() {
        UngroundedElectricDipole*  Obj = new UngroundedElectricDipole("UngroundedElectricDipole");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  UngroundedElectricDipole
    //      Method:  ~UngroundedElectricDipole
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    UngroundedElectricDipole::~UngroundedElectricDipole () {

    }  // -----  end of method UngroundedElectricDipole::~UngroundedElectricDipole  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  UngroundedElectricDipole
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void UngroundedElectricDipole::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  UngroundedElectricDipole
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void UngroundedElectricDipole::Release() {
        delete this;
    }

}		// -----  end of Lemma  name  -----

