#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class BigNumber {
public:
    BigNumber(std::string s) {

        this->sign = +1;
        this->data.resize(s.length(), 0);
        int numbert_starts_at = 0;

        if (s[0] == '-') {
            this->sign = -1;
            numbert_starts_at = 1;
        }

        if (s[0] == '+') {
            numbert_starts_at = 1;
        }

        if (numbert_starts_at == s.length()) {
            std::cout << s << " is not number!\n";
            std::exit(1);
        }

        for (size_t i = numbert_starts_at; i < s.length(); ++i) {
            size_t n = cifers.find_first_of(s[i]);

            if (n == std::string::npos) {
                std::cout << "Not a number: '" << s[i] << "' in '" << s << "' at position " << i << "!\n";
                std::exit(1);
            }
            else {
                this->data[s.length() - 1 - i] = int(n);
            };
        }

        remove_zeros_from_start();
    };

    BigNumber() {
        this->data.clear();
        this->data.push_back(0);
        this->sign = +1;
    }

    BigNumber operator *(int x) {
        BigNumber output;
        output.data.clear();
        int carry = 0;

        int other_sign = +1;
        if (x < 0)
            other_sign = -1;

        x = abs(x);


        for (auto it = this->data.begin(); it != this->data.end(); ++it) {
            int result = *it * x + carry;
            carry = result / 10;
            result = result % 10;
            output.data.push_back(result);
        }

        if (carry != 0)
            output.data.push_back(carry);


        output.sign = this->sign * other_sign;
        output.remove_zeros_from_start();

        return output;
    };

    int operator [](size_t pos) const {
        if (pos < this->data.size())
            return this->data[pos];
        return 0;
    }

    friend BigNumber abs(const BigNumber& x) {
        BigNumber output;
        output.sign = +1;
        output.data = x.data;
        output.remove_zeros_from_start();
        return output;
    }

    friend BigNumber operator -(const BigNumber lhs, const BigNumber& rhs) {
        if (rhs.sign < 0)
            return lhs + -(rhs);

        BigNumber lhs_abs = abs(lhs);
        BigNumber rhs_abs = abs(rhs);

        if (rhs_abs > lhs_abs)
            return -(rhs - lhs);

        if (lhs.sign < 0)
            return -(lhs_abs + rhs);

        BigNumber output;
        int carry = 0;
        size_t max_size = std::max(lhs.data.size(), rhs.data.size()) + 1;

        output.data.resize(max_size, 0);

        for (size_t i = 0; i < max_size; i++) {
            int result = lhs[i] - rhs[i] - carry;
            if (result < 0) {
                result += 10;
                carry = 1;
            }
            else
                carry = 0;

            output.data[i] = result;
        }

        output.remove_zeros_from_start();

        return output;
    }

    friend BigNumber operator +(const BigNumber lhs, const BigNumber& rhs) {
        if (lhs.sign > 0 and rhs.sign > 0) { // 1 + 10 = 11; 10 + 1 = 11
            BigNumber output;
            int carry = 0;
            size_t max_size = std::max(lhs.data.size(), rhs.data.size()) + 1;

            output.data.resize(max_size, 0);

            for (size_t i = 0; i < max_size; i++) {
                int result = lhs[i] + rhs[i] + carry;
                carry = result / 10;
                result = result % 10;

                output.data[i] = result;
            }

            output.remove_zeros_from_start();
            return output;
        }

        BigNumber lhs_abs = abs(lhs);
        BigNumber rhs_abs = abs(rhs);

        if (lhs.sign < 0 and rhs.sign < 0) // -1 + (-10) = -11 ; -10 + (-1) = -11
            return -(lhs_abs + rhs_abs);

        if (lhs_abs >= rhs_abs) { // 10 + (-1) ; -10 + 1
            if (lhs.sign > 0)
                return lhs - rhs_abs; // 10 + (-1) = 10 - |-1| = 10 - 1 = 9
            else
                return -(lhs_abs - rhs); // -10 + 1 = -(|-10| - 1) = -(10 - 1) = -9
        }
        else { // -1 + 10; 1 + (-10)
            if (lhs.sign > 0)
                return -(abs(rhs) - lhs); //1 + (-10) = -10 + 1 = -(|-10| - 1) = -9
            else
                return rhs - lhs_abs; // -1 + 10 = 10 - |-1| = 10 - 1 = 9
        }
    }

    std::string to_string() const {
        std::string result;

        if (this->sign < 0)
            result.push_back('-');

        if (this->data.size())
            for (auto it = this->data.rbegin(); it != this->data.rend(); ++it)
                result.push_back(cifers[*it]);
        else
            result = "0";

        return result;
    };

    BigNumber operator()(size_t start, size_t end) {
        BigNumber output;
        size_t len = start - end + 1;
        output.data.resize(len, 0);

        for (int i = 0; i < len; i++) {
            output.data[i] = this->data[end + i];
        }

        return output;
    }

    BigNumber operator <<(size_t x) {
        BigNumber output;
        output.data.resize(this->data.size() + x, 0);

        for (size_t i = 0; i < this->data.size(); i++) {
            output.data[i + x] = this->data[i];
        }

        return output;
    }

    const BigNumber operator +() const {
        BigNumber output;
        output.sign = this->sign;
        output.data = this->data;
        output.remove_zeros_from_start();
        return output;
    }

    const BigNumber operator -() const {
        BigNumber output;
        output.sign = -this->sign;
        output.data = this->data;
        output.remove_zeros_from_start();
        return output;
    }

    friend inline bool operator ==(const BigNumber& lhs, const BigNumber& rhs) {
        return lhs.to_string() == rhs.to_string();
    }

    friend inline bool operator !=(const BigNumber& lhs, const BigNumber& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const BigNumber& l, const BigNumber& r) {
        if (l.sign < 0 and r.sign > 0)
            return true;

        if (l.sign > 0 and r.sign < 0)
            return false;

        if (l.sign < 0 and r.sign < 0)
            return -r < -l;

        if (l.data.size() < r.data.size())
            return true;

        if (l.data.size() > r.data.size())
            return false;

        size_t len = l.data.size();
        for (size_t i = 0; i < len; i++) {
            size_t pos = len - i - 1;
            if (l.data[pos] != r.data[pos])
                return l.data[pos] < r.data[pos];
        }

        return false;
    }

    friend inline bool operator > (const BigNumber& lhs, const BigNumber& rhs) { return rhs < lhs; }
    friend inline bool operator <=(const BigNumber& lhs, const BigNumber& rhs) { return !(lhs > rhs); }
    friend inline bool operator >=(const BigNumber& lhs, const BigNumber& rhs) { return !(lhs < rhs); }

    BigNumber mul_karatsuba(BigNumber& other) {
        // https://en.wikipedia.org/wiki/Karatsuba_algorithm
        if (this->sign < 0 or other.sign < 0) {
            std::cout << "mul_karatsuba() works only with positive numbers!\n";
            std::exit(1);
        }

        auto my_size = this->data.size();

        if (my_size < 2) {
            return other * this->data[0];
        }

        auto other_size = other.data.size();

        if (other_size < 2) {
            return *this * other.data[0];
        }

        size_t splitter = std::min(my_size, other_size) / 2;

        BigNumber high1 = (*this)(my_size - 1, splitter);
        BigNumber low1 = (*this)(splitter - 1, 0);

        BigNumber high2 = other(other_size - 1, splitter);
        BigNumber low2 = other(splitter - 1, 0);

        BigNumber z0 = low1.mul_karatsuba(low2);

        BigNumber z1_1 = low1 + high1;
        BigNumber z1_2 = low2 + high2;
        BigNumber z1 = z1_1.mul_karatsuba(z1_2);

        BigNumber z2 = high1.mul_karatsuba(high2);

        return (z2 << (splitter * 2)) + z0 + (z1 << splitter) - ((z2 + z0) << splitter);
    }

    BigNumber mul_schonhage(BigNumber& other) {
        BigNumber result;
        result.data.resize(this->data.size() + other.data.size(), 0);

        for (size_t i = 0; i < this->data.size(); i++)
            for (size_t j = 0; j < other.data.size(); j++)
                result.data[i + j] += this->data[i] * other.data[j];
        int overflow = 0;
        for (size_t i = 0; i < result.data.size(); i++) {
            result.data[i] += overflow;
            overflow = result.data[i] / 10;
            result.data[i] %= 10;
        }
        result.remove_zeros_from_start();
        return result;
    }

