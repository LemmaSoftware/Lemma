/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/05/2010
  @version  $Id: instrument.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef __INSTRUMENT_H
#define __INSTRUMENT_H

#include "LemmaObject.h"

namespace Lemma {

	// =======================================================================
	//        Class:  Instrument
    /// \ingroup LemmaCore
	/// \brief    Abstract class for instruments
	/// \details
	// =======================================================================
	class Instrument : public LemmaObject {

		friend std::ostream &operator<<(std::ostream &stream,
			const Instrument &ob);

		public:

			// ====================  LIFECYCLE     ===========================

            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            YAML::Node Serialize() const;

			// ====================  OPERATORS     ===========================

			// ====================  OPERATIONS    ===========================
            //virtual void ForwardModelOnOctreeMesh()=0;

			// ====================  ACCESS        ===========================

			// ====================  INQUIRY       ===========================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const {
                return this->CName;
            }

			// ====================  DATA MEMBERS  ===========================

		protected:

			// ====================  LIFECYCLE     ===========================

            /// Default protected constructor.
			Instrument ( const YAML::Node& node );

			/// Default protected constructor.
			Instrument ( );

			/// Default protected constructor.
			~Instrument ();

		private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "Instrument";

	}; // -----  end of class  Instrument  -----

}		// -----  end of Lemma  name  -----

#endif // __INSTRUMENT_H
