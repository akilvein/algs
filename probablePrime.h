long mulmod(long a, long b, long mod) {
    long x = 0,y = a % mod;
    while (b > 0) {
        if (b % 2 == 1) {
            x = (x + y) % mod;
        }

        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}

long modulo(long base, long exponent, long mod) {
    long x = 1;
    long y = base;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            x = (x * y) % mod;
        }
        y = (y * y) % mod;
        exponent = exponent / 2;
    }

    return x % mod;
}

bool mr(long p,int iterations) {
    if (p < 2) {
        return false;
    }

    if (p != 2 && p % 2 == 0) {
        return false;
    }

    long s = p - 1;
    while (s % 2 == 0) {
        s /= 2;
    }

    for (int i = 0; i < iterations; i++) {
        long a = rand() % (p - 1) + 1;
        long temp = s;
        long mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1) {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }

        if (mod != p - 1 && temp % 2 == 0) {
            return false;
        }
    }
    return true;
}

inline bool isPrime(long n) {
    for (long f = 2; f * f <= n; f++) {
        if (n % f == 0) {
            return false;
        }
    }

    return true;
}
