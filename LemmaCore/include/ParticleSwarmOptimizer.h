/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/27/2015 01:50:59 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#ifndef  PARTICLESWARMOPTIMIZER_INC
#define  PARTICLESWARMOPTIMIZER_INC

#include "LemmaObject.h"

namespace Lemma {

struct Particle{
    Real       fitness;
    VectorXr   best;
};

/**
  \brief
  \details
 */
class ParticleSwarmOptimizer : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const ParticleSwarmOptimizer &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static ParticleSwarmOptimizer* New();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    // ====================  INQUIRY       =======================

#ifdef HAVE_YAMLCPP
    /**
     *  Uses YAML to serialize this object.
     *  @return a YAML::Node
     */
    YAML::Node Serialize() const;

    /**
     *   Constructs an object from a YAML::Node.
     */
    static ParticleSwarmOptimizer* DeSerialize(const YAML::Node& node);
#endif

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    ParticleSwarmOptimizer (const std::string& name);

#ifdef HAVE_YAMLCPP
    /** Protected DeDerializing constructor, use factory DeSerialize  method*/
    ParticleSwarmOptimizer (const YAML::Node& node);
#endif

    /** Default protected destructor, use Delete */
    ~ParticleSwarmOptimizer ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

}; // -----  end of class  ParticleSwarmOptimizer  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef PARTICLESWARMOPTIMIZER_INC  -----

