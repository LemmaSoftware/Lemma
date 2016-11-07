/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file

  @author   Trevor Irons
  @date     05/18/2012
 **/

#ifndef  KERNELEM1DBASE_INC
#define  KERNELEM1DBASE_INC

#include "LemmaObject.h"

namespace Lemma {

    // Transverse electric or magnetic mode
    enum EMMODE {TM, TE, NONE};

    // ===================================================================
    //  Class:  KernelEM1DBase
    /**
      @class
      \brief   Pure virtual base class of KernelEm1D
      \details Defines interface for HankelTransform classes
     */
    // ===================================================================
    class KernelEM1DBase : public LemmaObject {

        friend class KernelEM1DManager;

        public:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            KernelEM1DBase ( ) : LemmaObject( ) {

            }

            /// Default protected constructor.
            ~KernelEM1DBase () {
            }

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /** Returns the Complex bessel argument to be evaluated for a given
			 * lambda value
             */
            virtual Complex BesselArg(const Real& lambda)=0;

            /** Returns the complex bessel argument. Same as above, but does not
             *  call reflection coefficient calculation in KernelEM1DReflBase.
             *  @param[in] lambda is the hankel wave value.
             */
            virtual Complex RelBesselArg(const Real& lambda)=0;

            // ====================  ACCESS        =======================
            virtual void SetIk(const int& ik)=0;

            virtual void SetMode(const EMMODE& mode)=0;

            int GetManagerIndex() {
                return this->managerIdx;
            }

            // ====================  INQUIRY       =======================
            virtual int GetNumRel()=0;

            virtual int GetBesselOrder()=0;

            virtual Complex GetZm()=0;

            virtual Complex GetYm()=0;

            virtual Complex GetZs()=0;

            virtual Complex GetKs()=0;

        protected:

            // ====================  DATA MEMBERS  =========================

            /** Where does this kernel live on the vector managed by the manager */
            int  managerIdx;

        private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "KernelEM1DBase";

    }; // -----  end of class  KernelEM1DBase  -----

} // namespace Lemma

#endif   // ----- #ifndef KERNELEM1DBASE_INC  -----
