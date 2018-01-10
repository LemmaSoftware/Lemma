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


#ifndef  FHTKEY51_INC
#define  FHTKEY51_INC

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
    class FHTKey51 : public HankelTransform {

        friend std::ostream &operator<<(std::ostream &stream, const FHTKey51 &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /** Default locked constructor, use NewSP */
        FHTKey51 ( const ctor_key& );

        /** DeSerializing locked constructor, use DeSerialize */
        FHTKey51 ( const YAML::Node& node, const ctor_key& );

        /** Default destructor */
        ~FHTKey51 ();

        /**
         *  Factory method for generating objects.
         *  @return std::shared_ptr<FHTKey51>
         */
        static std::shared_ptr<FHTKey51> NewSP();

        /** YAML Serializing method
         */
        YAML::Node Serialize() const;

        /**
         *   Constructs an object from a YAML::Node.
         */
        static std::shared_ptr< FHTKey51 > DeSerialize(const YAML::Node& node);

        // ====================  OPERATORS     =======================

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
        virtual inline std::string GetName() const ;

        protected:

        private:

        // ====================  DATA MEMBERS  =========================

        // Shared Filter Weights
		static const Eigen::Matrix<Real, 51, 3>  WT51;

        /// Holds answer, dimensions are NumConv, and NumberRelated.
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> Zans;

        /** ASCII string representation of the class name */
        static constexpr auto CName = "FHTKey51";

    }; // -----  end of class  FHTKey51  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef FHTKEY51_INC  -----

