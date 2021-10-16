#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class BigNumber {
public:
    BigNumber(std::string s) {
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

    BigNumber sub(const BigNumber& other) {
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

    std::cout << a.to_string() << "\n";

    BigNumber c = a.sub(b);
    std::cout << c.to_string() << "\n";

    std::cout << a.cut(5, 3).to_string() << "\n";
    std::cout << a.cut(2, 0).to_string() << "\n";
}
