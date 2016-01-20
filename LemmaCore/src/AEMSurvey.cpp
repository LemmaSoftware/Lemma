/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/24/2013 04:09:04 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#include "AEMSurvey.h"

namespace Lemma {

// ====================  FRIEND METHODS  =====================

std::ostream &operator<<(std::ostream &stream, const AEMSurvey &ob) {

    stream << *(LemmaObject*)(&ob);

    return stream;
}

// ====================  LIFECYCLE     =======================

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  AEMSurvey
// Description:  constructor (protected)
//--------------------------------------------------------------------------------------
AEMSurvey::AEMSurvey (const std::string& name) : LemmaObject(name) {

}  // -----  end of method AEMSurvey::AEMSurvey  (constructor)  -----


//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  New()
// Description:  public constructor
//--------------------------------------------------------------------------------------
AEMSurvey* AEMSurvey::New() {
    AEMSurvey*  Obj = new AEMSurvey("AEMSurvey");
    Obj->AttachTo(Obj);
    return Obj;
}

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  ~AEMSurvey
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
AEMSurvey::~AEMSurvey () {

    for (unsigned int isc=0; isc<Sources.size(); ++isc) Sources[isc]->Delete();

}  // -----  end of method AEMSurvey::~AEMSurvey  (destructor)  -----

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  Delete
// Description:  public destructor
//--------------------------------------------------------------------------------------
void AEMSurvey::Delete() {
    this->DetachFrom(this);
}

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  Release
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
void AEMSurvey::Release() {
    delete this;
}

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  GetSource
//--------------------------------------------------------------------------------------
DipoleSource* AEMSurvey::GetSource ( const int& isource ) {
    return Sources[isource] ;
}		// -----  end of method AEMSurvey::GetSource  -----

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  GetFrequencies
//--------------------------------------------------------------------------------------
VectorXr AEMSurvey::GetFrequencies (  ) {
    return Freqs;
}		// -----  end of method AEMSurvey::GetFrequencies  -----


//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  GetNumberOfSources
//--------------------------------------------------------------------------------------
int AEMSurvey::GetNumberOfSources (  ) {
    return  static_cast<int>(Sources.size());
}		// -----  end of method AEMSurvey::GetNumberOfSources  -----


}		// -----  end of Lemma  name  -----
