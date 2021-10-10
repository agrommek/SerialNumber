/**
 @file    SerialNumber.h
 @brief   Header file for SerialNumber class (RFC1982)
 @author  Andreas Grommek
 @version 1.1.0
 @date    2021-10-10
 @section license_serialnumber_h License
  
 The MIT Licence (MIT)
 
 Copyright (c) 2021 Andreas Grommek
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

/**
 @mainpage Class for easy and safe comparison of and limited arithmetic on serial numbers
 
 @section intro_sec Introduction

 This library provides a template class to implement SerialNumbers as
 described in RFC1982 (<https://datatracker.ietf.org/doc/html/rfc1982>).
 
 SerialNumbers consist of unsigned integers. The special thing is their
 behavior with comparison. 

 Comparisons are only defined and allowed between two objects of type 
 SerialNumber, or between a SerialNumber object and a numberic value. 

 SerialNumbers (or a SerialNumber object an a plain number) compare equal
 if and only if their values (seen as unsigned integer) are equal.
 Otherwise they are considered unequal. Operators "lower than", "greater
 than", "equal or lower than" and "equal or greater than" are defined
 according to RFC1982. This means a number which has "just wrapped around" 
 and is mathematically smaller than reference number might, in fact, be
 considered the larger serial number.

 s1 is said to be less than s2 if, and only if, s1 is not equal to s2,
 and

 @verbatim
 (i1 < i2 and i2 - i1 < 2^(SERIAL_BITS - 1)) or
 (i1 > i2 and i1 - i2 > 2^(SERIAL_BITS - 1))
 @endverbatim

 s1 is said to be greater than s2 if, and only if, s1 is not equal to
 s2, and

 @verbatim
 (i1 < i2 and i2 - i1 > 2^(SERIAL_BITS - 1)) or
 (i1 > i2 and i1 - i2 < 2^(SERIAL_BITS - 1))
 @endverbatim

 Note that there are some pairs of values s1 and s2 for which s1 is
 not equal to s2, but for which s1 is neither greater than, nor less
 than, s2.  An attempt to use these ordering operators on such pairs
 of values produces an undefined result. This is the case for all 
 SerialNumbers with a "distance" of exactly 2^(SERIAL_BITS-1). The 
 standard leaves the decision how to handle this case to the implementer.
 In this library all comparisons between SerialNumbers (or a SerialNumber
 an a plain integer) with this "distance" will return false except, of
 course, the "is unequal" operator.

 According to RFC1982, addition of positive numbers to SerialNumbers up 
 to a size of (2^(SERIAL_BITS - 1) - 1) (e.g. 127 for uint8_t) would be
 ok and defined. However, this class does not implement this - there is no
 good way to signal too-large addends. If you ever need to add some number
 to a SerialNumber, use assignment, like this:

 @verbatim
 SerialNumber<uint16_t> s{0};  // create and initialize
 s = s.value() + 23;           // add 23 to SerialNumber s
 @endverbatim

 However, you have to make sure for yourself that you do not add more
 than allowed.

 Prefix and postfix increment operators (<tt>++x</tt> and <tt>x++</tt>, 
 respectively) are implemented and behave as usual.

 @b All other arithmetic operations are left undefined and thus not
 implemented in this library. This is mandated by the standard and a 
 design descision!

 @section note_on_data_types A note on data types for comparisons
 
 When comparing two SerialNumber objects, the sizes of the underlying
 date types  @b must be identical. Otherwise the compiler generates an error.

 When comparing a SerialNumber object to a plain number, the number is 
 first cast to the underlying data type of the SerialNumber object. This
 can result in some non-intuitive results, e.g.
 
 @verbatim
 SerialNumber<uint8_t> sn{10};
 sn == 10 + 256; // returns true, surprisingly:
                 // static_cast<uint8_t>(266) == 10 
 @endverbatim
 
 It is even possible to compare SerialNumber objects to negative numbers
 or floating point numbers. This is not really useful, but at least
 generates no compiler error. However, comparing to negative floating
 point numbers does generate a compiler error due to the 'only one 
 implicit conversion' rule.
 To be on the safe side (and prevent yourself shooting in  the foot), 
 either only use numbers of the correct data type for comparons to 
 SerialNumber object or use an explicit cast.
 
 @verbatim
 SerialNumber<uint16_t> sn16{65000};
 uint16_t c16 = 17000;
 uint32_t c32 = 17000;
 
 sn16 == c16;                        // good, same data type (uint16_t)
 sn16 == static_cast<uint16_t>(c32); // good, make your intent explicit
 sn16 == c32;                        // works, but implicit cast might 
                                     // result in hard to debug bugs
 @endverbatim
 
 @section examples Examples

 @subsection example0 A trivial example

 Set two SerialNumbers, s1 and s2, to have the same value:

 @verbatim
 SerialNumber<uint8_t> s1{50}; // using uint8_t --> range 0..255
 SerialNumber<uint8_t> s2{50};
 @endverbatim
 
 Results:
 
 @verbatim
 s1 == s2 --> true
 
 s1 != s2 --> false

 s1 <  s2 --> false
 s1 <= s2 --> true
 s1 >  s2 --> false
 s1 >= s2 --> true

 s2 <  s1 --> false
 s2 <= s1 --> true
 s2 >  s1 --> false
 s2 >= s1 --> true
 @endverbatim
 
 No surprises here.
 
 @subsection example1 A non-suprising example
 
 Set two SerialNumbers, s1 and s2, so than <tt>s1.value()== 10</tt> and 
 <tt>s2.value() == 30</tt>:

 @verbatim
 SerialNumber<uint8_t> s1{10}; // using uint8_t --> range 0..255
 SerialNumber<uint8_t> s2{30};
 @endverbatim

 The absolute difference of values beween s1 and s2 is 30, which is less than 
 <tt>2^(8-1) = 128</tt> (the critical distance for uint8_t). Results:

 @verbatim
 s1 == s2 --> false
 
 s1 != s2 --> true

 s1 <  s2 --> true
 s1 <= s2 --> true
 s1 >  s2 --> false
 s1 >= s2 --> false

 s2 <  s1 --> false
 s2 <= s1 --> false
 s2 >  s1 --> true
 s2 >= s1 --> true
 @endverbatim
 
 No surprises here.
 
 @subsection example2 A slightly surpirsing example

 Set two SerialNumbers, s1 and s2, so than <tt>s1.value()== 10</tt> and 
 <tt>s2.value() == 250</tt>:

 @verbatim
 SerialNumber<uint8_t> s1{10};  // using uint8_t --> range 0..255
 SerialNumber<uint8_t> s2{250};
 @endverbatim

 The difference of values beween s1 and s2 is 240, which is greater than 
 <tt>2^(8-1) = 128</tt> (the critical distance for uint8_t). The "distance"
 of counting 240 upwards, wrapping around after 255 and go on counting to
 the value of 10 is shorter, i.e. 26 (16 from 240 to 256==0, 10 from 0
 to 10). This is why s2 is actually considered @b smaller than s1!
 Results:

 @verbatim
 s1 == s2 --> false
 
 s1 != s2 --> true

 s1 <  s2 --> false
 s1 <= s2 --> false
 s1 >  s2 --> true
 s1 >= s2 --> true

 s2 <  s1 --> true
 s2 <= s1 --> true
 s2 >  s1 --> false
 s2 >= s1 --> false
 @endverbatim

 See https://datatracker.ietf.org/doc/html/rfc1982 for details.

 @subsection example_distance Example with critical distance
 
 Set two SerialNumbers, s1 and s2, to have the ciritcal distance:

 @verbatim
 SerialNumber<uint8_t> s1{10}; // using uint8_t --> range 0..255
 SerialNumber<uint8_t> s2{138};
 @endverbatim

 The difference of values beween s1 and s2 is 138 - 10 = 128 = 2^(8-1).
 Thus:
 @verbatim
 s1 == s2 --> false
 
 s1 != s2 --> true

 s1 <  s2 --> false
 s1 <= s2 --> false
 s1 >  s2 --> false
 s1 >= s2 --> false

 s2 <  s1 --> false
 s2 <= s1 --> false
 s2 >  s1 --> false
 s2 >= s1 --> false
 @endverbatim
 
 In other words: When "(s1 < s2) == false" and "(s1 > s2) == false", this
 does @b not necessarily imply that "(s1 == s2) == true".

 @section compatibilty Compatibility
 
 Although written originally for the Arduino platform, there is nothing
 which prevents the library from being used on any other platform.
 The code is pure C++. Feel free to adapt to your needs.

 As of now, there are only specialized definitions of the constructor for
 uint8_t, uint16_t, uint32_t, uint64_t, and uint128_t data types.   
 This has the following consequences:

   - It is now impossibe to instantiate the template class with other data types!
   - Use of operators is also limited to these "allowed" classes, even if 
     instantiation of the comparison operators is in no way restricted.
   - No explicit instantiations in separate .cpp file is necessary to restict
     use of SerialNumbers to unsigned integers.

 Note that data types uintX_t are optional in C++. To prevent compatibility
 issues, the template specializations are wrapped in <tt>#ifdefs</tt>. 
 The macro constants <tt>UINTx_MAX</tt> are guaranteed to be defined if 
 and only of the type exists. If you should ever need SerialNumbers with
 another data type, define an additional specialization for the 
 constructor, like so:

 @verbatim 
 #include <SerialNumber.h>
 template<> SerialNumber<unsigned int>::SerialNumber(unsigned int sn) : n{sn} {}
 @endverbatim
 
 @section author Author

 Andreas Grommek

 @section license License
 
 The MIT Licence (MIT)
 
 Copyright (c) 2021 Andreas Grommek

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 @version  1.1.0
 @date     2021-10-10
*/

