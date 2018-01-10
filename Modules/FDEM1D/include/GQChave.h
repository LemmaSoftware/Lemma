/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/02/2010
  @version  $Id: hankeltransformgaussianquadrature.h 199 2014-12-29 19:25:20Z tirons $
 **/

#ifndef  _HANKELTRANSFORMGAUSSIANQUADRATURE_h_INC
#define  _HANKELTRANSFORMGAUSSIANQUADRATURE_h_INC

#include "HankelTransform.h"
#include "KernelEM1DBase.h"

#ifdef HAVE_BOOST_SPECIAL_FUNCTIONS
#include "boost/math/special_functions.hpp"
#endif

namespace Lemma {


	// =======================================================================
	//        Class:  GQChave
	/// \brief  Calculates hankel transform using gaussian quadrature.
	/// \details  Accurate but slow, this is a port of Alan Chave's public domain
    /// fortran code
	// =======================================================================
	class GQChave : public HankelTransform {

		friend std::ostream &operator<<(std::ostream &stream, const GQChave &ob);
		public:

			// ====================  LIFECYCLE     ===========================

			/// Default locked constructor.
			GQChave ( const ctor_key& );

            /** DeSerializing locked constructor, use DeSerialize */
            GQChave ( const YAML::Node& node, const ctor_key& );

            /// Default destructor
			~GQChave ();

            /**
             *  Returns shared_ptr to new GQChave.
             *  Location is
             *  initialized to (0,0,0) type and polarization are
             *  initialized  to nonworking values that will throw
             *  exceptions if used.
             */
			static std::shared_ptr<GQChave> NewSP();

