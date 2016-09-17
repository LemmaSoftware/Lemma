/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/01/2010
  @version  $Id: data.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  _DATA_H_INC
#define  _DATA_H_INC

#include "LemmaObject.h"

namespace Lemma {

    // ===================================================================
    //        Class:  Data
    /**
     *  \ingroup LemmaCore
     *  \brief Abstract class for data from an instrument
     *  \details Currently provides no functionality, but this will likely
     *          change in the near future.
     */
    // ===================================================================
    class Data : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const Data &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /** Copies the *structure* of this class to a new object and
             * returns a pointer to this object. Memory management is
             * the responsibility of the receiver!. The data values are
             * not copied.
             */
            virtual Data* Clone()=0;

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /** Clears all data arrays and containers
             */
            virtual void Zero()=0;

            /** Data2 needs to be compatible with the calling class. Data2 will
             * be cast into the calling class type. Most of the time this
             * cast will not perform anything, as Data2 *is* the same type
             * as the calling class.
             */
            virtual Real Norm(Data* Data2)=0;

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const {
                return CName;
            }

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor. */
            Data (  );

            /** Default protected constructor. */
            ~Data ();

            // ====================  DATA MEMBERS  =========================

        private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "Data";

    }; // -----  end of class  Data  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef _DATA_H_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
