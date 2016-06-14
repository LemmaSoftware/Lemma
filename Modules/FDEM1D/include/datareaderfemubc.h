/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     01/03/2013
  @version  $Id$
 **/

#ifndef __DATAREADERFEMUBC_H
#define __DATAREADERFEMUBC_H

#include "datareader.h"
#include "datafem.h"

namespace Lemma {


// ===================================================================
//        Class:  DataReaderFemUBC
/// \brief  Reads UBC-format observation files
/// \details This class reads UBC-format EM1DFM observation files and
/// populates a DataFEM class.  Assumptions: the frequencies and coil
/// geometries remain fixed throughout a survey.
// ===================================================================
	class DataReaderFemUBC : DataReader {

		friend std::ostream &operator<<(std::ostream &stream,
			const DataReaderFemUBC &ob);


	public:

	// ====================  LIFECYCLE     =======================

		static DataReaderFemUBC* New();

		void Delete();

	// ====================  OPERATORS     =======================

	// ====================  OPERATIONS    =======================

		void ReadData(const std::string &datafile,int vbs = 0);

	// ====================  ACCESS        =======================

		///Attach the data object
		void SetDataFEM(DataFEM* inputtemp);


	// ====================  INQUIRY       =======================
		/// Return the data object
		DataFEM* GetData();

	protected:

	// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		DataReaderFemUBC (const std::string &name);

		/// Default protected constructor.
		~DataReaderFemUBC ();

		void Release();

		/// Load read data into data object
		void SendData();

	// ====================  DATA MEMBERS  =========================

	DataFEM* InputData;

	int nObs;
	int nFreq;
	MatrixXr DataMatrix;
	MatrixXr Uncertainties;
	Vector3Xr PositionVec;
	VectorXr Freqs;
	VectorXr TxMom;
	Vector3Xr TxRxSep;
	Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> TxOrientation;
	Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> RxOrientation;
	VectorXr ScaleFac;



	private:

}; // -----  end of class  DataReaderFemUBC  -----
} // end of namespace Lemma
#endif

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
