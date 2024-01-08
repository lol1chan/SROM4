#include "Header.h"

using namespace std;
using namespace chrono;

std::array<std::array<int, array_size>, array_size> FieldGF::multiplicativeMatrix;

FieldGF::FieldGF() {
    data.fill(0);
}

FieldGF::FieldGF(const std::array<int, array_size>& coefficients) {
    data = coefficients;
}

FieldGF FieldGF::zero() const {
    return FieldGF();
}

void FieldGF::one() { 
    data.fill(1);
}

std::string FieldGF::toString() const {
    std::string result;
    for (int i = array_size - 1; i >= 0; i--) {
        result += std::to_string(data[i]);
    }
    return result;
}

FieldGF FieldGF::fromString(const std::string& str) {
    FieldGF result;
    for (std::size_t i = 0; i < array_size && i < str.length(); ++i) {
        result.data[array_size - i - 1] = str[i] - '0';
    }
    return result;
}

//std::string FieldGF::toString() const {
//    std::string result;
//    for (int i = 0; i < array_size; i++) {
//        result += std::to_string(data[i]);
//    }
//    return result;
//}

//FieldGF FieldGF::fromString(const std::string& str) {
//    FieldGF result;
//    for (std::size_t i = 0; i < array_size && i < str.length(); ++i) {
//        result.data[i] = str[i] - '0';
//    }
//    return result;
//}

std::string generateRandomBinaryString() {
    std::string result;
    for (std::size_t i = 0; i < array_size - 1; ++i) {
        result += std::to_string(rand() % 2);
    }
    return result;
}


void FieldGF::calculateMultiplicativeMatrix() {
    int p = 2 * array_size + 1;
    std::array<int, array_size> array;
    array[0] = 1;

    for (int i = 1; i < array_size; i++) {
        array[i] = (array[i - 1] * 2) % p;
    }

    for (int i = 0; i < array_size; ++i) {
        for (int j = 0; j < array_size; ++j) {
            int two_i = array[i];
            int two_j = array[j];

            if ((two_i + two_j) % p == 1 ||
                (two_i - two_j + p) % p == 1 ||
                ((p - two_i) + two_j) % p == 1 ||
                ((p - two_i) - two_j + p) % p == 1) {
                FieldGF::multiplicativeMatrix[i][j] = 1;
            }
            else {
                FieldGF::multiplicativeMatrix[i][j] = 0;
            }
        }
    }
}


FieldGF FieldGF::operator+(const FieldGF& other) const {
    FieldGF result;
    for (std::size_t i = 0; i < array_size; i++) {
        result.data[i] = (data[i] ^ other.data[i]);
    }
    return result;
}

FieldGF FieldGF::operator<<(int positions) const {
    FieldGF result;
    for (int i = 0; i < array_size; ++i) {
        result.data[i] = data[(i + positions) % array_size];
    }
    return result;
}

FieldGF FieldGF::operator>>(int positions) const {
    FieldGF result;
    for (int i = 0; i < array_size; ++i) {
        result.data[i] = data[(i - positions + array_size) % array_size];
    }
    return result;
}

FieldGF FieldGF::mirror() {
    FieldGF result;
    for (int i = 0; i < array_size; i++) {
        result.data[array_size - 1 - i] = data[i];    
    }

    return result;
}

FieldGF FieldGF::operator*(const FieldGF& other) const {
    FieldGF result;

    FieldGF u = *this;
    FieldGF v = other;
    u = u.mirror();
    v = v.mirror();
    FieldGF temp;


    for (int i = 0; i < array_size; ++i) {


        for (int j = 0; j < array_size; j++) {

            int s = 0;
            for (int k = 0; k < array_size; k++) {
                s += u.data[k] * multiplicativeMatrix[j][k];
            }
            temp.data[j] = s & 1;
        }
        int ss = 0;
        for (int j = 0; j < array_size; j++) {
            int s = 0;
            for (int k = 0; k < array_size; k++) {
                s += temp.data[k] * v.data[k];
            }
            ss += s & 1;
        }



        u = u.shiftLeft();
        v = v.shiftLeft();

        result.data[i] = ss & 1;
    }

    result = result.toSquare();
    return result;
}




FieldGF FieldGF::toSquare() const {
    FieldGF result;
    for (int i = array_size - 1; i > 0; i--) {
        result.data[i - 1] = data[i];
    }
    result.data[array_size - 1] = data[0];
    return result;
}

FieldGF FieldGF::shiftLeft() const {
    FieldGF result;
    for (int i = array_size - 1; i > 0; i--) {
        result.data[i] = data[i - 1];
    }
    result.data[0] = data[array_size - 1];
    return result;
}



FieldGF FieldGF::pow(const FieldGF& power) const {
    FieldGF result;
    result.one();
    FieldGF temp = *this;

    for (int i = 0; i < array_size; i++) {
        if (power.data[i] == 1) {
            result = result * temp;
        }
        temp = temp.toSquare();
    }

    return result;
}




