
#include <iostream>
#include <vector>

int main()
{
    const int out_base = 36;
    const int in_base = 10;
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
            x *= in_base;
            x += abc.find(s[i]);

            y = x / out_base;

            if (y != 0 or temp.size() != 0)
                temp.push_back(abc[y]);

            x -= out_base * y;
        }

        result.insert(result.begin(), abc[x]);
        s = temp;
    }

    std::cout << result;
}
