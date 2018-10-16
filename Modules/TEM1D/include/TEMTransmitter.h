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
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  TEMTRANSMITTER_INC
#define  TEMTRANSMITTER_INC

#include "PolygonalWireAntenna.h"

namespace Lemma {

/**
  \brief   Describes a TEM transmitter
  \details Flexible class desribing a source used by TEM instrument.
           Can be used by Airborne or Ground instruments.
 */
class TEMTransmitter : public PolygonalWireAntenna {

    friend std::ostream &operator<<(std::ostream &stream, const TEMTransmitter &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    explicit TEMTransmitter (const ctor_key& );

    /** Default protected constructor, use New */
    TEMTransmitter (const YAML::Node& node, const ctor_key& );

    /** Default protected destructor, use Delete */
    ~TEMTransmitter ();

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static std::shared_ptr<TEMTransmitter> NewSP();

    /**
     *  Performs a deep copy of this and returns pointer
     */
    std::shared_ptr<TEMTransmitter> Clone();

    YAML::Node Serialize() const;

    static std::shared_ptr<TEMTransmitter> DeSerialize(const YAML::Node& node);

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================

    /** Sets the repetition frequency
     */
    void SetRepFrequency(const Real& fr, const FREQUENCYUNITS& units);

    void SetWaveform( const VectorXr& times, const VectorXr& amps, const TIMEUNITS& units );

    // ====================  INQUIRY       =======================

    /**
     *  @return the waveform times in seconds
     */
    VectorXr GetWfmTimes();

    /** @return the waveform current in amps
     */
    VectorXr GetWfmAmps();

    /** Returns the name of the underlying class, similiar to Python's type */
    virtual std::string GetName() const {
        return this->CName;
    }

    protected:

    // ====================  LIFECYCLE     =======================

    private:

    static constexpr auto CName = "TEMTransmitter";

    // ====================  DATA MEMBERS  =========================

    Real                    repFreq;
    FREQUENCYUNITS          repFreqUnits;
    VectorXr                wfmTimes;
    VectorXr                wfmAmps;

}; // -----  end of class  TEMTransmitter  -----

} // namespace Lemma

#endif   // ----- #ifndef TEMTRANSMITTER_INC  -----

