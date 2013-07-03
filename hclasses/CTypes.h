/******************************************************************************
 * 
 * Project:  Helper classes
 * Purpose:  Generic functions
 * Author:   Maarten Plieger, plieger "at" knmi.nl
 * Date:     2013-06-01
 *
 ******************************************************************************
 *
 * Copyright 2013, Royal Netherlands Meteorological Institute (KNMI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 ******************************************************************************/


#ifndef CTypes_H
#define CTypes_H
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include <typeinfo>
#include <exception>
#include <stdlib.h>
#include <regex.h>
#include "CDebugger.h"

#define END NULL


#ifndef CTYPESCOMPATON
#define toLowerCaser toLowerCase
#define replacer replace
#endif 


namespace CT{
  using namespace std;
  
  /**
   * Vector of pointers, which frees pointers upon deletion
   */
  template <class T2>
  class PointerList:public std::vector<T2>{
    public:
    ~PointerList(){free();}
    void free(){for(size_t j=0;j<this->size();j++){delete (*this)[j];(*this)[j]=NULL;}}
    T2 get(int j){return (*this)[j];}
    void add(T2 t){push_back(t);}
  };

  /**
   * Vector of objects on the stack
   */
  template <class T3>
  class StackList:public std::vector<T3>{
    public:
    ~StackList(){}
    void add(T3 t){push_back(t);}
    StackList(){}
    /*Copy constructor*/
    StackList(StackList <T3>const &f){for(size_t j=0;j<f.size();j++){add(f[j]);}}
  };
    
  /**
   * Basetype object
   */
  class basetype{
    public:
         virtual void init() = 0;
         virtual ~basetype(){}
      int id;
      size_t count;
      basetype *next,*prev,*start,*end;//For linked list
  };
  
  /**
   * Create a linked list of objects of type basetype
   * @param the object array to linke
   * @param nr the length of the array
   */
  template <class T>
  void CTlink(T *object,int nr){
    for(int j=0;j<nr;j++){
      object[j].next=&object[j+1];
      object[j+1].prev=&object[j];
      object[j].start=&object[0];
      object[j].end=&object[nr];
    }
    object[0].prev=NULL;object[nr].next=NULL;
    object[nr].start=&object[0];
    object[nr].end=&object[nr];
  }

class string:public basetype{
  private:
    int allocated;
    size_t privatelength; // Length of string
    size_t bufferlength;  // Length of buffer
    void _Free();
    void _Allocate(int _length);
    const char* strrstr(const char *x, const char *y);
    char _tohex(char in);
    char _fromhex(char in);
    char *value;
    void init(){value=NULL;count=0;allocated=0;privatelength=0;bufferlength=0;}
    DEF_ERRORFUNCTION();
  public:
    /**
     * Default constructor
     */
    string();
    
    /**
     *Copy constructor
     */
    string(string const &f);
    
    /**
     * assign operator
     * @param f The input string
     */
    string& operator= (string const& f);
    
    /**
     * assign operator
     * @param f The input character array
     */
    string& operator= (const char*const &f);
    
    /**
     * assign operator
     * @param f The input integer (checking for 0 or NULL pointer);
     */
/*    
 *    TODO DOES NOT WORK YET
 *    string& operator= (int const& f){
      #ifdef CTYPES_DEBUG
      CDBDebug("string::operator= (int *const &f)\n");
      #endif
      init();return *this;
    }*/
    
    /**
     * addition assignment operator
     * @param f The input string
     */
    string& operator+= (string const& f);
    
    /**
     * addition assignment operator
     * @param f The input character array
     */
    string& operator+= (const char*const &f);
    
    /**
     * addition operator
     * @param f The input string
     */
    string& operator+ (string const& f);
    
    /**
     * addition operator
     * @param f The input character array
     */
    string& operator+ (const char*const &f);
    
    /**
     * Copy constructor which initialize the string with a character array
     * @param _value The character array to copy
     * @param _length the length of the character array
     */
    string(const char * _value,size_t _length){init();copy(_value,_length);}

    /**
     * Copy constructor which initialize the string with a character array
     * @param _value The character array to copy
     */
    string(const char * _value){
      #ifdef CTYPES_DEBUG
      CDBDebug("string(const char * _value)\n");
      #endif      
      init();copy(_value,strlen(_value));
    }
 
    /**
     * Copy constructor which initialize the string with the contents of a string pointer
     * @param _string Pointer to the string to copy
     */
    string(CT::string*_string){init();copy(_string);}
    
    /**
     * Destructor
     */
    virtual ~string(){_Free();  }

    /**
     * returns length of the string
     * @return length
     */
    size_t length(){return privatelength;}
    