FieldGF FieldGF::inverse() const {
    if (data == std::array<int, array_size>{0}) {
        throw std::runtime_error("inverse for zero?");
    }

    FieldGF result(*this);
    FieldGF temp(*this);

    for (int i = 0; i < array_size - 2; ++i) {
        temp = temp.toSquare();
        result = result * temp;
    }

    result = result.toSquare();

    return result;
}

FieldGF FieldGF::trace() const {
    FieldGF result;
    int temp = 0;
    for (int i = 0; i < array_size; i++) {
        temp ^= data[i];
    }
    result.data[0] = temp;
    return result;
}

void FieldGF::displayMultiplicativeMatrix() {
    std::cout << "Multiplicative Matrix:" << std::endl;

    for (int i = 0; i < array_size; ++i) {
        for (int j = 0; j < array_size; ++j) {
            std::cout << std::setw(2) << multiplicativeMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {

    FieldGF A;
    FieldGF B;
    FieldGF C;

    FieldGF::calculateMultiplicativeMatrix();
    //FieldGF::displayMultiplicativeMatrix();


    A = A.fromString("10011000110100101001101100001100110100000000100110000100010101111110101001000011001100000101011010011110010100010101010010100001110001101100101110000001011111111011101000011111101");
    B = B.fromString("01001011100010111011001000110000011000001101111000010110000000000100111010000111010110011101010000000101000110111001011001000000011001010001001001010101101010000100010001110111011");
    C = C.fromString("11100010010111110011110011011001100000101010110100101011101000001001110000000000000111000000101100011000111000001000110000011100101111111001100010100000010000111001000110110011001");
   


    std::cout << "A: " << A.toString() << std::endl;
    std::cout << "B: " << B.toString() << std::endl;
    std::cout << "C: " << C.toString() << std::endl;

    FieldGF sum = A + B;
    std::cout << "A + B = " << sum.toString() << std::endl;

    FieldGF product = A * B;
    std::cout << "A * B = " << product.toString() << std::endl;

    FieldGF square = A.toSquare();
    std::cout << "A * A = " << square.toString() << std::endl;

    FieldGF power = A.pow(C);
    std::cout << "A^C = " << power.toString() << std::endl;

    FieldGF tr = A.trace();
    std::cout << "Trace(A) " << tr.toString() << std::endl;

    FieldGF inv = A.inverse();
    std::cout << "Inverse(A) " << inv.toString() << std::endl;


    //const int iterations = 100;

    //srand(static_cast<unsigned>(time(nullptr)));


    //double addTime = 0.0;
    //double multiplyTime = 0.0;
    //double powTime = 0.0;
    //double traceTime = 0.0;
    //double inverseTime = 0.0;

    //for (int i = 0; i < iterations; ++i) {

    //    FieldGF a = FieldGF::fromString(generateRandomBinaryString());
    //    FieldGF b = FieldGF::fromString(generateRandomBinaryString());


    //    auto start = std::chrono::high_resolution_clock::now();
    //    FieldGF resultAdd = a + b;
    //    auto end = std::chrono::high_resolution_clock::now();
    //    addTime += std::chrono::duration<double>(end - start).count();


    //    start = std::chrono::high_resolution_clock::now();
    //    FieldGF resultMultiply = a * b;
    //    end = std::chrono::high_resolution_clock::now();
    //    multiplyTime += std::chrono::duration<double>(end - start).count();


    //    start = std::chrono::high_resolution_clock::now();
    //    FieldGF resultPow = a.pow(b);
    //    end = std::chrono::high_resolution_clock::now();
    //    powTime += std::chrono::duration<double>(end - start).count();


    //    start = std::chrono::high_resolution_clock::now();
    //    FieldGF resultTrace = a.trace();
    //    end = std::chrono::high_resolution_clock::now();
    //    traceTime += std::chrono::duration<double>(end - start).count();


    //    start = std::chrono::high_resolution_clock::now();
    //    FieldGF resultInverse = a.inverse();
    //    end = std::chrono::high_resolution_clock::now();
    //    inverseTime += std::chrono::duration<double>(end - start).count();
    //}

    //double avgAddTime = addTime / iterations;
    //double avgMultiplyTime = multiplyTime / iterations;
    //double avgPowTime = powTime / iterations;
    //double avgTraceTime = traceTime / iterations;
    //double avgInverseTime = inverseTime / iterations;

    //std::cout << "Average Time for Addition: " << avgAddTime << " seconds\n";
    //std::cout << "Average Time for Multiplication: " << avgMultiplyTime << " seconds\n";
    //std::cout << "Average Time for Power: " << avgPowTime << " seconds\n";
    //std::cout << "Average Time for Trace: " << avgTraceTime << " seconds\n";
    //std::cout << "Average Time for Inverse: " << avgInverseTime << " seconds\n";



    return 0;
}