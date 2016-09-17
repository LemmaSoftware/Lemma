/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/01/2010
  @version  $Id: datareader.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  DATAREADER_H_INC
#define  DATAREADER_H_INC

#include "LemmaObject.h"
#include "Data.h"

namespace Lemma {

// ===================================================================
//        Class:  DataReader
/// \ingroup LemmaCore
/// \brief  Abstract class whose derived members read data files and
/// converts them to Data Classes
/// \details
// ===================================================================
class DataReader : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const DataReader &ob);

    public:

        // ====================  LIFECYCLE     =======================

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        // ====================  ACCESS        =======================
        virtual Data* GetData()=0;

        // ====================  INQUIRY       =======================
        /** Returns the name of the underlying class, similiar to Python's type */
        virtual inline std::string GetName() const {
            return CName;
        }

    protected:

        // ====================  LIFECYCLE     =======================

        /// Default protected constructor.
        DataReader ( );

        /// Default protected constructor.
        ~DataReader ( );

        // ====================  DATA MEMBERS  =========================

    private:

        /** ASCII string representation of the class name */
        static constexpr auto CName = "DataReader";

}; // -----  end of class  DataReader  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef DATAREADER_H_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
