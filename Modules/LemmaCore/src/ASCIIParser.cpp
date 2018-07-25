/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/23/2013 02:33:41 PM
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@Lemmasoftware.org
 * @copyright Copyright (c) 2013,2018 Trevor Irons
 */

#include "ASCIIParser.h"

namespace Lemma {


    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const ASCIIParser &ob) {
        stream << ob.Serialize()  << "\n"; 
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ASCIIParser
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    ASCIIParser::ASCIIParser ( const ctor_key& key ) : LemmaObject( key ), input(),
            CommentString("//"), BufferSize(255) {

    }  // -----  end of method ASCIIParser::ASCIIParser  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  NewSP()
    // Description:  public smart pointer factory constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr< ASCIIParser > ASCIIParser::NewSP() {
        return  std::make_shared<ASCIIParser>( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  ASCIIParser::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();;
        node.SetTag( GetName() );
        node["CommentString"] = CommentString;
        node["BufferSize"] = BufferSize;
        return node;
    }		// -----  end of method ASCIIParser::Serialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<ASCIIParser> ASCIIParser::DeSerialize ( const YAML::Node& node ) {
        if (node.Tag() != "ASCIIParser") {
            throw  DeSerializeTypeMismatch( "ASCIIParser", node.Tag());
        }
        return std::make_shared< ASCIIParser >( node, ctor_key() ); //, ctor_key() );
    }		// -----  end of method ASCIIParser::DeSerialize  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ~ASCIIParser
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    ASCIIParser::~ASCIIParser () {

    }  // -----  end of method ASCIIParser::~ASCIIParser  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string ASCIIParser::GetName (  ) const {
        return CName;
    }		// -----  end of method ASCIIParser::GetName  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ASCIIParser
    // Description:  DeSerializing constructor (protected)
    //--------------------------------------------------------------------------------------
    ASCIIParser::ASCIIParser (const YAML::Node& node, const ctor_key&  key) : LemmaObject(node, key) {
        this->CommentString = node["CommentString"].as<std::string>();
        this->BufferSize = node["BufferSize"].as<int>();
    }  // -----  end of method ASCIIParser::ASCIIParser  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  Open
    //--------------------------------------------------------------------------------------
    void ASCIIParser::Open ( const std::string& fname ) {
        input.open(fname.c_str(), std::ios::in);
        if (input.fail()) {
           throw  GenericFileIOError(this, fname);
        }
        return ;
    }		// -----  end of method ASCIIParser::Open  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  Close
    //--------------------------------------------------------------------------------------
    void ASCIIParser::Close (  ) {
        input.close();
        return ;
    }		// -----  end of method ASCIIParser::Close  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ReadReals
    //--------------------------------------------------------------------------------------
    std::vector<Real> ASCIIParser::ReadReals ( const int& nr ) {
        std::string buf;
        char *dump = new char[BufferSize];
        std::vector<Real> vals(0);
        while (input >> buf) {
            if (buf.substr(0, CommentString.size()) == CommentString) {
                input.getline(dump, BufferSize);
            } else {
                vals.push_back( atof(buf.c_str() ));
            }
            if (static_cast<int>(vals.size()) == nr) {
                delete [] dump;
                return vals;
            }
        }
        delete [] dump;
        return vals;
    }		// -----  end of method ASCIIParser::ReadReals  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ReadInts
    //--------------------------------------------------------------------------------------
    std::vector<int> ASCIIParser::ReadInts ( const int& nr ) {
        std::string buf;
        char *dump = new char[BufferSize];
        std::vector<int> vals(0);
        while (input >> buf) {
            if (buf.substr(0, CommentString.size()) == CommentString) {
                input.getline(dump, BufferSize);
            } else {
                vals.push_back( atoi(buf.c_str() ));
            }
            if (static_cast<int>(vals.size()) == nr) {
                delete [] dump;
                return vals;
            }
        }
        delete [] dump;
        return vals;
    }		// -----  end of method ASCIIParser::ReadInts  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  ReadStings
    //--------------------------------------------------------------------------------------
    std::vector<std::string> ASCIIParser::ReadStrings ( const int& nr ) {
        std::string buf;
        char *dump = new char[BufferSize];
        std::vector<std::string> vals(0);
        while (input >> buf) {
            if (buf.substr(0, CommentString.size()) == CommentString) {
                input.getline(dump, BufferSize);
            } else {
                vals.push_back( buf );
            }
            if (static_cast<int>(vals.size()) == nr) {
                delete [] dump;
                return vals;
            }
        }
        delete [] dump;
        return vals;
    }		// -----  end of method ASCIIParser::ReadInts  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  SetCommentString
    //--------------------------------------------------------------------------------------
    void ASCIIParser::SetCommentString ( const std::string& key ) {
        CommentString = key;
    }		// -----  end of method ASCIIParser::SetCommentString  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  SetBufferSize
    //--------------------------------------------------------------------------------------
    void ASCIIParser::SetBufferSize ( const int& size ) {
        BufferSize = size;
    }		// -----  end of method ASCIIParser::SetCommentString  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  GetFileLocation
    //--------------------------------------------------------------------------------------
    std::streamoff ASCIIParser::GetFileLocation (  ) {
        return input.tellg();
    }		// -----  end of method ASCIIParser::GetFileLocation  -----

    //--------------------------------------------------------------------------------------
    //       Class:  ASCIIParser
    //      Method:  JumpToLocation
    //--------------------------------------------------------------------------------------
    void ASCIIParser::JumpToLocation ( const std::streamoff& loc ) {
        input.seekg( loc );
        return ;
    }		// -----  end of method ASCIIParser::JumpToLocation  -----

}		// -----  end of Lemma  name  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp syntax=cpp.doxygen: */
