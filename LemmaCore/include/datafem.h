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

#ifndef DATAFEM_H
#define DATAFEM_H

#include "data.h"

namespace Lemma {

// ===================================================================
//        Class:  DataFEM
/// \brief
/// \details
// ===================================================================
	class DataFEM : public Data {

		friend std::ostream &operator<<(std::ostream &stream,
			const DataFEM &ob);

	public:

	// ====================  LIFECYCLE     =======================

	static DataFEM* New();

	DataFEM* Clone();

	void Delete();

	// ====================  OPERATORS     =======================


	// ====================  OPERATIONS    =======================

	void Zero();

	Real Norm(Data* Data2);


	// ====================  ACCESS        =======================

	/// Set data dimensions
	void SetSize(const int &nobs,const int &nfreq);

	/// Input the data
	void SetData(const MatrixXr &inputdata);

	/// Input uncertainties
	void SetUncertainties(const MatrixXr &uncertain);

	/// Input the positions
	void SetPositions(const Vector3Xr &positions);

	/// Set the frequencies
	void SetFreq(const VectorXr &freqs);

	/// Set Tx Orientation
	void SetTxOrientation(const Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> &txorientation);

	/// Set Rx Orientation
	void SetRxOrientation(const Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> &rxorientation);

	/// Set the transmitter moments
	void SetTxMom(const VectorXr &txmom);

	/// Set Tx/Rx Separation
	void SetTxRxSep(const Vector3Xr &txrxsep);

	/// Set Scale Factor
	void SetScaleFac(const VectorXr &scalefac);

	// ====================  INQUIRY       =======================

	/// Get number of frequencies used
	int GetnFreq();

	/// Get number of observation locations
	int GetnObs();

	/// Get the data cube
	MatrixXr GetFEMDataCube();

	/// Get the uncertainties
	MatrixXr GetFEMUncertainCube();

	/// Get a single data coefficient
	Real GetDataCoeff(const int &x, const int &y);

	/// Get the frequency array
	VectorXr GetFreqs();

	/// Get the position array
	Vector3Xr GetXYZ();

	/// Get the position for a single datum
	VectorXr GetSingleXYZ(const int &x);

	/// Get array of transmitter orientations
	Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> GetTxOrien();

	/// Get array of receiver orientations
	Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> GetRxOrien();

	/// Get array of Tx Moments
	VectorXr GetTxMom();

	/// Get Tx/Rx Separation
	Vector3Xr GetTxRxSep();

	/// Get Scale Factor
	VectorXr GetScaleFac();

	protected:

	// ====================  LIFECYCLE     =======================

		/// Default protected constructor.
		DataFEM (const std::string &name);

		/// Default protected constructor.
		~DataFEM ();

		void Release();

	// ====================  DATA MEMBERS  =========================

	/// Number of frequencies
	int nFreq;

	/// Number of observation locations
	int nObs;

	/// Data cube containing the inphase and quadrature
	MatrixXr FEMDataCube;

	/// Data cube containing the uncertainties
	MatrixXr FEMUncertainCube;

	/// Array of frequencies
	VectorXr freq;

	/// Position vector, with z as height above the ground surface (pos)
	Vector3Xr xyz;

	/// Array of Tx orientations (x,y,z) for each freq
	Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> TxOrientation;

	/// Array of Rx orientations (x,y,z) for each freq
	Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> RxOrientation;

	/// Transmitter moments, one for each frequency
	VectorXr TxMom;

	/// Tx/Rx Separation in local, right hand system with x aligned with flight
	Vector3Xr TxRxSep;

	/// Scale factor as described in EM1DFM manual
	VectorXr ScaleFac;


	private:

}; // -----  end of class  DataFEM  -----
} // end of namespace Lemma
#endif

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */

