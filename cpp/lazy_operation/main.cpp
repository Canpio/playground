
#include <iostream>
#include <string>

template <typename DerivedT, typename T>
struct VectorExpression {
    const DerivedT &derive() const {
        return *(static_cast<const DerivedT *>(this));
    }
};

template <typename T>
struct Vector : public VectorExpression<Vector<T>, T> {
    int _len;
    T *_dptr;

    Vector() : _len(0), _dptr(nullptr) {}
    Vector(T *array, int n) : _dptr(array), _len(n) {}

    inline T eval(int i) const {
        return _dptr[i];
    }

    template <typename DerivedT>
    Vector<T> &operator=(const VectorExpression<DerivedT, T> &expr_) {
        const DerivedT &expr = expr_.derive();
        for (int i = 0; i < _len; ++i) {
            _dptr[i] = expr.eval(i);
        }
        return *this;
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const Vector<T> &vec) {
    for (int i = 0; i < vec._len; ++i) {
        out << vec._dptr[i] << " ";
    }
    return out;
}

template <typename Op, typename LhsT, typename RhsT, typename T>
struct BinaryOp : public VectorExpression<BinaryOp<Op, LhsT, RhsT, T>, T> {
    const LhsT &_lhs;
    const RhsT &_rhs;

    BinaryOp(const LhsT &lhs, const RhsT &rhs) : _lhs(lhs), _rhs(rhs) {}

    inline T eval(int i) const {
        return Op::calc(_lhs.eval(i), _rhs.eval(i));
    }
};

template <typename T>
struct OpAdd {
    inline static T calc(const T &a, const T &b) {
        return a + b;
    }
};

template <typename T>
struct OpMul {
    inline static T calc(const T &a, const T &b) {
        return a * b;
    }
};

template <typename T>
struct OpMin {
    inline static T calc(const T &a, const T &b) {
        return a - b;
    }
};

template <typename T>
struct OpDiv {
    inline static T calc(const T &a, const T &b) {
        return b == 0 ? 0 : a / b;
    }
};

template <typename LhsT, typename RhsT, typename T>
inline BinaryOp<OpAdd<T>, LhsT, RhsT, T> operator+(const VectorExpression<LhsT, T> &lhs,
                                                   const VectorExpression<RhsT, T> &rhs) {
    return BinaryOp<OpAdd<T>, LhsT, RhsT, T>(lhs.derive(), rhs.derive());
}

template <typename LhsT, typename RhsT, typename T>
inline BinaryOp<OpMin<T>, LhsT, RhsT, T> operator-(const VectorExpression<LhsT, T> &lhs,
                                                   const VectorExpression<RhsT, T> &rhs) {
    return BinaryOp<OpMin<T>, LhsT, RhsT, T>(lhs.derive(), rhs.derive());
}

template <typename LhsT, typename RhsT, typename T>
inline BinaryOp<OpMul<T>, LhsT, RhsT, T> operator*(const VectorExpression<LhsT, T> &lhs,
                                                   const VectorExpression<RhsT, T> &rhs) {
    return BinaryOp<OpMul<T>, LhsT, RhsT, T>(lhs.derive(), rhs.derive());
}

template <typename LhsT, typename RhsT, typename T>
inline BinaryOp<OpDiv<T>, LhsT, RhsT, T> operator/(const VectorExpression<LhsT, T> &lhs,
                                                   const VectorExpression<RhsT, T> &rhs) {
    return BinaryOp<OpDiv<T>, LhsT, RhsT, T>(lhs.derive(), rhs.derive());
}

int main() {
    const int n = 3;
    float a[n] = {2, 4, 1};
    float b[n] = {8, 3, 6};
    float c[n] = {0, 0, 0};

    Vector<float> vec_a(a, n);
    Vector<float> vec_b(b, n);
    Vector<float> vec_c(c, n);

    vec_c = vec_a + vec_b * vec_a;
    std::cout << vec_c << std::endl;

    vec_c = vec_a / vec_b;
    std::cout << vec_c << std::endl;

    return 0;
}