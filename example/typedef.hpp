#include <cstdlib>
#include <memory>
#include <variant>

template<typename T1>
class yoption {
    struct _yNode {
        _yNode() {}
        _yNode(_yNode&& other) noexcept{ }
        bool operator<(const _yNode &) const { return false; }
        _yNode& operator=(_yNode&& other) noexcept { return *this; }
    };
    struct _yOnce {
        _yOnce() {}
        _yOnce(_yOnce&& other) noexcept{ }
        bool operator<(const _yOnce &) const { return false; }
        _yOnce& operator=(_yOnce&& other) noexcept { return *this; }
    };
    struct _ySome {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        _ySome(){
            // T1 p1_;
         }
        _ySome(_ySome&& other) noexcept 
            : p1_(std::move(other.p1_))
        {
        }
        bool operator<(const _ySome &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _ySome& operator=(_ySome&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };

    std::variant<_yNode, _yOnce, _ySome> value_;
    yoption(const std::variant<_yNode, _yOnce, _ySome> &value) : value_(value) {}

  public:
    yoption() = default;

    yoption(yoption<T1>&& other) noexcept :value_(other.value_){ }

    static yoption<T1> yNode() {
        return yoption<T1> { _yNode {  } };
    }
    static yoption<T1> yOnce() {
        return yoption<T1> { _yOnce {  } };
    }
    static yoption<T1> ySome(const T1 &p1) {
        return yoption<T1> { _ySome { p1 } };
    }

    bool is_yNode() const { return std::holds_alternative<_yNode>(value_); }
    bool is_yOnce() const { return std::holds_alternative<_yOnce>(value_); }
    bool is_ySome() const { return std::holds_alternative<_ySome>(value_); }

    const _ySome &as_ySome() const { return std::get<_ySome>(value_); }

    bool operator<(const yoption<T1> &rhs) const { return value_ < rhs.value_; }
    yoption<T1>& operator=(yoption<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class zoption {
    struct _zSome {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        _zSome(){
            // T1 p1_;
         }
        _zSome(_zSome&& other) noexcept 
            : p1_(std::move(other.p1_))
        {
        }
        bool operator<(const _zSome &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _zSome& operator=(_zSome&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };
    struct _zOnce {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        _zOnce(){
            // T1 p1_;
         }
        _zOnce(_zOnce&& other) noexcept 
            : p1_(std::move(other.p1_))
        {
        }
        bool operator<(const _zOnce &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _zOnce& operator=(_zOnce&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };
    struct _zNode {
        _zNode() {}
        _zNode(_zNode&& other) noexcept{ }
        bool operator<(const _zNode &) const { return false; }
        _zNode& operator=(_zNode&& other) noexcept { return *this; }
    };

    std::variant<_zSome, _zOnce, _zNode> value_;
    zoption(const std::variant<_zSome, _zOnce, _zNode> &value) : value_(value) {}

  public:
    zoption() = default;

    zoption(zoption<T1>&& other) noexcept :value_(other.value_){ }

    static zoption<T1> zSome(const T1 &p1) {
        return zoption<T1> { _zSome { p1 } };
    }
    static zoption<T1> zOnce(const T1 &p1) {
        return zoption<T1> { _zOnce { p1 } };
    }
    static zoption<T1> zNode() {
        return zoption<T1> { _zNode {  } };
    }

    bool is_zSome() const { return std::holds_alternative<_zSome>(value_); }
    bool is_zOnce() const { return std::holds_alternative<_zOnce>(value_); }
    bool is_zNode() const { return std::holds_alternative<_zNode>(value_); }

    const _zSome &as_zSome() const { return std::get<_zSome>(value_); }
    const _zOnce &as_zOnce() const { return std::get<_zOnce>(value_); }

    bool operator<(const zoption<T1> &rhs) const { return value_ < rhs.value_; }
    zoption<T1>& operator=(zoption<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class zlist {
    struct _zNil {
        _zNil() {}
        _zNil(_zNil&& other) noexcept{ }
        bool operator<(const _zNil &) const { return false; }
        _zNil& operator=(_zNil&& other) noexcept { return *this; }
    };
    struct _zCons {
        std::shared_ptr<zlist<T1>> p1_;
        T1 p2_;

        zlist<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }

        _zCons(){
            p1_ = nullptr;
            // T1 p2_;
         }
        _zCons(_zCons&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
        {
            other.p1_ = nullptr;
        }
        bool operator<(const _zCons &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
        _zCons& operator=(_zCons&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_zNil, _zCons> value_;
    zlist(const std::variant<_zNil, _zCons> &value) : value_(value) {}

  public:
    zlist() = default;

    zlist(zlist<T1>&& other) noexcept :value_(other.value_){ }

    static zlist<T1> zNil() {
        return zlist<T1> { _zNil {  } };
    }
    static zlist<T1> zCons(const zlist<T1> &p1, const T1 &p2) {
        return zlist<T1> { _zCons { std::make_shared<zlist<T1>>(p1), p2 } };
    }

    bool is_zNil() const { return std::holds_alternative<_zNil>(value_); }
    bool is_zCons() const { return std::holds_alternative<_zCons>(value_); }

    const _zCons &as_zCons() const { return std::get<_zCons>(value_); }

    bool operator<(const zlist<T1> &rhs) const { return value_ < rhs.value_; }
    zlist<T1>& operator=(zlist<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class xlist {
    struct _xNil {
        _xNil() {}
        _xNil(_xNil&& other) noexcept{ }
        bool operator<(const _xNil &) const { return false; }
        _xNil& operator=(_xNil&& other) noexcept { return *this; }
    };
    struct _xCons {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        _xCons(){
            // T1 p1_;
         }
        _xCons(_xCons&& other) noexcept 
            : p1_(std::move(other.p1_))
        {
        }
        bool operator<(const _xCons &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _xCons& operator=(_xCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };
    struct _xConss {
        std::shared_ptr<xlist<T1>> p1_;
        T1 p2_;

        xlist<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }

        _xConss(){
            p1_ = nullptr;
            // T1 p2_;
         }
        _xConss(_xConss&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
        {
            other.p1_ = nullptr;
        }
        bool operator<(const _xConss &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
        _xConss& operator=(_xConss&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_xNil, _xCons, _xConss> value_;
    xlist(const std::variant<_xNil, _xCons, _xConss> &value) : value_(value) {}

  public:
    xlist() = default;

    xlist(xlist<T1>&& other) noexcept :value_(other.value_){ }

    static xlist<T1> xNil() {
        return xlist<T1> { _xNil {  } };
    }
    static xlist<T1> xCons(const T1 &p1) {
        return xlist<T1> { _xCons { p1 } };
    }
    static xlist<T1> xConss(const xlist<T1> &p1, const T1 &p2) {
        return xlist<T1> { _xConss { std::make_shared<xlist<T1>>(p1), p2 } };
    }

    bool is_xNil() const { return std::holds_alternative<_xNil>(value_); }
    bool is_xCons() const { return std::holds_alternative<_xCons>(value_); }
    bool is_xConss() const { return std::holds_alternative<_xConss>(value_); }

    const _xCons &as_xCons() const { return std::get<_xCons>(value_); }
    const _xConss &as_xConss() const { return std::get<_xConss>(value_); }

    bool operator<(const xlist<T1> &rhs) const { return value_ < rhs.value_; }
    xlist<T1>& operator=(xlist<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class atree {
    struct _aLeaf {
        _aLeaf() {}
        _aLeaf(_aLeaf&& other) noexcept{ }
        bool operator<(const _aLeaf &) const { return false; }
        _aLeaf& operator=(_aLeaf&& other) noexcept { return *this; }
    };
    struct _aNode1 {
        std::shared_ptr<atree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<atree<T1>> p3_;

        atree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        atree<T1> p3() const { return *p3_; }

        _aNode1(){
            p1_ = nullptr;
            // T1 p2_;
            p3_ = nullptr;
         }
        _aNode1(_aNode1&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
            , p3_(other.p3_)
        {
            other.p1_ = nullptr;
            other.p3_ = nullptr;
        }
        bool operator<(const _aNode1 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _aNode1& operator=(_aNode1&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
                delete p3_;
                p3_ = other.p3_;
                other.p3_ = nullptr;
            }
            return *this;
        }
    };

    std::variant<_aLeaf, _aNode1> value_;
    atree(const std::variant<_aLeaf, _aNode1> &value) : value_(value) {}

  public:
    atree() = default;

    atree(atree<T1>&& other) noexcept :value_(other.value_){ }

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
    atree<T1>& operator=(atree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class btree {
    struct _bLeaf {
        _bLeaf() {}
        _bLeaf(_bLeaf&& other) noexcept{ }
        bool operator<(const _bLeaf &) const { return false; }
        _bLeaf& operator=(_bLeaf&& other) noexcept { return *this; }
    };
    struct _bNode1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        _bNode1(){
            // T1 p1_;
         }
        _bNode1(_bNode1&& other) noexcept 
            : p1_(std::move(other.p1_))
        {
        }
        bool operator<(const _bNode1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _bNode1& operator=(_bNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };
    struct _bNode2 {
        std::shared_ptr<btree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<btree<T1>> p3_;

        btree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        btree<T1> p3() const { return *p3_; }

        _bNode2(){
            p1_ = nullptr;
            // T1 p2_;
            p3_ = nullptr;
         }
        _bNode2(_bNode2&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
            , p3_(other.p3_)
        {
            other.p1_ = nullptr;
            other.p3_ = nullptr;
        }
        bool operator<(const _bNode2 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _bNode2& operator=(_bNode2&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
                delete p3_;
                p3_ = other.p3_;
                other.p3_ = nullptr;
            }
            return *this;
        }
    };

    std::variant<_bLeaf, _bNode1, _bNode2> value_;
    btree(const std::variant<_bLeaf, _bNode1, _bNode2> &value) : value_(value) {}

  public:
    btree() = default;

    btree(btree<T1>&& other) noexcept :value_(other.value_){ }

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
    btree<T1>& operator=(btree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class dtree {
    struct _dLeaf {
        _dLeaf() {}
        _dLeaf(_dLeaf&& other) noexcept{ }
        bool operator<(const _dLeaf &) const { return false; }
        _dLeaf& operator=(_dLeaf&& other) noexcept { return *this; }
    };
    struct _dNode1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }

        _dNode1(){
            // T1 p1_;
         }
        _dNode1(_dNode1&& other) noexcept 
            : p1_(std::move(other.p1_))
        {
        }
        bool operator<(const _dNode1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _dNode1& operator=(_dNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };
    struct _dNode2 {
        T1 p1_;
        std::shared_ptr<dtree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        dtree<T1> p2() const { return *p2_; }

        _dNode2(){
            // T1 p1_;
            p2_ = nullptr;
         }
        _dNode2(_dNode2&& other) noexcept 
            : p1_(std::move(other.p1_))
            , p2_(other.p2_)
        {
            other.p2_ = nullptr;
        }
        bool operator<(const _dNode2 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _dNode2& operator=(_dNode2&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                delete p2_;
                p2_ = other.p2_;
                other.p2_ = nullptr;
            }
            return *this;
        }
    };

    std::variant<_dLeaf, _dNode1, _dNode2> value_;
    dtree(const std::variant<_dLeaf, _dNode1, _dNode2> &value) : value_(value) {}

  public:
    dtree() = default;

    dtree(dtree<T1>&& other) noexcept :value_(other.value_){ }

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
    dtree<T1>& operator=(dtree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class etree {
    struct _eLeaf {
        _eLeaf() {}
        _eLeaf(_eLeaf&& other) noexcept{ }
        bool operator<(const _eLeaf &) const { return false; }
        _eLeaf& operator=(_eLeaf&& other) noexcept { return *this; }
    };
    struct _eNode1 {
        T1 p1_;
        std::shared_ptr<etree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        etree<T1> p2() const { return *p2_; }

        _eNode1(){
            // T1 p1_;
            p2_ = nullptr;
         }
        _eNode1(_eNode1&& other) noexcept 
            : p1_(std::move(other.p1_))
            , p2_(other.p2_)
        {
            other.p2_ = nullptr;
        }
        bool operator<(const _eNode1 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _eNode1& operator=(_eNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                delete p2_;
                p2_ = other.p2_;
                other.p2_ = nullptr;
            }
            return *this;
        }
    };
    struct _eNode2 {
        std::shared_ptr<etree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<etree<T1>> p3_;

        etree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        etree<T1> p3() const { return *p3_; }

        _eNode2(){
            p1_ = nullptr;
            // T1 p2_;
            p3_ = nullptr;
         }
        _eNode2(_eNode2&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
            , p3_(other.p3_)
        {
            other.p1_ = nullptr;
            other.p3_ = nullptr;
        }
        bool operator<(const _eNode2 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _eNode2& operator=(_eNode2&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
                delete p3_;
                p3_ = other.p3_;
                other.p3_ = nullptr;
            }
            return *this;
        }
    };

    std::variant<_eLeaf, _eNode1, _eNode2> value_;
    etree(const std::variant<_eLeaf, _eNode1, _eNode2> &value) : value_(value) {}

  public:
    etree() = default;

    etree(etree<T1>&& other) noexcept :value_(other.value_){ }

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
    etree<T1>& operator=(etree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class ftree {
    struct _fLeaf {
        _fLeaf() {}
        _fLeaf(_fLeaf&& other) noexcept{ }
        bool operator<(const _fLeaf &) const { return false; }
        _fLeaf& operator=(_fLeaf&& other) noexcept { return *this; }
    };
    struct _fNode1 {
        T1 p1_;
        std::shared_ptr<ftree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        ftree<T1> p2() const { return *p2_; }

        _fNode1(){
            // T1 p1_;
            p2_ = nullptr;
         }
        _fNode1(_fNode1&& other) noexcept 
            : p1_(std::move(other.p1_))
            , p2_(other.p2_)
        {
            other.p2_ = nullptr;
        }
        bool operator<(const _fNode1 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _fNode1& operator=(_fNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                delete p2_;
                p2_ = other.p2_;
                other.p2_ = nullptr;
            }
            return *this;
        }
    };
    struct _fNode2 {
        std::shared_ptr<ftree<T1>> p1_;
        std::shared_ptr<ftree<T1>> p2_;
        T1 p3_;

        ftree<T1> p1() const { return *p1_; }
        ftree<T1> p2() const { return *p2_; }
        const T1 &p3() const { return p3_; }

        _fNode2(){
            p1_ = nullptr;
            p2_ = nullptr;
            // T1 p3_;
         }
        _fNode2(_fNode2&& other) noexcept 
            : p1_(other.p1_)
            , p2_(other.p2_)
            , p3_(std::move(other.p3_))
        {
            other.p1_ = nullptr;
            other.p2_ = nullptr;
        }
        bool operator<(const _fNode2 &rhs) const {
            return std::tie(*p1_, *p2_, p3_) < std::tie(*rhs.p1_, *rhs.p2_, rhs.p3_);
        }
        _fNode2& operator=(_fNode2&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                delete p2_;
                p2_ = other.p2_;
                other.p2_ = nullptr;
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_fLeaf, _fNode1, _fNode2> value_;
    ftree(const std::variant<_fLeaf, _fNode1, _fNode2> &value) : value_(value) {}

  public:
    ftree() = default;

    ftree(ftree<T1>&& other) noexcept :value_(other.value_){ }

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
    ftree<T1>& operator=(ftree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
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

        _gNode1(){
            // T1 p1_;
            p2_ = nullptr;
            p3_ = nullptr;
         }
        _gNode1(_gNode1&& other) noexcept 
            : p1_(std::move(other.p1_))
            , p2_(other.p2_)
            , p3_(other.p3_)
        {
            other.p2_ = nullptr;
            other.p3_ = nullptr;
        }
        bool operator<(const _gNode1 &rhs) const {
            return std::tie(p1_, *p2_, *p3_) < std::tie(rhs.p1_, *rhs.p2_, *rhs.p3_);
        }
        _gNode1& operator=(_gNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                delete p2_;
                p2_ = other.p2_;
                other.p2_ = nullptr;
                delete p3_;
                p3_ = other.p3_;
                other.p3_ = nullptr;
            }
            return *this;
        }
    };
    struct _gNode2 {
        std::shared_ptr<gtree<T1>> p1_;
        T1 p2_;

        gtree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }

        _gNode2(){
            p1_ = nullptr;
            // T1 p2_;
         }
        _gNode2(_gNode2&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
        {
            other.p1_ = nullptr;
        }
        bool operator<(const _gNode2 &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
        _gNode2& operator=(_gNode2&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };
    struct _gLeaf {
        _gLeaf() {}
        _gLeaf(_gLeaf&& other) noexcept{ }
        bool operator<(const _gLeaf &) const { return false; }
        _gLeaf& operator=(_gLeaf&& other) noexcept { return *this; }
    };

    std::variant<_gNode1, _gNode2, _gLeaf> value_;
    gtree(const std::variant<_gNode1, _gNode2, _gLeaf> &value) : value_(value) {}

  public:
    gtree() = default;

    gtree(gtree<T1>&& other) noexcept :value_(other.value_){ }

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
    gtree<T1>& operator=(gtree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
class newTree {
    struct _newLeaf {
        _newLeaf() {}
        _newLeaf(_newLeaf&& other) noexcept{ }
        bool operator<(const _newLeaf &) const { return false; }
        _newLeaf& operator=(_newLeaf&& other) noexcept { return *this; }
    };
    struct _newNode {
        std::shared_ptr<newTree<T1>> p1_;
        zoption<T1> p2_;
        std::shared_ptr<newTree<T1>> p3_;

        newTree<T1> p1() const { return *p1_; }
        const zoption<T1> &p2() const { return p2_; }
        newTree<T1> p3() const { return *p3_; }

        _newNode(){
            p1_ = nullptr;
            // zoption<T1> p2_;
            p3_ = nullptr;
         }
        _newNode(_newNode&& other) noexcept 
            : p1_(other.p1_)
            , p2_(std::move(other.p2_))
            , p3_(other.p3_)
        {
            other.p1_ = nullptr;
            other.p3_ = nullptr;
        }
        bool operator<(const _newNode &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _newNode& operator=(_newNode&& other) noexcept {
            if(this != &other) {
                delete p1_;
                p1_ = other.p1_;
                other.p1_ = nullptr;
                p2_ = std::move(other.p2_);
                delete p3_;
                p3_ = other.p3_;
                other.p3_ = nullptr;
            }
            return *this;
        }
    };

    std::variant<_newLeaf, _newNode> value_;
    newTree(const std::variant<_newLeaf, _newNode> &value) : value_(value) {}

  public:
    newTree() = default;

    newTree(newTree<T1>&& other) noexcept :value_(other.value_){ }

    static newTree<T1> newLeaf() {
        return newTree<T1> { _newLeaf {  } };
    }
    static newTree<T1> newNode(const newTree<T1> &p1, const zoption<T1> &p2, const newTree<T1> &p3) {
        return newTree<T1> { _newNode { std::make_shared<newTree<T1>>(p1), p2, std::make_shared<newTree<T1>>(p3) } };
    }

    bool is_newLeaf() const { return std::holds_alternative<_newLeaf>(value_); }
    bool is_newNode() const { return std::holds_alternative<_newNode>(value_); }

    const _newNode &as_newNode() const { return std::get<_newNode>(value_); }

    bool operator<(const newTree<T1> &rhs) const { return value_ < rhs.value_; }
    newTree<T1>& operator=(newTree<T1>&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
};

template<typename T1>
using ctree = btree;
template<typename T1>
using htree = ftree;
template<typename T1>
using xoption = yoption;


// generated by HOL2Cpp
