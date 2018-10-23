/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/07/2011
  @version  $Id: digitalfilterintegrator.h 124 2014-02-07 04:26:26Z tirons $
 **/

#ifndef  DIGITALFILTERINTEGRATOR_INC
#define  DIGITALFILTERINTEGRATOR_INC

#include "integrator.h"
#include "integrationkernel.h"

namespace Lemma {

    // ===================================================================
    //  Class:  DigitalFilterIntegrator
    /**
      \brief   Reimplimentation of Walt Anderson's digital filtering
               algorithms which are public domain.
      \details Walt Anderson wrote several routines for digital filtering
               that reused a lot of code. The original fortran codes are
               available at:
        ftp://ftpext.usgs.gov/pub/cr/co/denver/musette/pub/anderson/
               This is an abstract class. Concrete classes must define the
               a function to make arguments.
               Template specilizations are found in the implimentation file
               digitalfilterintegrator.cpp. This was necessary to support
               the design paradigm of Lemma.
     */
    // ===================================================================
    template <typename Scalar>
    class DigitalFilterIntegrator : public Integrator {

        /** Prints out basic info about the class, Complex implimentation */

        template <typename Scalar2>
        friend std::ostream &operator<<(std::ostream &stream,
                        const DigitalFilterIntegrator<Scalar2> &ob);

        public:

            // ====================  LIFECYCLE     =======================

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /** Sets the number of desired convolutions. From Anderson, where
                this value is called NB:
                NUMBER OF LAGGED CONVOLUTIONS DESIRED (NB.GE.1).  USE
                NB=1 IF B=BMIN=BMAX (I.E.,NO LAGGING DESIRED).  USE
                NB>1 IF B IS LAGGED IN (BMIN,BMAX), WHERE
                BMIN=BMAX*DEXP(-.1D0*(NB-1)) DOES NOT UNDERFLOW THE DEXP
                RANGE.  THE B-LAGGED SPACING IS .1D0 IN LOG-SPACE. FOR
                CONVENIENCE IN SPLINE INTERPOLATION LATER, EACH B IN
                (BMIN,BMAX) IS RETURNED IN ARRAY ARG(I),I=1,NB, WHERE
                ARG(I+1)/ARG(I)=DEXP(.1D0) FOR ALL I.  IF BMAX>BMIN>0 IS
                GIVEN, THEN AN EFFECTIVE VALUE OF NB IS DETERMINED AS
                NB=DINT(10.*DLOG(BMAX/BMIN))+I, WHERE I>1 IS RECOMMENDED,
                PARTICULARLY IF USING SUBSEQUENT SPLINE INTERPOLATION FOR
                A DIFFERENT B-SPACING THAN USED IN THE DIGITAL FILTER. IF
                SPLINE INTERPOLATION IS TO BE USED LATER, IT IS GENERALLY
                BEST TO USE DLOG(ARG(I)) INSTEAD OF ARG(I) -VS- DANS(I),
                FOR I=1,NB.
            */
            void SetNumConv(const int& i);

            /** Evaluates related integrals */
            void ComputeRelated(const Real& rho,
                IntegrationKernel<Scalar>* Kernel);

