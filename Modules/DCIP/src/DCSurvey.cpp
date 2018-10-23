/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/08/2014 01:52:04 PM
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@lemmasoftware.org
 * @copyright Copyright (c) 2014, 2018 Trevor Irons
 */

#include	"DCSurvey.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const DCSurvey &ob) {
        stream << ob.Serialize()  << "\n";
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  DCSurvey
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    DCSurvey::DCSurvey (const ctor_key& key) : LemmaObject(key) {

    }  // -----  end of method DCSurvey::DCSurvey  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  DCSurvey
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    DCSurvey::DCSurvey (const YAML::Node& node, const ctor_key& key) : LemmaObject(node, key) {

        if (node.Tag() != "DCSurvey") {
            throw std::runtime_error("DCSurvey->DeSerialize cannot deserialize non-DCSurvey");
        }

        std::map<YAML::Node, int>  eMap;
        for(YAML::const_iterator it=node["Electrodes"].begin(); it!=node["Electrodes"].end(); ++it) {
            std::string e = it->first.as<std::string>();
            Electrodes.push_back( DCIPElectrode::DeSerialize( node["Electrodes"][e] ) );
            eMap[ node["Electrodes"][Electrodes.size()-1] ] = Electrodes.size() -1 ;
            OrderedElectrodeLabels.push_back(e);
            ElectrodeLabelMap[e] = std::pair< std::shared_ptr<DCIPElectrode>, int>(Electrodes[Electrodes.size()-1], Electrodes.size()-1);
        }

        //std::cout << "J-0\n" << node["Injections"]["J-0"] << std::endl;
        int ij = 0 ;
        for(YAML::const_iterator it=node["Injections"].begin(); it!=node["Injections"].end(); ++it) {

            //std::cout << "it->first" << it->first << std::endl;

            A_Electrodes.push_back( ElectrodeLabelMap[ it->second["A"]["Label"].as<std::string>() ].second );
            B_Electrodes.push_back( ElectrodeLabelMap[ it->second["B"]["Label"].as<std::string>() ].second );
            J_Electrodes.push_back( it->second["J"].as<Real>() );
            //std::string Jstr = it->first.as<std::string>(); // J-1

            M_Electrodes.push_back( std::vector<int>() );
            N_Electrodes.push_back( std::vector<int>() );

            for (int ii=0; ii<it->second["Measurements"]["Number"].as<int>(); ++ii) {
                std::string Mstr = std::string("V-") + to_string(ii);
                //std::cout << "measurements" << it->second["Measurements"][Mstr]["M"]["Label"] << std::endl;
                M_Electrodes[ij].push_back( ElectrodeLabelMap[ it->second["Measurements"][Mstr]["M"]["Label"].as<std::string>() ].second );
                N_Electrodes[ij].push_back( ElectrodeLabelMap[ it->second["Measurements"][Mstr]["N"]["Label"].as<std::string>() ].second );
            }
            ++ ij;
        }

    }  // -----  end of method DCSurvey::DCSurvey  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr<DCSurvey> DCSurvey::NewSP() {
        return std::make_shared<DCSurvey>( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  ~DCSurvey
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    DCSurvey::~DCSurvey () {

    }  // -----  end of method DCSurvey::~DCSurvey  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  DCSurvey::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( this->GetName() );
        node["NumberOfElectrodes"] = Electrodes.size();

        // All the electrodes
        for (std::map<std::string, std::pair< std::shared_ptr<DCIPElectrode>, int> >::const_iterator it  = ElectrodeLabelMap.begin();
                                                                                    it != ElectrodeLabelMap.end(); ++it) {
            node["Electrodes"][ it->first ] = it->second.first->Serialize();
        }

        // Injections and Measurements
        for (unsigned int ic=0; ic<A_Electrodes.size(); ++ic) {
            std::string strLab = std::string("J-") + to_string(ic);
            node["Injections"][strLab]["A"] = node["Electrodes"][ OrderedElectrodeLabels[ A_Electrodes[ic] ] ];
            node["Injections"][strLab]["B"] = node["Electrodes"][ OrderedElectrodeLabels[ B_Electrodes[ic] ] ];
            node["Injections"][strLab]["J"] = J_Electrodes[ic];
            node["Injections"][strLab]["Measurements"]["Number"] = M_Electrodes[ic].size();
            for (unsigned int iv=0; iv<M_Electrodes[ic].size(); ++iv) {
                node["Injections"][strLab]["Measurements"][std::string("V-") + to_string(iv)]["M"] =
                    node["Electrodes"][ OrderedElectrodeLabels[M_Electrodes[ic][iv]] ];
                node["Injections"][strLab]["Measurements"][std::string("V-") + to_string(iv)]["N"] =
                    node["Electrodes"][ OrderedElectrodeLabels[N_Electrodes[ic][iv]] ];
            }
        }

        return node;
    }		// -----  end of method DCSurvey::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<DCSurvey> DCSurvey::DeSerialize ( const YAML::Node& node  ) {
        if ( node.Tag() != DCSurvey::CName ) {
            throw  DeSerializeTypeMismatch( DCSurvey::CName, node.Tag());
        }
        return std::make_shared< DCSurvey > ( node, ctor_key() );
    }		// -----  end of method DCSurvey::DeSerialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  PoundElectrode
    //--------------------------------------------------------------------------------------
    int DCSurvey::PoundElectrode ( std::shared_ptr<DCIPElectrode> Electrode, const std::string& tag, const int&nodeID ) {
        Electrodes.push_back(Electrode);
        if (tag != "NULL") {
            OrderedElectrodeLabels.push_back(tag);
            //ElectrodeTagMap[tag] = Electrode;
            ElectrodeLabelMap[tag] = std::pair< std::shared_ptr<DCIPElectrode>, int> (Electrode, Electrodes.size()-1);
            Electrode->SetLabel(tag);
        } else {
            OrderedElectrodeLabels.push_back( std::string("E") + to_string(Electrodes.size()-1) );
            //ElectrodeTagMap[std::string("E") + to_string(Electrodes.size()-1)] = Electrode;
            ElectrodeLabelMap[std::string("E") + to_string(Electrodes.size()-1)] =
                std::pair< std::shared_ptr<DCIPElectrode>, int>(Electrode, Electrodes.size()-1);
            Electrode->SetLabel( std::string("E") + to_string(Electrodes.size()-1) );
        }
        return  static_cast<int>( Electrodes.size() ) ;
    }		// -----  end of method DCSurvey::PoundElectrode  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  PoundElectrode
    //--------------------------------------------------------------------------------------
    std::shared_ptr<DCIPElectrode> DCSurvey::PoundElectrode ( const Vector3r& loc, const std::string& tag, const int& nodeID ) {
        auto Electrode = DCIPElectrode::NewSP();
            Electrode->SetLocation( loc );
        Electrodes.push_back(Electrode);
        if (tag != "NULL") {
            OrderedElectrodeLabels.push_back(tag);
            ElectrodeLabelMap[tag] = std::pair< std::shared_ptr<DCIPElectrode>, int> (Electrode, Electrodes.size()-1);
            Electrode->SetLabel(tag);
        } else {
            OrderedElectrodeLabels.push_back( std::string("E") + to_string(Electrodes.size()-1) );
            ElectrodeLabelMap[std::string("E") + to_string(Electrodes.size()-1)] =
                std::pair< std::shared_ptr<DCIPElectrode>, int>(Electrode, Electrodes.size()-1);
            Electrode->SetLabel( std::string("E") + to_string(Electrodes.size()-1) );
        }
        return  Electrode;
    }		// -----  end of method DCSurvey::PoundElectrode  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  PoundElectrode
    //--------------------------------------------------------------------------------------
