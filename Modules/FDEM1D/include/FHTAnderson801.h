/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/26/2009
  @version  $Id: hankeltransformhankel2.h 201 2015-01-03 00:07:47Z tirons $
 **/

#ifndef __FHTANDERSON801_H
#define __FHTANDERSON801_H

#include "KernelEM1DBase.h"
#include "KernelEM1DSpec.h"
#include "CubicSplineInterpolator.h"
#include "HankelTransform.h"

namespace Lemma {

// ==========================================================================
//        Class:  FHTAnderson801
/** \brief   Computes the Hankel transform of orders 0 and 1 using lagged
             and related convolutions.
    \details A rewrite of work by Anderson who wrote a FORTRAN program
             that he released while working at the USGS:
             Anderson, W. L., 1989, A hybrid fast hankel transform algorithm for
             electromagnetic modeling: Geophysics, 54, 263-266.

             This function does not provide the Hybrid functionality however, merely the
             digital filter implimentation.

             The  transform evaluates an integral of the form:
             \f[ \int_0^\infty K(\lambda) J_I (\lambda r) ~ d \lambda
             \f]
             Where \f$ K(\lambda) \f$ is some kernel function. The value
             \f$ J_I \f$ is the Bessel function of order
             \f$I, I \in \{0,1\} \f$
             The kernel function is unique for each source and is computed
             in the class CalculateKernel. The value \f$r\f$ is the radial
             distance away from the centre of the grid \f$ r=\sqrt{x^2 + y^2} \f$
             The Hankel transform is useful as it allows a double fourier
             transform to be written as a single integral:
             \f[ \mathop {\int \!\!\! \int}_{\!\!\!\!\!-\infty}^{\,\,\infty}
             F(k_x^2 + k_y^2)
                 e^{\imath (k_x x + k_y y)} dk_x \, dk_y = 2 \pi
                 \int_0^\infty K(\lambda) J_I (\lambda r) ~ d \lambda
             \f]
             This can only be done where there is radial symmetry. Hence
             its application to 1D solutions here.
    \note In previous versions of Lemma, this class was called HankelTransformHankel2,
        which more closely follows Anderson's procedural routine names, but was non-descriptive
        regarding where the algorithm is derived from.
 */
// ==========================================================================

class FHTAnderson801 : public HankelTransform {

    friend std::ostream &operator<<(std::ostream &stream, const FHTAnderson801 &ob);

    struct ctor_key {};

    public:

        // ====================  LIFECYCLE     ==============================
        /**
         *  Returns shared_ptr to new FHTAnderson801.
         */
        static std::shared_ptr<FHTAnderson801> NewSP();

        /**
         *  Returns unique_ptr to new FHTAnderson801.
         */
        static std::unique_ptr<FHTAnderson801> NewUP();

        /// Default locked constructor
        FHTAnderson801( const ctor_key& );

        /** Locked deserializing constructor. */
		FHTAnderson801 ( const YAML::Node& node, const ctor_key& );

        /// Default destructor
        ~FHTAnderson801();