            /** Evaluates the integral
            @param[in]tol  REQUESTED TRUNCATION TOLERANCE AT BOTH FILTER TAILS
              FOR ADAPTIVE CONVOLUTION FOR ALL NB TRANSFORMS.  THE
              TRUNCATION CRITERION IS ESTABLISHED DURING CONVOLUTION IN
              A FIXED ABSCISSA RANGE (USING WEIGHTS 426-461) OF THE
              COSINE FILTER AS THE MAXIMUM ABSOLUTE CONVOLVED PRODUCT
              TIMES TOL.  THE CONVOLUTION SUMMATION IS TERMINATED
              ON EITHER SIDE OF THE FIXED RANGE WHENEVER THE ABSOLUTE
              PRODUCT .LE. THE TRUNCATION CRITERION.  IN GENERAL, A
              DECREASING TOLERANCE WILL PRODUCE HIGHER ACCURACY SINCE
              MORE FILTER WEIGHTS ARE USED (UNLESS EXPONENT UNDERFLOW
              OCCURS IN THE TRANSFORM INPUT FUNCTION EVALUATION).
              ONE MAY SET TOL=0.D0 TO OBTAIN MAXIMUM ACCURACY FOR ALL
              NB DOUBLE-PRECISION COSINE TRANSFORMS IN DANS(NB).
              HOWEVER, THE ACTUAL RELATIVE ERRORS CANNOT BE EXPECTED TO
              BE SMALLER THAN ABOUT .1D-12 REGARDLESS OF THE TOLERANCE
              VALUE USED, SINCE DOUBLE-PRECISION FILTER WEIGHTS AND A
              DOUBLE-PRECISION FUNCTION ARE USED. IN ANY EVENT,
              ONE SHOULD ALWAYS CHOOSE TOL<<DESIRED RELATIVE ERROR.
           ** ACCURACY WARNING **  SOME HIGHLY OSCILLATORY FUNCTIONS
              FUN(G) AND (OR) LIMITING CASES OF B NEAR MACHINE-ZERO
              (OR INFINITY) SHOULD BE AVOIDED, OTHERWISE UNSATISFACTORY
              RESULTS (E.G., RELATIVE & ABSOLUTE ERRORS>>TOL) MAY OCCUR.
              @param[in] ntol  NUMBER OF CONSECUTIVE TIMES THE TRUNCATION CRITERION (TOL)
              IS TO BE MET AT EITHER FILTER TAIL BEFORE FILTER
              TRUNCATION OCCURS.  NTOL=1 SHOULD BE USED FOR INPUT
              FUNCTIONS THAT DO NOT HAVE MANY ZEROS IN (0,INFINITY). FOR
              OSCILLATORY FUNCTIONS WITH MANY ZEROS, NTOL>1 MAY BE USED
              TO INSURE A PREMATURE CUTOFF DOES NOT OCCUR FOR TRUNCATION
              (SEE USE OF ITOL,NTOL,TOL IN THE CODE BELOW).
            @param[in] rho integration argument argument
            */
            void Compute(const Real& rho, const int& ntol, const Real& tol);

            /** Attaches the integration kernel */
            void AttachKernel(IntegrationKernel<Scalar> *Kernel);

            /** Detaches the integration kernel */
            void DetachKernel( );

            // ====================  ACCESS        =======================

            /** Returns the Vector of Abscissa arguments
             */
            VectorXr  GetAbscissaArguments();

            Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic > GetAnswer();

            /** Returns the number of evaluations of kernel */
            int GetNumFun();
            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor. */
            DigitalFilterIntegrator (const std::string& name);

            /** Default protected constructor. */
            ~DigitalFilterIntegrator ();

            // ====================  DATA MEMBERS  =========================

            /** A rewrite of Anderson's Pseudo-subroutine. */
            void StoreRetreive(const int& idx, const int& lag, const Real& y,
                        Scalar& Zsum, const int& jrel, Scalar& C);

            // ====================  OPERATIONS    ==============================
            /** Sets the filter weights as prescribed by Anderson. This is called
                automatically when the SetBesselOrder(int ) is called. This is
                a protected function and cannot be called directly.
             */
            virtual void SetFilterWeights()=0;

            /** Computes the absolute maximum of the scalar */
            Scalar AbsMax(const Scalar& C, const Scalar& Cmax);

            // ====================  DATA MEMBERS  ==============================

            /** The hankel transform wavenumber embedded in the integral */
            Real Lambda;

            /** Number of times kernel was evaluated */
            int NumFun;

            /** Number of lagged convolutions
                must be greater or equal to 1
                It is set automatically in the @see Compute function so
                that  \f$ \rho \exp\left( -.1*(\mathtt{NumConv} -1) \right) \f$
                does not underflow the exponent range
             */
            int NumConv;

            /** Number of related kernels */
            int NumRel;

            /** Used as base for filter abscissa generation */
            Real ABSCISSA;

            /** Also used in abscissa generation \f$ ABSE = \exp{.1} \f$ */
            const Real ABSE;

            /** Also used in abscissa generation \f$ ABSER = 1 / \exp{.1} \f$ */
            const Real ABSER;

            /** Counter for calculated */
            int icount;