#ifdef LEMMAUSEVTK
    std::shared_ptr<DCIPElectrode> DCSurvey::PoundElectrode( const int& nodeID, vtkDataSet* Mesh, const std::string& tag ) {
        auto Electrode = DCIPElectrode::NewSP();
        double* loc = Mesh->GetPoint(nodeID);
        Electrode->SetLocation( Vector3r(loc[0], loc[1], loc[2]) );
        Electrodes.push_back(Electrode);
        if (tag != "NULL") {
            OrderedElectrodeLabels.push_back(tag);
            ElectrodeLabelMap[tag] = std::pair< std::shared_ptr<DCIPElectrode>, int> (Electrode, Electrodes.size()-1);
            Electrode->SetLabel(tag);
        } else {
            OrderedElectrodeLabels.push_back( std::string("E") + to_string(Electrodes.size()-1) );
            ElectrodeLabelMap[std::string("E") + to_string(Electrodes.size()-1)] =
                std::pair< std::shared_ptr<DCIPElectrode>, int>(Electrode, Electrodes.size()-1);
            Electrode->SetLabel( std::string("E") + to_string(Electrodes.size()-1) );
        }
        return Electrode;
    }		// -----  end of method DCSurvey::PoundElectrode  -----
#endif

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  PullElectrodes
    //--------------------------------------------------------------------------------------
    void DCSurvey::PullElectrodes (  ) {
        Electrodes.clear();
        OrderedElectrodeLabels.clear();
        ElectrodeLabelMap.clear();
        A_Electrodes.clear();
        B_Electrodes.clear();
        M_Electrodes.clear();
        N_Electrodes.clear();

        return ;
    }		// -----  end of method DCSurvey::PullElectrodes  -----


    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  AddInjection
    //--------------------------------------------------------------------------------------
    int DCSurvey::AddInjection ( std::shared_ptr<DCIPElectrode> A, std::shared_ptr<DCIPElectrode> B, const Real& J ) {
        bool fA = false;
        bool fB = false;
        for (unsigned int i=0; i<Electrodes.size(); ++i) {
            if (Electrodes[i] == A) {
                A_Electrodes.push_back(i);
                M_Electrodes.push_back( std::vector<int>() );
                N_Electrodes.push_back( std::vector<int>() );
                fA = true;
            }
            if (Electrodes[i] == B) {
                B_Electrodes.push_back(i);
                fB = true;
            }
            if (fA == true && fB == true) break;
        }
        if (!fA) {
            throw std::runtime_error( "Injection point A not found" );
        }
        if (!fB) {
            throw std::runtime_error( "Injection point B not found" );
        }
        J_Electrodes.push_back(J);
        return A_Electrodes.size()-1; // we want index
    }		// -----  end of method DCSurvey::AddInjection  -----


    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  AddMeasurement
    //--------------------------------------------------------------------------------------
    void DCSurvey::AddMeasurement ( const int& iJ, std::shared_ptr<DCIPElectrode> M, std::shared_ptr<DCIPElectrode> N  ) {
        bool fM = false;
        bool fN = false;
        for (unsigned int i=0; i<Electrodes.size(); ++i) {
            if (Electrodes[i] == M) {
                M_Electrodes[iJ].push_back(i);
                fM = true;
            }
            if (Electrodes[i] == N) {
                N_Electrodes[iJ].push_back(i);
                fN = true;
            }
            if (fM == true && fN == true) break;
        }
        if (!fM) {
            throw std::runtime_error( "Injection point M not found" );
        }
        if (!fN) {
            throw std::runtime_error( "Injection point N not found" );
        }
        return ;
    }		// -----  end of method DCSurvey::AddMeasurement  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  AddMeasurement
    //--------------------------------------------------------------------------------------

    void DCSurvey::AddMeasurement ( const int& iJ, const std::string& M, const std::string& N ) {

        std::pair< std::shared_ptr<DCIPElectrode>, int> Mp = ElectrodeLabelMap[M];
        std::pair< std::shared_ptr<DCIPElectrode>, int> Np = ElectrodeLabelMap[N];

        if (Mp.first == NULL) {
            throw std::runtime_error( "Injection point M not found" );
        }
        if (Np.first == NULL) {
            throw std::runtime_error( "Injection point N not found" );
        }

        M_Electrodes[iJ].push_back( Mp.second );
        N_Electrodes[iJ].push_back( Np.second );

        return ;
    }		// -----  end of method DCSurvey::AddMeasurement  -----


    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  GetA
    //--------------------------------------------------------------------------------------
    void DCSurvey::GetA (const int& iA, int& iB, Real& J ) {
        iB = Electrodes[A_Electrodes[iA]]->GetNodeID();
        J = J_Electrodes[iA];
        return ;
    }		// -----  end of method DCSurvey::GetA  -----

    //--------------------------------------------------------------------------------------
    //       Class:  DCSurvey
    //      Method:  GetB
    //--------------------------------------------------------------------------------------
    void DCSurvey::GetB (const int& iA, int& iB, Real& J ) {
        iB = Electrodes[B_Electrodes[iA]]->GetNodeID();
        J = -J_Electrodes[iA];
        return ;
    }		// -----  end of method DCSurvey::GetA  -----


}		// -----  end of Lemma  name  -----

