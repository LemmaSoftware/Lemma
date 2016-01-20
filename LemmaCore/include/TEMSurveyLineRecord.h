/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/10/2014 12:15:03 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  TEMSURVEYLINERECORD_INC
#define  TEMSURVEYLINERECORD_INC

#include "LemmaObject.h"
#include "TEMReceiver.h"
#include "TEMTransmitter.h"
#include "TEMSurveyLineRecordData.h"
#include "instrumenttem.h"

namespace Lemma {

    /**
      \brief    Class holding everything necessary to create a TEM record.
      \details  A TEM record represents the nexus of a transmitter with
                receiver(s) and pulse moment(s). Therefore a record can contain
                multiple sounding curves.
     */
    class TEMSurveyLineRecord : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const TEMSurveyLineRecord &ob);

        friend class TEMSurveyLine;

        public:

        // ====================  LIFECYCLE     =======================

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        /**
         *  Perfoms forward modelling given model.
         *  @param[in] model is the earth model responsible for the response
         *  @param[in] additiveNoise is whether or not to add noise to the response, defaults to false
         *  @return the modelled data.
         */
        TEMSurveyLineRecordData* ForwardModel(LayeredEarthEM* model, bool additiveNoise=false);

        /**
         *  @param[in] nseq is the number of pulse sequences
         */
        void SetNumberOfPulseSequences( const int& nseq );

        /**
         *  @return the number of pulse sequences
         */
        int GetNumberOfPulseSequences();

        /**
         *  Sets the PulseSequence and Receiver pair used to model a *SINGLE* response curve.
         *  @param[in] Seq is the Pulse Sequence
         *  @param[in] Rx is the receiver for that pulse sequence
         */
        void SetTransmitterReceiverPair( const int& ii, TEMTransmitter* Seq, TEMReceiver* Rx );

        // ====================  ACCESS        =======================

        /**
         *  @return a pointer to the specified receiver
         */
        TEMReceiver*   GetReceiver(const int& irec);

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
        static TEMSurveyLineRecord* DeSerialize(const YAML::Node& node);
#endif

        protected:



        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        TEMSurveyLineRecord (const std::string& name);

#ifdef HAVE_YAMLCPP
        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        TEMSurveyLineRecord (const YAML::Node& node);
#endif

        /** Default protected destructor, use Delete */
        ~TEMSurveyLineRecord ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        /**
         *   Delete is private, this class is managed through TEMSurvey
         */
        void Delete();

        // ====================  DATA MEMBERS  =========================

        int     numberOfPulseSequences;

        std::vector<TEMTransmitter*>    Transmitters;
        std::vector<TEMReceiver*>       Receivers;

    }; // -----  end of class  TEMSurveyLineRecord  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef TEMSURVEYLINERECORD_INC  -----
