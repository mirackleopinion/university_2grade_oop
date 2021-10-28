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
            auto n = cifers.find_first_of(s[i]);

            if (n == std::string::npos) {
                std::cout << "Not a number: '" << s[i] << "' in '" << s << "' at position " << i << "!\n";
                std::exit(1);
            }else {
                this->data[s.length() - 1 - i] = n;
            };
        }

        remove_zeros_from_start();
        if (this->is_debug) {
            this->id = rand();
            std::cout << "String constructor " << this->id << " " << this->to_string() << "\n";
        }
    };

    BigNumber() {
        this->data.clear();
        this->data.push_back(0);
        this->sign = +1;
        this->id = 0;
        if (this->is_debug) {
            this->id = rand();
            std::cout << "Empty constructor " << this->id << " " << this->to_string() << "\n";
        }
    }

    ~BigNumber() {
        if (this->is_debug)
            std::cout << "Destructor " << this->id << " " << this->to_string() << "\n";
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

    // TODO sign
    BigNumber operator +(const BigNumber &other) {
        BigNumber output;
        int carry = 0;
        int first, second;
        size_t max_size = std::max(this->data.size(), other.data.size()) + 1;

        output.data.resize(max_size, 0);

        for (size_t i = 0; i < max_size; i++) {
            if (i < this->data.size())
                first = this->data[i]; 
            else 
                first = 0;

            if (i < other.data.size())
                second = other.data[i];
            else
                second = 0;

            int result = first + second + carry;
            carry = result / 10;
            result = result % 10;

            output.data[i] = result;
        }

        output.remove_zeros_from_start();

        return output;
    };

    // TODO sign
    BigNumber operator -(const BigNumber &other) {
        BigNumber output;
        int carry = 0;
        int first, second;
        size_t max_size = std::max(this->data.size(), other.data.size()) + 1;

        output.data.resize(max_size, 0);

        for (size_t i = 0; i < max_size; i++) {
            if (i < this->data.size())
                first = this->data[i];
            else
                first = 0;

            if (i < other.data.size())
                second = other.data[i];
            else
                second = 0;

            

            int result = first  - second - carry;
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
    };

    std::string to_string() {
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

    BigNumber operator()(int start, int end) {
        BigNumber output;
        int len = start - end + 1;
        output.data.resize(len, 0);

        for (int i = 0; i < len; i++) {
            output.data[i] = this->data[end + i];
        }

        return output;
    }

    bool operator ==(BigNumber& other) {
            return this->to_string() == other.to_string();
    }

    bool operator !=(BigNumber& other) {
        return this->to_string() != other.to_string();
    }

    BigNumber operator <<(int x) {
        BigNumber output;
        output.data.resize(this->data.size() + x, 0);

        for (size_t i = 0; i < this->data.size(); i++) {
            output.data[i + x] = this->data[i];
        }

        return output;
    }

    BigNumber operator +() {
        BigNumber output;
        output.sign = this->sign;
        output.data = this->data;
        output.remove_zeros_from_start();
        return output;
    }

    BigNumber operator -() {
        BigNumber output;
        output.sign = -this->sign;
        output.data = this->data;
        output.remove_zeros_from_start();
        return output;
    }

    bool operator < (BigNumber& other) {
        if (*this == other)
            return false;

        if (this->sign < 0 and other.sign >0)
            return true;

        if (this->sign > 0 and other.sign <0)
            return false;


        if (this->sign < 0 and other.sign < 0) {
            BigNumber not_this = -(*this);
            BigNumber not_other = -other;
            return  not_other < not_this;
        }
            


        if (this->data.size() < other.data.size())
            return true;

        if (this->data.size() > other.data.size())
            return false;

        for (int i = other.data.size() - 1; i >= 0; i--) {
            if (this->data[i] == other.data[i])
                continue;

            return this->data[i] < other.data[i];
        }

        return true;
    }


    bool operator <= (BigNumber& other)  {
        return (*this < other || ((*this) == other));
    }

    bool operator >= (BigNumber& other)  {
        return !(*this < other);
    }

    bool operator > (BigNumber& other)  {
        return !(*this <= other);
    }

    BigNumber mul_karatsuba(BigNumber& other) {
        // https://en.wikipedia.org/wiki/Karatsuba_algorithm
        if (this->sign < 0 or other.sign < 0) {
            std::cout << "mul_karatsuba() works only with positive numbers!\n";
            std::exit(1);
        }

        int my_size = this->data.size();
        
        if (my_size < 2) {
            return other * this->data[0];
        }

        int other_size = other.data.size();

        if (other_size < 2) {
            return *this * other.data[0];
        }
        
        int splitter = std::min(my_size, other_size) / 2;

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
    int id;
    const bool is_debug = false;
};


int main()
{
    BigNumber a("-1");
    BigNumber b("-1");
    std::cout << "a != b:" << (a != b) << " \n";
    std::cout << "a == b:" << (a == b) << " \n";

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

    std::cout << "+a: " << (+a).to_string() << " \n";
    std::cout << "+b: " << (+b).to_string() << " \n";
    std::cout << "+c: " << (+c).to_string() << " \n";
    std::cout << "+d: " << (+d).to_string() << " \n";
    std::cout << "+e: " << (+e).to_string() << " \n";
    std::cout << "+f: " << (+f).to_string() << " \n";
    std::cout << "+g: " << (+g).to_string() << " \n";
    std::cout << "+h: " << (+h).to_string() << " \n";
    std::cout << "+i: " << (+i).to_string() << " \n";

    std::cout << "-a: " << (-a).to_string() << " \n";
    std::cout << "-b: " << (-b).to_string() << " \n";
    std::cout << "-c: " << (-c).to_string() << " \n";
    std::cout << "-d: " << (-d).to_string() << " \n";
    std::cout << "-e: " << (-e).to_string() << " \n";
    std::cout << "-f: " << (-f).to_string() << " \n";
    std::cout << "-g: " << (-g).to_string() << " \n";
    std::cout << "-h: " << (-h).to_string() << " \n";
    std::cout << "-i: " << (-i).to_string() << " \n";

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



    /*
    std::string s;

    std::cout << "First: ";
    std::cin >> s;
    BigNumber a(s);

    std::cout << "Second: ";
    std::cin >> s;
    BigNumber b(s);

    std::cout << "Karatsuba multiplication: " << a.to_string() << " * " << b.to_string() << " = " << a.mul_karatsuba(b).to_string() << "\n";
    std::cout << "Schonhage-Strassen multiplication: " << a.to_string() << " * " << b.to_string() << " = " << a.mul_schonhage(b).to_string() << "\n";
    */
}
