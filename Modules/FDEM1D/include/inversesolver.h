/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/14/2010
  @version  $Id: inversesolver.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  INVERSESOLVER_H_INC
#define  INVERSESOLVER_H_INC

#include "LemmaObject.h"
#include "data.h"
#include "instrument.h"
#include "octreegrid.h"

namespace Lemma {


    // ===================================================================
    //  Class:  InverseSolver
    /**
      @class  InverseSolver
      \brief   Abstract class for inverse problem solver.
      \details General solution of inverse problems.
     */
    // ===================================================================
    class InverseSolver : public LemmaObject {

        friend  std::ostream &operator<<(std::ostream &stream,
                const InverseSolver &ob);

        friend class OctreeGrid;

        public:

            // ====================  LIFECYCLE     =======================

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            /** @brief Sets the obsered data, any instance of Data.
             *  @details This may either be field data, or forward modelled
             *  data.
             *  @param[in] ObsData a pointer to the observed data class.
             */
            void SetObservedData(Data* ObsData);

            /** @brief Sets the instument that is used to create the data.
             *  @details This is used for all forward modelling.
             *  @param[in] inst is a pointer to the instrument class.
             */
            void SetInstrument(Instrument* inst);

            /** @brief Returns the predicted data.
             *  @return a pointer to the predicted data.
             */
            Data* GetPredictedData();

            // ====================  INQUIRY       =======================

            /** Returns the number of iterations it took to converge to a
             *  solution.
             */
            virtual int NumberOfIterations()=0;

            /** Returns true if the inversion converged successfully.
             */
            virtual bool Success()=0;

            /** Returns a vector of \f$ \phi_M \f$
             */
            virtual VectorXr GetPhiMVector()=0;

            /** Returns a vector of \f$ \phi_D \f$
             */
            virtual VectorXr GetPhiDVector()=0;

        protected:

            // ====================  OPERATIONS    =======================

            /// Used as a callback function from iterating mesh.
            virtual void FillInG(const Vector3r& pos, const Vector3r& step)=0;

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            InverseSolver (const std::string &name);

            /// Default protected constructor.
            ~InverseSolver ();

            // ====================  DATA MEMBERS  =========================

            /// The Observed Data
            Data* ObservedData;

            /// The Predicted Data
            Data* PredictedData;

            /// Instrument that makes the data
            Instrument*            ModelInstrument;

        private:

    }; // -----  end of class  InverseSolver  -----
}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef INVERSESOLVER_H_INC  -----
