/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 01:51:50 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#ifndef  DCSURVEY
#define  DCSURVEY

#include "LemmaObject.h"
#include "DCIPElectrode.h"

#ifdef HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"
#endif

#ifdef LEMMAUSEVTK
#include <vtkDataSet.h>
#endif

namespace Lemma {

    #define EINFINITY -9999

    /**
      \brief   Describes a DC survey.
      \details This class aims to encapulate any type of DC survey.
     */
    class DCSurvey : public LemmaObject {

        friend std::ostream &operator<<(std::ostream &stream,
                const DCSurvey &ob);

        public:

        // ====================  LIFECYCLE     =======================

        /**
         * @copybrief LemmaObject::New()
         * @copydetails LemmaObject::New()
         */
        static DCSurvey* New();

        /**
         *  @copybrief   LemmaObject::Delete()
         *  @copydetails LemmaObject::Delete()
         */
        void Delete();

        // ====================  OPERATORS     =======================

        // ====================  OPERATIONS    =======================

        /**
         *   Adds new electrode to the survey.
         *   @param[in] Electrode `Pounds' a new Electrode into place.
         *   @return integer index to this electrode.
         *   @note Single electrodes cannot be pulled once they are pounded. The reason for this is that
         *         every index then would become invalid. Furthurmore these are stored in a std::vector and
         *         removing arbitrary items is inefficient.
         *   @see PullElectrodes
         *   @param[in] label is an optional tag for electrode.
         *   @param[in] nodeID is the optional node ID on a mesh
         */
        int PoundElectrode( DCIPElectrode* Electrode, const std::string& label = "NULL", const int& nodeID=-1 );

        /** Alternative Factory method of setting electrodes. IN this manner all memoy management
         *  is handled by DCSurvey.
         *  @note there is no logic preventing one from pounding coincident electrodes. Don't do it.
         *  @note Single electrodes cannot be pulled once they are pounded. The reason for this is that
         *         every index then would become invalid. Furthurmore these are stored in a std::vector and
         *         removing arbitrary items is inefficient.
         *  @see PullElectrodes
         *  @param[in] loc is the location of the electrode.
         *  @param[in] label is an optional label for electrode.
         *  @param[in] nodeID is the optional node ID on a mesh
         */
        DCIPElectrode* PoundElectrode( const Vector3r& loc, const std::string& label = "NULL", const int& nodeID=-1  );

#ifdef LEMMAUSEVTK
        DCIPElectrode* PoundElectrode( const int& nodeID, vtkDataSet* Mesh, const std::string& label = "NULL" );
#endif

        /**
         *  Pulls all electrodes and removes connections.
         */
        void PullElectrodes();

        /**
         *   Adds an injection point.
         *   @param[in] A is the positive current electrode
         *   @param[in] B is the negative current electrode
         *   @param[in] J is the current intensity
         *   @see AddInjection(const int& iA, const int& iB, const Real& J)
         *   @see AddInjection(const int& iA, const Real& J)
         *   @see AddInjection(DCIPElectrode* A, const Real& J)
         */
        int AddInjection( DCIPElectrode* A, DCIPElectrode* B, const Real& J);

        /**
         *   Adds an injection point.
         *   @param[in] A is the positive current electrode.
         *   @param[in] J is the current intensity.
         *   B,  the negative current electrode is taken to be infinity
         *   @see AddInjection(const int& iA, const int& iB, const Real& J)
         *   @see AddInjection(const int& iA, const Real& J)
         *   @see AddInjection(DCIPElectrode* A, DCIPElectrode* B, const Real& J)
         */
        int AddInjection( DCIPElectrode* A, const Real& J );

        /**
         *   Adds an injection point.
         *   @param[in] A is the positive current electrode index
         *   @param[in] B is the negative current electrode index
         *   @param[in] J is the current intensity
         *   @see AddInjection(DCIPElectrode* A, DCIPElectrode* B, const Real& J)
         *   @see AddInjection(DCIPElectrode* A, const Real& J)
         *   @see AddInjection(const int& iA, const Real& J)
         */
        int AddInjection( const int& iA, const int& iB, const Real& J );

        /**
         *   Adds an injection point with the negative electrode at infinity.
         *   @param[in] A is the positive current electrode index
         *   @param[in] J is the current intensity
         *   @see AddInjection(DCIPElectrode* A, DCIPElectrode* B, const Real& J)
         *   @see AddInjection(DCIPElectrode* A, const Real& J)
         *   @see AddInjection(const int& iA, const int& iB, const Real& J)
         */
        int AddInjection( const int& iA, const Real& J );

        /** Adds a potential measurement via the electrode pair comparison \f$ M - N \f$.
         *   @param[in] M is a pointer to the `positive' electrode
         *   @param[in] N is a pointer to the `negative' electrode
         *   @param[in] iJ is the current injection index to associate with this measurement
         */
        void AddMeasurement( const int& iJ, DCIPElectrode* M, DCIPElectrode* N );

        /** Adds a potential measurement via the electrode pair comparison \f$ M - N \f$.
         *   @param[in] M is the `positive' electrode string label name
         *   @param[in] N is the `negative' electrode string label name
         *   @param[in] iJ is the current injection index to associate with this measurement
         *   @see AddMeasurement( const int& iJ, DCIPElectrode* M, DCIPElectrode* N )
         */
        void AddMeasurement( const int& ij, const std::string& M, const std::string& N);

        /**
         *  Used to probe for the index and current of an injection
         *  @param[in] ij is the current injection indes
         *  @param[out] ii is the node index
         *  @param[out] jj is the current
         */
        void GetA( const int& ij, int& ii, Real& jj );

        /**
         *  Used to probe for the index and current of an injection
         *  @param[in] ij is the current injection indes
         *  @param[out] ii is the node index
         *  @param[out] jj is the current
         */
        void GetB( const int& ij, int& ii, Real& jj );


        // ====================  ACCESS        =======================

        // ====================  INQUIRY       =======================

        #ifdef HAVE_YAMLCPP
        YAML::Node Serialize() const;
        static DCSurvey* DeSerialize(const YAML::Node& node);
        #endif

        protected:

        // ====================  LIFECYCLE     =======================

        /** Default protected constructor, use New */
        DCSurvey (const std::string& name);

        #ifdef HAVE_YAMLCPP
        /** Default protected constructor, use New */
        DCSurvey (const YAML::Node& node);
        #endif

        /** Default protected destructor, use Delete */
        ~DCSurvey ();

        /**
         *  @copybrief   LemmaObject::Release()
         *  @copydetails LemmaObject::Release()
         */
        void Release();

        private:

        // ====================  DATA MEMBERS  =========================

        /** The electrodes */
        std::vector<DCIPElectrode*>                Electrodes;

        /** Tags for the electrodes, lines etc. Ordered with Electrodes std::vector */
        std::vector<std::string>                   OrderedElectrodeLabels;

        /** Map of tags for the electrodes, lines etc. */
        std::map<std::string, std::pair<DCIPElectrode*, int> >      ElectrodeLabelMap;

        /** The A Injection electrodes */
        std::vector<int>     A_Electrodes;

        /** The A Injection electrodes */
        std::vector<int>     B_Electrodes;

        /** The A Injection electrodes */
        std::vector<Real>    J_Electrodes;

        /** First Potential electrodes */
        std::vector< std::vector<int> > M_Electrodes;

        /** Second Potential electrodes */
        std::vector< std::vector<int> > N_Electrodes;

    }; // -----  end of class  DCSurvey  -----


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef DCSURVEY  -----
