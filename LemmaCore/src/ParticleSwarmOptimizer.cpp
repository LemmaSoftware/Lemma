/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      01/27/2015 01:59:03 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2015, XRI Geophysics, LLC
 * @copyright Copyright (c) 2015, Trevor Irons
 */

#include "ParticleSwarmOptimizer.h"

namespace Lemma {



    // ====================  FRIEND METHODS  =====================
#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const ParticleSwarmOptimizer &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream, const ParticleSwarmOptimizer& ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }
#endif

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  ParticleSwarmOptimizer
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    ParticleSwarmOptimizer::ParticleSwarmOptimizer (const std::string& name) : LemmaObject(name) {

    }  // -----  end of method ParticleSwarmOptimizer::ParticleSwarmOptimizer  (constructor)  -----

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  ParticleSwarmOptimizer
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    ParticleSwarmOptimizer::ParticleSwarmOptimizer (const YAML::Node& node) : LemmaObject(node) {

    }  // -----  end of method ParticleSwarmOptimizer::ParticleSwarmOptimizer  (constructor)  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    ParticleSwarmOptimizer* ParticleSwarmOptimizer::New() {
        ParticleSwarmOptimizer*  Obj = new ParticleSwarmOptimizer("ParticleSwarmOptimizer");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  ~ParticleSwarmOptimizer
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    ParticleSwarmOptimizer::~ParticleSwarmOptimizer () {

    }  // -----  end of method ParticleSwarmOptimizer::~ParticleSwarmOptimizer  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void ParticleSwarmOptimizer::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void ParticleSwarmOptimizer::Release() {
        delete this;
    }


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  ParticleSwarmOptimizer::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( this->Name );
        // FILL IN CLASS SPECIFICS HERE
        return node;
    }		// -----  end of method ParticleSwarmOptimizer::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  ParticleSwarmOptimizer
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    ParticleSwarmOptimizer* ParticleSwarmOptimizer::DeSerialize ( const YAML::Node& node  ) {
        ParticleSwarmOptimizer* Object = new ParticleSwarmOptimizer(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
            return Object ;
    }		// -----  end of method ParticleSwarmOptimizer::DeSerialize  -----
#endif


}		// -----  end of Lemma  name  -----