            /** Index of special case low side convolution filter weights */
            int ilow;

            /** Index of special case right side convolution filter weights */
            int ihi;

            /** Kernel Calculator */
            IntegrationKernel<Scalar> *IntKernel;

            /** Holds answer, dimensions are NumConv, and NumberRelated. */
            Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> Ans;

            /** Filter weight coefficients.
             */
            VectorXr  FilterWeights;

            /** Stored abscissa arguments
             */
            VectorXr  Arg;

            /** Work from Anderson
             */
            Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> Work;

            /// Key used internally
            Eigen::Matrix<int, Eigen::Dynamic, 1> Key;

        private:

    }; // -----  end of class  DigitalFilterIntegrator  -----


    /////////////////////////////////////////
    /////////////////////////////////////////
    //      Templated implimentation       //
    /////////////////////////////////////////
    /////////////////////////////////////////

    template <typename Scalar>
    std::ostream &operator<<(std::ostream &stream,
			const DigitalFilterIntegrator<Scalar> &ob) {
		stream << *(Integrator*)(&ob);
		return stream;
	}

    // ====================  LIFECYCLE     =======================

    template <typename Scalar>
    DigitalFilterIntegrator<Scalar>::
            DigitalFilterIntegrator(const std::string& name) :
        Integrator(name), Lambda(0), NumFun(0), NumConv(0), NumRel(0),
        ABSCISSA(0),
        ABSE(1.10517091807564762),   //   exp(.1)
        ABSER(0.904837418035959573), // 1/exp(.1)
        ilow(0), ihi(0), IntKernel(NULL) {
    }

    template <typename Scalar>
    DigitalFilterIntegrator<Scalar>::~DigitalFilterIntegrator( ) {
    }

    template <typename Scalar>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic >
        DigitalFilterIntegrator<Scalar>::GetAnswer() {
        return Ans;
    }

    // ====================  OPERATIONS    =======================

    template <typename Scalar>
    void DigitalFilterIntegrator<Scalar>::SetNumConv(const int& i) {
        this->NumConv = i;
    }

    template <typename Scalar>
    void DigitalFilterIntegrator<Scalar>::
            AttachKernel(IntegrationKernel<Scalar> *ck) {
		if (this->IntKernel == ck) return;
        if (this->IntKernel != NULL) {
			this->IntKernel->DetachFrom(this);
		}
		ck->AttachTo(this);
		this->IntKernel = ck;
	}


    template <typename Scalar>
	void DigitalFilterIntegrator<Scalar>::DetachKernel( ) {
		if (this->IntKernel != NULL) {
			this->IntKernel->DetachFrom(this);
		}
		this->IntKernel = NULL;
	}

//     template < >
//     inline Complex DigitalFilterIntegrator<Complex>::AbsMax(const Complex& C,
//         const Complex& Cmax) {
// 		return Complex(std::max(std::abs(real(C)), std::real(Cmax)),
// 					   std::max(std::abs(imag(C)), std::imag(Cmax)) );
//     }
//     template < >
//     Real DigitalFilterIntegrator<Real>::AbsMax(const Real& C,
//         const Real& Cmax) {
//         return std::max(C, Cmax);
//     }

