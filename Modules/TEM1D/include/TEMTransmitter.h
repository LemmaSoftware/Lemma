/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/29/2014 11:43:17 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  TEMTRANSMITTER_INC
#define  TEMTRANSMITTER_INC

#include "PolygonalWireAntenna.h"

#ifdef HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"
#endif

namespace Lemma {

/**
  \brief   Describes a TEM pulse sequence
  \details Flexible class desribing a pulse sequence used by
           a TEM instruement. Can be used by Airborne or Ground
           instruments.
 */
class TEMTransmitter : public PolygonalWireAntenna {

    friend std::ostream &operator<<(std::ostream &stream, const TEMTransmitter &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static TEMTransmitter* New();

    /**
     *  Performs a deep copy of this and returns pointer
     */
    TEMTransmitter* Clone();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    /** Sets the repetition frequency
     */
    void SetRepFrequency(const Real& fr, const FREQUENCYUNITS& units);

    void SetWaveform( const VectorXr& times, const VectorXr& amps, const TIMEUNITS& units );

    /**
     *  @return the waveform times in seconds
     */
    VectorXr GetWfmTimes();

    /** @return the waveform current in amps
     */
    VectorXr GetWfmAmps();

    // ====================  INQUIRY       =======================

#ifdef HAVE_YAMLCPP
    YAML::Node Serialize() const;
    static TEMTransmitter* DeSerialize(const YAML::Node& node);
#endif

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    TEMTransmitter (const std::string& name);

#ifdef HAVE_YAMLCPP
    /** Default protected constructor, use New */
    TEMTransmitter (const YAML::Node& node);
#endif

    /** Default protected destructor, use Delete */
    ~TEMTransmitter ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

    Real                    repFreq;
    FREQUENCYUNITS          repFreqUnits;
    VectorXr                wfmTimes;
    VectorXr                wfmAmps;

}; // -----  end of class  TEMTransmitter  -----

} // namespace Lemma

#endif   // ----- #ifndef TEMTRANSMITTER_INC  -----

