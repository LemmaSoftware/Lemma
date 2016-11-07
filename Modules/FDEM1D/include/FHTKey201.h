/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/11/2014 03:33:08 PM
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2014, Trevor Irons
 */


#ifndef  FHTKEY201_INC
#define  FHTKEY201_INC

#include "HankelTransform.h"

namespace Lemma {

    /**
      \brief   Impliments the fast Hankel transform as outlines by Key 2011
      \details This filter uses 51, 101, or 201 filter points and supports both lagged and related
                kernel arguments. This algorithm is a port of code carrying the following copyright and
                restriction:
                %------------------------------------------------------------------%
                % Copyright (c) 2012 by the Society of Exploration Geophysicists.  %
                % For more information, go to http://software.seg.org/2012/0003 .  %
                % You must read and accept usage terms at:                         %
                % http://software.seg.org/disclaimer.txt before use.               %
                %------------------------------------------------------------------%
     */
    class FHTKey201 : public HankelTransform {

        friend std::ostream &operator<<(std::ostream &stream, const FHTKey201 &ob);

        struct ctor_key {};

        public:

        // ====================  LIFECYCLE     =======================

        /** Default locked constructor, use NewSP */
        FHTKey201 ( const ctor_key& );

        /** DeSerializing locked constructor, use NewSP */
        FHTKey201 ( const YAML::Node& node, const ctor_key& );

        /** Default protected destructor, use Delete */
        ~FHTKey201 ();

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static std::shared_ptr<FHTKey201> NewSP();

        /** YAML Serializing method
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static std::shared_ptr< FHTKey201 > DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        Complex Zgauss(const int &ikk, const EMMODE &imode,
                            const int &itype, const Real &rho,
                            const Real &wavef, std::shared_ptr<KernelEM1DBase> Kernel);

        /// Computes related kernels, if applicable, otherwise this is
        /// just a dummy function.
        void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DBase> Kernel);

        void ComputeRelated(const Real& rho, std::vector< std::shared_ptr<KernelEM1DBase> > KernelVec);

        void ComputeRelated(const Real& rho, std::shared_ptr<KernelEM1DManager> KernelManager);

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        /** Returns the name of the underlying class, similiar to Python's type */
        virtual inline std::string GetName() const {
            return CName;
        }

        protected:

        // ====================  LIFECYCLE     =======================

        private:

        // ====================  DATA MEMBERS  =========================

        // Shared Filter Weights
		static const Eigen::Matrix<Real, 201, 3>  WT201;

        /// Holds answer, dimensions are NumConv, and NumberRelated.
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> Zans;

        /** ASCII string representation of the class name */
        static constexpr auto CName = "FHTKey201";

    }; // -----  end of class  FHTKey201  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef FHTKEY201_INC  -----

