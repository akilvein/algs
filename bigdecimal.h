#ifndef BIGDECIMAL_H
#define BIGDECIMAL_H

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class BigDecimal {
private:
    vector<unsigned long long> mDigits;
    const static int mBase = 1000000000;

public:
    explicit BigDecimal(unsigned int value) {
        mDigits.reserve(100000);
        mDigits.push_back(value % mBase);
        while (value > (mBase - 1)) {
            value /= mBase;
            mDigits.push_back(value % mBase);
        }
    }

    BigDecimal() {
        mDigits.reserve(100000);
    }

    BigDecimal operator+(const BigDecimal &other) {
        BigDecimal result;

        unsigned long long borrow = 0;
        size_t size = max(other.mDigits.size(), mDigits.size());
        for (size_t i = 0; i < size; i++) {
            unsigned long long sum = borrow;
            borrow = 0;
            if (i < other.mDigits.size())
                sum += other.mDigits[i];
            if (i < mDigits.size())
                sum += mDigits[i];

            if (sum > (mBase - 1)) {
                borrow = 1;
                sum -= mBase;
            }

            result.mDigits.push_back(sum);
        }

        if (borrow)
            result.mDigits.push_back(borrow);


        return result;
    }

    BigDecimal operator<<=(int pos) {
        for (int i = 0; i < pos; i++)
            mDigits.insert(mDigits.begin(), 0);

        return *this;
    }

    BigDecimal operator*(unsigned long long digit) const{
        BigDecimal result;

        unsigned long long borrow = 0;
        for (size_t i = 0; i < mDigits.size(); i++) {
            unsigned long long sum = borrow;
            borrow = 0;

            sum += mDigits[i] * digit;

            borrow = sum / mBase;
            result.mDigits.push_back(sum %mBase);
        }

        if (borrow)
            result.mDigits.push_back(borrow);

        return result;
    }



    BigDecimal operator*(const BigDecimal &other) {
        BigDecimal result;

        for (size_t i = 0; i < mDigits.size(); i++) {
            unsigned long long borrow = 0;
            size_t pos = i;

            for (size_t j = 0; j < other.mDigits.size(); j++) {
                if (pos >= result.mDigits.size())
                    result.mDigits.push_back(0);

                unsigned long long sum = borrow;
                borrow = 0;

                sum += other.mDigits[j] * mDigits[i];
                sum += result.mDigits[pos];
                borrow = sum / mBase;

                result.mDigits[pos++] = sum % mBase;
            }

            if (borrow) {
                if (result.mDigits.size() <= pos)
                    result.mDigits.push_back(0);
                result.mDigits[pos] = borrow;
            }
        }

        return result;
    }

    void print() {
        for (int i = mDigits.size() - 1; i >= 0; i--) {
            unsigned long long digit = mDigits[i];

            if (i != mDigits.size() - 1) {
                cout << setw(9) << setfill('0');
            }

            cout << digit;
        }

        cout << endl;
    }
};

#endif // BIGDECIMAL_H