#ifndef SerialNumber_h
#define SerialNumber_h

/* Declaration of the SerialName class template */

template <class T>
class SerialNumber {
    public:
        // constructor
        SerialNumber(T sn=T(0)); ///< constructor
        
        // copy constructor
        SerialNumber(const SerialNumber& sn) = default; ///< Copy constructor
        
        // getter method
        T value(void) const;

        // assignment operator for plain numbers
        SerialNumber& operator= (T sn);
        
        // assignment operator for SerialNumbers
        SerialNumber& operator= (const SerialNumber<T>& sn);

        // prefix increment operator (no parameters)
        SerialNumber& operator++ ();

        // postfix increment operator (one int parameter)
        SerialNumber operator++ (int);

    private:
        T n;
};

#include "SerialNumberClass.hpp"

/*
There are three overloads for each comparison operator:

    bool operatorX (SerialNumber<T>, SerialNumber<T>)
    bool operatorX (T              , SerialNumber<T>)
    bool operatorX (SerialNumber<T>, T)

Comparing two SerialNumber objects with different data types is not supported.
*/

/* Declaration of comparison operators for SerialNumber objects */

// equality operator
template <class T, class U>
bool operator== (const SerialNumber<T>& sn1, const SerialNumber<T>& sn2);

template <class T, class U>
bool operator== (const SerialNumber<T>& sn1, U sn2);

