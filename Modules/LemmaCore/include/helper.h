/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      10/02/2014 02:49:55 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#pragma once

#ifndef  HELPER_INC
#define  HELPER_INC

#include "lemma.h"
#include "yaml-cpp/yaml.h"

namespace Lemma {

/** \addtogroup LemmaCore
 * @{
 */

    /**
     *  Convenience function for string conversion
     *  @param[in] t input value to be converted to string
     *  @return string representation of input value
     */
    template <class T>
    inline std::string to_string (const T& t) {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const FREQUENCYUNITS& Units);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const TIMEUNITS& Units);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const MAGUNITS& Units);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const TEMPUNITS& Units);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const FEMCOILORIENTATION& Units);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const ORIENTATION& Units);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const FIELDCOMPONENT& Comp);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const HANKELTRANSFORMTYPE& Htype);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const FIELDCALCULATIONS& Htype);
    /// convert enums to string saves repeated code useful for YAML serializing
    std::string enum2String(const WINDOWTYPE& Wtype);

    // other way around is a template, where template argument lets us know
    // which specialisation to use.
    template <typename T>
    T string2Enum( const std::string& str );

    // Handy little class that indents a stream.
    // Based on solution provided here, todo may need to add to some managing class which keeps
    // track of nesting levels? But perhaps not. A Lemma class will contain pointers to other Lemma
    // classes. But those are not specifically listed out.
    // http://stackoverflow.com/questions/9599807/how-to-add-indention-to-the-stream-operator
    class IndentingOStreambuf : public std::streambuf {
        std::streambuf*     myDest;
        bool                myIsAtStartOfLine;
        std::string         myIndent;
        std::ostream*       myOwner;
    protected:
        virtual int         overflow( int ch )
        {
            if ( myIsAtStartOfLine && ch != '\n' ) {
                myDest->sputn( myIndent.data(), myIndent.size() );
            }
            myIsAtStartOfLine = ch == '\n';
            return myDest->sputc( ch );
        }
    public:
        explicit            IndentingOStreambuf(
                                std::streambuf* dest, int indent = 4 )
            : myDest( dest )
            , myIsAtStartOfLine( true )
            , myIndent( indent, ' ' )
            , myOwner( NULL )
        {
        }
        explicit            IndentingOStreambuf(
                                std::ostream& dest, int indent = 4 )
            : myDest( dest.rdbuf() )
            , myIsAtStartOfLine( true )
            , myIndent( indent, ' ' )
            , myOwner( &dest )
        {
            myOwner->rdbuf( this );
        }
        virtual             ~IndentingOStreambuf()
        {
            if ( myOwner != NULL ) {
                myOwner->rdbuf( myDest );
            }
        }
    };

/** @}*/

} // end namespace Lemma

///////////////////////////////////////////////////////
// YAML Serializing helper functions. Can we move this into helper.h
namespace YAML {

template<>
struct convert<Lemma::Complex> {
  static Node encode(const Lemma::Complex& rhs) {
    Node node;
    node["real"] = rhs.real();
    node["imag"] = rhs.imag();

    // No labels
    //node.push_back(rhs.real());
    //node.push_back(rhs.imag());

    node.SetTag( "Complex" ); // too verbose?
    return node;
  }

  static bool decode(const Node& node, Lemma::Complex& rhs) {
    // Disabled due to overly verbose output. Just believe...
    if( node.Tag() != "Complex" ) {
        return false;
    }
    rhs = Lemma::Complex( node["real"].as<Lemma::Real>(), node["imag"].as<Lemma::Real>()  );
    // no label style
    //rhs = Lemma::Complex( node[0].as<Lemma::Real>(), node[1].as<Lemma::Real>()  );
    return true;
  }

};

template<>
struct convert<Lemma::Vector3Xr> {
  static Node encode(const Lemma::Vector3Xr& rhs) {
    Node node;
    node["size"] = rhs.cols();
    //node["rows"] = rhs.rows(); // == 3
    for (int ic=0; ic<rhs.cols(); ++ic) {
        node[ic].push_back( rhs(0, ic) );
        node[ic].push_back( rhs(1, ic) );
        node[ic].push_back( rhs(2, ic) );
    }
    node.SetTag( "Vector3Xr" );
    return node;
  }

