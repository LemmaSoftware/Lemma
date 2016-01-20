/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     08/09/2010
  @version  $Id: quasinewtonbfgs.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  QUASINEWTONBFGS_INC
#define  QUASINEWTONBFGS_INC

#include "inversesolver.h"

namespace Lemma {


    // ===================================================================
    //  Class:  QuasiNewtonBFGS
    /**
      @class  QuasiNewtonBFGS
      \brief  Implimentation of a quasi-newton BFGS solver applied to
              inverse problems.
      \details Based of the algorithm described in Nocedal and Wright book.
     */
    // ===================================================================
    class QuasiNewtonBFGS : public InverseSolver {

        friend  std::ostream &operator<<(std::ostream &stream,
                const QuasiNewtonBFGS &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /** Returns a pointer to a new object of type QuasiNewtonBFGS.
             * It allocates all necessary memory.
             */
            static QuasiNewtonBFGS* New();

            /**
             * @copybrief LemmaObject::Delete()
             * @copydetails LemmaObject::Delete()
             */
            void Delete();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            /** @copybrief InverseSolver::NumberOfIterations()
             *  @copydetails InverseSolver::NumberOfIterations()
             */
            int NumberOfIterations();

            /** @copybrief InverseSolver::Success()
             *  @copydetails InverseSolver::Success()
             */
            bool Success();

            /** @copybrief InverseSolver::GetPhiMVector()
             *  @copydetails InverseSolver::GetPhiMVector()
             */
            VectorXr GetPhiMVector();

            /** @copybrief InverseSolver::GetPhiDVector()
             *  @copydetails InverseSolver::GetPhiDVector()
             */
            VectorXr GetPhiDVector();

            /// TODO remove this, test
            void PrintNorm();

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            QuasiNewtonBFGS (const std::string& cname);

            /// Default protected constructor.
            ~QuasiNewtonBFGS ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  OPERATIONS    =======================

            /// Just a filler
            void FillInG(const Vector3r& pos, const Vector3r& step);

            // ====================  DATA MEMBERS  =========================

        private:

    }; // -----  end of class  QuasiNewtonBFGS  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef QUASINEWTONBFGS_INC  -----
