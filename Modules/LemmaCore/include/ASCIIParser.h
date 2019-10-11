/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/23/2013 02:31:24 PM
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@LemmaSoftware.org
 * @copyright Copyright (c) 2013, 2018 Trevor Irons
 */

#ifndef  ASCIIPARSER_INC
#define  ASCIIPARSER_INC

#include "LemmaObject.h"
#include <fstream>

namespace Lemma {

/**
 * \ingroup LemmaCore
 * \brief   Provides ASCII input file parsing
 * \details Able to accomodate various inputs and comment styles
 */
class ASCIIParser : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const ASCIIParser &ob);
    /*
     *  This key is used to lock the constructors
     */
    //struct ctor_key {};

    public:

    // ====================  LIFECYCLE     =======================

    /** Default constructor
     * @note This method is locked
     * @see ASCIIParser::NewSP
     */
    explicit ASCIIParser ( const ctor_key& );

    /**
     * DeSerializing constructor.
     * @note This method is locked, and cannot be called directly.
     *       The reason that the method is public is to enable the use
     *       of make_shared whilst enforcing the use of shared_ptr,
     *       in c++-17, this curiosity may be resolved.
     * @see ASCIIParser::DeSerialize
     */
    explicit ASCIIParser ( const YAML::Node& node, const ctor_key& );

    /** Default  destructor
     *  @note This should never be called explicitly, use NewSP
     */
    virtual ~ASCIIParser ();

    /**
     *  Factory method for generating concrete class.
     *  @return a std::shared_ptr of type ASCIIParser
     */
    static std::shared_ptr< ASCIIParser >  NewSP();

    /**
     *  Constructs an object from a YAML serialization
     *  @return a std::shared_ptr of type ASCIIParser
     */
    static std::shared_ptr< ASCIIParser >  DeSerialize( const YAML::Node& node );

    /**
     *   Constructs an object from a string representation of a YAML::Node. This is primarily
     *   used in Python wrapping
     */
    static std::shared_ptr<ASCIIParser> DeSerialize( const std::string& node ) {
        return ASCIIParser::DeSerialize(YAML::LoadFile(node));
    }

    /**
     *  Uses YAML to serialize this object.
     *  @return a YAML::Node
     */
    virtual YAML::Node Serialize() const;

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    /** Opens the file up for reading. Throws an exception if something bad happens.
        @param[in] fname is the filename to be parsed.
     */
    void Open (const std::string& fname);


    /** Closes the file. Throws an exception if something bad happens.
     */
    void Close ( );

    /** Reads a series of Reals.
     *  @param[in] nr is the number of reals to read. Enter -1 for the entire file
     *  @todo throw exception if no file is open
     */
    std::vector<Real> ReadReals( const int& nr);

    /** Reads a series of Reals.
     *  @param[in] nr is the number of ints to read. Enter -1 for the entire file
     *  @todo throw exception if no file is open
     */
    std::vector<int> ReadInts( const int& nr);

    /** Reads a series of space delimited strings
     *  @param[in] nr is the number of strings to read. Enter -1 for the entire file
     *  @todo throw exception if no file is open
     */
    std::vector< std::string > ReadStrings( const int& nr);

    /**
     * @param[in] loc is the point in the file to jump to. Uses seekg
     */
    void JumpToLocation(const std::streamoff& loc);

    // ====================  ACCESS        =======================

    /** Sets the comment identifier key.
     * @param[in] key is a string identifying comments. All text after the key will be
     *  ignored by the parser. Default is //
     */
    void SetCommentString( const std::string& key );

    /** Sets the buffer size. This affects the maximum number of columns in a line. Defaults
     *  is 255.
     *  @param[in] BufferSize is the size of the buffer to use
     */
    void SetBufferSize( const int& BufferSize);

    /**
     * @return the current position in the file, as reported by istream::tellg
     */
    std::streamoff GetFileLocation();

    /** Returns the name of the underlying class, similiar to Python's type */
    virtual std::string GetName() const;

    // ====================  INQUIRY       =======================

    protected:

    private:

    /** Copy constructor */
    ASCIIParser( const ASCIIParser& ) = delete;

    // ====================  DATA MEMBERS  =========================

    /** ASCII string representation of the class name */
    static constexpr auto CName = "ASCIIParser";

    /** c++ style file IO */
    std::fstream input;

    /** comment string, defaults to c++ style // */
    std::string  CommentString;

    /** Buffer size, max line width supported, defaults to 255 */
    int BufferSize;

}; // -----  end of class  ASCIIParser  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef ASCIIPARSER_INC  -----

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp syntax=cpp.doxygen */
