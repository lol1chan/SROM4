#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <chrono>
#include <iomanip>

#define array_size 179

class FieldGF {
private:
    static std::array<std::array<int, array_size>, array_size> multiplicativeMatrix;
    std::array<int, array_size> data;

public:
    FieldGF();
    FieldGF(const std::array<int, array_size>& coefficients);

    FieldGF operator+(const FieldGF& other) const;
    FieldGF operator*(const FieldGF& other) const;
    FieldGF toSquare() const;
    FieldGF pow(const FieldGF& power) const;
    FieldGF inverse() const;
    FieldGF shiftLeft() const;
    FieldGF trace() const;
    FieldGF mirror();
    FieldGF operator<<(int positions) const;
    FieldGF operator>>(int positions) const;

    FieldGF zero() const;

    void one();

    std::string toString() const;
    static FieldGF fromString(const std::string& str);

    static void calculateMultiplicativeMatrix();
    static void displayMultiplicativeMatrix();
};