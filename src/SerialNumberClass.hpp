/**
 @file    SerialNumberClass.hpp
 @brief   Implementation file for SerialNumber class (RFC1982)
 @author  Andreas Grommek
 @version 1.1.0
 @date    2021-10-10
 @section license_serialnumber_class_hpp License
  
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

/*
Define specializations for constructor, but DO NOT provide a general definition.

 --> It is now impossibe to instantiate the template class with other data types!
 --> Use of operators is also limited to these "allowed" classes, even if 
     instantiation of the comparison operators is in no way restricted.
 --> No explicit instantiations in separate .cpp file is necessary to restict
     use of SerialNumbers to unsigned integers.
     
Only define specializations for unsigned integers. The comparisons will not 
work correctly for signed integers as of now.
 
Note that data types uintX_t are optional in C++. To prevent compatibility
issues, the template specializations are wrapped in #ifdefs. The macro constants 
UINTx_MAX are guaranteed to be defined if an only of the type exists.

If you should ever need SerialNumbers with another data type, define another
specialization for the constructor, like so:
 
 #include <SerialNumber.h>
 template<> SerialNumber<unsigned int>::SerialNumber(unsigned int sn) : n{sn} {}
*/
#ifdef UINT8_MAX
template<>
SerialNumber<uint8_t>::SerialNumber(uint8_t sn) : n{sn} {}
#endif

#ifdef UINT16_MAX
template<>
SerialNumber<uint16_t>::SerialNumber(uint16_t sn) : n{sn} {}
#endif

#ifdef UINT32_MAX
template<>
SerialNumber<uint32_t>::SerialNumber(uint32_t sn) : n{sn} {}
#endif

#ifdef UINT64_MAX
template<>
SerialNumber<uint64_t>::SerialNumber(uint64_t sn) : n{sn} {}
#endif

#ifdef UINT128_MAX
template<>
SerialNumber<uint128_t>::SerialNumber(uint128_t sn) : n{sn} {}
#endif

/**
 * @brief  Getter function for the stored SerialNumber
 * @return The stored serial number
 * @note   We could have defined a typecast operator. However, this
 *         leads to problems for the compiler to figure out which
 *         overload to use in which situation.
 */
template <class T>
T SerialNumber<T>::value(void) const {
    return n;
}

/**
 * @brief  Assignment operator for plain numbers
 */
template <class T>
SerialNumber<T>& SerialNumber<T>::operator= (T sn) {
    n = sn;
    return *this;
}

/**
 * @brief  Assignment operator for SerialNumbers
 */
template <class T>
SerialNumber<T>& SerialNumber<T>::operator= (const SerialNumber<T>& sn) {
    n = sn.n;
    return *this;
}

/**
 * @brief  Prefix increment operator
 */
template <class T>
SerialNumber<T>& SerialNumber<T>::operator++ () {
    ++n;
    return *this;
}

/**
 * @brief  Postfix increment operator
 */
template <class T>
SerialNumber<T> SerialNumber<T>::operator++ (int) {
    SerialNumber temp{n};
    ++n;
    return temp;
}
