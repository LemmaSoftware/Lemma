/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/25/2009
  @version  $Id: LemmaObject.cpp 266 2015-04-01 03:24:00Z tirons $
 **/

#include "LemmaObject.h"

namespace Lemma {

    std::ostream &operator<<(std::ostream &stream,
                const LemmaObject &ob) {

      stream << "Class name= " << ob.Name  << "\n";
      return stream;
    }

    #ifdef HAVE_YAMLCPP
    YAML::Emitter& operator << (YAML::Emitter& out, const LemmaObject& ob) {
    out << YAML::BeginMap;
    out << YAML::Key <<"Class Name"    << YAML::Value << ob.Name;
    return out;
    }
    #endif

    // ====================  LIFECYCLE     ==============================

    // Constructor
    LemmaObject::LemmaObject(const std::string& name) :
        NumberOfReferences(0), Name(name) {
    }

    #ifdef HAVE_YAMLCPP
    LemmaObject::LemmaObject(const YAML::Node &node) :
        NumberOfReferences(0), Name(node.Tag()) {

    }
    #endif

    // Destructor
    LemmaObject::~LemmaObject() {
        if (this->NumberOfReferences != 0) {
            throw DeleteObjectWithReferences(this);
        }
    }

    // ====================  OPERATIONS    ==============================

    void LemmaObject::AttachTo (LemmaObject* ptrIn) {
        this->NumberOfReferences++;
        this->RefPtrList.push_back(ptrIn);
    }

    void LemmaObject::DetachFrom (LemmaObject* ptrIn) {
        bool found(false);
        std::vector<LemmaObject*>::iterator iter = this->RefPtrList.begin();
        while (iter != this->RefPtrList.end() ) {
            if (*iter == ptrIn) {
                this->RefPtrList.erase(iter);
                this->NumberOfReferences--;
                found = true;
                break;
            }
            ++iter;
        }
        if (!found) {
            std::cout << *this;
            std::cerr << "Function call DetachFrom Failed \n"
                      << "Caller name " << this->Name << " address=" << this <<
                      " detaching from " <<  ptrIn->Name << " address= " << ptrIn<< "\n"
                      <<  "This class was not listed as attached to this\n";
            exit(EXIT_FAILURE);
        }

        // If there are no remaining references, free up memory.
        if (NumberOfReferences == 0) {
            this->Release();
        }
    }

    // ====================  INQUIRY       ==============================

    unsigned int LemmaObject::GetNumberOfReferences() {
        return this->NumberOfReferences;
    }

    std::vector<LemmaObject*> LemmaObject::GetReferences() {
        return RefPtrList;
    }

    std::string LemmaObject::GetName() {
        return Name;
    }

    // ====================  ACCESS        ==============================

    // ====================  ACCESS        ==============================

    // ====================  OPERATORS     ==============================

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    // Exception classes
    DeleteObjectWithReferences::DeleteObjectWithReferences() :
        runtime_error("DELETED OBJECT WITH REFERENCES") {
        }

    DeleteObjectWithReferences::
            DeleteObjectWithReferences(LemmaObject* ptr) :
        runtime_error("DELETED OBJECT WITH REFERENCES") {
            std::cerr << "DELETED OBJECT WITH REFERENCES THROWN BY INSTANCE OF  "
                      << ptr->GetName() << std::endl;
            std::cerr << ptr->RefPtrList.size() << " connection(s) remain\n";
            for (unsigned int i=0; i<ptr->RefPtrList.size(); ++i) {
                std::cerr << "\tConnection " << i << " is " << ptr->RefPtrList[i]->GetName();
                if (ptr->RefPtrList[i]->GetName().empty())
                    std::cerr << "a deleted object! PLEASE REPORT BUG!\n";
                else
                std::cerr << "\n" << *ptr->RefPtrList[i] << std::endl;
            }
        }

    DeSerializeTypeMismatch::DeSerializeTypeMismatch(LemmaObject *ptr, const std::string& got) :
        runtime_error("DESERIALIZE TYPE MISMATCH") {
            std::cerr << "Expected " << ptr->GetName() << " got " << got << std::endl;
        }

    RequestToReturnNullPointer::
        RequestToReturnNullPointer(LemmaObject *ptr) :
        runtime_error("REQUEST TO RETURN NULL POINTER"){
            std::cout << "Thrown by instance of "
                      << ptr->GetName() << std::endl;
        }

    MatFileCannotBeOpened::
        MatFileCannotBeOpened() :
        runtime_error("MATFILE CANNOT BE OPENED"){}

    AssignmentOutOfBounds::
        AssignmentOutOfBounds(LemmaObject *ptr) :
        runtime_error("ASSIGNMENT OUT OF BOUNDS"){
            std::cout << "Thrown by instance of "
                      << ptr->GetName() << std::endl;
       }

	GenericFileIOError::
		GenericFileIOError(LemmaObject *ptr, const std::string &filename) : runtime_error("FILE I/O ERROR"){
			std::cout << std::endl;
			std::cout << "FILE I/O ERROR" << std::endl;
			std::cout << std::endl;
			std::cout << "Thrown by instance of "
				<< ptr->GetName() << std::endl;
			std::cout << "  while trying to access " << filename << std::endl;
			std::cout << std::endl;
		}

} // end of namespace Lemma
