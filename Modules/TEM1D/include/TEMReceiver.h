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
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2014, Trevor Irons
 */
#pragma once

#ifndef  TEMRECEIVER_INC
#define  TEMRECEIVER_INC

#include <random>
//#include "yaml-cpp/yaml.h"
#include <LemmaCore>
#include <FDEM1D>

//#include <boost/random.hpp>
//#include <boost/random/normal_distribution.hpp>

namespace Lemma {

/**
  \brief   Describes a TEM receiver
  \details A general class for TEM receivers
 */
class TEMReceiver : public FieldPoints {

    friend std::ostream &operator<<(std::ostream &stream, const TEMReceiver &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /** Default locked constructor. */
    explicit TEMReceiver ( const ctor_key& );

    /** Locked deserializing constructor. */
    TEMReceiver (const YAML::Node& node, const ctor_key&);

    /** Default destructor. */
    ~TEMReceiver ();

    /**
     *  Factory method for generating concrete class.
     *  @return a std::shared_ptr of type TEMReceiver
     */
    static std::shared_ptr<TEMReceiver> NewSP();

    /**
     *  Uses YAML to serialize this object.
     *  @note The actual calculation results are not serialized, currently.
     *  @return a YAML::Node
     */
    YAML::Node Serialize() const;

    /**
     *   Constructs an object from a YAML::Node.
     *   @param[in] node is a YAML node containing the serialized class information
     *   @return a std::shared_ptr object of TEMReceiver
     */
    static std::shared_ptr<TEMReceiver> DeSerialize(const YAML::Node& node);


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

    /** Returns the name of the underlying class, similiar to Python's type */
    virtual std::string GetName() const {
        return this->CName;
    }

    protected:

    // ====================  LIFECYCLE     =======================

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    /** ASCII string representation of the class name */
    static constexpr auto CName = "TEMReceiver";

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

