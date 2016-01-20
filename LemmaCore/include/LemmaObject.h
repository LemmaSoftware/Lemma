/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/25/2009
  @version  $Id: LemmaObject.h 199 2014-12-29 19:25:20Z tirons $
 **/

#ifndef __LEMMAOBJECT_H
#define __LEMMAOBJECT_H

#include "helper.h"
#include "lemma.h"

#ifdef HAVE_YAMLCPP
#include "yaml-cpp/yaml.h"
#endif

namespace Lemma {

// ==========================================================================
//        Class:  LemmaObject
/** \brief    Abstract class providing simple reference counting and memory
  *           management.
  * \details  Since objects can be members of other objects, and may be
  *           of multiple objects it becomes useful to count the number of
  *           Classes an object is a member of. Objects cannot be deleted that
  *           contain references to other objects.
  */
// ==========================================================================

/** Base class for all of Lemma. Reference counted object provides simple
 * reference counting. This allows for much easier maintainance of shared
 * objects among classes.
 */
class LemmaObject {

    /** Recursively prints information about this object
     */
    friend std::ostream &operator<<(std::ostream &stream, const LemmaObject &ob);

    #ifdef HAVE_YAMLCPP
    friend YAML::Emitter& operator << (YAML::Emitter& out, const LemmaObject &ob) ;
    #endif

    friend class DeleteObjectWithReferences;

    public:

        // Needed because some derived classes have Eigen vectors as members,
        // causing alignment issues when vectorisation is enabled.
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        // ====================  OPERATORS     ==============================

        // ====================  OPERATIONS    ==============================

        /**
         * Deletes this objects self reference. If there are no remaining
         * references, Release is called freeing all memory. Otherwise Release
         * is called if/when the last reference is removed.
         */
        virtual void Delete()=0;

        /** Attaches this object to another object */
        void AttachTo(LemmaObject *);

        /** Detaches this object from another object */
        void DetachFrom(LemmaObject *);

        // ====================  ACCESS        ==============================

        // ====================  INQUIRY       ==============================

        /** Returns the number of objects attached to this. */
        unsigned int GetNumberOfReferences();

        /** Returns address(s) to class(es) of which this is a member
         */
        std::vector<LemmaObject*> GetReferences();

        /** Returns name of this model */
        std::string GetName();

        #ifdef HAVE_YAMLCPP
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
        virtual YAML::Node Serialize() const{
            return YAML::Node();
        };
        #endif

    protected:

        // ====================  LIFECYCLE     ==============================

        /** Protected default constructor. This is an abstract class and
         *  cannot be instantiated.
         */
        LemmaObject (const std::string &name);

        #ifdef HAVE_YAMLCPP
        /** Protected DeDerializing constructor, use factory DeSerialize  method*/
        LemmaObject (const YAML::Node& node);
        #endif

        // ====================  DATA MEMBERS  ==============================
        /** Protected default destructor. This is an abstract class and
         *  cannot be instantiated.
         */
        virtual ~LemmaObject ();

        /**
         *  Releases all memory back to the heap. Just calls the protected
         *  destructor at the derived class level.
         */
        virtual void Release()=0;

        // ====================  OPERATIONS    ==============================

        // ====================  DATA MEMBERS  ==============================

        /** Number of other classes claiming this as a member. */
        unsigned int NumberOfReferences;

        /** List of the addresses of the other classes referencing this as a
         * member
         */
        std::vector<LemmaObject *> RefPtrList;

        /** Stores an ASCII string representation of the class name */
        std::string Name;

}; // -----  end of class  LemmaObject  -----

    /////////////////////////////////////////////////////////////////
    // BOILERPLATE MACROS
    #define DESERIALIZECHECK( node, Object ) { \
        if (node.Tag() != Object->GetName()) {  \
            throw  DeSerializeTypeMismatch(Object, node.Tag()) ; \
        } } \

    /////////////////////////////////////////////////////////////////
    // Error Classes

    /** Error called when ReferenceCounting breaks. If an object is
     * deleted that has refences still, this is thrown.
     */
    class DeleteObjectWithReferences : public std::runtime_error {

        public:

            /** Call this method internally when an object is deleted with
             * references.
             */
            DeleteObjectWithReferences(LemmaObject *ptr);

            /** @deprecated this error may be thrown, but the error message is much
            * less insightful than
            * DeleteObjectWithReferences(LemmaObject* ptr).
            */
            DeleteObjectWithReferences( );
    };


    /** Error called when DeSerializing breaks. If the node type is not the expected one
     *  this error is thown.
     */
    class DeSerializeTypeMismatch : public std::runtime_error {
        public:
            DeSerializeTypeMismatch(LemmaObject *ptr, const std::string& got);

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
