
# Safe and easy comparison of and limited arithmetic on serial numbers

## Introduction

This library provides a template class to implement SerialNumbers as described in [RFC1982](<https://datatracker.ietf.org/doc/html/rfc1982>). SerialNumbers consist of unsigned integers. The special thing is their behavior with comparison.

Comparisons are only defined and allowed between two objects of type SerialNumber, or between a SerialNumber object and a numeric value.

SerialNumbers (or a SerialNumber object an a plain number) compare equal if and only if their values (seen as unsigned integer) are equal. Otherwise they are considered unequal. Operators "lower than", "greater than", "equal or lower than" and "equal or greater than" are defined according to RFC1982. This means a number which has "just wrapped around" and is mathematically smaller than the reference number might, in fact, be considered the larger serial number.

s1 is said to be less than s2 if, and only if, s1 is not equal to s2, and

    (i1 < i2 and i2 - i1 < 2^(SERIAL_BITS - 1)) or
    (i1 > i2 and i1 - i2 > 2^(SERIAL_BITS - 1))

s1 is said to be greater than s2 if, and only if, s1 is not equal to
s2, and


    (i1 < i2 and i2 - i1 > 2^(SERIAL_BITS - 1)) or
    (i1 > i2 and i1 - i2 < 2^(SERIAL_BITS - 1))

Note that there are some pairs of values s1 and s2 for which s1 is not equal to s2, but for which s1 is neither greater than, nor less than, s2.  An attempt to use these ordering operators on such pairs of values produces an undefined result. This is the case for all SerialNumbers with a "distance" of exactly `2^(SERIAL_BITS-1)`. The standard leaves the decision how to handle this case to the implementer. In this library all comparisons between SerialNumbers (or a SerialNumber an a plain integer) with this "distance" will return false except, of course, the "is unequal" operator.

According to RFC1982, addition of positive numbers to SerialNumbers up to a size of `(2^(SERIAL_BITS - 1) - 1)` (e.g. 127 for uint8_t) would be ok and defined. However, this is not implemented here - there is no good way to signal too-large addends. If you ever need to add some number to a SerialNumber, use assignment, like this:

    SerialNumber<uint16_t> s{0};  // create and initialize
    s = s.value() + 23;           // add 23 to SerialNumber s

However, you have to make sure for yourself that you do not add more than allowed.

Prefix and postfix increment operators (`++x`> and `x++`, respectively) are implemented and behave as usual. *All* other arithmetic operations are left undefined and thus not implemented in this library. This is mandated by the standard and a design descision!

## A note on data types for comparisons

When comparing two SerialNumber objects, the sizes of the underlying date types  @b must be identical. Otherwise the compiler generates an error.

When comparing a SerialNumber object to a plain number, the number is first cast to the underlying data type of the SerialNumber object. This can result in some non-intuitive results, e.g.

    SerialNumber<uint8_t> sn{10}; // SerialNumber based on uint8_t
    sn == 10 + 256;               // surprisingly returns true as static_cast<uint8_t>(266) == 10

It is even possible to compare SerialNumber objects to negative numbers or floating point numbers. This is not really useful, but at least generates no compiler error. However, comparing to negative floating point numbers does generate a compiler error due to the 'only one implicit conversion' rule. To be on the safe side (and prevent yourself shooting in  the foot), either only use numbers of the correct data type for comparons to SerialNumber object or use an explicit cast.

    SerialNumber<uint16_t> sn16{65000};
    uint16_t c16 = 17000;
    uint32_t c32 = 17000;

    sn16 == c16;                        // good, same data type (uint16_t)
    sn16 == static_cast<uint16_t>(c32); // good, make your intent explicit
    sn16 == c32;                        // works, but implicit cast might result in hard to debug bugs

## Examples

### A trivial example

Set two SerialNumbers, s1 and s2, to have the same value:

    SerialNumber<uint8_t> s1{50}; // using uint8_t --> range 0..255
    SerialNumber<uint8_t> s2{50};

Results:

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

No surprises here.

### A non-suprising example

Set two SerialNumbers, s1 and s2, so than <tt>s1.value()== 10</tt> and `s2.value() == 30`:

    SerialNumber<uint8_t> s1{10}; // using uint8_t --> range 0..255
    SerialNumber<uint8_t> s2{30};

The absolute difference of values beween s1 and s2 is 30, which is less than `2^(8-1) = 128` (the critical distance for uint8_t). Results:

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

No surprises here.

### A slightly surpirsing example

Set two SerialNumbers, s1 and s2, so than <tt>s1.value()== 10</tt> and `s2.value() == 250`:

    SerialNumber<uint8_t> s1{10};  // using uint8_t --> range 0..255
    SerialNumber<uint8_t> s2{250};

The difference of values beween s1 and s2 is 240, which is greater than `2^(8-1) = 128` (the critical distance for uint8_t). The "distance" of counting 240 upwards, wrapping around after 255 and go on counting to the value of 10 is shorter, i.e. 26 (16 from 240 to 256==0, 10 from 0 to 10). This is why s2 is actually considered *smaller* than s1! Results:

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

See [RFC1982](https://datatracker.ietf.org/doc/html/rfc1982) for details.

### Example with critical distance
 
Set two SerialNumbers, s1 and s2, to have the ciritcal distance:


    SerialNumber<uint8_t> s1{10}; // using uint8_t --> range 0..255
    SerialNumber<uint8_t> s2{138};

The difference of values beween s1 and s2 is 138 - 10 = 128 = 2^(8-1). Thus:

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

In other words: When `(s1 < s2) == false` and `(s1 > s2) == false`, this does  *not* necessarily imply that `(s1 == s2) == true`.

## Compatibility

Although written originally for the Arduino platform, there is nothing which prevents the library from being used on any other platform. The code is pure C++. Feel free to adapt to your needs.

As of now, there are only specialized definitions of the constructor for `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, and `uint128_t` data types. This has the following consequences:

 - It is now impossibe to instantiate the template class with other data types!
 - Use of operators is also limited to these "allowed" classes, even if instantiation of the comparison operators is in no way restricted.
 - No explicit instantiations in separate .cpp file is necessary to restict use of SerialNumbers to unsigned integers.

Note that data types `uintX_t` are optional in C++. To prevent compatibility issues, the template specializations are wrapped in `#ifdef`s. The macro constants `UINTx_MAX` are guaranteed to be defined if and only of the type exists. If you should ever need SerialNumbers with another data type, define an additional specialization for the constructor, like so:

    #include <SerialNumber.h>
    template<> SerialNumber<unsigned int>::SerialNumber(unsigned int sn) : n{sn} {}