    /**
     * returns the internal bufferlength of the string
     * @return internal bufferlength
     */
    size_t getbufferlength(){return bufferlength;}
    
    /**
     * Copy a character array into the string
     * @param _value The character array to copy
     * @param _length the length of the character array
     */
    void copy(const char * _value,size_t _length);
    
    /**
     * Copy a string pointer into the array
     * @param _string Pointer to the string to copy
     */
    void copy(const CT::string*_string){if(_string==NULL){_Free();return;}copy(_string->value,_string->privatelength);};

    /**
     * Copy a character array into the string
     * @param _value The character array to copy
     */
    void copy(const char * _value){ if(_value==NULL){_Free();return;}copy(_value,strlen(_value));};
    
    /**
     * Appends a pointer to a string object to this string object
     * @param string* The string pointer to append
     */
    void concat(const CT::string*_string){concat(_string->value,_string->privatelength);}
    
    /**
     * Appends a string object to this string object
     * @param string The string to append
     */
    void concat(const CT::string _string){concat(_string.value,_string.privatelength);}
    
    /**
     * Appends an array of characters with specified length to this string object
     * @param value The character array to append
     * @param len The length of the character array
     */
    void concat(const char*_value,size_t len);

    /**
     * Appends an array of characters terminated with a '\0' character.
     * @param value The 0-terminated character array to append
     */
    void concat(const char*_value){if(_value==NULL)return;concat(_value,strlen(_value));};
    
    /**
     * Returns the char value at the specified index.
     * @param index The index of the character to get.
     */
    char charAt(size_t index);
    
    /**
     * Sets a character in the string object at specified location
     * @param location The location to set
     * @param character The character to set
     */
    void setChar(size_t location,const char character);
    
    /**
     * Compares this string to the specified object. The result is true if the given argument is not null and representing the same sequence of characters as this object.
     * @param value The character array to compare
     * @param length The length of the character array to compare
     */
    bool equals(const char *value,size_t length);
    
    /**
     * Compares this string to the specified object. The result is true if the given argument is not null and representing the same sequence of characters as this object.
     * @param value  The 0-terminated character array to compare
     */
    bool equals(const char *value);
    
    /**
     * Compares this string to the specified object. The result is true if the given argument is not null and representing the same sequence of characters as this object.
     * @param string*  Pointer to the string object to compare
     */
    bool equals(CT::string* string);

    /**
     * Compares this string to the specified object. The result is true if the given argument is not null and representing the same sequence of characters as this object.
     * @param string Copy of the string object to compare
     */
    bool equals(CT::string string);

    bool equalsIgnoreCase(const char *_value,size_t _length);
    
    bool equalsIgnoreCase(const char *_value);
    
    bool equalsIgnoreCase(CT::string* _string);
    
    bool equalsIgnoreCase(CT::string string);
    
    /**
     * Tests for a posix regular expression against the string object, returns true if matches.
     * @param pattern The 0-terminated character array containing the regular expression
     */
    bool testRegEx(const char *pattern);

    /**
     * Returns the index within this string of the first occurrence of the specified character. 
     * If a character with value ch occurs in the character sequence represented by this String object, then the index of the first such occurrence is returned
     * @param search The character array to look for
     * @param length The length of the character array
     * @return -1 if not found, otherwise the index of the character sequence in this string object
     */
    int indexOf(const char* search,size_t length);
    
    /**
     * Returns the index within this string of the first occurrence of the specified character. 
     * If a character with value ch occurs in the character sequence represented by this String object, then the index of the first such occurrence is returned
     * @param search The 0-terminated character array to look for
     * @return -1 if not found, otherwise the index of the character sequence in this string object
     */
    int indexOf(const char* search){return indexOf(search,strlen(search));};
    
    /**
     * Returns the index within this string of the last occurrence of the specified character
     * @param search The character array to look for
     * @param length The length of the character array
     * @return -1 if not found, otherwise the last index of the character sequence in this string object
     */
    int lastIndexOf(const char* search,size_t _length);
    
    /**
     * Returns the index within this string of the last occurrence of the specified character
     * @param search The character array to look for
     * @param search The 0-terminated character array to look for
     * @return -1 if not found, otherwise the last index of the character sequence in this string object
     */
    int lastIndexOf(const char* search){return lastIndexOf(search,strlen(search));};
    
    /**
     * String to unicode
     */
    void toUnicodeSelf();
    
    /**
     * String to uppercase
     */
    void toUpperCaseSelf();
    
    /**
     * String to lowercase
     */
    void toLowerCaseSelf();
    
    /**
     * Return lowercase string
     */
    CT::string toLowerCaser(){
      CT::string t;
      t.copy(value,privatelength);
      t.toLowerCaseSelf();
      return t;
    }
    
