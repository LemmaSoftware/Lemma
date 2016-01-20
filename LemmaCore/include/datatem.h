/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/23/2010
  @version  $Id: datatem.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  DATATEM_INC
#define  DATATEM_INC

#include "data.h"

namespace Lemma {

    // ===================================================================
    //        Class:  DataTEM
    /// \brief  Class for TEM data.
    /// \details  A lot to do here. Just a placeholder.
    // ===================================================================
    class DataTEM : public Data {

        friend std::ostream &operator<<(std::ostream &stream,
            const DataTEM &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /** Returns pointer to new object
             */
            static DataTEM* New();

            /** @copybrief Data::Clone()
             *  @copydetails Data::Clone()
             */
            DataTEM* Clone();

            /**
             * @copybrief LemmaObject::Delete()
             * @copydetails LemmaObject::Delete()
             */
            void Delete();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            void Zero();

            /** Data2 needs to be compatible with DataTEM, that is
             *  to say that it needs to be able to be cast into the
             *  form of DataTEM. It also needs to be aligned with
             *  the calling class.
             */
            Real Norm(Data* Data2);

            // ====================  ACCESS        =======================

			/// Set data dimensions
			void SetSize(const int &nObs,const int &nGates);

			/// Input the data
			void SetData(const MatrixXr &inputdata);

			/// Input the positions
			void SetPositions(const Vector3Xr &positions);

			/// Return number of time gates
			int GetnGates();

			/// Return number of soundings
			int GetnObs();

			/// Return data matrix
			MatrixXr GetData();

			/// Return single data coefficient
			Real GetDataCoeff(const int &x,const int &y);

			/// Return time
			VectorXr GetTimes();

			/// Return positions
			Vector3Xr GetPositions();

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            DataTEM (const std::string &name);

            /// Default protected constructor.
            ~DataTEM ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  DATA MEMBERS  =========================

			MatrixXr TEMDataCube;

			VectorXr times;

			Vector3Xr locations;

			int nGates;

			int nObs;

    private:

    }; // -----  end of class  DataTEM  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef DATATEM_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
