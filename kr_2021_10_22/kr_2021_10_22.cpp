
#include <iostream>
#include <vector>

int main()
{
    const int base = 36;
    std::string abc = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string s;

    std::cout << "Enter your number: ";
    std::cin >> s;

    std::string result;

    while (s != ""){
        std::string temp;
        int x = 0;
        int y;

        for (auto i = 0; i < s.size(); i++) {
            x *= 10;
            x += char(s[i]) - '0';

            y = x / base;

            if (y != 0 or temp.size() != 0)
                temp.push_back(abc[y]);

            x -= base * y;
        }

        result.insert(result.begin(), abc[x]);
        s = temp;
    }

    std::cout << result;
}
