/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/28/2010
 **/

#ifndef __hankeltransform_h
#define __hankeltransform_h

#include "KernelEM1DSpec.h"
#include "KernelEM1DManager.h"

namespace Lemma {

        class KernelEM1DBase;

        // ===================================================================
        //        Class:  HankelTransform
        /// \ingroup FDEM1D
        /// \brief   Abstract class for hankel transforms
        /// \details
        // ===================================================================
        class HankelTransform : public LemmaObject {

            public:

                /// Prints out basic info about the class
                friend std::ostream &operator<<(std::ostream &stream,
                        const HankelTransform &ob);

                // ====================  LIFECYCLE     =======================

                // ====================  OPERATORS     =======================

                // ====================  OPERATIONS    =======================

                /// @todo this is actually a deprecated function. Only Chave
                ///   uses this.
                /// Performs numerical integration using Gaussian quadrature
                /// ikk:   type of kernel depending on source and receiver couple
                /// imode: a switch for TE(0) and TM(1) mode
                /// itype: order of Bessel function
                /// rho is argument to integral
                /// wavef is the propogation constant of free space
                /// = omega * sqrt( EP*AMU )  amu = 4 pi e-7  ep = 8.85e-12
                /// @note compare performance of passing Kernel, instead consider
                ///       passing by reference and using Kernel.get() instead.
                virtual Complex Zgauss(const int &ikk, const EMMODE &imode,
                            const int &itype, const Real &rho,
                            const Real &wavef, KernelEM1DBase* Kernel)=0;

                /// Computes related kernels, if applicable, otherwise this is
                /// just a dummy function.
                virtual void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DBase> Kernel);

                virtual void ComputeRelated(const Real& rho, std::vector< std::shared_ptr<KernelEM1DBase> > KernelVec);

                virtual void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DManager> KernelManager);

                // ====================  ACCESS        =======================

                // ====================  INQUIRY       =======================
                /** Returns the name of the underlying class, similiar to Python's type */
                virtual inline std::string GetName() const = 0 ;


                virtual Real GetABSER() { return 0; }

                virtual void ComputeLaggedRelated( const Real& rhomax, const int& nlag, std::shared_ptr<KernelEM1DManager> Manager ) {
                }

                virtual void SetLaggedArg( const Real& rho ) {}

                // ====================  DATA MEMBERS  =======================

            protected:

                // ====================  LIFECYCLE     =======================

                /// Default protected constructor.
                HankelTransform ( const ctor_key& );

                /// Default protected constructor.
                HankelTransform ( const YAML::Node& node, const ctor_key& );

                /// Default protected constructor.
                ~HankelTransform ( );

            private:

                /** ASCII string representation of the class name */
                static constexpr auto CName = "HankelTransform";

    }; // -----  end of class  HankelTransform  -----

}

#endif // __hankeltransform_h
