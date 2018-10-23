/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/08/2011
  @version  $Id: sintransintegrationkernel.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef  SINTRANSINTEGRATIONKERNEL_INC
#define  SINTRANSINTEGRATIONKERNEL_INC

#include "integrationkernel.h"

namespace Lemma {

    // ===================================================================
    //  Class:  SinTransIntegrationKernel
    /**
      \brief    Integration kernel for testing sinine transform.
      \details  Evaluates sinine at the argument, using lagged convolution.
                all of these kernels have exact solutions.
     */
    // ===================================================================
    class SinTransIntegrationKernel : public IntegrationKernel<Real> {

        public:

            // ====================  LIFECYCLE     =======================

            /** Returns a pointer to a new object of type SinTransIntegrationKernel.
             * It allocates all necessary memory.
             */
            static SinTransIntegrationKernel* New();

            /** Deletes this object. Delete also disconnects any
             * attachments to this object.
             */
            void Delete();

            // ====================  OPERATORS     =======================

            Real Argument(const Real&x, const int& iRelated);

            int GetNumRel();

            /// Sets the value 'A' used in test cases
            void SetA(const Real& A);

            /** Sets the integral to evaluate (1, 2, 3)
             */
            void SetIntegral(const int& i);

            /** Returns the analytical answer at A and B */
            Real GetAnalytical(const Real& B);
            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /** Deletes this object. Delete also disconnects any
             * attachments to this object.
             */
            void Release();

            /// Default protected constructor.
            SinTransIntegrationKernel (const std::string& name);

            /// Default protected constructor.
            ~SinTransIntegrationKernel ();

            // ====================  DATA MEMBERS  =========================

            /** Value used in test cases  */
            Real A;

            /** Test integral to evaluate */
            int Integral;

        private:

    }; // -----  end of class  SinTransIntegrationKernel  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef SINTRANSINTEGRATIONKERNEL_INC  -----
