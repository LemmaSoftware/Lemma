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
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */


#ifndef  FHTKEY101_INC
#define  FHTKEY101_INC

#include "hankeltransform.h"

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
    class FHTKey101 : public HankelTransform {

        friend std::ostream &operator<<(std::ostream &stream,
                const FHTKey101 &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static FHTKey101* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        Complex Zgauss(const int &ikk, const EMMODE &imode,
                            const int &itype, const Real &rho,
                            const Real &wavef, KernelEm1DBase *Kernel);

        /// Computes related kernels, if applicable, otherwise this is
        /// just a dummy function.
        void ComputeRelated(const Real& rho, KernelEm1DBase* Kernel);

        void ComputeRelated(const Real& rho, std::vector< KernelEm1DBase* > KernelVec);

        void ComputeRelated(const Real& rho, KernelEM1DManager* KernelManager);

        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        FHTKey101 (const std::string& name);

        /** Default protected destructor, use Delete */
        ~FHTKey101 ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        // Shared Filter Weights
		static const Eigen::Matrix<Real, 101, 3>  WT101;

        /// Holds answer, dimensions are NumConv, and NumberRelated.
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> Zans;

    }; // -----  end of class  FHTKey101  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef FHTKEY101_INC  -----

