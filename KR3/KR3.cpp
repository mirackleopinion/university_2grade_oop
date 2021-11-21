#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

// variant 6 (One man'sj unk is another man's treasure)
const long n = 308737337;
const long e = 17657;
const std::vector<long> coded = {
     279604812, 23295830, 153283840, 227200722, 44931545, 105047949, 23295830, 306850011, 197827502, 152539466,
     227200722, 69010571, 23295830, 211670599, 227200722, 5595583, 197827502, 227200722, 105047949, 23295830,
     3071939, 18553803, 19316748, 153283840, 184458268, 227200722, 44931545, 105047949, 23295830, 306850011,
     197827502, 227200722, 18553803, 184458268, 153283840, 105047949, 197827502, 69010571, 184458268, 153283840
};


long long modPow(long base, long exponent, long modulus) {
    if (exponent == 0)
        return 1;

    if (exponent == 1)
        return base;

    if (exponent % 2 == 1) // odd exponent
        return (base * modPow(base, exponent - 1, modulus)) % modulus;

    long long halfexp = modPow(base, exponent / 2, modulus);  // even exponent
    return (halfexp * halfexp) % modulus;
};

int main() {
    std::unordered_map<long, char> coded_to_ascii;

    for (long i = 0; i < 255; i++)
        coded_to_ascii[modPow(i, e, n)] = (char)i;

    for (long letter : coded)
        std::cout << coded_to_ascii[letter];
    std::cout << "\n";
}