template <class T, class U>
bool operator== (T sn1, const SerialNumber<U>& sn2);

// inequality operator
template <class T, class U>
bool operator!= (const SerialNumber<T>& sn1, const SerialNumber<T>& sn2);

template <class T, class U>
bool operator!= (const SerialNumber<T>& sn1, U sn2);

template <class T, class U> 
bool operator!= (T sn1, const SerialNumber<U>& sn2);

// lower-than operator
template <class T, class U>
bool operator< (const SerialNumber<T>& sn1, const SerialNumber<T>& sn2);

template <class T, class U>
bool operator< (const SerialNumber<T>& sn1, U sn2);

template <class T, class U>
bool operator< (T sn1, const SerialNumber<U>& sn2);

// greater-than operator
template <class T, class U>
bool operator> (const SerialNumber<T>& sn1, const SerialNumber<T>& sn2);

template <class T, class U>
bool operator> (const SerialNumber<T>& sn1, U sn2);

template <class T, class U>
bool operator> (T sn1, const SerialNumber<U>& sn2);

// lower-or-equal operator
template <class T, class U>
bool operator<= (const SerialNumber<T>& sn1, const SerialNumber<T>& sn2);

template <class T, class U>
bool operator<= (const SerialNumber<T>& sn1, U sn2);

template <class T, class U>
bool operator<= (T sn1, const SerialNumber<U>& sn2);

// greater-or-equal operator
template <class T, class U>
bool operator>= (const SerialNumber<T>& sn1, const SerialNumber<T>& sn2);

template <class T, class U>
bool operator>= (const SerialNumber<T>& sn1, U sn2);

template <class T, class U>
bool operator>= (T sn1, const SerialNumber<U>& sn2);

#include "SerialNumberOperators.hpp"

#endif // SerialNumber_h
