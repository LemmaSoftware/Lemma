/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06-13-2016
 **/

#ifndef __LEMMAOBJECT_H
#define __LEMMAOBJECT_H

#include "helper.h"
#include "lemma.h"
#include "yaml-cpp/yaml.h"

#include <chrono>
#include <ctime>
#include <memory>

#include <iostream>
#include <iomanip>
#include <codecvt>

namespace Lemma {

/**
  * \ingroup  LemmaCore
  * \brief    Abstract class providing common interface for Lemma Objects.
  * \details  Lemma objects can be members of other Lemma, and may be members
  *           of multiple objects. Since updates should be atomic, and objects
  *           can be large, it becomes useful to count the number of
  *           Classes an object is a member of.
  *           Before C++-11, this was done internally in Lemma, with the inclusion of
  *           more sophisticated smart pointers, this logic has been offloaded to the
  *           standard. All Lemma objects should be created as C++-11 Smart pointers, using
  *           the supplied New method. Calls to Delete are no longer necessary or available.
  */
class LemmaObject {

    /**
     *  Streams class information as YAML::Node
     */
    //friend YAML::Emitter& operator << (YAML::Emitter& out, const LemmaObject &ob) ;

    protected:
        struct ctor_key{};

    public:

        // Needed because many derived classes have Eigen vectors as members,
        // causing alignment issues when vectorisation is enabled.
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        // ====================  LIFECYCLE     ==============================

        /**
         *  Uses YAML to serialize this object.
         *  @return a YAML::Node
         *  FOR NOT LemmaObject does not write out any YAML info,
         *      in the future the reference count could be logged? But the utility
         *      of that is minimal.
         *  @note Not every Lemma class needs to be Serializable, for instance HankelTransform
         *        classes will never need to be Serialized. There may be a need to differentiate these
         *        two families in to a LemmaInternalClass without serializing and perhaps this class for
         *        all external classes that might need to be serialized.
         */
        virtual YAML::Node Serialize() const {
            std::cout.precision( 20 );
            YAML::Node node = YAML::Node();
            //node.SetStyle(YAML::EmitterStyle::Flow);
            node.SetTag( GetName() );

            // ctime throws warning with MSVC and may not look right in certain locales
            //std::time_t now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
            //std::string ser_time =  std::string( std::ctime(&now) );
            //ser_time.pop_back();
            //node["Serialized"] = ser_time;

            // Alternative formulation
            std::time_t now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
            std::stringstream out;

            // use locale format
            //out.imbue(std::locale("")); // use whatever is on the system
            //out << std::put_time(std::localtime(& now), L"%c") ; // locale on system
            // ISO-8601 format;
            //out << std::put_time(std::localtime(& now), "%F %T %z");

            // Use thread safe variant to suppress MSVC warning
            struct tm timeinfo;
            #if _MSC_VER && !__INTEL_COMPILER
            localtime_s(&timeinfo, &now);
            #else
            localtime_r(&now, &timeinfo);
            #endif
            // ISO-8601 format;
            out << std::put_time(&timeinfo, "%F %T %z");

	        node["Serialized"] = out.str();
            node["Lemma_VERSION"] = LEMMA_VERSION;
            return node;
        };

        // ====================  OPERATORS     ==============================

        // ====================  OPERATIONS    ==============================

        // ====================  ACCESS        ==============================

        virtual std::string Print() {
            YAML::Emitter out;
            out << this->Serialize();
            return out.c_str();
        }

        // ====================  INQUIRY       ==============================

        /** Returns the name of the underlying class; Run-time type information (RTTI). This approach
            Was chosen over typeid due to name mangling among various compilers, and the need for consistency
            in Serialized objects.
         */
        virtual std::string GetName() const;

    protected:

        // ====================  LIFECYCLE     ==============================

        /** Protected default constructor. This is an abstract class and
         *  cannot be instantiated.
         */
        LemmaObject ( const ctor_key& );

        /** Protected DeSerializing constructor */
        LemmaObject ( const YAML::Node& node, const ctor_key& );

        /* Disable copying Lemma Object */
        LemmaObject( const LemmaObject& ) = delete;

        /** Protected default destructor. This is an abstract class and
         *  cannot be instantiated. Virtual is necessary so that if base class destructor is
         *  called, we get the right behaviour.
         */
        virtual ~LemmaObject();

    private:

        // ====================  DATA MEMBERS  ==============================

        /** ASCII string representation of the class name */
        static constexpr auto CName = "LemmaObject";

}; // -----  end of class  LemmaObject  -----

    /////////////////////////////////////////////////////////////////
    // Error Classes

    /** Error called when DeSerializing breaks. If the node type is not the expected one
     *  this error is thown.
     */
    class DeSerializeTypeMismatch : public std::runtime_error {
        public:
            DeSerializeTypeMismatch(const std::string& expected, const std::string& got);

    };

    /** If an assignment is made that is out of bounts, throw this.
     */
    class AssignmentOutOfBounds : public std::runtime_error {
        public:
            /** Throw when an assignment is out of bounds.
             *  @param[in] ptr is a pointer to the class throwing the exception.
             */
            AssignmentOutOfBounds(LemmaObject *ptr);
    };

    /** If a pointer to a class is requested, but it is NULL valued, throw this
     */
    class RequestToReturnNullPointer : public std::runtime_error {
        public:
            /** Thrown when the pointer is NULL
             *  @param[in] ptr is a pointer to the class throwing the exception.
             */
            RequestToReturnNullPointer(LemmaObject *ptr);
    };

    /** If an error in opening a .mat file is encountered, throw this.
     */
    class MatFileCannotBeOpened : public std::runtime_error {
        /** thown when a mat file fails to be opened.
         */
            public: MatFileCannotBeOpened();
    };

	/** Generic file I/O error. */
	class GenericFileIOError : public std::runtime_error {
			public: GenericFileIOError(LemmaObject *ptr, const std::string &filename);
	};

}
#endif // __LEMMAOBJECT_H
