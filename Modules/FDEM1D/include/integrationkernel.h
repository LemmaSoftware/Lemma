/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/07/2011
  @version  $Id: integrationkernel.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  INTEGRATIONKERNEL_INC
#define  INTEGRATIONKERNEL_INC

#include "LemmaObject.h"

namespace Lemma {

    // ===================================================================
    //  Class:  IntegrationKernel
    /**
      \brief    Abstract class that computes arguments for numerical
                integrators.
      \details  Needs to provide Argument function, as well as be able to
                set necessary integration parameters.
     */
    // ===================================================================
    template <typename T>
    class IntegrationKernel : public LemmaObject {

        public:

            // ====================  LIFECYCLE     =======================


            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /** Returns the argument of a function at a given argument. Also
             *  capable or returning related function values.
             */
            //template <typename T>
            virtual T Argument(const Real& x, const int& iRelated)=0;

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            virtual int GetNumRel()=0;

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            IntegrationKernel (const std::string& name);

            /// Default protected constructor.
            ~IntegrationKernel ();

            // ====================  DATA MEMBERS  =========================

        private:

    }; // -----  end of class  IntegrationKernel  -----

    template <typename T>
    IntegrationKernel<T>::IntegrationKernel(const std::string& name) :
        LemmaObject(name) {
    }

    template<typename T>
    IntegrationKernel<T>::~IntegrationKernel( ) {
    }

//     template <typename T>
//     T IntegrationKernel<T>::Argument(const Real& x, const int& iRelated) {
//         return 11.;
//     }
//
//     template <typename T>
//     int IntegrationKernel<T>::GetNumRel( ) {
//         return 0;
//     }

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef INTEGRATIONKERNEL_INC  -----
