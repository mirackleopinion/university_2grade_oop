#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class BigNumber {
public:
    BigNumber(std::string s) {
        if (s.length() == 0) {
            std::cout << "Empty string!\n";
            std::exit(1);
        }

        this->data.resize(s.length(), 0);
        for (int i = 0; i < s.length(); ++i) {
            auto n = cifers.find_first_of(s[i]);

            if (n == std::string::npos) {
                std::cout << "Not a number: '" << s[i] << "' in '" << s << "' at position " << i << "!\n";
                std::exit(1);
            }else {
                this->data[s.length() - 1 - i] = n;
            };
        }

        remove_zeros_from_start();
    };

    BigNumber(std::vector<int> v) {
        if (v.size() == 0) {
            std::cout << "Empty vector!\n";
            std::exit(1);
        }
        this->data = v;

        remove_zeros_from_start();
    }

    BigNumber multiply(int x) {
        std::vector<int> new_data;
        int carry = 0;

        for (auto it = this->data.begin(); it != this->data.end(); ++it) {
            int result = *it * x + carry;
            carry = result / 10;
            result = result % 10;
            new_data.push_back(result);
        }

        if (carry != 0) {
            new_data.push_back(carry);
        }

        return BigNumber(new_data);
    };

    BigNumber add(const BigNumber &other) {
        std::vector<int> new_data;
        int carry = 0;
        int first, second;
        int max_size = std::max(this->data.size(), other.data.size()) + 1;

        new_data.resize(max_size, 0);

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

            new_data[i] = result;
        }

        return BigNumber(new_data);
    };

    BigNumber sub(const BigNumber &other) {
        std::vector<int> new_data;
        int carry = 0;
        int first, second;
        int max_size = std::max(this->data.size(), other.data.size()) + 1;

        new_data.resize(max_size, 0);

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
            
            new_data[i] = result;
        }

        return BigNumber(new_data);
    };

    void remove_zeros_from_start() {
        while (this->data[this->data.size() - 1] == 0 and this->data.size() > 1)
        {
            this->data.pop_back();
        }
    };

    std::string to_string() {
        std::string result;
        
        for (auto it = this->data.rbegin(); it != this->data.rend(); ++it)
        { result.push_back(cifers[*it]);}

        return result;
    };

    BigNumber cut(int start, int end) {
        std::vector<int> new_data;
        int len = start - end + 1;
        new_data.resize(len, 0);

        for (int i=0; i< len; i++) {
            new_data[i] = this->data[end + i];
        }

        return BigNumber(new_data);
    }

    BigNumber shift_to_left(int x) {
        std::vector<int> new_data;
        new_data.resize(this->data.size() + x, 0);

        for (int i = 0; i < this->data.size(); i++) {
            new_data[i + x] = this->data[i];
        }

        return BigNumber(new_data);
    }

    BigNumber mul_karatsuba(BigNumber& other) {
        // https://en.wikipedia.org/wiki/Karatsuba_algorithm
        int my_size = this->data.size();
        
        if (my_size < 2) {
            return other.multiply(this->data[0]);
        }

        int other_size = other.data.size();

        if (other_size < 2) {
            return this->multiply(other.data[0]);
        }
        
        int splitter = std::min(my_size, other_size) / 2;

        BigNumber high1 = this->cut(my_size - 1, splitter);
        BigNumber low1 = this->cut(splitter - 1, 0);

        BigNumber high2 = other.cut(other_size - 1, splitter);
        BigNumber low2 = other.cut(splitter - 1, 0);

        BigNumber z0 = low1.mul_karatsuba(low2);

        BigNumber z1_1 = low1.add(high1);
        BigNumber z1_2 = low2.add(high2);
        BigNumber z1 = z1_1.mul_karatsuba(z1_2);
        
        BigNumber z2 = high1.mul_karatsuba(high2);

        BigNumber result_1 = z2.shift_to_left(splitter * 2);
        BigNumber result_2 = result_1.add(z0);
        BigNumber result_3 = result_2.add(z1.shift_to_left(splitter));
        BigNumber result_4 = z2.add(z0).shift_to_left(splitter);

        return result_3.sub(result_4);
    }

private:
    std::vector<int> data;
    const std::string cifers = "0123456789";
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