    template <typename Scalar>
    VectorXr  DigitalFilterIntegrator<Scalar>::GetAbscissaArguments() {
        return this->Arg;
    }

//     ///////////////////////////////////////////
// 	// Computes the transform
//
//     template < >
//     void DigitalFilterIntegrator<Real>::Compute(const Real& rho,
//         const int& ntol, const Real& tol) {
//
// 		Real y1 = this->ABSCISSA/rho;
// 		this->Key.setZero();
//
// 		// Check to make sure everything is set
// 		if (rho<=0) {
// 			throw std::runtime_error("In DigitalFilterIntegrator Argument rho < 0.");
// 		}
//
// 		if (this->NumConv<1) {
// 			throw std::runtime_error("In DigitalFilterIntegrator NumConv is less than 1.");
// 		}
//
// 		if (this->IntKernel == NULL) {
// 			throw std::runtime_error("In DigitalFilterIntegrator Unset Kernel Calculator");
// 		}
//
// 		Arg = VectorXr::Zero(this->NumConv);
// 		Real y = std::pow(rho*ABSER, this->NumConv-1);
//
// 		if (y <= 0) {
// 			std::cerr << "Exponent Underflow Error";
// 			throw std::underflow_error("Exponent underflow");
// 		}
//
// 		this->Work.resize(Eigen::NoChange, this->IntKernel->GetNumRel());
//
//         // Main Loop
// 		int itol = 0;
// 		int none = 0;
// 		this->NumFun = 0;
// 		int idx = 0;
// 		int istore = 0;
// 		Real Sum(0.);
// 		Real Cmax(0.);
// 		Real C(0.);
// 		Ans.resize(this->NumConv, this->IntKernel->GetNumRel());
// 		Ans.setZero();
//  		// 1010 Loop
//  		for (int ilag=0; ilag < this->NumConv; ++ilag) {
//   			istore = this->NumConv - 1 - ilag;
//   			if (ilag > 0) y1 *= ABSE;
//   			Arg(istore) = ABSCISSA/y1;
//
//  			// 1000 Loop
//  			for (int irel=0; irel < this->IntKernel->GetNumRel(); ++irel) {
// 				//this->SetBesselOrder(this->Ckernel->GetBesselOrder(irel));
//   				none = 0;
//   				itol = ntol;
//   				Sum = Real(0);
//   				Cmax = Real(0);
//   				y = y1;
// 				// Begin right side convolution at weight 298
// 				// counting from 0
//   				idx = ilow;
//   				y *= ABSE;
// 				// Code Block 20 in Anderson
// 				do {
//  					this->StoreRetreive(idx, ilag, y, Sum, irel, C);
//                     Cmax = AbsMax(C, Cmax);
// 					++idx;
// 					y *= ABSE;
// 				} while (idx <= ihi);
// 				//if (real(Cmax) == 0 && imag(Cmax) == 0) none = 1;
// 				if (Cmax == Real(0)) none = 1;
// 				Cmax *= tol;
// 				// Code Block 30 in Anderson
// 				do {
//  					this->StoreRetreive(idx, ilag, y, Sum, irel, C);
// 					if (std::abs(C) <= Cmax) {
// 						--itol;
// 						if (itol < 0 || idx > FilterWeights.size()-1) break;
// 					} else {
// 						itol = ntol;
// 					}
// 					++idx;
// 					y *= ABSE;
// 				} while (idx < FilterWeights.size());
// 				itol = ntol;
// 				y = y1;
// 				// Code Block 60 in Anderson
// 				idx = ilow-1;
// 				do {
//  					this->StoreRetreive(idx, ilag, y, Sum, irel, C);
// 					if (std::abs(C) <= Cmax &&  none == 0) {
// 						--itol;
// 						if (itol < 0 || idx < 0) break;
// 					} else {
// 						itol = ntol;
// 					}
// 					--idx;
// 					y *= ABSER;
// 				} while (idx>=0);
//  				Ans(istore, irel) = Sum/Arg(istore);
//   			}  // End of 1000 loop
//   		}      // End of 1010 loop
// 	}
//
//
//     template < >
// 	void DigitalFilterIntegrator<Complex>::Compute(const Real &rho,
//             const int& ntol, const Real &tol) {
//
// 		Real y1 = this->ABSCISSA/rho;
//
// 		this->Key.setZero();
//
// 		// Check to make sure everything is set
// 		if (rho<=0) {
// 			throw std::runtime_error("In DigitalFilterIntegrator Argument rho < 0.");
// 		}
//
// 		if (this->NumConv<1) {
// 			throw std::runtime_error("In DigitalFilterIntegrator NumConv is less than 1.");
// 		}
//
// 		if (this->IntKernel == NULL) {
// 			throw std::runtime_error("In DigitalFilterIntegrator Unset Kernel Calculator");
// 		}
//
// 		Arg = VectorXr::Zero(this->NumConv);
// 		Real y = std::pow(rho*ABSER, this->NumConv-1);
//
// 		if (y <= 0) {
// 			std::cerr << "Exponent Underflow Error";
// 			throw std::underflow_error("Exponent underflow");
// 		}
//
// 		this->Work.resize(Eigen::NoChange, this->IntKernel->GetNumRel());
//
//         // Main Loop
// 		int itol = 0;
// 		int none = 0;
// 		this->NumFun = 0;
// 		int idx = 0;
// 		int istore = 0;
// 		Complex Zsum(0.);
// 		Complex Cmax(0.);
// 		Complex C(0.);
// 		Ans.resize(this->NumConv, this->IntKernel->GetNumRel());
// 		Ans.setZero();
//  		// 1010 Loop
//  		for (int ilag=0; ilag < this->NumConv; ++ilag) {
//   			istore = this->NumConv - 1 - ilag;
//   			if (ilag > 0) y1 *= ABSE;
//   			Arg(istore) = ABSCISSA/y1;
//  			// 1000 Loop
//  			for (int irel=0; irel < this->IntKernel->GetNumRel(); ++irel) {
// 				//this->SetBesselOrder(this->Ckernel->GetBesselOrder(irel));
//   				none = 0;
//   				itol = ntol;
//   				Zsum = Complex(0);
//   				Cmax = Complex(0);
//   				y = y1;
// 				// Begin right side convolution at weight 298
// 				// counting from 0
//   				idx = ilow;
//   				y *= ABSE;
// 				// Code Block 20 in Anderson
// 				do {
//  					this->StoreRetreive(idx, ilag, y, Zsum, irel, C);
//                     Cmax = AbsMax(C, Cmax);
// 					++idx;
// 					y *= ABSE;
// 				} while (idx <= ihi);
// 				//if (real(Cmax) == 0 && imag(Cmax) == 0) none = 1;
// 				if (Cmax == Complex(0)) none = 1;
// 				Cmax *= tol;
// 				// Code Block 30 in Anderson
// 				do {
//  					this->StoreRetreive(idx, ilag, y, Zsum, irel, C);
// 					if ( std::abs(real(C)) <= real(Cmax) &&
// 						 std::abs(imag(C)) <= imag(Cmax) ) {
// 						--itol;
// 						if (itol < 0 || idx > FilterWeights.size()-1) break;
// 					} else {
// 						itol = ntol;
// 					}
// 					++idx;
// 					y *= ABSE;
// 				} while (idx < FilterWeights.size());
// 				itol = ntol;
// 				y = y1;
// 				// Code Block 60 in Anderson
// 				idx = ilow-1;
// 				do {
//  					this->StoreRetreive(idx, ilag, y, Zsum, irel, C);
// 					if ( std::abs(real(C)) <= real(Cmax) &&
// 						 std::abs(imag(C)) <= imag(Cmax) &&
// 						 none == 0 ) {
// 						--itol;
// 						if (itol < 0 || idx < 0) break;
// 					} else {
// 						itol = ntol;
// 					}
// 					--idx;
// 					y *= ABSER;
// 				} while (idx>=0);
//  				Ans(istore, irel) = Zsum/Arg(istore);
//   			}  // End of 1000 loop
//   		}      // End of 1010 loop
// 	}

    template <typename Scalar>
    int DigitalFilterIntegrator<Scalar>::GetNumFun() {
        return NumFun;
    }

    // generic rewrite of store-retreive 'pseudo-subroutine'
    template <typename Scalar>
    void DigitalFilterIntegrator<Scalar>::StoreRetreive(const int &idx,
            const int& lag, const Real& y, Scalar& Sum,
            const int& jrel, Scalar& C) {

		int look = idx+lag;
 		int	iq = look/FilterWeights.size();
 		int	ir = look%FilterWeights.size();
		int iroll = iq*(FilterWeights.size()-1);

		if(this->Key(ir) <= iroll) {
			this->Key(ir) = iroll + ir;
			this->Lambda = y;
 			++this->NumFun;
 			for (int ir2=0; ir2<IntKernel->GetNumRel(); ++ir2) {
 			    this->Work(ir, ir2) = this->IntKernel->Argument(this->Lambda, ir2);
 			}
		}

		C = this->Work(ir, jrel) * this->FilterWeights(idx);
		Sum += C;
		return;

	}

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef DIGITALFILTERINTEGRATOR_INC  -----
