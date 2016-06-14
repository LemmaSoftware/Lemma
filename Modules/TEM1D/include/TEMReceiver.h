/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 02:36:47 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  TEMRECEIVER_INC
#define  TEMRECEIVER_INC

#include "receiverpoints.h"

//#include <boost/random.hpp>
//#include <boost/random/normal_distribution.hpp>
#include <random>

namespace Lemma {

#ifdef HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"
#endif


/**
  \brief
  \details
 */
class TEMReceiver : public ReceiverPoints {

    friend std::ostream &operator<<(std::ostream &stream,
            const TEMReceiver &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static TEMReceiver* New();

    /**
     *   @return a deep copy of this
     */
    TEMReceiver* Clone();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    /**
     * Uses the standard deviations provided to provide an instance
     * of the expected noise. A realisation of the noise.
     */
    VectorXr SampleNoise();

    // ====================  ACCESS        =======================

    /**
     *    Sets the gate integration windows
     */
    void SetWindows(const VectorXr& centres, const VectorXr& widths, const TIMEUNITS& Units);

    /**
     *   Sets the moment of the receiver. Defaults to 1 for normalized response.
     */
    void SetMoment( const Real&  moment);

    /**
     *  Sets std deviation values for noise channels.
     *  @param[in] noise must be of the same length as the number of windows.
     */
    void SetNoiseSTD( const VectorXr& noise );

    /**
     *  Gets std deviation values for noise channels.
     *  @return noise std dev.
     */
    VectorXr GetNoiseSTD(  );

    /**
     *  @param[in] refTime sets the reference time, defaults to 0
     */
    void SetReferenceTime( const Real& refTime, const TIMEUNITS& units );

    /**
     *  What field component to return
     */
    void SetComponent ( const FIELDCOMPONENT& COMP );

    /**
     *   @param[in] loc is the location of the dipole receiver
     */
    void SetRxLocation ( const Vector3r& loc );

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
    static TEMReceiver* DeSerialize(const YAML::Node& node);
     #endif

    /**
     * @return centre point of gates in sec
     */
    VectorXr GetWindowCentres();

    /**
     * @return width of gates in sec
     */
    VectorXr GetWindowWidths();

    /**
     *  @return the reference time
     */
    Real    GetReferenceTime();
    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    TEMReceiver (const std::string& name);

     #ifdef HAVE_YAMLCPP
    /** Default protected de-serializing constructor, use factory DeSerialize */
    TEMReceiver (const YAML::Node& node);
    #endif

    /** Default protected destructor, use Delete */
    ~TEMReceiver ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

    Real            moment;
    Real            referenceTime;
    Vector3r        nHat;
    FIELDCOMPONENT  component;

    VectorXr        windowCentres;
    VectorXr        windowWidths;
    VectorXr        noiseSTD;
    //Vector3r        location;

}; // -----  end of class  TEMReceiver  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMRECEIVER_INC  -----

