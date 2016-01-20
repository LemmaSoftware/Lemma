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
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#ifndef  ASCIIPARSER_INC
#define  ASCIIPARSER_INC

#include "LemmaObject.h"
#include <fstream>

namespace Lemma {

/**
  \brief   Provides ASCII input file parsing
  \details Able to accomodate various inputs and comment styles
 */
class ASCIIParser : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const ASCIIParser &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /** @copybrief LemmaObject::New()
     *  @copydetails LemmaObject::New()
     */
    static ASCIIParser* New();

    /** @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

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
    void JumpToLocation(const int& loc);

    // ====================  ACCESS        =======================

    /** Sets the comment identifier key.
     * @param[in] key is a string identifying comments. All text after the key will be
     *  ignored by the parser. Default is //
     */
    void SetCommentString( const std::string& key );

    /** Sets the buffer size. This affects the maximum number of column in a line. Defaults
     *  is 255.
     *  @param[in] BufferSize is the size of the buffer to use
     */
    void SetBufferSize( const int& BufferSize);

    /**
     * @return the current position in the file, as reported by istream::tellg
     */
    int GetFileLocation();

    // ====================  INQUIRY       =======================

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    ASCIIParser (const std::string& name);

    /** Default protected destructor, use Delete */
    ~ASCIIParser ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    // ====================  DATA MEMBERS  =========================

    /** c++ style file IO */
    std::fstream input;

    /** comment string, defaults to c++ style // */
    std::string  CommentString;

    /** Buffer size, max line width supported, defaults to 255 */
    int BufferSize;

}; // -----  end of class  ASCIIParser  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef ASCIIPARSER_INC  -----