    /**
     * Decodes URL to string
     */
    void decodeURLSelf();
    
    /**
     * Encodes string to URL
     */
    void encodeURLSelf();
    
    /**
     * Removes spaces in this string
     */
    void trimSelf();
    
    /**
     * Returns a new string with removed spaces
     */
    string trimr(){CT::string r;r.copy(value,privatelength);r.trimSelf();return r;}
    
    /**
     * Function which returns an array of strings
     * the count value is set in all string object, which indicates the length of the array
     * @param _value The token to split the string on
     */
    string * splitToArray(const char * _value);
    
    /**
     * Function which returns a pointer of std::vector which holds a list of string pointers 
     * deleting the std::vector pointer will also delete the strings inside
     * @param _value The token to split the string on
     */
    PointerList<CT::string*>* splitToPointer(const char * _value);
    
    /**
     * Function which returns a std::vector on the stack with a list of strings allocated on the stack
     * Data is automatically freed
     * @param _value The token to split the string on
     */
    StackList<CT::string> splitToStack(const char * _value);
    
    /**
     * Print like printf to this string
     * @param a The string to print
     */
    void print(const char *a, ...);
    
    /**
     * Like printf, but concatenates the string
     * @param a The string to print
     */
    void printconcat(const char *a, ...);
    
    /**
     * Get a character array with the string data
     * @return the character array
     */
    const char * c_str();
    
    /** Replace all strings with another string
     * @param substr the character array to replace
     * @param substrl the length of the character array to replace
     * @param newString the new character array to replace with
     * @param newStringl The length of the character array to replace with
     * @return Zero on success
     */
    int replaceSelf(const char *substr,size_t substrl,const char *newString,size_t newStringl);
    
    /** Replace all strings with another string
     * @param substr the string to replace
     * @param newString the new stringto replace with
     * @return Zero on success
     */
    int replaceSelf(CT::string *substr,CT::string *newString){return replaceSelf(substr->value,substr->privatelength,newString->value,newString->privatelength);}
    
    
    /** Replace all strings with another string
     * @param substr the character array to replace
     * @param newString the new string to replace with
     * @return Zero on success
     */
    int replaceSelf(const char *substr,CT::string *newString){return replaceSelf(substr,strlen(substr),newString->value,newString->privatelength);}
    
    
    /** Replace all strings with another string
     * @param substr the string to replace
     * @param newString the new character array to replace with
     * @return Zero on success
     */
    int replaceSelf(CT::string *substr,const char *newString){return replaceSelf(substr->value,substr->privatelength,newString,strlen(newString));}
    
    /** Replace all strings with another string
     * @param substr the character array to replace
     * @param newString the new character array to replace with
     * @return Zero on success
     */
    int replaceSelf(const char *substr,const char *newString){return replaceSelf(substr,strlen(substr),newString,strlen(newString));}
    
    /** Replace all strings with another string and returns the new string
     * @param substr the character array to replace
     * @param newString the new character array to replace with
     * @return the subsetted string
     */
    CT::string replacer(const char * old,const char *newstr){string r;r.copy(value,privatelength);r.replaceSelf(old,newstr);return r;}

    /**
      * Subset the string from start till end
      * @param string Te input string to subset
      * @param start Where to subset from
      * @param end Where to subset to (-1 means till the end of the string)
      * @return Zero on success
      */
    int substringSelf(CT::string *string, size_t start,size_t end);
    
    /**
     * Subset the string from start till end
     * @param start Where to subset from
     * @param end Where to subset to (-1 means till the end of the string)
     * @return Zero on success
     */
    int substringSelf(size_t start,size_t end){substringSelf(this, start, end);return 0;}
    
    /**
     * Returns a subsetted string from start till end
     * @param start Where to subset from
     * @param end Where to subset to (-1 means till the end of the string)
     * @return string with the subsetted string
     */
    CT::string substringr(size_t start,size_t end){CT::string r;r.substringSelf(this,start,end);return r;}
    
    /**
     * Adjusts the size of the string
     */
    void setSize(int size){
      if(size<0){
        copy("",0);
        return;
      }
      if(size<int(privatelength)){
        value[size]='\0';
        privatelength=size;
      }
    }
    
    /**
     * Converts the string to a float number
     */
    float toFloat(){float fValue=(float)atof(value);return fValue;}
    
    /**
     * Converts the string to a double number
     */
    double toDouble(){double fValue=(double)atof(value);return fValue;}

    /**
     * Converts the string to an integer number
     */
    int toInt(){int dValue=(int)atoi(value);return dValue;}
  };

}

#endif