private:
    void remove_zeros_from_start() {
        while (this->data[this->data.size() - 1] == 0 and this->data.size() > 1)
            this->data.pop_back();

        if (this->data.size() == 1 and this->data[0] == 0)
            this->sign = +1;
    };

    int sign;
    std::vector<int> data;
    const std::string cifers = "0123456789";
};


int main()
{

    // test ==, !=
    BigNumber a("-1");
    BigNumber b("-1");
    std::cout << "a != b:" << (a != b) << " \n";
    std::cout << "a == b:" << (a == b) << " \n";

    // test BigNumber * int
    BigNumber c("-5");
    BigNumber d("-2");
    BigNumber e("0");
    BigNumber f("-0");
    BigNumber g("2");
    BigNumber h("1");
    BigNumber i("5432");
    std::cout << "a: " << a.to_string() << " \n";
    std::cout << "b: " << b.to_string() << " \n";
    std::cout << "c: " << c.to_string() << " \n";
    std::cout << "d: " << d.to_string() << " \n";
    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "f: " << f.to_string() << " \n";
    std::cout << "g: " << g.to_string() << " \n";
    std::cout << "h: " << h.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "i(3, 2) .. i(1,0): " << i(3, 2).to_string() << " .. " << i(1, 0).to_string() << " \n";

    std::cout << "a*2: " << (a * 2).to_string() << " \n";
    std::cout << "b*2: " << (b * 2).to_string() << " \n";
    std::cout << "c*2: " << (c * 2).to_string() << " \n";
    std::cout << "d*2: " << (d * 2).to_string() << " \n";
    std::cout << "e*2: " << (e * 2).to_string() << " \n";
    std::cout << "f*2: " << (f * 2).to_string() << " \n";
    std::cout << "g*2: " << (g * 2).to_string() << " \n";

    std::cout << "a*0: " << (a * 0).to_string() << " \n";
    std::cout << "b*0: " << (b * 0).to_string() << " \n";
    std::cout << "c*0: " << (c * 0).to_string() << " \n";
    std::cout << "d*0: " << (d * 0).to_string() << " \n";
    std::cout << "e*0: " << (e * 0).to_string() << " \n";
    std::cout << "f*0: " << (f * 0).to_string() << " \n";
    std::cout << "g*0: " << (g * 0).to_string() << " \n";

    std::cout << "a*-2: " << (a * -2).to_string() << " \n";
    std::cout << "b*-2: " << (b * -2).to_string() << " \n";
    std::cout << "c*-2: " << (c * -2).to_string() << " \n";
    std::cout << "d*-2: " << (d * -2).to_string() << " \n";
    std::cout << "e*-2: " << (e * -2).to_string() << " \n";
    std::cout << "f*-2: " << (f * -2).to_string() << " \n";
    std::cout << "g*-2: " << (g * -2).to_string() << " \n";

    // test unary+
    std::cout << "+a: " << (+a).to_string() << " \n";
    std::cout << "+b: " << (+b).to_string() << " \n";
    std::cout << "+c: " << (+c).to_string() << " \n";
    std::cout << "+d: " << (+d).to_string() << " \n";
    std::cout << "+e: " << (+e).to_string() << " \n";
    std::cout << "+f: " << (+f).to_string() << " \n";
    std::cout << "+g: " << (+g).to_string() << " \n";
    std::cout << "+h: " << (+h).to_string() << " \n";
    std::cout << "+i: " << (+i).to_string() << " \n";

    // test unary-
    std::cout << "-a: " << (-a).to_string() << " \n";
    std::cout << "-b: " << (-b).to_string() << " \n";
    std::cout << "-c: " << (-c).to_string() << " \n";
    std::cout << "-d: " << (-d).to_string() << " \n";
    std::cout << "-e: " << (-e).to_string() << " \n";
    std::cout << "-f: " << (-f).to_string() << " \n";
    std::cout << "-g: " << (-g).to_string() << " \n";
    std::cout << "-h: " << (-h).to_string() << " \n";
    std::cout << "-i: " << (-i).to_string() << " \n";

    //test <
    std::cout << "a < i: " << (a < i) << " \n";
    std::cout << "i < a: " << (i < a) << " \n";
    std::cout << "i < i: " << (i < i) << " \n";
    std::cout << "a < a: " << (a < a) << " \n";


    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "g: " << g.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "e < g: " << (e < g) << " \n";
    std::cout << "e < i: " << (e < i) << " \n";
    std::cout << "g < e: " << (g < e) << " \n";
    std::cout << "g < i: " << (g < i) << " \n";
    std::cout << "i < e: " << (i < e) << " \n";
    std::cout << "i < g: " << (i < g) << " \n";

    std::cout << "c: " << c.to_string() << " \n";
    std::cout << "d: " << d.to_string() << " \n";

    std::cout << "c < d: " << (c < d) << " \n";
    std::cout << "d < c: " << (d < c) << " \n";

    // test >
    std::cout << "a: " << a.to_string() << " \n";
    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "a > a: " << (a > a) << " \n";
    std::cout << "a > e: " << (a > e) << " \n";
    std::cout << "a > i: " << (a > i) << " \n";

    std::cout << "e > e: " << (e > e) << " \n";
    std::cout << "e > a: " << (e > a) << " \n";
    std::cout << "e > i: " << (e > i) << " \n";

    std::cout << "i > i: " << (i > i) << " \n";
    std::cout << "i > a: " << (i > a) << " \n";
    std::cout << "i > e: " << (i > e) << " \n";

    // test >=
    std::cout << "a: " << a.to_string() << " \n";
    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "a >= a: " << (a >= a) << " \n";
    std::cout << "a >= e: " << (a >= e) << " \n";
    std::cout << "a >= i: " << (a >= i) << " \n";

    std::cout << "e >= e: " << (e >= e) << " \n";
    std::cout << "e >= a: " << (e >= a) << " \n";
    std::cout << "e >= i: " << (e >= i) << " \n";

    std::cout << "i >= i: " << (i >= i) << " \n";
    std::cout << "i >= a: " << (i >= a) << " \n";
    std::cout << "i >= e: " << (i >= e) << " \n";

    // test <=
    std::cout << "a: " << a.to_string() << " \n";
    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "a <= a: " << (a <= a) << " \n";
    std::cout << "a <= e: " << (a <= e) << " \n";
    std::cout << "a <= i: " << (a <= i) << " \n";

    std::cout << "e <= e: " << (e <= e) << " \n";
    std::cout << "e <= a: " << (e <= a) << " \n";
    std::cout << "e <= i: " << (e <= i) << " \n";

    std::cout << "i <= i: " << (i <= i) << " \n";
    std::cout << "i <= a: " << (i <= a) << " \n";
    std::cout << "i <= e: " << (i <= e) << " \n";

    std::cout << "i >= i: " << (i >= i) << " \n";
    std::cout << "i >= a: " << (i >= a) << " \n";
    std::cout << "i >= e: " << (i >= e) << " \n";

    // test ==
    std::cout << "a: " << a.to_string() << " \n";
    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "a == a: " << (a == a) << " \n";
    std::cout << "a == e: " << (a == e) << " \n";
    std::cout << "a == i: " << (a == i) << " \n";

    std::cout << "e == e: " << (e == e) << " \n";
    std::cout << "e == a: " << (e == a) << " \n";
    std::cout << "e == i: " << (e == i) << " \n";

    std::cout << "i == i: " << (i == i) << " \n";
    std::cout << "i == a: " << (i == a) << " \n";
    std::cout << "i == e: " << (i == e) << " \n";

    // test !=
    std::cout << "a: " << a.to_string() << " \n";
    std::cout << "e: " << e.to_string() << " \n";
    std::cout << "i: " << i.to_string() << " \n";

    std::cout << "a != a: " << (a != a) << " \n";
    std::cout << "a != e: " << (a != e) << " \n";
    std::cout << "a != i: " << (a != i) << " \n";

    std::cout << "e != e: " << (e != e) << " \n";
    std::cout << "e != a: " << (e != a) << " \n";
    std::cout << "e != i: " << (e != i) << " \n";

    std::cout << "i != i: " << (i != i) << " \n";
    std::cout << "i != a: " << (i != a) << " \n";
    std::cout << "i != e: " << (i != e) << " \n";

    // test +
    BigNumber m1("-1");
    BigNumber m10("-10");
    BigNumber p1("1");
    BigNumber p10("10");

    std::cout << "1 + 10: " << (p1 + p10).to_string() << " \n";
    std::cout << "10 + 1: " << (p10 + p1).to_string() << " \n";
    std::cout << "-1 + -10: " << (m1 + m10).to_string() << " \n";
    std::cout << "-10 + -1: " << (m10 + m1).to_string() << " \n";
    std::cout << "1 + 1: " << (p1 + p1).to_string() << " \n";
    std::cout << "10 + 10: " << (p10 + p10).to_string() << " \n";
    std::cout << "-1 + 10: " << (m1 + p10).to_string() << " \n";
    std::cout << "10 + -1: " << (p10 + m1).to_string() << " \n";
    std::cout << "1 + -10: " << (p1 + m10).to_string() << " \n";
    std::cout << "-10 + 1: " << (m10 + p1).to_string() << " \n";
    std::cout << "-1 + -1: " << (m1 + m1).to_string() << " \n";
    std::cout << "-10 + -10: " << (m10 + m10).to_string() << " \n";

    // test -
    std::cout << "1 - 10: " << (p1 - p10).to_string() << " \n";
    std::cout << "10 - 1: " << (p10 - p1).to_string() << " \n";
    std::cout << "-1 - -10: " << (m1 - m10).to_string() << " \n";
    std::cout << "-10 - -1: " << (m10 - m1).to_string() << " \n";
    std::cout << "1 - 1: " << (p1 - p1).to_string() << " \n";
    std::cout << "10 - 10: " << (p10 - p10).to_string() << " \n";
    std::cout << "-1 - 10: " << (m1 - p10).to_string() << " \n";
    std::cout << "10 - -1: " << (p10 - m1).to_string() << " \n";
    std::cout << "1 - -10: " << (p1 - m10).to_string() << " \n";
    std::cout << "-10 - 1: " << (m10 - p1).to_string() << " \n";
    std::cout << "-1 - -1: " << (m1 - m1).to_string() << " \n";
    std::cout << "-10 - -10: " << (m10 - m10).to_string() << " \n";



    std::string s;

    std::cout << "First: ";
    std::cin >> s;
    BigNumber first(s);

    std::cout << "Second: ";
    std::cin >> s;
    BigNumber second(s);

    std::cout << "Karatsuba multiplication: " << first.to_string() << " * " << second.to_string() << " = " << first.mul_karatsuba(second).to_string() << "\n";
    std::cout << "Schonhage-Strassen multiplication: " << first.to_string() << " * " << second.to_string() << " = " << first.mul_schonhage(second).to_string() << "\n";

}