            /** YAML Serializing method
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static std::shared_ptr< GQChave > DeSerialize(const YAML::Node& node);

			// ====================  OPERATORS     ===========================

			// ====================  OPERATIONS    ===========================

			/// Performs numerical integration using Gaussian quadrature
			/// ikk:   type of kernel depending on source and receiver couple
			/// imode: a switch for TE(0) and TM(1) mode
			/// itype: order of Bessel function
			/// rho is argument to integral
			/// wavef is the propogation constant of free space
			/// = omega * sqrt( EP*AMU )  amu = 4 pi e-7  ep = 8.85e-12
            //template <EMMODE T>
			Complex Zgauss(const int &ikk, const EMMODE &imode,
							const int &itype, const Real &rho,
							const Real &wavef, KernelEM1DBase* Kernel);

			// ====================  ACCESS        ============================

			// ====================  INQUIRY       ============================

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual inline std::string GetName() const ;

			// ====================  DATA MEMBERS  ============================

		protected:

			// ====================  OPERATIONS    ============================

			/// Modified by Yoonho Song to branch cut, June, 1996
			/// Separate Gaussian quarature integral by two interval
			/// first:  integal from 0 to wavenumber of free space
			/// second: integral from wavenunmber of free space to infinity
			///         for large arguments, it uses continued fraction also
			/// It is recommended to use nl = 1 to 6, nu =7
			/// PERFORMS AUTOMATIC CALCULATION OF BESSEL TRANSFORM TO SPECIFIED
			/// RELATIVE AND ABSOLUTE ERROR
			///
			/// ARGUMENT LIST:
			///
			/// BESR,BESI-REAL AND IMAGINARY PARTS RETURNED BY BESAUX
			/// iorder-ORDER OF THE BESSEL FUNCTION
			/// NL-LOWER LIMIT FOR GAUSS ORDER TO START COMPUTATION
			/// NU-UPPER LIMIT FOR GAUSS ORDER
			/// NU,NL=1,...7 SELECTS 3,7,15,31,63,127,AND 255 POINT GAUSS
			/// QUADRATURE BETWEEN THE ZERO CROSSINGS OF THE BESSEL FUNCTION
			/// R-ARGUMENT OF THE BESSEL FUNCTION
			/// RERR,AERR-RELATIVE AND ABSOLUTE ERROR FOR TERMINATION
			/// BESAUX TERMINATES WHEN INCREASING THE GAUSS ORDER DOES NOT
			/// CHANGE THE RESULT BY MORE THAN RERR OR WHEN THE ABSOLUTE ERROR
			/// IS LESS THAN AERR OR WHEN A GAUSS ORDER OF NU IS REACHED.
			/// NPCS-NUMBER OF PIECES INTO WHICH EACH PARTIAL INTEGRAND
			/// IS DIVIDED,
			/// ORDINARILY SET TO ONE. FOR VERY SMALL VALUES OF R WHERE
			/// THE KERNEL FUNCTION IS APPRECIABLE ONLY OVER THE FIRST FEW
			/// LOOPS OF THE BESSEL FUNCTION, NPCS MAY BE INCREASED TO ACHIEVE
			/// REASONABLE ACCURACY.
			/// NEW  IF NEW=1, THE INTEGRANDS ARE COMPUTED AND SAVED AT EACH
			/// GAUSS
			/// ORDER. IF NEW=2, PREVIOUSLY COMPUTED INTEGRANDS ARE USED. NOTE
			/// THAT ORDER,R, AND NPCS MUST NOT BE CHANGED WHEN SETTING NEW=2.
			/// IERR-ERROR PARAMETER
			/// IERR=0--NORMAL RETURN
			/// IERR=1--RESULT NOT ACCURATE TO RERR DUE TO TOO LOW A GAUSS
			///  ORDER OR CONVERGENCE NOT ACHIEVED IN BESTRX
            //template <EMMODE T>
			void Besautn(Real &besr, Real &besi, const int &iorder,
							const int &nl, const int &nu, const Real &rho,
							const Real &rerr, const Real &aerr,
							const int &npcs, int &inew, const Real &aorb,
							KernelEM1DBase* Kernel);

			/// COMPUTES BESSEL TRANSFORM OF SPECIFIED ORDER DEFINED AS
			/// INTEGRAL(FUNCT(X)*J-SUB-ORDER(X*R)*DX) FROM X=0 TO INFINITY
			/// COMPUTATION IS ACHIEVED BY INTEGRATION BETWEEN THE ASYMPTOTIC
			/// ZERO CROSSINGS OF THE BESSEL FUNCTION USING GAUSS QUADRATURE.
			/// THE RESULTING SERIES OF PARTIAL INTEGRANDS IS SUMMED BY
			/// CALCULATING THE PADE APPROXIMANTS TO SPEED UP CONVERGENCE.
			/// ARGUMENT LIST:
			/// BESR,BESI  REAL AND IMAGINARY PARTS RETURNED BY BESTRN
			/// iorder  ORDER OF THE BESSEL FUNCTIONC  NG  NUMBER OF GAUSS
			/// POINTS TO USE IN THE QUADRATURE ROUTINE.
			/// NG=1 THROUGH 7 SELECTS 3,7,15,31,63,126,AND 255 TERMS.
			///   R     ARGUMENT OF THE BESSEL FUNCTION
			///   RERR,AERR  SPECIFIED RELATIVE AND ABSOLUTE ERROR FOR THE
			///           CALCULATION.  THE INTEGRATION
			///  TERMINATES WHEN AN ADDITIONAL TERM DOES NOT CHANGE THE
			///  RESULT BY MORE THAN RERR*RESULT+AERR
			///  NPCS  NUMBER OF PIECES INTO WHICH EACH PARTIAL I
			///        NTEGRAND IS DIVIDED,
			///        ORDINARILY SET TO ONE. FOR VERY SMALL VALUES OF RANGE
			///        WHERE THE KERNEL FUNCTION IS APPRECIABLE ONLY OVER THE
			///        FIRST FEW LOOPS OF THE BESSEL FUNCTION, NPCS MAY BE
			///        INCREASED TO ACHIEVE REASONABLE ACCURACY. NOTE THAT
			///        NPCS AFFECTS ONLY THE PADE
			///        SUM PORTION OF THE INTEGRATION, OVER X(NSUM) TO INFINITY.
			/// XSUM  VECTOR OF VALUES OF THE KERNEL ARGUMENT OF FUNCT FOR WHICH
			/// EXPLICIT CALCULATION OF THE INTEGRAL IS DESIRED, SO THAT THE
			/// INTEGRAL OVER 0 TO XSUM(NSUM) IS ADDED TO THE INTEGRAL OVER
			/// XSUM(NSUM) TO INFINITY WITH THE PADE METHOD INVOKED ONLY FOR
			/// THE LATTER. THIS ALLOWS THE PADE SUMMATION METHOD TO BE
			/// OVERRIDDEN AND SOME TYPES OF SINGULARITIES TO BE HANDLED.
			/// NSUM  NUMBER OF VALUES IN XSUM, MAY BE ZERO.
			/// NEW   DETERMINES METHOD OF KERNEL CALCULATION
			/// NEW=0 MEANS  CALCULATE BUT DO NOT SAVE INTEGRANDS
			/// NEW=1 MEANS CALCULATE KERNEL BY CALLING FUNCT-SAVE KERNEL
			///       TIMES BESSEL FUNCTION
			/// NEW=2 MEANS USE SAVED KERNELS TIMES BESSEL FUNCTIONS IN
			///       COMMON /BESINT/. NOTE THAT ORDER,R,NPCS,XSUM, AND
			///       NSUM MAY NOT BE CHANGED WHEN SETTING NEW=2.
			/// IERR  ERROR PARAMETER
			///       0 NORMAL RETURN-INTEGRAL CONVERGED
			///       1 MEANS NO CONVERGENCE AFTER NSTOP TERMS IN THE PADE SUM
			///
			/// SUBROUTINES REQUIRED:
			/// BESQUD,PADECF,CF,ZEROJ,DOT,JBESS
			///  A.CHAVE IGPP/UCSD
			/// NTERM IS MAXIMUM NUMBER OF BESSEL FUNCTION LOOPS STORED IF
			/// NEW.NE.0
			/// NSTOP IS MAXIMUM Number of Pade terms
            //template <EMMODE T>
 			void Bestrn( Real &BESR, Real &BESI, const int &iorder,
 					const int &NG, const Real &R,
 					const Real &RERR, const Real &AERR, const int &npcs,
 					VectorXi &XSUM, int &NSUM, int &NEW,
 					int &IERR, int &NCNTRL, const Real &AORB,
					KernelEM1DBase* Kernel);

			/// CALCULATES THE INTEGRAL OF F(X)*J-SUB-N(X*R) OVER THE
			/// INTERVAL A TO B AT A SPECIFIED GAUSS ORDER THE RESULT IS
			/// OBTAINED USING A SEQUENCE OF 1, 3, 7, 15, 31, 63, 127, AND 255
			/// POINT INTERLACING GAUSS FORMULAE SO THAT NO INTEGRAND
			/// EVALUATIONS ARE WASTED. THE KERNEL FUNCTIONS MAY BE
			/// SAVED SO THAT BESSEL TRANSFORMS OF SIMILAR KERNELS ARE COMPUTED
			/// WITHOUT NEW EVALUATION OF THE KERNEL.  DETAILS ON THE FORMULAE
			/// ARE GIVEN IN 'THE OPTIMUM ADDITION OF POINTS TO QUADRATURE
			/// FORMULAE' BY T.N.L. PATTERSON, MATHS.COMP. 22,847-856 (1968).
			/// GAUSS WEIGHTS TAKEN FROM COMM. A.C.M. 16,694-699 (1973)
			/// ARGUMENT LIST:
			/// A      LOWER LIMIT OF INTEGRATION
			/// B      UPPER LIMIT OF INTEGRATION
			/// BESR,BESI RETURNED INTEGRAL VALUE REAL AND IMAGINARY PARTS
			/// NG NUMBER OF POINTS IN THE GAUSS FORMULA.  NG=1,...7
			///        SELECTS 3,7,15,31,63,127,AND 255 POINT QUADRATURE.
			///  NEW SELECTS METHOD OF KERNEL EVALUATION
			///    NEW=0 CALCULATES KERNELS BY CALLING F - NOTHING SAVED
			///    NEW=1 CALCULATES KERNELS BY CALLING F AND SAVES KERNEL TIMES
			///           BESSEL FUNCTION IN COMMON /BESINT/
			///    NEW=2 USES SAVED KERNEL TIMES BESSEL FUNCTIONS IN
			///           COMMON /BESINT/
			/// iorder ORDER OF THE BESSEL FUNCTION
			/// R    ARGUMENT OF THE BESSEL FUNCTION
			/// F     F(X) IS THE EXTERNAL INTEGRAND SUBROUTINE
			/// A.CHAVE IGPP/UCSDC
			/// MAXIMUM NUMBER OF BESSEL FUNCTION LOOPS THAT CAN BE SAVED
            //template <EMMODE T>
			void Besqud(const Real &A, const Real &B, Real &BESR, Real &BESI,
							const int &NG, const int &NEW, const int &iorder,
							const Real &R, KernelEM1DBase* Kernel);

			/// COMPUTES SUM(S(I)),I=1,...N BY COMPUTATION OF PADE APPROXIMANT
			/// USING CONTINUED FRACTION EXPANSION.  FUNCTION IS DESIGNED TO BE
			/// CALLED SEQUENTIALLY AS N IS INCREMENTED FROM 1 TO ITS FINAL
			/// VALUE.  THE NTH CONTINUED FRACTION COEFFICIENT IS CALCULATED AND
			/// STORED AND THE NTH CONVERGENT RETURNED.  IT IS UP TO THE USER TO
			/// STOP THE CALCULATION WHEN THE DESIRED ACCURACY IS ACHIEVED.
			/// ALGORITHM FROM HANGGI ET AL., Z.NATURFORSCH. 33A,402-417 (1977)
			/// IN THEIR NOTATION, VECTORS CFCOR,CFCOI ARE LOWER CASE D,
			/// VECTORS DR, DI ARE UPPER CASE D, VECTORS XR,XI ARE X, AND
			/// VECTORS SR,SI ARE S
			/// A.CHAVE IGPP/UCSD
			void Padecf(Real &SUMR, Real &SUMI, const int &N);

			/// EVALUATES A COMPLEX CONTINUED FRACTION BY RECURSIVE DIVISION
			/// STARTING AT THE BOTTOM, AS USED BY PADECF
			/// RESR,RESI ARE REAL AND IMAGINARY PARTS RETURNED
			/// CFCOR,CFCOI ARE REAL AND IMAGINARY VECTORS OF CONTINUED FRACTION
			/// COEFFICIENTS
			void CF(        Real& RESR, Real &RESI,
							Eigen::Matrix<Real, 100, 1> &CFCOR,
							Eigen::Matrix<Real, 100, 1> &CFCOI,
							const int &N);


			/// COMPUTES ZERO OF BESSEL FUNCTION OF THE FIRST KIND FROM
			/// MCMAHON'S ASYMPTOTIC EXPANSION
			/// NZERO-NUMBER OF THE ZERO
			/// iorder-ORDER OF THE BESSEL FUNCTION (0 OR 1)
			Real ZeroJ(const int &ZERO, const int &IORDER);

			/// COMPUTES BESSEL FUNCTION OF ORDER "ORDER" AND ARGUMENT X BY
			/// CALLING NBS ROUTINES J0X AND J1X (REAL*8 BUT APPROXIMATELY
			/// REAL*4 ACCURACY).
			/// FOR MORE ACCURACY JBESS COULD BE CHANGED TO CALL, FOR EXAMPLE,
			/// THE IMSL ROUTINES MMBSJ0,MMBSJ1 << SEE C// BELOW >>
			Real Jbess(const Real &X, const int &IORDER);

			/// COMPUTES DOT PRODUCT OF TWO D.P. VECTORS WITH NONUNIT
			/// INCREMENTING ALLOWED. REPLACEMENT FOR BLAS SUBROUTINE SDOT.
			/// Currently does no checking, kind of stupid.
			/// The fortran version will wrap around if (inc*N) > X1.size()
			/// but not in a nice way.
			Real _dot(const int&N,
				const Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> &X1,
							const int &INC1,
				const Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> &X2,
				const int &INC2);

			// ====================  DATA MEMBERS  ============================

            static const Real PI2;
			static const Real X01P;
			static const Real XMAX;
			static const Real XSMALL;
			static const Real J0_X01;
			static const Real J0_X02;
			static const Real J0_X11;
			static const Real J0_X12;
			static const Real FUDGE;
			static const Real FUDGEX;
			static const Real TWOPI1;
			static const Real TWOPI2;
			static const Real RTPI2;
			static const Real XMIN;
			static const Real J1_X01;
			static const Real J1_X02;
			static const Real J1_X11;
			static const Real J1_X12;

			/// Highest gauss order used, Was NG
			int HighestGaussOrder;

			/// Total number of partial integrals on last call, was NI
			int NumberPartialIntegrals;

			/// Total number of function calls, was NF
			int NumberFunctionEvals;

			int          np;
			int          nps;

            /////////////////////////////////////////////////////////////
			// Eigen members

            // Shared constant values
			static const VectorXr  WT;
			static const VectorXr  WA;

			Eigen::Matrix<int,  100, 1>    Nk;
			//Eigen::Matrix<Real, 255, 100>  karg;
			//Eigen::Matrix<Real, 510, 100>  kern;
			Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>  karg;
			Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>  kern;

			// Was Besval COMMON block
			Eigen::Matrix<Real, 100, 1> Xr;
			Eigen::Matrix<Real, 100, 1> Xi;
			Eigen::Matrix<Real, 100, 1> Dr;
			Eigen::Matrix<Real, 100, 1> Di;
			Eigen::Matrix<Real, 100, 1> Sr;
			Eigen::Matrix<Real, 100, 1> Si;
			Eigen::Matrix<Real, 100, 1> Cfcor;
			Eigen::Matrix<Real, 100, 1> Cfcoi;

		private:

            /** ASCII string representation of the class name */
            static constexpr auto CName = "FHTKey51";

	}; // -----  end of class  GQChave  -----

    //////////////////////////////////////////////////////////////
    // Exception Classes

    /** If the lower integration limit is greater than the upper limit, throw this
     * error.
     */
    class LowerGaussLimitGreaterThanUpperGaussLimit :
        public std::runtime_error {
        /** Thrown when the LowerGaussLimit is greater than the upper limit.
         */
        public: LowerGaussLimitGreaterThanUpperGaussLimit();
    };

}	   // -----  end of Lemma  name  -----

#endif   // ----- #ifndef _HANKELTRANSFORMGAUSSIANQUADRATURE_h_INC  -----