  static bool decode(const Node& node, Lemma::Vector3Xr& rhs) {
    if( node.Tag() != "Vector3Xr" ) {
        return false;
    }
    rhs.resize( Eigen::NoChange, node["size"].as<int>() );
    for (unsigned int ic=0; ic<node.size(); ++ic) {
        int ir=0;
        for(YAML::const_iterator it=node[ic].begin();it!=node[ic].end();++it) {
            rhs(ir, ic) = it->as<Lemma::Real>();
            ++ir;
        }
    }
    return true;
  }
};

template<>
struct convert<Lemma::VectorXr> {
  static Node encode(const Lemma::VectorXr& rhs) {
    Node node;
    node["size"] = rhs.size();
    for (int ic=0; ic<rhs.size(); ++ic) {
        node["data"].push_back( rhs(ic) );
    }
    node.SetTag( "VectorXr" );
    return node;
  }

  static bool decode(const Node& node, Lemma::VectorXr& rhs) {
    if( node.Tag() != "VectorXr" ) {
        return false;
    }
    rhs.resize( node["size"].as<int>() );
    int ir=0;
    for(YAML::const_iterator it=node["data"].begin(); it!=node["data"].end(); ++it) {
        rhs(ir) = it->as<Lemma::Real>();
        ++ir;
    }
    return true;
  }
};

template<>
struct convert<Lemma::VectorXcr> {
  static Node encode(const Lemma::VectorXcr& rhs) {
    Node node;
    node["size"] = rhs.size();
    for (int ic=0; ic<rhs.size(); ++ic) {
        node["data"].push_back( rhs(ic) );
    }
    node.SetTag( "VectorXcr" );
    return node;
  }

  static bool decode(const Node& node, Lemma::VectorXcr& rhs) {
    if( node.Tag() != "VectorXcr" ) {
        return false;
    }
    rhs.resize( node["size"].as<int>() );
    int ir=0;
    for(YAML::const_iterator it=node["data"].begin(); it!=node["data"].end(); ++it) {
        rhs(ir) = it->as<Lemma::Complex>();
        ++ir;
    }
    return true;
  }
};


template<>
struct convert<Lemma::VectorXi> {
  static Node encode(const Lemma::VectorXi& rhs) {
    Node node;
    node["size"] = rhs.size();
    for (int ic=0; ic<rhs.size(); ++ic) {
        node["data"].push_back( rhs(ic) );
    }
    node.SetTag( "VectorXi" );
    return node;
  }

  static bool decode(const Node& node, Lemma::VectorXi& rhs) {
    if( node.Tag() != "VectorXi" ) {
        return false;
    }
    rhs.resize( node["size"].as<int>() );
    int ir=0;
    for(YAML::const_iterator it=node["data"].begin(); it!=node["data"].end(); ++it) {
        rhs(ir) = it->as<int>();
        ++ir;
    }
    return true;
  }
};

template<>
struct convert<Lemma::Vector3r> {
  static Node encode(const Lemma::Vector3r& rhs) {
    Node node;
    for (int ic=0; ic<rhs.size(); ++ic) {
        node[0].push_back( rhs(ic) );
    }
    node.SetTag( "Vector3r" );
    return node;
  }

  static bool decode(const Node& node, Lemma::Vector3r& rhs) {
    if( node.Tag() != "Vector3r" ) {
        return false;
    }
    int ir=0;
    for(YAML::const_iterator it=node[0].begin(); it!=node[0].end(); ++it) {
        rhs(ir) = it->as<Lemma::Real>();
        ++ir;
    }
    return true;
  }
};

}

#endif   // ----- #ifndef HELPER_INC  -----

