/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/12/2014 10:20:18 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  QWEKEY_INC
#define  QWEKEY_INC

#include "HankelTransform.h"

#include <Eigen/Eigenvalues>

#ifdef HAVE_BOOST_SPECIAL_FUNCTIONS
#include "boost/math/special_functions.hpp"
#include "boost/math/special_functions/bessel.hpp"
#endif

namespace Lemma {

    /** breakpoint to use in division of domain, based on zeros of bessel function or
        regular nPi spacing.
     */
    enum sZeroType{J0, J1, NPI};

    /**
      \ingroup FDEM1D
      \brief   Port of Key's quadrature with extrapolation Hankel transform algorithm.
      \details Details of the algorithm can be found in Key2011. This code is a port
               of the published algorithm, which contains the following notice:
        %------------------------------------------------------------------%
        % Copyright (c) 2012 by the Society of Exploration Geophysicists.  %
        % For more information, go to http://software.seg.org/2012/0003 .  %
        % You must read and accept usage terms at:                         %
        % http://software.seg.org/disclaimer.txt before use.               %
        %------------------------------------------------------------------%
     */
    class QWEKey : public HankelTransform {

        friend std::ostream &operator<<(std::ostream &stream, const QWEKey &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /** Default locked constructor, use NewSP */
        QWEKey ( const ctor_key& );

        /** DeSerializing locked constructor, use DeSerialize */
        QWEKey ( const YAML::Node& node, const ctor_key& );

        /** Default destructor */
        ~QWEKey ();

        /**
         *  Factory method for generating objects.
         *   @return std::shared_ptr< QWEKey >
         */
        static std::shared_ptr<QWEKey> NewSP();

        /** YAML Serializing method
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static std::shared_ptr< QWEKey > DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        void TestPrivate(const int& N);

        // ====================  OPERATIONS    =======================

        Complex Zgauss(const int &ikk, const EMMODE &imode,
                            const int &itype, const Real &rho,
                            const Real &wavef, KernelEM1DBase* Kernel);

        /// Computes related kernels, if applicable, otherwise this is
        /// just a dummy function.
        void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DBase> Kernel);

        void ComputeRelated(const Real& rho, std::vector< std::shared_ptr<KernelEM1DBase> > KernelVec);

        void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DManager> KernelManager);

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual std::string GetName() const ;

        protected:

        // ====================  LIFECYCLE     =======================

        /** Calculates Gauss quadrature weights of order N on the interval -1,1
            Algorithm from p 129 in:
            Trefethen, L. N., 2000, Spectral methods in MATLAB: Society for
            Industrial and Applied Mathematics (SIAM), volume 10 of Software,
            Environments, and Tools.
        */
        void GaussQuadWeights(const int& N);

        /** Returns the quadrature intervals and Bessel function weights used for the
            QWE method.

         */
        void BesselWeights( const sZeroType& sType);

        /**  Computes an infinite integral using the partial sum of quadrature terms
             accelerated by sequence extrapolation using the Shanks transformation
             implemented with Wynn's epsilon algorithm.
         */
        void QWE(const Real& rho);

        /** Calls the underlying kernel functions evaluated as necessary
         */
        void getEyKernel(const int& i, const int& idx, const Real& rho);

        private:

        // ====================  DATA MEMBERS  =========================

        /** Relative tolerance, default is 1e-6 */
        Real    RelTol;

        /** Absolute tolerance, default is 1e-24 */
        Real    AbsTol;

        /** Quadrature order, higher is more accurate but more expensive. Eefault is 9 */
        int     nQuad;

        /** in QWE partial integrals before Shanks recurive algorithm. Defaults to 1  */
        int nDelay;

        /** Maximum number of intervals to integrate over . Defaults to 40  */
        int nIntervalsMax;

        /** Weighing of gaussian quadrature points */
        VectorXr GaussWeights;

        /** Abscissa locations of quadrature points */
        VectorXr GaussAbscissa;

        /** Breakpoints for dividing up the global integral */
        VectorXr xInt;

        /** All quadrature points between all breakpoints */
        VectorXr Bx;

        /** J0 weights */
        VectorXr BJ0;

        /** J1 weights */
        VectorXr BJ1;

        /** array of lambda arguments */
        VectorXr Lambda;

        /** array of lambda arguments */
        VectorXr Intervals;

        MatrixXcr TS;
        VectorXi  Tn;
        MatrixXcr Textrap;
        MatrixXr  TrelErr;
        MatrixXr  TabsErr;

        /** Container to hold bessel arguments */
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic > Zwork;

        /** Container to hold bessel arguments */
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic > Zans;

        /** Manager for related kernels to evaluate */
        std::shared_ptr<KernelEM1DManager>  KernelManager;

        /** ASCII string representation of the class name */
        static constexpr auto CName = "QWEKey";

    }; // -----  end of class  QWEKey  -----

} // -----  end of Lemma  name  -----

#endif   // ----- #ifndef QWEKEY_INC  -----


