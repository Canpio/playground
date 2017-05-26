#include <iostream>

template<typename DerivedT>
struct VectorExpression
{
    const DerivedT& derive() const {
        return *(static_cast<const DerivedT*>(this));
    }
};

struct Vector : public VectorExpression<Vector>
{
    int _len;
    float* _dptr;

    Vector(): _len(0), _dptr(nullptr) {}
    Vector(float* array, int n): _dptr(array), _len(n) {}

    inline float eval(int i) const {
        return _dptr[i];
    }

    template<typename DerivedT>
    Vector& operator=(const VectorExpression<DerivedT>& expr_) {
        const DerivedT& expr = expr_.derive();
        for (int i = 0; i < _len; ++i) {
            _dptr[i] = expr.eval(i);
        }
        return *this;
    }
};

std::ostream& operator<<(std::ostream& out, const Vector& vec) {
    for (int i = 0; i < vec._len; ++i) {
        out << vec._dptr[i] << " ";
    }
    return out;
}

template<typename Op, typename LhsT, typename RhsT>
struct BinaryOp : public VectorExpression<BinaryOp<Op, LhsT, RhsT> >
{
    const LhsT& _lhs;
    const RhsT& _rhs;

    BinaryOp(const LhsT& lhs, const RhsT& rhs):
        _lhs(lhs), _rhs(rhs) {}
    
    inline float eval(int i) const {
        return Op::calc(_lhs.eval(i), _rhs.eval(i));
    }
};

struct OpAdd
{
    inline static float calc(const float& a, const float& b) {
        return a + b;
    }
};

struct OpMul
{
    inline static float calc(const float& a, const float& b) {
        return a * b;
    }
};

struct OpMin
{
    inline static float calc(const float& a, const float& b) {
        return a - b;
    }
};

struct OpDiv 
{
    inline static float calc(const float& a, const float& b) {
        return b == 0 ? 0 : a / b;
    }
};

template<typename LhsT, typename RhsT>
inline BinaryOp<OpAdd, LhsT, RhsT> operator+(const VectorExpression<LhsT>& lhs,
                                             const VectorExpression<RhsT>& rhs) {
    return BinaryOp<OpAdd, LhsT, RhsT>(lhs.derive(), rhs.derive());
}

template<typename LhsT, typename RhsT>
inline BinaryOp<OpMin, LhsT, RhsT> operator-(const VectorExpression<LhsT>& lhs,
                                             const VectorExpression<RhsT>& rhs) {
    return BinaryOp<OpMin, LhsT, RhsT>(lhs.derive(), rhs.derive());
}

template<typename LhsT, typename RhsT>
inline BinaryOp<OpMul, LhsT, RhsT> operator*(const VectorExpression<LhsT>& lhs,
                                             const VectorExpression<RhsT>& rhs) {
    return BinaryOp<OpMul, LhsT, RhsT>(lhs.derive(), rhs.derive());
}

template<typename LhsT, typename RhsT>
inline BinaryOp<OpDiv, LhsT, RhsT> operator/(const VectorExpression<LhsT>& lhs,
                                             const VectorExpression<RhsT>& rhs) {
    return BinaryOp<OpDiv, LhsT, RhsT>(lhs.derive(), rhs.derive());
}

int main() {
    const int n = 3;
    float a[n] = {2,4,1};
    float b[n] = {8,3,6};
    float c[n] = {0,0,0};

    Vector vec_a(a, n);
    Vector vec_b(b, n);
    Vector vec_c(c, n);

    vec_c = vec_a + vec_b * vec_a;
    std::cout << vec_c << std::endl;

    return 0;
}