/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      05/02/2018 09:46:38 PM
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2018, University of Utah
 * @copyright Copyright (c) 2018, Trevor Irons & Lemma Software, LLC
 */

#ifndef  FHT_INC
#define  FHT_INC

#pragma once
#include "HankelTransform.h"
#include "CubicSplineInterpolator.h"

namespace Lemma {

    /**
      \ingroup FDEM1D
      \brief   Impliments lagged and related fast Hankel transform through
               digital filtering.
      \details A general Fast Hankel Transform routine which uses the digital
               filter apporach. Both lagged and related kernels are supported in
               order to minimize kernel function calls.
               This approach performs a complete sweep of the
               coefficients , for a variant that uses a longer filter which may
               be truncated, see FHTAnderson801.
               @see FHTAnderson801
               @see GQChave
               @see QWEKey
     */
    template < HANKELTRANSFORMTYPE Type >
    class FHT : public HankelTransform {

        friend std::ostream &operator<<(std::ostream &stream, const FHT<Type> &ob) {
            stream << ob.Serialize(); // << "\n";
            return stream;
        }

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * Default protected constructor, use NewSP methods to construct
         * @see FHT::NewSP
         */
        explicit FHT (const ctor_key& key ) : HankelTransform( key ) {
        }

        /**
         * Protected DeDerializing constructor, use factory DeSerialize  method.
         * @see FHT::DeSerialize
         */
        FHT (const YAML::Node& node, const ctor_key& key) : HankelTransform(node, key) {
        }

        /** Default protected destructor, use smart pointers (std::shared_ptr) */
        ~FHT () {
        }

        /**
         *  Factory method for generating concrete class.
         *  @return a std::shared_ptr of type FHT
         */
        static std::shared_ptr< FHT > NewSP() {
            return std::make_shared< FHT >( ctor_key() );
        }

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *   @see FHT::DeSerialize
         */
        YAML::Node Serialize() const {
            YAML::Node node = HankelTransform::Serialize();
            node.SetTag( this->GetName() ); // + enum2String(Type) );
            //node.SetTag( enum2String(Type) );
            //node["var"] = 0;
            return node;
        }

        /**
         *   Constructs an FHT object from a YAML::Node.
         *   @see FHT::Serialize
         */
        static std::shared_ptr<FHT> DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        Complex Zgauss(const int&, const Lemma::EMMODE&, const int&, const Real&,
            const Real&, Lemma::KernelEM1DBase* Kernel) {
            // TODO, in 101 or 51 we never reach here!!
            std::cout << "Zgauss " << std::endl;
            return this->Zans(0, Kernel->GetManagerIndex());
        }

        /// Computes related kernels, if applicable, otherwise this is
        /// just a dummy function.
        void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DBase> Kernel) {
        }

        void ComputeRelated(const Real& rho, std::vector< std::shared_ptr<KernelEM1DBase> > KernelVec) {
        }

        void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DManager> KernelManager);

        void ComputeLaggedRelated(const Real& rho, const int& nlag, std::shared_ptr<KernelEM1DManager> KernelManager);

        // ====================  ACCESS        =======================

        /**
         *  @param[in] rho is the argument for lagged convolution evaluation from the
         *             spline after calculation.
         */
        void SetLaggedArg(const Real& rho) {
            for (int i=0; i<Zans.cols(); ++ i) {
                Zans(0, i) = Complex( splineVecReal[i]->Interpolate(rho),
                                      splineVecImag[i]->Interpolate(rho) );
            }
            return ;
        }

        // ====================  INQUIRY       =======================

        /**
         *  @return filter asbscissa spacing
         */
        inline Real GetABSER();
        //{
        //    return 0; //this->WT(0,0)/this->WT(1,0);
        //}

        /** Returns the name of the underlying class, similiar to Python's type */
        inline std::string GetName() const {
            return enum2String(Type); //this->CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        // ====================  DATA MEMBERS  =========================

        private:

        // Filter Weights, these are specialized for each template type
		static const Eigen::Matrix<Real, Eigen::Dynamic, 3>  WT;

        /// Spines for lagged convolutions (real part)
        std::vector <std::shared_ptr<CubicSplineInterpolator> > splineVecReal;

        /// Spines for lagged convolutions (imaginary part)
        std::vector < std::shared_ptr<CubicSplineInterpolator> > splineVecImag;

        /// Holds answer, dimensions are NumConv, and NumberRelated.
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> Zans;

        /** ASCII string representation of the class name */
        //static constexpr auto CName = "FHT";

    }; // -----  end of class  FHT  ----

    // Specialisations
    // Note that ANDERSON801, CHAVE, QWEKEY will throw errors as they are not consistent
    //      part of this class
    template < HANKELTRANSFORMTYPE Type >
    Real FHT< Type >::GetABSER() {
        return WT(0,0)/WT(1,0);
    }

    /* specializations could provide slighly better performance */
