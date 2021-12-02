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

    int operator [](size_t pos) const 
    {
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

    BigNumber& operator =(const BigNumber& other) {
        this->data = other.data;
        this->sign = other.sign;
        return (*this);
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

    friend BigNumber operator %(BigNumber lhs, BigNumber& rhs) {
        return lhs - ((lhs / rhs).mul_karatsuba(rhs));
    }

    friend BigNumber operator /(const BigNumber lhs, const BigNumber& rhs) {
        if (rhs == BigNumber("0")) {
            std::cout << " Divizion by zero!\n";
            std::exit(1);
        }

        BigNumber _1 = abs(rhs);
        BigNumber _2 = _1 * 2;
        BigNumber _3 = _1 * 3;
        BigNumber _4 = _1 * 4;
        BigNumber _5 = _1 * 5;
        BigNumber _6 = _1 * 6;
        BigNumber _7 = _1 * 7;
        BigNumber _8 = _1 * 8;
        BigNumber _9 = _1 * 9;

        BigNumber result;
        result.sign = lhs.sign * rhs.sign;

        BigNumber temp;

        for (size_t i = 0; i < lhs.data.size(); i++) {
            temp.data.insert(temp.data.begin(), 1, 0);
            temp.data[0] = lhs.data[lhs.data.size() - i - 1];
            temp.remove_zeros_from_start();
             
            if (temp < _5) { // 01234
                if (temp < _3) { // 012
                    if (temp < _2) { // 01
                        if (temp < _1) { // 0
                            result.data.insert(result.data.begin(), 0);
                        }
                        else { // 1
                            result.data.insert(result.data.begin(), 1);
                            temp = temp - _1;
                        }
                    }
                    else { // 2
                        result.data.insert(result.data.begin(), 2);
                        temp = temp - _2;
                    }
                
                }
                else { // 34
                    if (temp < _4) { // 3
                        result.data.insert(result.data.begin(), 3);
                        temp = temp - _3;
                    }
                    else { // 4
                        result.data.insert(result.data.begin(), 4);
                        temp = temp - _4;
                    }
                }
            }
            else { // 56789
                if (temp < _8) { // 567
                    if (temp < _7) { // 56
                        if (temp < _6) { // 5
                            result.data.insert(result.data.begin(), 5);
                            temp = temp - _5;
                        }
                        else { // 6
                            result.data.insert(result.data.begin(), 6);
                            temp = temp - _6;
                        }
                    }
                    else { // 7
                        result.data.insert(result.data.begin(), 7);
                        temp = temp - _7;
                    }
                }
                else { //89
                    if (temp < _9) { // 8
                        result.data.insert(result.data.begin(), 8);
                        temp = temp - _8;
                    }
                    else { // 9
                        result.data.insert(result.data.begin(), 9);
                        temp = temp - _9;
                    }
                }
            }
        }
        
        result.remove_zeros_from_start();
        return result;
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
        if (start >= this->data.size())
            start = this->data.size() - 1;

        if (end > start)
            return BigNumber("0");

        BigNumber output;
        size_t len = start - end + 1;
        output.data.resize(len, 0);

        for (int i = 0; i < len; i++) {
            output.data[i] = this->data[end + i];
        }

        return output;
    }

    BigNumber operator <<=(size_t x) {
        this->data.insert(this->data.begin(), x, 0);
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
    
    friend BigNumber gcd(const BigNumber a, const BigNumber& b)
    {   
        if (a == b)
            return a;

        if (a > b)
            return gcd(a - b, b);
        else
            return gcd(b - a, a);
    }
   
    BigNumber pow(const BigNumber& power) {
        if (power.sign < 0) {
            std::cout << "Negative power!\n";
            std::exit(1);
        }
        
        BigNumber _1("1");
        BigNumber _0("0");

        if (power == _0)
            return _0;

        BigNumber k("1");
        BigNumber result = *this;


        while (k < power) {
            k = k + _1;
            result = this->mul_karatsuba(result);
        }
        return result;
    }

    BigNumber mul_karatsuba(BigNumber& other) {
        // https://en.wikipedia.org/wiki/Karatsuba_algorithm
        /*if (this->sign < 0 or other.sign < 0) {
            std::cout << "mul_karatsuba() works only with positive numbers!\n";
            std::exit(1);
        }
        */

        auto my_size = this->data.size();

        if (my_size < 2) {
            return other * (this->data[0] * this->sign);
        }

        auto other_size = other.data.size();

        if (other_size < 2) {
            return *this * (other.data[0] * other.sign);
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

        BigNumber result = (z2 << (splitter * 2)) + z0 + (z1 << splitter) - ((z2 + z0) << splitter);
        result.sign = result.sign = other.sign * this->sign;
        return result;
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

    BigNumber mul_toom_cook(BigNumber& other) {

        //https://hrwiki.ru/wiki/Toom%E2%80%93Cook_multiplication

        auto my_size = this->data.size();

        if (my_size < 2) {
            return other * (this->data[0] * this->sign);
        }

        auto other_size = other.data.size();

        if (other_size < 2) {
            return *this * (other.data[0] * other.sign);
        }

        size_t splitter = std::min(this->data.size()/3, other.data.size()/3)+1;

        BigNumber m0 = (*this)(splitter - 1, 0);
        BigNumber m1 = (*this)(2*splitter - 1, splitter);
        BigNumber m2 = (*this)(my_size - 1, 2*splitter);

        BigNumber n0 = other(splitter - 1, 0);
        BigNumber n1 = other(2 * splitter - 1, splitter);
        BigNumber n2 = other(other_size - 1, 2 * splitter);

        BigNumber p_0 = m0;
        BigNumber p_1 = m0 + m1 + m2;
        BigNumber p_m1 = m0 - m1 + m2;
        BigNumber p_m2 = m0 - (m1 * 2) + (m2 * 4);
        BigNumber p_inf = m2;

        BigNumber q_0 = n0;
        BigNumber q_1 = n0 + n1 + n2;
        BigNumber q_m1 = n0 - n1 + n2;
        BigNumber q_m2 = n0 - (n1 * 2) + (n2 * 4);
        BigNumber q_inf = n2;

        BigNumber r_0 = p_0.mul_toom_cook(q_0);
        BigNumber r_1 = p_1.mul_toom_cook(q_1);
        BigNumber r_m1 = p_m1.mul_toom_cook(q_m1);
        BigNumber r_m2 = p_m2.mul_toom_cook(q_m2);
        BigNumber r_inf = p_inf.mul_toom_cook(q_inf);

        BigNumber _3("3"), _2("2");

        BigNumber x0 = r_0;
        BigNumber x4 = r_inf;
        BigNumber x3 = (r_m2 - r_1) / _3;
        BigNumber x1 = (r_1 - r_m1) / _2;
        BigNumber x2 = r_m1 - r_0;
        x3 = (x2 - x3) / _2 + (r_inf * 2);
        x2 = x2 + x1 - x4;
        x1 = x1 - x3;

        BigNumber result = x0 
            + (x1 << splitter) 
            + (x2 << (splitter * 2)) 
            + (x3 << (splitter * 3)) 
            + (x4 << (splitter * 4));
        result.sign = other.sign * this->sign;
        return result;
    }

    friend bool chek_sol_strassen(BigNumber& a, BigNumber& p) {
        BigNumber _1("1");
        BigNumber _2("2");

        if (gcd(a, p) == _1) 
            return false;
        BigNumber K = (p - _1) / _2;
        BigNumber j = ((p - _1) / _2).mul_karatsuba(a) % p;

        BigNumber jakobi = get_jacobi(a, p);
        //Если j != J(a,p), то число p наверняка не является простым. 
        if (j != jakobi)
            return false;
        //Если j = J(a,p), то вероятность того, что число p не является простым, не больше 50 процентов.
        return true;
    }

    std::vector<BigNumber> prime_factorization() {
        std::vector<BigNumber> result;
        BigNumber x = (*this);
        BigNumber _1("1"), _0("0");
        BigNumber half = x / BigNumber("2");
        for (BigNumber i("2"); i <= half; i = i + _1) {
            while (x % i == _0) {
                result.push_back(i);
                x = x / i;
            }
        }
        return result;
    }

    friend BigNumber get_legendre_symbol(BigNumber a, BigNumber p) {
        // https://mathworld.wolfram.com/LegendreSymbol.html
        BigNumber _0("0"), _1("1"), _m1("-1");
        BigNumber a_mod_p = a % p;

        if (a_mod_p == _0)
            return _0;

        for (BigNumber i("1"); i <= p; i = i + _1) {
            if (i.mul_karatsuba(i) % p == a_mod_p)
                return _1;
        }

        return _m1;
    }
    friend BigNumber get_jacobi(BigNumber a, BigNumber p) {
        // https://mathworld.wolfram.com/JacobiSymbol.html
        BigNumber result("1");
        auto prime_factorization = p.prime_factorization();

        for (const auto& prime_divider : prime_factorization)
            result = get_legendre_symbol(a, prime_divider).mul_karatsuba(result);

        return result;
    }

    
    friend bool chek_lehmann(BigNumber& a, BigNumber& p) {
        BigNumber _1("1");
        BigNumber _m1("-1");
        BigNumber _2("2");

        BigNumber K = (p - _1) / _2;
        BigNumber check = a.pow(p);
        // Если a^((p-1)/2) != 1 или -1 (mod p), то p не является простым.
        return !(check != _1 or check != _m1);
    }

    friend bool chek_rabin_miller(BigNumber& a, BigNumber& p) {
        BigNumber _0("0"), _1("1"), _2("2");

        BigNumber pm1 = p - BigNumber("1");

        BigNumber m = pm1;
        BigNumber b("0");

        while (m % _2 == _0)
        {
            m = m / _2;
            b = b + _1;
        }

        BigNumber j("0");
        BigNumber z = (a.pow(m)) % p;

        // 3) Если z = 1 или если z = p - 1, то p проходит проверку и может быть простым числом. 
        if (!((z == _1) or (z == pm1)))
            return false;

        while (true) {
            // 4) Если j > 0 и z = 1, то p не является простым числом.
            if ((j > _0) and (z == _1))
                return false;
            // 5.1) Установите j = j + 1. 
            j = j + _1;
            // 5.2) Если j < b и z != p - 1, установите z = z2 mod p и вернитесь на этап (4)
            if ((j < b) and (z != pm1))
                z = z.pow(_2) % p;
            else
                break;
        }
        // 5.3) Если z = p - 1, то p проходит проверку и может быть простым числом.
        if (z != pm1)
            return false;
        // 6) Если j = b и z != (p - 1), то p не является простым числом. 
        if (j == b and z != pm1)
            return false;

        return true;
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
    std::string s;

    std::cout << "First: ";
    std::cin >> s;
    BigNumber first(s);

    std::cout << "Second: ";
    std::cin >> s;
    BigNumber second(s);

    std::cout << "Karatsuba multiplication: " << first.to_string() << " * " << second.to_string() << " = " << first.mul_karatsuba(second).to_string() << "\n";
    std::cout << "Schonhage-Strassen multiplication: " << first.to_string() << " * " << second.to_string() << " = " << first.mul_schonhage(second).to_string() << "\n";
    std::cout << "Cook division: " << first.to_string() << " / " << second.to_string() << " = " << (first / second).to_string() << "\n";

    BigNumber r = first.mul_toom_cook(second);
    std::cout << "Cook4: " << first.to_string() << " * " << second.to_string() << " = " << r.to_string() << "\n";


    BigNumber a("2");
    BigNumber p("11");

    std::cout << "chek_sol_strassen: " << chek_sol_strassen(a, p) ? "True" : "False";
    std::cout << "chek_lehmann: " << chek_lehmann(a, p) ? "True" : "False";
    std::cout << "chek_rabin_miller: " << chek_rabin_miller(a, p) ? "True" : "False";
    }
