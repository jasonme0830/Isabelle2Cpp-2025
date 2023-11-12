#include <cstdlib>
#include <memory>
#include <variant>

class snat {
    struct _sZero {
        bool operator<(const _sZero &) const { return false; }
    };
    struct _sSucc {
        std::shared_ptr<snat> p1_;

        snat p1() const { return *p1_; }

        bool operator<(const _sSucc &rhs) const {
            return std::tie(*p1_) < std::tie(*rhs.p1_);
        }
    };

    std::variant<_sZero, _sSucc> value_;
    snat(const std::variant<_sZero, _sSucc> &value) : value_(value) {}

  public:
    snat() = default;

    static snat sZero() {
        return snat { _sZero {  } };
    }
    static snat sSucc(const snat &p1) {
        return snat { _sSucc { std::make_shared<snat>(p1) } };
    }

    bool is_sZero() const { return std::holds_alternative<_sZero>(value_); }
    bool is_sSucc() const { return std::holds_alternative<_sSucc>(value_); }

    const _sSucc &as_sSucc() const { return std::get<_sSucc>(value_); }

    bool operator<(const snat &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class slist {
    struct _sNil {
        bool operator<(const _sNil &) const { return false; }
    };
    struct _sCons {
        T1 p1_;
        std::shared_ptr<slist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        slist<T1> p2() const { return *p2_; }

        bool operator<(const _sCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };

    std::variant<_sNil, _sCons> value_;
    slist(const std::variant<_sNil, _sCons> &value) : value_(value) {}

  public:
    slist() = default;

    static slist<T1> sNil() {
        return slist<T1> { _sNil {  } };
    }
    static slist<T1> sCons(const T1 &p1, const slist<T1> &p2) {
        return slist<T1> { _sCons { p1, std::make_shared<slist<T1>>(p2) } };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }

    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }

    bool operator<(const slist<T1> &rhs) const { return value_ < rhs.value_; }
};

class sbool {
    struct _sTrue {
        bool operator<(const _sTrue &) const { return false; }
    };
    struct _sFalse {
        bool operator<(const _sFalse &) const { return false; }
    };

    std::variant<_sTrue, _sFalse> value_;
    sbool(const std::variant<_sTrue, _sFalse> &value) : value_(value) {}

  public:
    sbool() = default;

    static sbool sTrue() {
        return sbool { _sTrue {  } };
    }
    static sbool sFalse() {
        return sbool { _sFalse {  } };
    }

    bool is_sTrue() const { return std::holds_alternative<_sTrue>(value_); }
    bool is_sFalse() const { return std::holds_alternative<_sFalse>(value_); }

    bool operator<(const sbool &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class soption {
    struct _None {
        bool operator<(const _None &) const { return false; }
    };
    struct _Some {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _Some &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };

    std::variant<_None, _Some> value_;
    soption(const std::variant<_None, _Some> &value) : value_(value) {}

  public:
    soption() = default;

    static soption<T1> None() {
        return soption<T1> { _None {  } };
    }
    static soption<T1> Some(const T1 &p1) {
        return soption<T1> { _Some { p1 } };
    }

    bool is_None() const { return std::holds_alternative<_None>(value_); }
    bool is_Some() const { return std::holds_alternative<_Some>(value_); }

    const _Some &as_Some() const { return std::get<_Some>(value_); }

    bool operator<(const soption<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class alist {
    struct _sNil {
        bool operator<(const _sNil &) const { return false; }
    };
    struct _sCons {
        T1 p1_;
        std::shared_ptr<alist<T1>> p2_;

        const T1 &p1() const { return p1_; }
        alist<T1> p2() const { return *p2_; }

        bool operator<(const _sCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };

    std::variant<_sNil, _sCons> value_;
    alist(const std::variant<_sNil, _sCons> &value) : value_(value) {}

  public:
    alist() = default;

    static alist<T1> sNil() {
        return alist<T1> { _sNil {  } };
    }
    static alist<T1> sCons(const T1 &p1, const alist<T1> &p2) {
        return alist<T1> { _sCons { p1, std::make_shared<alist<T1>>(p2) } };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value_); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value_); }

    const _sCons &as_sCons() const { return std::get<_sCons>(value_); }

    bool operator<(const alist<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class tree {
    struct _Leaf {
        bool operator<(const _Leaf &) const { return false; }
    };
    struct _Node {
        std::shared_ptr<tree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<tree<T1>> p3_;

        tree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        tree<T1> p3() const { return *p3_; }

        bool operator<(const _Node &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };

    std::variant<_Leaf, _Node> value_;
    tree(const std::variant<_Leaf, _Node> &value) : value_(value) {}

  public:
    tree() = default;

    static tree<T1> Leaf() {
        return tree<T1> { _Leaf {  } };
    }
    static tree<T1> Node(const tree<T1> &p1, const T1 &p2, const tree<T1> &p3) {
        return tree<T1> { _Node { std::make_shared<tree<T1>>(p1), p2, std::make_shared<tree<T1>>(p3) } };
    }

    bool is_Leaf() const { return std::holds_alternative<_Leaf>(value_); }
    bool is_Node() const { return std::holds_alternative<_Node>(value_); }

    const _Node &as_Node() const { return std::get<_Node>(value_); }

    bool operator<(const tree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1>
class stree {
    struct _sbool {
        bool operator<(const _sbool &) const { return false; }
    };
    struct _nat {
        bool operator<(const _nat &) const { return false; }
    };
    struct _tNode {
        std::shared_ptr<stree<T1>> p1_;
        std::shared_ptr<stree<T1>> p2_;
        std::shared_ptr<stree<T1>> p3_;

        stree<T1> p1() const { return *p1_; }
        stree<T1> p2() const { return *p2_; }
        stree<T1> p3() const { return *p3_; }

        bool operator<(const _tNode &rhs) const {
            return std::tie(*p1_, *p2_, *p3_) < std::tie(*rhs.p1_, *rhs.p2_, *rhs.p3_);
        }
    };
    struct _Node {
        std::shared_ptr<stree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<stree<T1>> p3_;

        stree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        stree<T1> p3() const { return *p3_; }

        bool operator<(const _Node &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };
    struct _nNode {
        nat p1_;
        std::shared_ptr<stree<T1>> p2_;

        const nat &p1() const { return p1_; }
        stree<T1> p2() const { return *p2_; }

        bool operator<(const _nNode &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
    };
    struct _lNode {
        sbool p1_;
        std::shared_ptr<stree<T1>> p2_;
        slist<T1> p3_;

        const sbool &p1() const { return p1_; }
        stree<T1> p2() const { return *p2_; }
        const slist<T1> &p3() const { return p3_; }

        bool operator<(const _lNode &rhs) const {
            return std::tie(p1_, *p2_, p3_) < std::tie(rhs.p1_, *rhs.p2_, rhs.p3_);
        }
    };

    std::variant<_sbool, _nat, _tNode, _Node, _nNode, _lNode> value_;
    stree(const std::variant<_sbool, _nat, _tNode, _Node, _nNode, _lNode> &value) : value_(value) {}

  public:
    stree() = default;

    static stree<T1> sbool() {
        return stree<T1> { _sbool {  } };
    }
    static stree<T1> nat() {
        return stree<T1> { _nat {  } };
    }
    static stree<T1> tNode(const stree<T1> &p1, const stree<T1> &p2, const stree<T1> &p3) {
        return stree<T1> { _tNode { std::make_shared<stree<T1>>(p1), std::make_shared<stree<T1>>(p2), std::make_shared<stree<T1>>(p3) } };
    }
    static stree<T1> Node(const stree<T1> &p1, const T1 &p2, const stree<T1> &p3) {
        return stree<T1> { _Node { std::make_shared<stree<T1>>(p1), p2, std::make_shared<stree<T1>>(p3) } };
    }
    static stree<T1> nNode(const nat &p1, const stree<T1> &p2) {
        return stree<T1> { _nNode { p1, std::make_shared<stree<T1>>(p2) } };
    }
    static stree<T1> lNode(const sbool &p1, const stree<T1> &p2, const slist<T1> &p3) {
        return stree<T1> { _lNode { p1, std::make_shared<stree<T1>>(p2), p3 } };
    }

    bool is_sbool() const { return std::holds_alternative<_sbool>(value_); }
    bool is_nat() const { return std::holds_alternative<_nat>(value_); }
    bool is_tNode() const { return std::holds_alternative<_tNode>(value_); }
    bool is_Node() const { return std::holds_alternative<_Node>(value_); }
    bool is_nNode() const { return std::holds_alternative<_nNode>(value_); }
    bool is_lNode() const { return std::holds_alternative<_lNode>(value_); }

    const _tNode &as_tNode() const { return std::get<_tNode>(value_); }
    const _Node &as_Node() const { return std::get<_Node>(value_); }
    const _nNode &as_nNode() const { return std::get<_nNode>(value_); }
    const _lNode &as_lNode() const { return std::get<_lNode>(value_); }

    bool operator<(const stree<T1> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1, typename T2>
class my_type {
    struct _Constructor1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _Constructor1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _Constructor2 {
        T2 p1_;
        list<T1> p2_;

        const T2 &p1() const { return p1_; }
        const list<T1> &p2() const { return p2_; }

        bool operator<(const _Constructor2 &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
    };

    std::variant<_Constructor1, _Constructor2> value_;
    my_type(const std::variant<_Constructor1, _Constructor2> &value) : value_(value) {}

  public:
    my_type() = default;

    static my_type<T1, T2> Constructor1(const T1 &p1) {
        return my_type<T1, T2> { _Constructor1 { p1 } };
    }
    static my_type<T1, T2> Constructor2(const T2 &p1, const list<T1> &p2) {
        return my_type<T1, T2> { _Constructor2 { p1, p2 } };
    }

    bool is_Constructor1() const { return std::holds_alternative<_Constructor1>(value_); }
    bool is_Constructor2() const { return std::holds_alternative<_Constructor2>(value_); }

    const _Constructor1 &as_Constructor1() const { return std::get<_Constructor1>(value_); }
    const _Constructor2 &as_Constructor2() const { return std::get<_Constructor2>(value_); }

    bool operator<(const my_type<T1, T2> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1, typename T2>
class ttree {
    struct _aNode {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _aNode &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _bNode {
        T2 p1_;

        const T2 &p1() const { return p1_; }

        bool operator<(const _bNode &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _asNode {
        std::shared_ptr<ttree<T1, T2>> p1_;
        T1 p2_;
        std::shared_ptr<ttree<T1, T2>> p3_;

        ttree<T1, T2> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        ttree<T1, T2> p3() const { return *p3_; }

        bool operator<(const _asNode &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
    };
    struct _sNode {
        stree<T1> p1_;
        T2 p2_;
        stree<T1> p3_;

        const stree<T1> &p1() const { return p1_; }
        const T2 &p2() const { return p2_; }
        const stree<T1> &p3() const { return p3_; }

        bool operator<(const _sNode &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
    };
    struct _lNode {
        stree<T2> p1_;
        T1 p2_;

        const stree<T2> &p1() const { return p1_; }
        const T1 &p2() const { return p2_; }

        bool operator<(const _lNode &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
    };

    std::variant<_aNode, _bNode, _asNode, _sNode, _lNode> value_;
    ttree(const std::variant<_aNode, _bNode, _asNode, _sNode, _lNode> &value) : value_(value) {}

  public:
    ttree() = default;

    static ttree<T1, T2> aNode(const T1 &p1) {
        return ttree<T1, T2> { _aNode { p1 } };
    }
    static ttree<T1, T2> bNode(const T2 &p1) {
        return ttree<T1, T2> { _bNode { p1 } };
    }
    static ttree<T1, T2> asNode(const ttree<T1, T2> &p1, const T1 &p2, const ttree<T1, T2> &p3) {
        return ttree<T1, T2> { _asNode { std::make_shared<ttree<T1, T2>>(p1), p2, std::make_shared<ttree<T1, T2>>(p3) } };
    }
    static ttree<T1, T2> sNode(const stree<T1> &p1, const T2 &p2, const stree<T1> &p3) {
        return ttree<T1, T2> { _sNode { p1, p2, p3 } };
    }
    static ttree<T1, T2> lNode(const stree<T2> &p1, const T1 &p2) {
        return ttree<T1, T2> { _lNode { p1, p2 } };
    }

    bool is_aNode() const { return std::holds_alternative<_aNode>(value_); }
    bool is_bNode() const { return std::holds_alternative<_bNode>(value_); }
    bool is_asNode() const { return std::holds_alternative<_asNode>(value_); }
    bool is_sNode() const { return std::holds_alternative<_sNode>(value_); }
    bool is_lNode() const { return std::holds_alternative<_lNode>(value_); }

    const _aNode &as_aNode() const { return std::get<_aNode>(value_); }
    const _bNode &as_bNode() const { return std::get<_bNode>(value_); }
    const _asNode &as_asNode() const { return std::get<_asNode>(value_); }
    const _sNode &as_sNode() const { return std::get<_sNode>(value_); }
    const _lNode &as_lNode() const { return std::get<_lNode>(value_); }

    bool operator<(const ttree<T1, T2> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1, typename T2>
class ytree {
    struct _aNode {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _aNode &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _bNode {
        T2 p1_;

        const T2 &p1() const { return p1_; }

        bool operator<(const _bNode &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _asNode {
        ttree<T1, T2> p1_;
        T1 p2_;
        ttree<T1, T2> p3_;

        const ttree<T1, T2> &p1() const { return p1_; }
        const T1 &p2() const { return p2_; }
        const ttree<T1, T2> &p3() const { return p3_; }

        bool operator<(const _asNode &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
    };
    struct _sNode {
        stree<T1> p1_;
        T2 p2_;
        stree<T1> p3_;

        const stree<T1> &p1() const { return p1_; }
        const T2 &p2() const { return p2_; }
        const stree<T1> &p3() const { return p3_; }

        bool operator<(const _sNode &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
    };
    struct _lNode {
        stree<T2> p1_;
        T1 p2_;

        const stree<T2> &p1() const { return p1_; }
        const T1 &p2() const { return p2_; }

        bool operator<(const _lNode &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
    };

    std::variant<_aNode, _bNode, _asNode, _sNode, _lNode> value_;
    ytree(const std::variant<_aNode, _bNode, _asNode, _sNode, _lNode> &value) : value_(value) {}

  public:
    ytree() = default;

    static ytree<T1, T2> aNode(const T1 &p1) {
        return ytree<T1, T2> { _aNode { p1 } };
    }
    static ytree<T1, T2> bNode(const T2 &p1) {
        return ytree<T1, T2> { _bNode { p1 } };
    }
    static ytree<T1, T2> asNode(const ttree<T1, T2> &p1, const T1 &p2, const ttree<T1, T2> &p3) {
        return ytree<T1, T2> { _asNode { p1, p2, p3 } };
    }
    static ytree<T1, T2> sNode(const stree<T1> &p1, const T2 &p2, const stree<T1> &p3) {
        return ytree<T1, T2> { _sNode { p1, p2, p3 } };
    }
    static ytree<T1, T2> lNode(const stree<T2> &p1, const T1 &p2) {
        return ytree<T1, T2> { _lNode { p1, p2 } };
    }

    bool is_aNode() const { return std::holds_alternative<_aNode>(value_); }
    bool is_bNode() const { return std::holds_alternative<_bNode>(value_); }
    bool is_asNode() const { return std::holds_alternative<_asNode>(value_); }
    bool is_sNode() const { return std::holds_alternative<_sNode>(value_); }
    bool is_lNode() const { return std::holds_alternative<_lNode>(value_); }

    const _aNode &as_aNode() const { return std::get<_aNode>(value_); }
    const _bNode &as_bNode() const { return std::get<_bNode>(value_); }
    const _asNode &as_asNode() const { return std::get<_asNode>(value_); }
    const _sNode &as_sNode() const { return std::get<_sNode>(value_); }
    const _lNode &as_lNode() const { return std::get<_lNode>(value_); }

    bool operator<(const ytree<T1, T2> &rhs) const { return value_ < rhs.value_; }
};

template<typename T1, typename T2>
class ptree {
    struct _aNode {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        bool operator<(const _aNode &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _bNode {
        T2 p1_;

        const T2 &p1() const { return p1_; }

        bool operator<(const _bNode &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
    };
    struct _asNode {
        ttree<T2, T1> p1_;
        T2 p2_;
        ttree<T2, T1> p3_;

        const ttree<T2, T1> &p1() const { return p1_; }
        const T2 &p2() const { return p2_; }
        const ttree<T2, T1> &p3() const { return p3_; }

        bool operator<(const _asNode &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
    };
    struct _sNode {
        stree<T2> p1_;
        T1 p2_;
        stree<T2> p3_;

        const stree<T2> &p1() const { return p1_; }
        const T1 &p2() const { return p2_; }
        const stree<T2> &p3() const { return p3_; }

        bool operator<(const _sNode &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
    };
    struct _lNode {
        stree<T1> p1_;
        T2 p2_;

        const stree<T1> &p1() const { return p1_; }
        const T2 &p2() const { return p2_; }

        bool operator<(const _lNode &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
    };

    std::variant<_aNode, _bNode, _asNode, _sNode, _lNode> value_;
    ptree(const std::variant<_aNode, _bNode, _asNode, _sNode, _lNode> &value) : value_(value) {}

  public:
    ptree() = default;

    static ptree<T1, T2> aNode(const T1 &p1) {
        return ptree<T1, T2> { _aNode { p1 } };
    }
    static ptree<T1, T2> bNode(const T2 &p1) {
        return ptree<T1, T2> { _bNode { p1 } };
    }
    static ptree<T1, T2> asNode(const ttree<T2, T1> &p1, const T2 &p2, const ttree<T2, T1> &p3) {
        return ptree<T1, T2> { _asNode { p1, p2, p3 } };
    }
    static ptree<T1, T2> sNode(const stree<T2> &p1, const T1 &p2, const stree<T2> &p3) {
        return ptree<T1, T2> { _sNode { p1, p2, p3 } };
    }
    static ptree<T1, T2> lNode(const stree<T1> &p1, const T2 &p2) {
        return ptree<T1, T2> { _lNode { p1, p2 } };
    }

    bool is_aNode() const { return std::holds_alternative<_aNode>(value_); }
    bool is_bNode() const { return std::holds_alternative<_bNode>(value_); }
    bool is_asNode() const { return std::holds_alternative<_asNode>(value_); }
    bool is_sNode() const { return std::holds_alternative<_sNode>(value_); }
    bool is_lNode() const { return std::holds_alternative<_lNode>(value_); }

    const _aNode &as_aNode() const { return std::get<_aNode>(value_); }
    const _bNode &as_bNode() const { return std::get<_bNode>(value_); }
    const _asNode &as_asNode() const { return std::get<_asNode>(value_); }
    const _sNode &as_sNode() const { return std::get<_sNode>(value_); }
    const _lNode &as_lNode() const { return std::get<_lNode>(value_); }

    bool operator<(const ptree<T1, T2> &rhs) const { return value_ < rhs.value_; }
};

// generated by HOL2Cpp
