/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/18/2011
  @version  $Id: modelreader.h 201 2015-01-03 00:07:47Z tirons $
 **/

#ifndef __MODELREADER_H
#define __MODELREADER_H

#include "LemmaObject.h"
#include "layeredearthem.h"
#include "PolygonalWireAntenna.h"
#include "receiverpoints.h"

namespace Lemma {

// ===================================================================
//        Class:  ModelReader
/// \brief Model and instrument reader for forward modelling.
/// \details This is an abstract class.
// ===================================================================
class ModelReader : public LemmaObject {

	friend std::ostream &operator<<(std::ostream &stream,
		const ModelReader &ob);

	public:

		// ====================  LIFECYCLE     =======================

		static ModelReader* New();

		void Delete();

		// ====================  OPERATORS     =======================

		// ====================  OPERATIONS    =======================

		// ====================  ACCESS        =======================

		// ====================  INQUIRY       =======================



	protected:

		// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		ModelReader (const std::string &name);

		/// Default protected constructor.
		~ModelReader ();
        /**
        * @copybrief LemmaObject::Release()
        * @copydetails LemmaObject::Release()
        */
		void Release();
		// ====================  DATA MEMBERS  =========================

	private:

}; // -----  end of class  ModelReader  -----
} // end of namespace lemma
#endif // __MODELREADER_H

