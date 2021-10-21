#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

class BigNumber {
public:
    BigNumber(std::string s) {

        if (s.length() == 0) {
            std::cout << "Empty string!\n";
            std::exit(1);
        }

        this->data.resize(s.length(), 0);
        for (size_t i = 0; i < s.length(); ++i) {
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
        int carry = 0;

        for (auto it = this->data.begin(); it != this->data.end(); ++it) {
            int result = *it * x + carry;
            carry = result / 10;
            result = result % 10;
            output.data.push_back(result);
        }

        if (carry != 0)
            output.data.push_back(carry);

        output.remove_zeros_from_start();

        return output;
    };

    BigNumber operator +(const BigNumber &other) {
        BigNumber output;
        int carry = 0;
        int first, second;
        int max_size = std::max(this->data.size(), other.data.size()) + 1;

        output.data.resize(max_size, 0);

        for (int i = 0; i < max_size; i++) {
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

    BigNumber operator -(const BigNumber &other) {
        BigNumber output;
        int carry = 0;
        int first, second;
        int max_size = std::max(this->data.size(), other.data.size()) + 1;

        output.data.resize(max_size, 0);

        for (int i = 0; i < max_size; i++) {
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

    BigNumber operator <<(int x) {
        BigNumber output;
        output.data.resize(this->data.size() + x, 0);

        for (size_t i = 0; i < this->data.size(); i++) {
            output.data[i + x] = this->data[i];
        }

        return output;
    }

    BigNumber mul_karatsuba(BigNumber& other) {
        // https://en.wikipedia.org/wiki/Karatsuba_algorithm
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

private:
    void remove_zeros_from_start() {
        while (this->data[this->data.size() - 1] == 0 and this->data.size() > 1)
            this->data.pop_back();
    };

    std::vector<int> data;
    const std::string cifers = "0123456789";
    int id;
    const bool is_debug = false;
};


int main()
{

    std::string s;

    std::cout << "First: ";
    std::cin >> s;
    BigNumber a(s);

    std::cout << "Second: ";
    std::cin >> s;
    BigNumber b(s);

    std::cout << "Karatsuba multiplication: " << a.to_string() << " * " << b.to_string() << " = " << a.mul_karatsuba(b).to_string() << "\n";
}
