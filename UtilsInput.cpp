#pragma once

#include <iostream>
#include <limits>
#include <string>

namespace input_utils
{

    template <typename Predicate>
    long long readValidatedLongLong(std::istream& in, std::ostream& out,
        const std::string& prompt,
        const std::string& errorMessage,
        Predicate predicate) {
        long long value{};
        while (true) {
            out << prompt;
            if (!(in >> value)) {
                in.clear();
                in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                out << "Помилка: очікувалось ціле число. Спробуйте ще раз.\n";
                continue;
            }
            if (!predicate(value)) {
                out << errorMessage << '\n';
                continue;
            }
            return value;
        }
    }

    inline int readPositiveInt(std::istream& in, std::ostream& out, const std::string& prompt) {
        return static_cast<int>(readValidatedLongLong(
            in, out, prompt, "Помилка: число має бути додатним.",
            [](long long v) { return v > 0; }));
    }

    inline long long readNonNegativeLongLong(std::istream& in, std::ostream& out,
        const std::string& prompt) {
        return readValidatedLongLong(in, out, prompt, "Помилка: число не може бути від'ємним.",
            [](long long v) { return v >= 0; });
    }
}

