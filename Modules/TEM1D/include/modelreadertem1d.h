/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/19/2011
  @version  $Id: modelreadertem1d.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef __MODELREADERTEM1D_H
#define __MODELREADERTEM1D_H

#include "modelreader.h"

namespace Lemma {

// ===================================================================
//        Class:  ModelReaderTem1D
/// \brief Model reader class for forward modelling 1D TEM data
/// \details This is a mostly abstract class.
// ===================================================================
class ModelReaderTem1D : public ModelReader {

	friend std::ostream &operator<<(std::ostream &stream,
		const ModelReaderTem1D &ob);

	public:

		// ====================  LIFECYCLE     =======================

		static ModelReaderTem1D* New();

		void Delete();

		// ====================  OPERATORS     =======================

		// ====================  OPERATIONS    =======================

		//void ReadParameters(/* Pass file name */);

		// ====================  ACCESS        =======================

		// ====================  INQUIRY       =======================


	protected:

		// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		ModelReaderTem1D (const std::string &name);

		/// Default protected constructor.
		~ModelReaderTem1D ();

		void Release();

		/// Read in the Earth Model file and attach
		//void ReadModel();

		/// Read in the instrument parameters and attach
		//void ReadInstrument();

		/// Read in the waveform file (future)
		//void ReadWaveform();

		// ====================  DATA MEMBERS  =========================

		//std::string modelfile;

		//std::string instrumentfile;

		//std::string waveformfile;

	private:

}; // -----  end of class  ModelReaderTem1D  -----
} // end of namespace lemma
#endif // __MODELREADERTEM1D_H