//     template < >
//     Real FHT< FHTKEY201 >::GetABSER() {
//         return WT(0,0)/WT(1,0);
//     }
//
//     template < >
//     Real FHT< FHTKEY101 >::GetABSER() {
//         return WT(0,0)/WT(1,0);
//     }
//
//     template < >
//     Real FHT< FHTKEY51 >::GetABSER() {
//         return WT(0,0)/WT(1,0);
//     }

    //--------------------------------------------------------------------------------------
    //       Class:  FHT
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    template < HANKELTRANSFORMTYPE Type >
    void FHT<Type>::ComputeRelated ( const Real& rho, std::shared_ptr<KernelEM1DManager> KernelManager ) {

        int nrel = (int)(KernelManager->GetSTLVector().size());
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic > Zwork;
        Zans= Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>::Zero(1, nrel);
        Zwork.resize(WT.rows(), nrel);
        VectorXr lambda = WT.col(0).array()/rho;
        int NumFun = 0;
        int idx = 0;

        // Get Kernel values
        for (int ir=0; ir<lambda.size(); ++ir) {
            // irelated loop
            ++NumFun;
            KernelManager->ComputeReflectionCoeffs(lambda(ir), idx, rho);
            for (int ir2=0; ir2<nrel; ++ir2) {
                // Zwork* needed due to sign convention of filter weights
 			    Zwork(ir, ir2) = std::conj(KernelManager->GetSTLVector()[ir2]->RelBesselArg(lambda(ir)));
            }
        }

        for (int ir2=0; ir2<nrel; ++ir2) {
            Zans(0, ir2) = Zwork.col(ir2).dot(WT.col(KernelManager->GetSTLVector()[ir2]->GetBesselOrder() + 1))/rho;
        }
        return ;
    }		// -----  end of method FHT::ComputeRelated  -----


    //--------------------------------------------------------------------------------------
    //       Class:  FHT
    //      Method:  ComputeLaggedRelated
    //--------------------------------------------------------------------------------------
    template < HANKELTRANSFORMTYPE Type >
    void FHT<Type>::ComputeLaggedRelated ( const Real& rho, const int& nlag, std::shared_ptr<KernelEM1DManager> KernelManager ) {

        int nrel = (int)(KernelManager->GetSTLVector().size());

        Eigen::Matrix< Complex, Eigen::Dynamic, Eigen::Dynamic > Zwork;
        Zans= Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>::Zero(nlag, nrel);
        Zwork.resize(WT.rows()+nlag, nrel);  // Zwork needs to be expanded to filter length + nlag

        // lambda needs to be expanded to include lagged results
        VectorXr lambda = (VectorXr(WT.rows()+nlag) << WT.col(0).array()/rho, VectorXr::Zero(nlag)).finished();
        for (int ilam =WT.rows(); ilam< nlag+WT.rows(); ++ilam) {
            lambda(ilam) = lambda(ilam-1)/GetABSER();
        }

        int NumFun = 0;
        int idx = 0;

        VectorXr Arg(nlag);
        Arg(nlag-1) = rho;
        for (int ilag=nlag-2; ilag>=0; --ilag) {
            Arg(ilag) = Arg(ilag+1) * GetABSER();
        }

        // Get Kernel values
        for (int ir=0; ir<lambda.size(); ++ir) {
            // irelated loop
            ++NumFun;
            KernelManager->ComputeReflectionCoeffs(lambda(ir), idx, rho);
            for (int ir2=0; ir2<nrel; ++ir2) {
 			    Zwork(ir, ir2) = std::conj(KernelManager->GetSTLVector()[ir2]->RelBesselArg(lambda(ir)));
            }
        }

        // Inner product and scale
        int ilagr = nlag-1; // Zwork is in opposite order from Arg
        for (int ilag=0; ilag<nlag; ++ilag) {
            for (int ir2=0; ir2<nrel; ++ir2) {
                Zans(ilagr, ir2) = Zwork.col(ir2).segment(ilag,WT.rows()).dot( WT.col(KernelManager->GetSTLVector()[ir2]->GetBesselOrder()+1) ) / Arg(ilagr);
            }
            ilagr -= 1;
        }

        // make sure vectors are empty
        splineVecReal.clear();
        splineVecImag.clear();

        // Now do cubic spline
        for (int ii=0; ii<Zans.cols(); ++ii) {
            auto SplineR = CubicSplineInterpolator::NewSP();
            SplineR->SetKnots( Arg, Zans.col(ii).real() );
            splineVecReal.push_back(SplineR);

            auto SplineI = CubicSplineInterpolator::NewSP();
            SplineI->SetKnots( Arg, Zans.col(ii).imag() );
            splineVecImag.push_back(SplineI);
        }
        return ;
    }		// -----  end of method FHT::ComputeLaggedRelated  -----

}  // -----  end of namespace Lemma ----

#endif   // ----- #ifndef FHT_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
