/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     12/28/2011
  @version  $Id: datareadertem.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef __DATAREADERTEM_H
#define __DATAREADERTEM_H

#include "datareader.h"
#include "datatem.h"

namespace Lemma {
// ===================================================================
//        Class:  DataReaderTem
/// \brief
/// \details
// ===================================================================
class DataReaderTem : DataReader {

	public:

		// ====================  LIFECYCLE     =======================

		static DataReaderTem* New();

		void Delete();

		// ====================  OPERATORS     =======================

		// ====================  OPERATIONS    =======================

		/// Read in the data file
		void ReadData(const std::string &datafile);

		// ====================  ACCESS        =======================

		/// Attach the data object
		void SetDataTEM(DataTEM* inputtemp);

		/// Output the data object
		DataTEM* GetData();

		/// Return number of gates
		int GetnGates();

		/// Return number of soundings
		int GetnObs();

		// ====================  INQUIRY       =======================



	protected:

		// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		DataReaderTem (const std::string &name);

		/// Default protected constructor.
		~DataReaderTem ();

		void Release();

		// ====================  DATA MEMBERS  =========================

		/// Input data
		DataTEM* InputData;

	private:

}; // -----  end of class  Datareadertem  -----
} // end of namespace Lemma
#endif // __DATAREADERTEM_H
