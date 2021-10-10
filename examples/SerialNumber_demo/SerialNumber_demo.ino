/*
    SerialNumber Demo

    This example demonstrates the use of the SerialNumber library.
    https://github.com/agrommek/SerialNumber

    This example code is in the public domain.

    Andreas Grommek
    2021-10-10
*/

#include <SerialNumber.h>

// helper function for printing
void print_bool(bool b) {
    if (b) Serial.print(F("true"));
    else Serial.print(F("false"));
}

void setup() {
    Serial.begin(115200);

    // create a SerialNumber object based on uint16_t (range 0..65535)
    // and initialize it to 0
    SerialNumber<uint16_t> sn16{0};

    // increment by one, using increment operators
    sn16++; // postfix operator
    ++sn16; // prefix operator - use this one when possible, it is more efficient

    // show the value of the SerialNumber
    Serial.print(F("Value of sn16: "));
    Serial.println(sn16.value(), DEC);

    // test all comparison operators
    // compare a SerialNumber to a plain number
    SerialNumber<uint8_t> sn8{100}; // initialize to 100
    Serial.println(F("Testing all comparison operators..."));
    Serial.print(F("sn8.value(): "));
    Serial.println(sn8.value());

    for (size_t i=0; i<=255; i++) {
        Serial.print(F("sn8 < "));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        print_bool(sn8 < i);
        Serial.print(F(" | "));

        Serial.print(F("sn8 <= "));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        print_bool(sn8 <= i);
        Serial.print(F(" | "));

        Serial.print(F("sn8 == "));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        print_bool(sn8 == i);
        Serial.print(F(" | "));

        Serial.print(F("sn8 != "));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        print_bool(sn8 != i);
        Serial.print(F(" | "));

        Serial.print(F("sn8 >= "));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        print_bool(sn8 >= i);
        Serial.print(F(" | "));

        Serial.print(F("sn8 > "));
        Serial.print(i, DEC);
        Serial.print(F(": "));
        print_bool(sn8 > i);
        Serial.print(F(" | "));

        Serial.println();
    }

    Serial.println(F("Take note of line with i == 228."));
    Serial.println(F("There, i is neither larger, nor smaller nor equal to the value of sn8!"));
    Serial.println(F("Values if i > 228 are considered larger than sn8 (with sn8.value() == 100)."));

    // This will not compile!
    // Comparisons between SerialNumbers of different sizes are not supported!
    // sn8 == sn16;
}

void loop() {
    // nothing to to
}