        /**
         *   YAML Serializing method
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static std::shared_ptr< FHTAnderson801 > DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     ==============================

        // ====================  OPERATIONS    ==============================

        /// Sets the number of convolutions
        void SetNumConv(const int &i);

        /// Computes the hankel transform with arguments
        /// @param rho [input] rho is the hankel transform argument
        /// @param ntol [input] ntol is
        /// @param tol [input] tol is
        void Compute(const Real &rho, const int& ntol, const Real &tol);

        /// Computes the related
        void ComputeRelated(const Real &rho, std::shared_ptr<KernelEM1DBase> Kernel);

        /// Computes the related
        void ComputeRelated(const Real &rho,  std::vector< std::shared_ptr<KernelEM1DBase> > KernelVec);

        /// Computes the related
        void ComputeRelated(const Real &rho,  std::shared_ptr<KernelEM1DManager> Manager);

        /// Computes the related and lagged convolutions
        void ComputeLaggedRelated(const Real &rho, const int& nlag,  std::shared_ptr<KernelEM1DManager> Manager);

        // ====================  ACCESS        ==============================

        /// Returns the answer
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>  GetAnswer();

        /// Returns the arguments for lagged convolutions
        VectorXr GetArg() {return Arg;};

        /// Returns the value of Abscissa stepping
        Real GetABSER( ) { return ABSER; };

        /// Sets the lagged kernel index so that the proper value is returned
        void SetLaggedArg(const Real& rho);

        // ====================  INQUIRY       ==============================

        /// Calculates Hankel Transform using filtering.
        /// ikk:   type of kernel depending on source and receiver couple
        /// imode: a switch for TE(0) and TM(1) mode
        /// itype: order of Bessel function
        /// rho is argument to integral
        /// wavef is the propogation constant of free space
        /// = omega * sqrt( EP*AMU )  amu = 4 pi e-7  ep = 8.85e-12
        Complex Zgauss(const int &ikk, const EMMODE &imode,
                        const int &itype, const Real &rho,
                        const Real &wavef, std::shared_ptr<KernelEM1DBase> Kernel);

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual inline std::string GetName() const {
            return CName;
        }

    protected:

    private:

        // ====================  LIFECYCLE     ==============================

        /** A rewrite of Anderson's "Pseudo-subroutine" computed GOTO madness. */
        inline void StoreRetreive(const int &idx, const int &lag,
                        Complex &Zsum, const int &irel, Complex &C, const Real& rho0) {

            int look = idx+lag;
    		int	iq = look/801;
    		int	ir = look%801;
    		int iroll = iq*800;

    		if(this->Key[ir] <= iroll) {
			    this->Key[ir] = iroll + ir;
 	    		++this->NumFun;
                Manager->ComputeReflectionCoeffs(this->Lambda, idx, rho0);
 			    for (unsigned int ir2=0; ir2<this->kernelVec.size(); ++ir2) {
 			        this->Zwork(ir, ir2) = this->kernelVec[ir2]->RelBesselArg(this->Lambda);
 			    }
		    }

		    C = this->Zwork(ir, irel) * this->FilterWeights(this->BesselOrder, idx);
		    Zsum += C;
		    return;
	    }

        // ====================  OPERATIONS    ==============================

        void DeleteSplines();

        // ====================  DATA MEMBERS  ==============================

        /// The hankel transform wavenumber embedded in the integral
        Real Lambda;

        /// Number of times a kernel was evaluated
        int NumFun;

        /// Number of lagged convolutions
        /// must be greater or equal to 1
        /// It is set automatically in the @see Compute function so
        /// that  \f$ \rho \exp\left( -.1*(\mathtt{NumConv} -1) \right) \f$
        /// does not underflow the exponent range
        int NumConv;

        /// Number of related kernels
        int NumRel;

        /** Bessel transform order to use */
        int BesselOrder;

        /** Lag argument */
        int iLag;

        /* Should results be cached? Useful for repeated calculations of few receiver points */
        // turned out to have only marginal benefits in best case, and awful consequences in many
        //bool cacheResults;

        /** Related Kernel Manager */
        std::shared_ptr<KernelEM1DManager>     Manager;

        /// Used as base for filter abscissa generation
        static const Real ABSCISSA;

        /// Also used in abscissa generation \f$ ABSE = \exp{.1} \f$
        static const Real ABSE;

        /// Also used in abscissa generation \f$ ABSER = 1 / \exp{.1} \f$
        static const Real ABSER;

        /// Counter for calculated
        int icount;

        /// Kernel Calculator
        std::vector < std::shared_ptr<KernelEM1DBase> > kernelVec;

        /// Spines for lagged convolutions (real part)
        std::vector <std::shared_ptr<CubicSplineInterpolator> > splineVecReal;

        /// Spines for lagged convolutions (imaginary part)
        std::vector < std::shared_ptr<CubicSplineInterpolator> > splineVecImag;

        /// Key used internally
        Eigen::Matrix<int, 801, 1> Key;
        //int  Key[801];
        //Eigen::Matrix<int, Eigen::Dynamic, 1> Key;

        /// Filter weight coefficients. Set for either \f$J_0\f$ or \f$J_1\f$
        /// internally by protected function SetFilterWeights.
        /// Fixed sized will yield better performance. (not necessarily)
        //Eigen::Matrix<Real, 801, 1> FilterWeights;
        static const Eigen::Matrix<Real, 2, 801> FilterWeights;
        //static const Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> FilterWeights;

        /// Zwork from Anderson
        Eigen::Matrix<Complex, 801, Eigen::Dynamic> Zwork;
        //Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> Zwork;

        /// Holds answer, dimensions are NumConv, and NumberRelated.
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> Zans;

        /// Holds the arguments for lagged convolutions
        VectorXr Arg;

        /** ASCII string representation of the class name */
        static constexpr auto CName = "FHTAnderson801";

}; // -----  end of class  FHTAnderson801  -----

}

#endif // __FHTAnderson801_h
