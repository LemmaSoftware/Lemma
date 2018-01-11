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
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 * @copyright Copyright (c) 2018, Trevor Irons
 */

#include "AEMSurvey.h"

namespace Lemma {

// ====================  FRIEND METHODS  =====================

std::ostream &operator<<(std::ostream &stream, const AEMSurvey &ob) {
    stream << ob.Serialize() << "\n";
    return stream;
}

// ====================  LIFECYCLE     =======================

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  AEMSurvey
// Description:  constructor (protected)
//--------------------------------------------------------------------------------------
AEMSurvey::AEMSurvey (const ctor_key& key) : LemmaObject(key) {

}  // -----  end of method AEMSurvey::AEMSurvey  (constructor)  -----


//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  NewSP()
// Description:  public constructor
//--------------------------------------------------------------------------------------
std::shared_ptr<AEMSurvey> AEMSurvey::NewSP() {
    return std::make_shared<AEMSurvey>(ctor_key());
}

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  ~AEMSurvey
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
AEMSurvey::~AEMSurvey () {

}  // -----  end of method AEMSurvey::~AEMSurvey  (destructor)  -----

//--------------------------------------------------------------------------------------
//       Class:  AEMSurvey
//      Method:  GetSource
//--------------------------------------------------------------------------------------
std::shared_ptr<DipoleSource> AEMSurvey::GetSource ( const int& isource ) {
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
