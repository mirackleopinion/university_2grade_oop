#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class BigNumber {
public:
    BigNumber(std::string s) {
        for (int i = 0; i < s.length(); ++i) {
            auto it = this->data.begin();
            auto n = cifers.find_first_of(s[i]);

            if (n == std::string::npos) {
                std::cout << "Not a number: '" << s[i] << "' in '" << s << "' at position " << i << "!\n";
                std::exit(1);
            }else {
                this->data.insert(it, n);
            };
        }
    };

    BigNumber(std::vector<int> v) {
        this->data = v;
    }


    BigNumber multiply_at_int(int x) {
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


    std::string to_string() {
        std::string result;
        
        for (auto it = this->data.rbegin(); it != this->data.rend(); ++it)
        { result.push_back(cifers[*it]);}

        return result;
    };

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

    BigNumber c = a.multiply_at_int(3);
    std::cout << c.to_string() << "\n";
}
