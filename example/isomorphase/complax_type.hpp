#include <cstdlib>
#include <deque>
#include <memory>
#include <optional>
#include <variant>

template<typename T1>
class atree {
    struct _aLeaf {
        _aLeaf() {}
        _aLeaf(const _aLeaf& other){ }
        _aLeaf(_aLeaf&& other) noexcept{ }
        bool operator<(const _aLeaf &) const { return false; }
        _aLeaf& operator=(const _aLeaf& other) { return *this; }
        _aLeaf& operator=(_aLeaf&& other) noexcept { return *this; }
    };
    struct _aNode {
        std::shared_ptr<atree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<atree<T1>> p3_;

        atree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        atree<T1> p3() const { return *p3_; }


        _aNode(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3 )
            :p1_(std::make_shared<atree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<atree<T1>>(p3))
        {}
        _aNode(const _aNode& other)
            :p1_(std::make_shared<atree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<atree<T1>>(*other.p3_))
        {}
        _aNode(_aNode&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _aNode &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _aNode& operator=(const _aNode& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<atree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<atree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _aNode& operator=(_aNode&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_aLeaf, _aNode> value_;

  public:

    atree(const std::variant<_aLeaf, _aNode> &value) : value_(value) {}
    //深拷贝构造函数
    atree(const atree<T1>& other) { 
        if(std::holds_alternative<_aLeaf>(other.value_)){ 
            const _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_aNode>(other.value_)){ 
            const _aNode& other_node = std::get<_aNode>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    atree(atree<T1>&& other){
        if(std::holds_alternative<_aLeaf>(other.value_)){ 
            _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_aNode>(other.value_)){ 
            _aNode& other_node = std::get<_aNode>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static atree<T1> new_aLeaf() {
        return atree<T1> ( _aLeaf (  ) );
    }
    static atree<T1> new_aNode(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3) {
        return atree<T1> ( _aNode ( p1, p2, p3 ) );
    }

    bool is_aLeaf() const { return std::holds_alternative<_aLeaf>(value_); }
    bool is_aNode() const { return std::holds_alternative<_aNode>(value_); }
    const _aNode &as_aNode() const { return std::get<_aNode>(value_); }

    bool operator<(const atree<T1> &rhs) const { return value_ < rhs.value_; }
    atree<T1>& operator=(atree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_aLeaf>(other)){
                _aLeaf& other_value = std::get<_aLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_aNode>(other)){
                _aNode& other_value = std::get<_aNode>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    atree<T1>& operator=(const atree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_aLeaf>(other.value_)){ 
                const _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_aNode>(other.value_)){ 
                const _aNode& other_node = std::get<_aNode>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

class even_nat {
    struct _Even_Zero {
        _Even_Zero() {}
        _Even_Zero(const _Even_Zero& other){ }
        _Even_Zero(_Even_Zero&& other) noexcept{ }
        bool operator<(const _Even_Zero &) const { return false; }
        _Even_Zero& operator=(const _Even_Zero& other) { return *this; }
        _Even_Zero& operator=(_Even_Zero&& other) noexcept { return *this; }
    };
    struct _Even_Succ {
        odd_nat p1_;
        and p2_;
        odd_nat p3_;

        const odd_nat &p1() const { return p1_; }
        const and &p2() const { return p2_; }
        const odd_nat &p3() const { return p3_; }


        _Even_Succ(const odd_nat &p1, const and &p2, const odd_nat &p3 )
            :p1_(p1)
            ,p2_(p2)
            ,p3_(p3)
        {}
        _Even_Succ(const _Even_Succ& other)
            :p1_(other.p1_)
            ,p2_(other.p2_)
            ,p3_(other.p3_)
        {}
        _Even_Succ(_Even_Succ&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _Even_Succ &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
        _Even_Succ& operator=(const _Even_Succ& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_ = other.p2_; 
                p3_ = other.p3_; 
            } 
            return *this; 
        } 
        _Even_Succ& operator=(_Even_Succ&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_Even_Zero, _Even_Succ> value_;

  public:

    even_nat(const std::variant<_Even_Zero, _Even_Succ> &value) : value_(value) {}
    //深拷贝构造函数
    even_nat(const even_nat& other) { 
        if(std::holds_alternative<_Even_Zero>(other.value_)){ 
            const _Even_Zero& other_node = std::get<_Even_Zero>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_Even_Succ>(other.value_)){ 
            const _Even_Succ& other_node = std::get<_Even_Succ>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    even_nat(even_nat&& other){
        if(std::holds_alternative<_Even_Zero>(other.value_)){ 
            _Even_Zero& other_node = std::get<_Even_Zero>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_Even_Succ>(other.value_)){ 
            _Even_Succ& other_node = std::get<_Even_Succ>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static even_nat new_Even_Zero() {
        return even_nat ( _Even_Zero (  ) );
    }
    static even_nat new_Even_Succ(const odd_nat &p1, const and &p2, const odd_nat &p3) {
        return even_nat ( _Even_Succ ( p1, p2, p3 ) );
    }

    bool is_Even_Zero() const { return std::holds_alternative<_Even_Zero>(value_); }
    bool is_Even_Succ() const { return std::holds_alternative<_Even_Succ>(value_); }
    const _Even_Succ &as_Even_Succ() const { return std::get<_Even_Succ>(value_); }

    bool operator<(const even_nat &rhs) const { return value_ < rhs.value_; }
    even_nat& operator=(even_nat&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_Even_Zero>(other)){
                _Even_Zero& other_value = std::get<_Even_Zero>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_Even_Succ>(other)){
                _Even_Succ& other_value = std::get<_Even_Succ>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    even_nat& operator=(const even_nat& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_Even_Zero>(other.value_)){ 
                const _Even_Zero& other_node = std::get<_Even_Zero>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_Even_Succ>(other.value_)){ 
                const _Even_Succ& other_node = std::get<_Even_Succ>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1, typename T2>
class exp {
    struct _Term {
        trm<T1, T2> p1_;

        const trm<T1, T2> &p1() const { return p1_; }


        _Term(const trm<T1, T2> &p1 )
            :p1_(p1)
        {}
        _Term(const _Term& other)
            :p1_(other.p1_)
        {}
        _Term(_Term&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _Term &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _Term& operator=(const _Term& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
        } 
        _Term& operator=(_Term&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };
    struct _Sum {
        trm<T1, T2> p1_;
        std::shared_ptr<exp<T1, T2>> p2_;
        and p3_;

        const trm<T1, T2> &p1() const { return p1_; }
        exp<T1, T2> p2() const { return *p2_; }
        const and &p3() const { return p3_; }


        _Sum(const trm<T1, T2> &p1, const exp<T1, T2> &p2, const and &p3 )
            :p1_(p1)
            ,p2_(std::make_shared<exp<T1, T2>>(p2))
            ,p3_(p3)
        {}
        _Sum(const _Sum& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<exp<T1, T2>>(*other.p2_))
            ,p3_(other.p3_)
        {}
        _Sum(_Sum&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _Sum &rhs) const {
            return std::tie(p1_, *p2_, p3_) < std::tie(rhs.p1_, *rhs.p2_, rhs.p3_);
        }
        _Sum& operator=(const _Sum& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<exp<T1, T2>>(*other.p2_);
                p3_ = other.p3_; 
            } 
            return *this; 
        } 
        _Sum& operator=(_Sum&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_Term, _Sum> value_;

  public:

    exp(const std::variant<_Term, _Sum> &value) : value_(value) {}
    //深拷贝构造函数
    exp(const exp<T1, T2>& other) { 
        if(std::holds_alternative<_Term>(other.value_)){ 
            const _Term& other_node = std::get<_Term>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_Sum>(other.value_)){ 
            const _Sum& other_node = std::get<_Sum>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    exp(exp<T1, T2>&& other){
        if(std::holds_alternative<_Term>(other.value_)){ 
            _Term& other_node = std::get<_Term>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_Sum>(other.value_)){ 
            _Sum& other_node = std::get<_Sum>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static exp<T1, T2> new_Term(const trm<T1, T2> &p1) {
        return exp<T1, T2> ( _Term ( p1 ) );
    }
    static exp<T1, T2> new_Sum(const trm<T1, T2> &p1, const exp<T1, T2> &p2, const and &p3) {
        return exp<T1, T2> ( _Sum ( p1, p2, p3 ) );
    }

    bool is_Term() const { return std::holds_alternative<_Term>(value_); }
    bool is_Sum() const { return std::holds_alternative<_Sum>(value_); }
    const _Term &as_Term() const { return std::get<_Term>(value_); }
    const _Sum &as_Sum() const { return std::get<_Sum>(value_); }

    bool operator<(const exp<T1, T2> &rhs) const { return value_ < rhs.value_; }
    exp<T1, T2>& operator=(exp<T1, T2>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_Term>(other)){
                _Term& other_value = std::get<_Term>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_Sum>(other)){
                _Sum& other_value = std::get<_Sum>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    exp<T1, T2>& operator=(const exp<T1, T2>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_Term>(other.value_)){ 
                const _Term& other_node = std::get<_Term>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_Sum>(other.value_)){ 
                const _Sum& other_node = std::get<_Sum>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1, typename T2>
class clist {
    struct _cNil {
        _cNil() {}
        _cNil(const _cNil& other){ }
        _cNil(_cNil&& other) noexcept{ }
        bool operator<(const _cNil &) const { return false; }
        _cNil& operator=(const _cNil& other) { return *this; }
        _cNil& operator=(_cNil&& other) noexcept { return *this; }
    };
    struct _cCons {
        T1 p1_;
        std::shared_ptr<clist<T1, T2>> p2_;

        const T1 &p1() const { return p1_; }
        clist<T1, T2> p2() const { return *p2_; }


        _cCons(const T1 &p1, const clist<T1, T2> &p2 )
            :p1_(p1)
            ,p2_(std::make_shared<clist<T1, T2>>(p2))
        {}
        _cCons(const _cCons& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<clist<T1, T2>>(*other.p2_))
        {}
        _cCons(_cCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _cCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _cCons& operator=(const _cCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<clist<T1, T2>>(*other.p2_);
            } 
            return *this; 
        } 
        _cCons& operator=(_cCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_cNil, _cCons> value_;

  public:

    clist(const std::variant<_cNil, _cCons> &value) : value_(value) {}
    //深拷贝构造函数
    clist(const clist<T1, T2>& other) { 
        if(std::holds_alternative<_cNil>(other.value_)){ 
            const _cNil& other_node = std::get<_cNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_cCons>(other.value_)){ 
            const _cCons& other_node = std::get<_cCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    clist(clist<T1, T2>&& other){
        if(std::holds_alternative<_cNil>(other.value_)){ 
            _cNil& other_node = std::get<_cNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_cCons>(other.value_)){ 
            _cCons& other_node = std::get<_cCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static clist<T1, T2> new_cNil() {
        return clist<T1, T2> ( _cNil (  ) );
    }
    static clist<T1, T2> new_cCons(const T1 &p1, const clist<T1, T2> &p2) {
        return clist<T1, T2> ( _cCons ( p1, p2 ) );
    }

    bool is_cNil() const { return std::holds_alternative<_cNil>(value_); }
    bool is_cCons() const { return std::holds_alternative<_cCons>(value_); }
    const _cCons &as_cCons() const { return std::get<_cCons>(value_); }

    bool operator<(const clist<T1, T2> &rhs) const { return value_ < rhs.value_; }
    clist<T1, T2>& operator=(clist<T1, T2>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_cNil>(other)){
                _cNil& other_value = std::get<_cNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_cCons>(other)){
                _cCons& other_value = std::get<_cCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    clist<T1, T2>& operator=(const clist<T1, T2>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_cNil>(other.value_)){ 
                const _cNil& other_node = std::get<_cNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_cCons>(other.value_)){ 
                const _cCons& other_node = std::get<_cCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1, typename T2>
class dlist {
    struct _dNil {
        _dNil() {}
        _dNil(const _dNil& other){ }
        _dNil(_dNil&& other) noexcept{ }
        bool operator<(const _dNil &) const { return false; }
        _dNil& operator=(const _dNil& other) { return *this; }
        _dNil& operator=(_dNil&& other) noexcept { return *this; }
    };
    struct _dCons {
        T2 p1_;
        std::shared_ptr<dlist<T1, T2>> p2_;

        const T2 &p1() const { return p1_; }
        dlist<T1, T2> p2() const { return *p2_; }


        _dCons(const T2 &p1, const dlist<T1, T2> &p2 )
            :p1_(p1)
            ,p2_(std::make_shared<dlist<T1, T2>>(p2))
        {}
        _dCons(const _dCons& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<dlist<T1, T2>>(*other.p2_))
        {}
        _dCons(_dCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _dCons &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _dCons& operator=(const _dCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<dlist<T1, T2>>(*other.p2_);
            } 
            return *this; 
        } 
        _dCons& operator=(_dCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_dNil, _dCons> value_;

  public:

    dlist(const std::variant<_dNil, _dCons> &value) : value_(value) {}
    //深拷贝构造函数
    dlist(const dlist<T1, T2>& other) { 
        if(std::holds_alternative<_dNil>(other.value_)){ 
            const _dNil& other_node = std::get<_dNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_dCons>(other.value_)){ 
            const _dCons& other_node = std::get<_dCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    dlist(dlist<T1, T2>&& other){
        if(std::holds_alternative<_dNil>(other.value_)){ 
            _dNil& other_node = std::get<_dNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_dCons>(other.value_)){ 
            _dCons& other_node = std::get<_dCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static dlist<T1, T2> new_dNil() {
        return dlist<T1, T2> ( _dNil (  ) );
    }
    static dlist<T1, T2> new_dCons(const T2 &p1, const dlist<T1, T2> &p2) {
        return dlist<T1, T2> ( _dCons ( p1, p2 ) );
    }

    bool is_dNil() const { return std::holds_alternative<_dNil>(value_); }
    bool is_dCons() const { return std::holds_alternative<_dCons>(value_); }
    const _dCons &as_dCons() const { return std::get<_dCons>(value_); }

    bool operator<(const dlist<T1, T2> &rhs) const { return value_ < rhs.value_; }
    dlist<T1, T2>& operator=(dlist<T1, T2>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_dNil>(other)){
                _dNil& other_value = std::get<_dNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_dCons>(other)){
                _dCons& other_value = std::get<_dCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    dlist<T1, T2>& operator=(const dlist<T1, T2>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_dNil>(other.value_)){ 
                const _dNil& other_node = std::get<_dNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_dCons>(other.value_)){ 
                const _dCons& other_node = std::get<_dCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1, typename T2>
class flist {
    struct _fNil {
        _fNil() {}
        _fNil(const _fNil& other){ }
        _fNil(_fNil&& other) noexcept{ }
        bool operator<(const _fNil &) const { return false; }
        _fNil& operator=(const _fNil& other) { return *this; }
        _fNil& operator=(_fNil&& other) noexcept { return *this; }
    };
    struct _fCons {
        T1 p1_;
        clist<T2, T1> p2_;

        const T1 &p1() const { return p1_; }
        const clist<T2, T1> &p2() const { return p2_; }


        _fCons(const T1 &p1, const clist<T2, T1> &p2 )
            :p1_(p1)
            ,p2_(p2)
        {}
        _fCons(const _fCons& other)
            :p1_(other.p1_)
            ,p2_(other.p2_)
        {}
        _fCons(_fCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _fCons &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
        _fCons& operator=(const _fCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_ = other.p2_; 
            } 
            return *this; 
        } 
        _fCons& operator=(_fCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_fNil, _fCons> value_;

  public:

    flist(const std::variant<_fNil, _fCons> &value) : value_(value) {}
    //深拷贝构造函数
    flist(const flist<T1, T2>& other) { 
        if(std::holds_alternative<_fNil>(other.value_)){ 
            const _fNil& other_node = std::get<_fNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_fCons>(other.value_)){ 
            const _fCons& other_node = std::get<_fCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    flist(flist<T1, T2>&& other){
        if(std::holds_alternative<_fNil>(other.value_)){ 
            _fNil& other_node = std::get<_fNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_fCons>(other.value_)){ 
            _fCons& other_node = std::get<_fCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static flist<T1, T2> new_fNil() {
        return flist<T1, T2> ( _fNil (  ) );
    }
    static flist<T1, T2> new_fCons(const T1 &p1, const clist<T2, T1> &p2) {
        return flist<T1, T2> ( _fCons ( p1, p2 ) );
    }

    bool is_fNil() const { return std::holds_alternative<_fNil>(value_); }
    bool is_fCons() const { return std::holds_alternative<_fCons>(value_); }
    const _fCons &as_fCons() const { return std::get<_fCons>(value_); }

    bool operator<(const flist<T1, T2> &rhs) const { return value_ < rhs.value_; }
    flist<T1, T2>& operator=(flist<T1, T2>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_fNil>(other)){
                _fNil& other_value = std::get<_fNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_fCons>(other)){
                _fCons& other_value = std::get<_fCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    flist<T1, T2>& operator=(const flist<T1, T2>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_fNil>(other.value_)){ 
                const _fNil& other_node = std::get<_fNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_fCons>(other.value_)){ 
                const _fCons& other_node = std::get<_fCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1, typename T2>
class ilist {
    struct _iNil {
        _iNil() {}
        _iNil(const _iNil& other){ }
        _iNil(_iNil&& other) noexcept{ }
        bool operator<(const _iNil &) const { return false; }
        _iNil& operator=(const _iNil& other) { return *this; }
        _iNil& operator=(_iNil&& other) noexcept { return *this; }
    };
    struct _iCons {
        T1 p1_;
        clist<std::deque<T1>, T2> p2_;

        const T1 &p1() const { return p1_; }
        const clist<std::deque<T1>, T2> &p2() const { return p2_; }


        _iCons(const T1 &p1, const clist<std::deque<T1>, T2> &p2 )
            :p1_(p1)
            ,p2_(p2)
        {}
        _iCons(const _iCons& other)
            :p1_(other.p1_)
            ,p2_(other.p2_)
        {}
        _iCons(_iCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _iCons &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
        _iCons& operator=(const _iCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_ = other.p2_; 
            } 
            return *this; 
        } 
        _iCons& operator=(_iCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_iNil, _iCons> value_;

  public:

    ilist(const std::variant<_iNil, _iCons> &value) : value_(value) {}
    //深拷贝构造函数
    ilist(const ilist<T1, T2>& other) { 
        if(std::holds_alternative<_iNil>(other.value_)){ 
            const _iNil& other_node = std::get<_iNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_iCons>(other.value_)){ 
            const _iCons& other_node = std::get<_iCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    ilist(ilist<T1, T2>&& other){
        if(std::holds_alternative<_iNil>(other.value_)){ 
            _iNil& other_node = std::get<_iNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_iCons>(other.value_)){ 
            _iCons& other_node = std::get<_iCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static ilist<T1, T2> new_iNil() {
        return ilist<T1, T2> ( _iNil (  ) );
    }
    static ilist<T1, T2> new_iCons(const T1 &p1, const clist<std::deque<T1>, T2> &p2) {
        return ilist<T1, T2> ( _iCons ( p1, p2 ) );
    }

    bool is_iNil() const { return std::holds_alternative<_iNil>(value_); }
    bool is_iCons() const { return std::holds_alternative<_iCons>(value_); }
    const _iCons &as_iCons() const { return std::get<_iCons>(value_); }

    bool operator<(const ilist<T1, T2> &rhs) const { return value_ < rhs.value_; }
    ilist<T1, T2>& operator=(ilist<T1, T2>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_iNil>(other)){
                _iNil& other_value = std::get<_iNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_iCons>(other)){
                _iCons& other_value = std::get<_iCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    ilist<T1, T2>& operator=(const ilist<T1, T2>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_iNil>(other.value_)){ 
                const _iNil& other_node = std::get<_iNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_iCons>(other.value_)){ 
                const _iCons& other_node = std::get<_iCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1, typename T2>
class klist {
    struct _kNil {
        _kNil() {}
        _kNil(const _kNil& other){ }
        _kNil(_kNil&& other) noexcept{ }
        bool operator<(const _kNil &) const { return false; }
        _kNil& operator=(const _kNil& other) { return *this; }
        _kNil& operator=(_kNil&& other) noexcept { return *this; }
    };
    struct _kCons {
        T1 p1_;
        clist<klist<T1, T2>, T1> p2_;

        const T1 &p1() const { return p1_; }
        const clist<klist<T1, T2>, T1> &p2() const { return p2_; }


        _kCons(const T1 &p1, const clist<klist<T1, T2>, T1> &p2 )
            :p1_(p1)
            ,p2_(p2)
        {}
        _kCons(const _kCons& other)
            :p1_(other.p1_)
            ,p2_(other.p2_)
        {}
        _kCons(_kCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _kCons &rhs) const {
            return std::tie(p1_, p2_) < std::tie(rhs.p1_, rhs.p2_);
        }
        _kCons& operator=(const _kCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_ = other.p2_; 
            } 
            return *this; 
        } 
        _kCons& operator=(_kCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_kNil, _kCons> value_;

  public:

    klist(const std::variant<_kNil, _kCons> &value) : value_(value) {}
    //深拷贝构造函数
    klist(const klist<T1, T2>& other) { 
        if(std::holds_alternative<_kNil>(other.value_)){ 
            const _kNil& other_node = std::get<_kNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_kCons>(other.value_)){ 
            const _kCons& other_node = std::get<_kCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    klist(klist<T1, T2>&& other){
        if(std::holds_alternative<_kNil>(other.value_)){ 
            _kNil& other_node = std::get<_kNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_kCons>(other.value_)){ 
            _kCons& other_node = std::get<_kCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static klist<T1, T2> new_kNil() {
        return klist<T1, T2> ( _kNil (  ) );
    }
    static klist<T1, T2> new_kCons(const T1 &p1, const clist<klist<T1, T2>, T1> &p2) {
        return klist<T1, T2> ( _kCons ( p1, p2 ) );
    }

    bool is_kNil() const { return std::holds_alternative<_kNil>(value_); }
    bool is_kCons() const { return std::holds_alternative<_kCons>(value_); }
    const _kCons &as_kCons() const { return std::get<_kCons>(value_); }

    bool operator<(const klist<T1, T2> &rhs) const { return value_ < rhs.value_; }
    klist<T1, T2>& operator=(klist<T1, T2>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_kNil>(other)){
                _kNil& other_value = std::get<_kNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_kCons>(other)){
                _kCons& other_value = std::get<_kCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    klist<T1, T2>& operator=(const klist<T1, T2>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_kNil>(other.value_)){ 
                const _kNil& other_node = std::get<_kNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_kCons>(other.value_)){ 
                const _kCons& other_node = std::get<_kCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class etree {
    struct _eLeaf {
        _eLeaf() {}
        _eLeaf(const _eLeaf& other){ }
        _eLeaf(_eLeaf&& other) noexcept{ }
        bool operator<(const _eLeaf &) const { return false; }
        _eLeaf& operator=(const _eLeaf& other) { return *this; }
        _eLeaf& operator=(_eLeaf&& other) noexcept { return *this; }
    };
    struct _eNode {
        T1 p1_;
        std::optional<etree<T1>> p2_;
        std::optional<etree<T1>> p3_;

        const T1 &p1() const { return p1_; }
        const std::optional<etree<T1>> &p2() const { return p2_; }
        const std::optional<etree<T1>> &p3() const { return p3_; }


        _eNode(const T1 &p1, const std::optional<etree<T1>> &p2, const std::optional<etree<T1>> &p3 )
            :p1_(p1)
            ,p2_(p2)
            ,p3_(p3)
        {}
        _eNode(const _eNode& other)
            :p1_(other.p1_)
            ,p2_(other.p2_)
            ,p3_(other.p3_)
        {}
        _eNode(_eNode&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _eNode &rhs) const {
            return std::tie(p1_, p2_, p3_) < std::tie(rhs.p1_, rhs.p2_, rhs.p3_);
        }
        _eNode& operator=(const _eNode& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_ = other.p2_; 
                p3_ = other.p3_; 
            } 
            return *this; 
        } 
        _eNode& operator=(_eNode&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_eLeaf, _eNode> value_;

  public:

    etree(const std::variant<_eLeaf, _eNode> &value) : value_(value) {}
    //深拷贝构造函数
    etree(const etree<T1>& other) { 
        if(std::holds_alternative<_eLeaf>(other.value_)){ 
            const _eLeaf& other_node = std::get<_eLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_eNode>(other.value_)){ 
            const _eNode& other_node = std::get<_eNode>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    etree(etree<T1>&& other){
        if(std::holds_alternative<_eLeaf>(other.value_)){ 
            _eLeaf& other_node = std::get<_eLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_eNode>(other.value_)){ 
            _eNode& other_node = std::get<_eNode>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static etree<T1> new_eLeaf() {
        return etree<T1> ( _eLeaf (  ) );
    }
    static etree<T1> new_eNode(const T1 &p1, const std::optional<etree<T1>> &p2, const std::optional<etree<T1>> &p3) {
        return etree<T1> ( _eNode ( p1, p2, p3 ) );
    }

    bool is_eLeaf() const { return std::holds_alternative<_eLeaf>(value_); }
    bool is_eNode() const { return std::holds_alternative<_eNode>(value_); }
    const _eNode &as_eNode() const { return std::get<_eNode>(value_); }

    bool operator<(const etree<T1> &rhs) const { return value_ < rhs.value_; }
    etree<T1>& operator=(etree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_eLeaf>(other)){
                _eLeaf& other_value = std::get<_eLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_eNode>(other)){
                _eNode& other_value = std::get<_eNode>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    etree<T1>& operator=(const etree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_eLeaf>(other.value_)){ 
                const _eLeaf& other_node = std::get<_eLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_eNode>(other.value_)){ 
                const _eNode& other_node = std::get<_eNode>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
using btree = atree;
template<typename T1>
using ctree = atree;
template<typename T1>
using dtree = atree;
template<typename T1, typename T2>
using glist = clist;
template<typename T1, typename T2>
using hlist = clist;


// generated by HOL2Cpp
