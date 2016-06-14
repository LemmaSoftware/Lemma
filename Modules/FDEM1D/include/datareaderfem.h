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

#ifndef __DATAREADERFEM_H
#define __DATAREADERFEM_H

#include "datareader.h"
#include "datafem.h"

namespace Lemma {


// ===================================================================
//        Class:  DataReaderFem
/// \brief
/// \details
// ===================================================================
	class DataReaderFem : DataReader {

	public:

	// ====================  LIFECYCLE     =======================

		static DataReaderFem* New();

		void Delete();

	// ====================  OPERATORS     =======================

	// ====================  OPERATIONS    =======================

	// ====================  ACCESS        =======================
		Data* GetData();

	// ====================  INQUIRY       =======================



	protected:

	// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		DataReaderFem (const std::string &name);

		/// Default protected constructor.
		~DataReaderFem ();

		void Release();

	// ====================  DATA MEMBERS  =========================

	private:

}; // -----  end of class  DataReaderFem  -----
} // end of namespace Lemma
#endif
