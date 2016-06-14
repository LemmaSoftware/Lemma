/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     01/03/2013
  @version  $Id$
 **/

#ifndef __MODELREADERFEM1D_H
#define __MODELREADERFEM1D_H

#include "modelreader.h"

namespace Lemma {


// ===================================================================
//        Class:  ModelReaderFem1D
/// \brief
/// \details
// ===================================================================
	class ModelReaderFem1D : public ModelReader {

		friend std::ostream &operator<<(std::ostream &stream,
			const ModelReaderFem1D &ob);

	public:

	// ====================  LIFECYCLE     =======================

	static ModelReaderFem1D* New();

	void Delete();

	// ====================  OPERATORS     =======================

	// ====================  OPERATIONS    =======================

	// ====================  ACCESS        =======================

	// ====================  INQUIRY       =======================



	protected:

	// ====================  LIFECYCLE     =======================

	/// Default protected constructor.
	ModelReaderFem1D (const std::string &name);

	/// Default protected constructor.
	~ModelReaderFem1D ();

	void Release();

	// ====================  DATA MEMBERS  =========================

	private:

}; // -----  end of class  ModelReaderFem1D  -----
} // end of namespace Lemma
#endif // __MODELREADERFEM1D_H
