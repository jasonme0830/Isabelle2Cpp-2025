#include <cstdlib>
#include <memory>
#include <variant>

template<typename T1>
class atree {
    struct _aLeaf {
        bool operator<(const _aLeaf &) const { return false; }
    };
    struct _aNode1 {
        std::shared_ptr<atree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<atree<T1>> p3_;

        atree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        atree<T1> p3() const { return *p3_; }

        bool operator<(const _aNode1 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };

    std::variant<_aLeaf, _aNode1> value_;
    atree(const std::variant<_aLeaf, _aNode1> &value) : value_(value) {}

  public:
    atree() = default;

    static atree<T1> aLeaf() {
        return atree<T1> { _aLeaf {  } };
    }
    static atree<T1> aNode1(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3) {
        return atree<T1> { _aNode1 { std::make_shared<atree<T1>>(p1), p2, std::make_shared<atree<T1>>(p3) } };
    }

    bool is_aLeaf() const { return std::holds_alternative<_aLeaf>(value_); }
    bool is_aNode1() const { return std::holds_alternative<_aNode1>(value_); }

    const _aNode1 &as_aNode1() const { return std::get<_aNode1>(value_); }

    bool operator<(const atree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class btree {
    struct _bLeaf {
        bool operator<(const _bLeaf &) const { return false; }
    };
    struct _bNode1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _bNode1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _bNode2 {
        std::shared_ptr<btree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<btree<T1>> p3_;

        btree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        btree<T1> p3() const { return *p3_; }

        bool operator<(const _bNode2 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };

    std::variant<_bLeaf, _bNode1, _bNode2> value_;
    btree(const std::variant<_bLeaf, _bNode1, _bNode2> &value) : value_(value) {}

  public:
    btree() = default;

    static btree<T1> bLeaf() {
        return btree<T1> { _bLeaf {  } };
    }
    static btree<T1> bNode1(const T1 &p1) {
        return btree<T1> { _bNode1 { p1 } };
    }
    static btree<T1> bNode2(const btree<T1> &p1, const T1 &p2, const btree<T1> &p3) {
        return btree<T1> { _bNode2 { std::make_shared<btree<T1>>(p1), p2, std::make_shared<btree<T1>>(p3) } };
    }

    bool is_bLeaf() const { return std::holds_alternative<_bLeaf>(value_); }
    bool is_bNode1() const { return std::holds_alternative<_bNode1>(value_); }
    bool is_bNode2() const { return std::holds_alternative<_bNode2>(value_); }

    const _bNode1 &as_bNode1() const { return std::get<_bNode1>(value_); }
    const _bNode2 &as_bNode2() const { return std::get<_bNode2>(value_); }

    bool operator<(const btree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class dtree {
    struct _dLeaf {
        bool operator<(const _dLeaf &) const { return false; }
    };
    struct _dNode1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _dNode1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _dNode2 {
        T1 p1_;
        std::shared_ptr<dtree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        dtree<T1> p2() const { return *p2_; }

        bool operator<(const _dNode2 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };

    std::variant<_dLeaf, _dNode1, _dNode2> value_;
    dtree(const std::variant<_dLeaf, _dNode1, _dNode2> &value) : value_(value) {}

  public:
    dtree() = default;

    static dtree<T1> dLeaf() {
        return dtree<T1> { _dLeaf {  } };
    }
    static dtree<T1> dNode1(const T1 &p1) {
        return dtree<T1> { _dNode1 { p1 } };
    }
    static dtree<T1> dNode2(const T1 &p1, const dtree<T1> &p2) {
        return dtree<T1> { _dNode2 { p1, std::make_shared<dtree<T1>>(p2) } };
    }

    bool is_dLeaf() const { return std::holds_alternative<_dLeaf>(value_); }
    bool is_dNode1() const { return std::holds_alternative<_dNode1>(value_); }
    bool is_dNode2() const { return std::holds_alternative<_dNode2>(value_); }

    const _dNode1 &as_dNode1() const { return std::get<_dNode1>(value_); }
    const _dNode2 &as_dNode2() const { return std::get<_dNode2>(value_); }

    bool operator<(const dtree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class etree {
    struct _eLeaf {
        bool operator<(const _eLeaf &) const { return false; }
    };
    struct _eNode1 {
        T1 p1_;
        std::shared_ptr<etree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        etree<T1> p2() const { return *p2_; }

        bool operator<(const _eNode1 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };
    struct _eNode2 {
        std::shared_ptr<etree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<etree<T1>> p3_;

        etree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        etree<T1> p3() const { return *p3_; }

        bool operator<(const _eNode2 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };

    std::variant<_eLeaf, _eNode1, _eNode2> value_;
    etree(const std::variant<_eLeaf, _eNode1, _eNode2> &value) : value_(value) {}

  public:
    etree() = default;

    static etree<T1> eLeaf() {
        return etree<T1> { _eLeaf {  } };
    }
    static etree<T1> eNode1(const T1 &p1, const etree<T1> &p2) {
        return etree<T1> { _eNode1 { p1, std::make_shared<etree<T1>>(p2) } };
    }
    static etree<T1> eNode2(const etree<T1> &p1, const T1 &p2, const etree<T1> &p3) {
        return etree<T1> { _eNode2 { std::make_shared<etree<T1>>(p1), p2, std::make_shared<etree<T1>>(p3) } };
    }

    bool is_eLeaf() const { return std::holds_alternative<_eLeaf>(value_); }
    bool is_eNode1() const { return std::holds_alternative<_eNode1>(value_); }
    bool is_eNode2() const { return std::holds_alternative<_eNode2>(value_); }

    const _eNode1 &as_eNode1() const { return std::get<_eNode1>(value_); }
    const _eNode2 &as_eNode2() const { return std::get<_eNode2>(value_); }

    bool operator<(const etree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class ftree {
    struct _fLeaf {
        bool operator<(const _fLeaf &) const { return false; }
    };
    struct _fNode1 {
        T1 p1_;
        std::shared_ptr<ftree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        ftree<T1> p2() const { return *p2_; }

        bool operator<(const _fNode1 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };
    struct _fNode2 {
        std::shared_ptr<ftree<T1>> p1_;
        std::shared_ptr<ftree<T1>> p2_;
        T1 p3_;

        ftree<T1> p1() const { return *p1_; }
        ftree<T1> p2() const { return *p2_; }
        const T1 &p3() const { return p3_; }

        bool operator<(const _fNode2 &rhs) const {
            return std::tie(*p1_, *p2_, p3_) < std::tie(*rhs.p1_, *rhs.p2_, rhs.p3_);
        }
    };

    std::variant<_fLeaf, _fNode1, _fNode2> value_;
    ftree(const std::variant<_fLeaf, _fNode1, _fNode2> &value) : value_(value) {}

  public:
    ftree() = default;

    static ftree<T1> fLeaf() {
        return ftree<T1> { _fLeaf {  } };
    }
    static ftree<T1> fNode1(const T1 &p1, const ftree<T1> &p2) {
        return ftree<T1> { _fNode1 { p1, std::make_shared<ftree<T1>>(p2) } };
    }
    static ftree<T1> fNode2(const ftree<T1> &p1, const ftree<T1> &p2, const T1 &p3) {
        return ftree<T1> { _fNode2 { std::make_shared<ftree<T1>>(p1), std::make_shared<ftree<T1>>(p2), p3 } };
    }

    bool is_fLeaf() const { return std::holds_alternative<_fLeaf>(value_); }
    bool is_fNode1() const { return std::holds_alternative<_fNode1>(value_); }
    bool is_fNode2() const { return std::holds_alternative<_fNode2>(value_); }

    const _fNode1 &as_fNode1() const { return std::get<_fNode1>(value_); }
    const _fNode2 &as_fNode2() const { return std::get<_fNode2>(value_); }

    bool operator<(const ftree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class gtree {
    struct _gNode1 {
        T1 p1_;
        std::shared_ptr<gtree<T1>> p2_;
        std::shared_ptr<gtree<T1>> p3_;

        const T1 &p1() const { return p1_; }
        gtree<T1> p2() const { return *p2_; }
        gtree<T1> p3() const { return *p3_; }

        bool operator<(const _gNode1 &rhs) const {
            return std::tie(p1_, *p2_, *p3_) < std::tie(rhs.p1_, *rhs.p2_, *rhs.p3_);
        }
    };
    struct _gNode2 {
        std::shared_ptr<gtree<T1>> p1_;
        T1 p2_;

        gtree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }

        bool operator<(const _gNode2 &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
    };
    struct _gLeaf {
        bool operator<(const _gLeaf &) const { return false; }
    };

    std::variant<_gNode1, _gNode2, _gLeaf> value_;
    gtree(const std::variant<_gNode1, _gNode2, _gLeaf> &value) : value_(value) {}

  public:
    gtree() = default;

    static gtree<T1> gNode1(const T1 &p1, const gtree<T1> &p2, const gtree<T1> &p3) {
        return gtree<T1> { _gNode1 { p1, std::make_shared<gtree<T1>>(p2), std::make_shared<gtree<T1>>(p3) } };
    }
    static gtree<T1> gNode2(const gtree<T1> &p1, const T1 &p2) {
        return gtree<T1> { _gNode2 { std::make_shared<gtree<T1>>(p1), p2 } };
    }
    static gtree<T1> gLeaf() {
        return gtree<T1> { _gLeaf {  } };
    }

    bool is_gNode1() const { return std::holds_alternative<_gNode1>(value_); }
    bool is_gNode2() const { return std::holds_alternative<_gNode2>(value_); }
    bool is_gLeaf() const { return std::holds_alternative<_gLeaf>(value_); }

    const _gNode1 &as_gNode1() const { return std::get<_gNode1>(value_); }
    const _gNode2 &as_gNode2() const { return std::get<_gNode2>(value_); }

    bool operator<(const gtree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
using ctree = btree;
template<typename T1>
using htree = ftree;


// generated by HOL2Cpp
