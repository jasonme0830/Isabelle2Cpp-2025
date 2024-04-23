#include <cstdlib>
#include <memory>
#include <variant>
#include <list>

using namespace std;

template<typename T1>
class yoption {
    struct _yNode {
        _yNode() {}
        _yNode(const _yNode& other){ }
        _yNode(_yNode&& other) noexcept{ }
        bool operator<(const _yNode &) const { return false; }
        _yNode& operator=(const _yNode& other) { return *this; }
        _yNode& operator=(_yNode&& other) noexcept { return *this; }
    };
    struct _yOnce {
        _yOnce() {}
        _yOnce(const _yOnce& other){ }
        _yOnce(_yOnce&& other) noexcept{ }
        bool operator<(const _yOnce &) const { return false; }
        _yOnce& operator=(const _yOnce& other) { return *this; }
        _yOnce& operator=(_yOnce&& other) noexcept { return *this; }
    };
    struct _ySome {
        T1 p1_;

        const T1 &p1() const { return p1_; }


        _ySome(const T1 &p1 )
            :p1_(p1)
        {}
        _ySome(const _ySome& other)
            :p1_(other.p1_)
        {}
        _ySome(_ySome&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _ySome &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _ySome& operator=(const _ySome& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
        } 
        _ySome& operator=(_ySome&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
            }
            return *this;
        }
    };

    std::variant<_yNode, _yOnce, _ySome> value_;

  public:

    yoption(const std::variant<_yNode, _yOnce, _ySome> &value) : value_(value) {}
    //深拷贝构造函数
    yoption(const yoption<T1>& other) { 
        if(std::holds_alternative<_yNode>(other.value_)){ 
            const _yNode& other_node = std::get<_yNode>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_yOnce>(other.value_)){ 
            const _yOnce& other_node = std::get<_yOnce>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_ySome>(other.value_)){ 
            const _ySome& other_node = std::get<_ySome>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    yoption(yoption<T1>&& other){
        if(std::holds_alternative<_yNode>(other.value_)){ 
            _yNode& other_node = std::get<_yNode>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_yOnce>(other.value_)){ 
            _yOnce& other_node = std::get<_yOnce>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_ySome>(other.value_)){ 
            _ySome& other_node = std::get<_ySome>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static yoption<T1> new_yNode() {
        return yoption<T1> ( _yNode (  ) );
    }
    static yoption<T1> new_yOnce() {
        return yoption<T1> ( _yOnce (  ) );
    }
    static yoption<T1> new_ySome(const T1 &p1) {
        return yoption<T1> ( _ySome ( p1 ) );
    }

    bool is_yNode() const { return std::holds_alternative<_yNode>(value_); }
    bool is_yOnce() const { return std::holds_alternative<_yOnce>(value_); }
    bool is_ySome() const { return std::holds_alternative<_ySome>(value_); }
    const _ySome &as_ySome() const { return std::get<_ySome>(value_); }

    bool operator<(const yoption<T1> &rhs) const { return value_ < rhs.value_; }
    yoption<T1>& operator=(yoption<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_yNode>(other)){
                _yNode& other_value = std::get<_yNode>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_yOnce>(other)){
                _yOnce& other_value = std::get<_yOnce>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_ySome>(other)){
                _ySome& other_value = std::get<_ySome>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    yoption<T1>& operator=(const yoption<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_yNode>(other.value_)){ 
                const _yNode& other_node = std::get<_yNode>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_yOnce>(other.value_)){ 
                const _yOnce& other_node = std::get<_yOnce>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_ySome>(other.value_)){ 
                const _ySome& other_node = std::get<_ySome>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class zoption {
    struct _zSome {
        T1 p1_;

        const T1 &p1() const { return p1_; }


        _zSome(const T1 &p1 )
            :p1_(p1)
        {}
        _zSome(const _zSome& other)
            :p1_(other.p1_)
        {}
        _zSome(_zSome&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _zSome &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _zSome& operator=(const _zSome& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
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


        _zOnce(const T1 &p1 )
            :p1_(p1)
        {}
        _zOnce(const _zOnce& other)
            :p1_(other.p1_)
        {}
        _zOnce(_zOnce&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _zOnce &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _zOnce& operator=(const _zOnce& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
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
        _zNode(const _zNode& other){ }
        _zNode(_zNode&& other) noexcept{ }
        bool operator<(const _zNode &) const { return false; }
        _zNode& operator=(const _zNode& other) { return *this; }
        _zNode& operator=(_zNode&& other) noexcept { return *this; }
    };

    std::variant<_zSome, _zOnce, _zNode> value_;

  public:

    zoption(const std::variant<_zSome, _zOnce, _zNode> &value) : value_(value) {}
    //深拷贝构造函数
    zoption(const zoption<T1>& other) { 
        if(std::holds_alternative<_zSome>(other.value_)){ 
            const _zSome& other_node = std::get<_zSome>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_zOnce>(other.value_)){ 
            const _zOnce& other_node = std::get<_zOnce>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_zNode>(other.value_)){ 
            const _zNode& other_node = std::get<_zNode>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    zoption(zoption<T1>&& other){
        if(std::holds_alternative<_zSome>(other.value_)){ 
            _zSome& other_node = std::get<_zSome>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_zOnce>(other.value_)){ 
            _zOnce& other_node = std::get<_zOnce>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_zNode>(other.value_)){ 
            _zNode& other_node = std::get<_zNode>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static zoption<T1> new_zSome(const T1 &p1) {
        return zoption<T1> ( _zSome ( p1 ) );
    }
    static zoption<T1> new_zOnce(const T1 &p1) {
        return zoption<T1> ( _zOnce ( p1 ) );
    }
    static zoption<T1> new_zNode() {
        return zoption<T1> ( _zNode (  ) );
    }

    bool is_zSome() const { return std::holds_alternative<_zSome>(value_); }
    bool is_zOnce() const { return std::holds_alternative<_zOnce>(value_); }
    bool is_zNode() const { return std::holds_alternative<_zNode>(value_); }
    const _zSome &as_zSome() const { return std::get<_zSome>(value_); }
    const _zOnce &as_zOnce() const { return std::get<_zOnce>(value_); }

    bool operator<(const zoption<T1> &rhs) const { return value_ < rhs.value_; }
    zoption<T1>& operator=(zoption<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_zSome>(other)){
                _zSome& other_value = std::get<_zSome>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_zOnce>(other)){
                _zOnce& other_value = std::get<_zOnce>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_zNode>(other)){
                _zNode& other_value = std::get<_zNode>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    zoption<T1>& operator=(const zoption<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_zSome>(other.value_)){ 
                const _zSome& other_node = std::get<_zSome>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_zOnce>(other.value_)){ 
                const _zOnce& other_node = std::get<_zOnce>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_zNode>(other.value_)){ 
                const _zNode& other_node = std::get<_zNode>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class zlist {
    struct _zNil {
        _zNil() {}
        _zNil(const _zNil& other){ }
        _zNil(_zNil&& other) noexcept{ }
        bool operator<(const _zNil &) const { return false; }
        _zNil& operator=(const _zNil& other) { return *this; }
        _zNil& operator=(_zNil&& other) noexcept { return *this; }
    };
    struct _zCons {
        std::shared_ptr<zlist<T1>> p1_;
        T1 p2_;

        zlist<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }


        _zCons(const zlist<T1> &p1, const T1 &p2 )
            :p1_(std::make_shared<zlist<T1>>(p1))
            ,p2_(p2)
        {}
        _zCons(const _zCons& other)
            :p1_(std::make_shared<zlist<T1>>(*other.p1_))
            ,p2_(other.p2_)
        {}
        _zCons(_zCons&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _zCons &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
        _zCons& operator=(const _zCons& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<zlist<T1>>(*other.p1_);
                p2_ = other.p2_; 
            } 
            return *this; 
        } 
        _zCons& operator=(_zCons&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_zNil, _zCons> value_;

  public:

    zlist(const std::variant<_zNil, _zCons> &value) : value_(value) {}
    //深拷贝构造函数
    zlist(const zlist<T1>& other) { 
        if(std::holds_alternative<_zNil>(other.value_)){ 
            const _zNil& other_node = std::get<_zNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_zCons>(other.value_)){ 
            const _zCons& other_node = std::get<_zCons>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    zlist(zlist<T1>&& other){
        if(std::holds_alternative<_zNil>(other.value_)){ 
            _zNil& other_node = std::get<_zNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_zCons>(other.value_)){ 
            _zCons& other_node = std::get<_zCons>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static zlist<T1> new_zNil() {
        return zlist<T1> ( _zNil (  ) );
    }
    static zlist<T1> new_zCons(const zlist<T1> &p1, const T1 &p2) {
        return zlist<T1> ( _zCons ( p1, p2 ) );
    }

    bool is_zNil() const { return std::holds_alternative<_zNil>(value_); }
    bool is_zCons() const { return std::holds_alternative<_zCons>(value_); }
    const _zCons &as_zCons() const { return std::get<_zCons>(value_); }

    bool operator<(const zlist<T1> &rhs) const { return value_ < rhs.value_; }
    zlist<T1>& operator=(zlist<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_zNil>(other)){
                _zNil& other_value = std::get<_zNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_zCons>(other)){
                _zCons& other_value = std::get<_zCons>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    zlist<T1>& operator=(const zlist<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_zNil>(other.value_)){ 
                const _zNil& other_node = std::get<_zNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_zCons>(other.value_)){ 
                const _zCons& other_node = std::get<_zCons>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class xlist {
    struct _xNil {
        _xNil() {}
        _xNil(const _xNil& other){ }
        _xNil(_xNil&& other) noexcept{ }
        bool operator<(const _xNil &) const { return false; }
        _xNil& operator=(const _xNil& other) { return *this; }
        _xNil& operator=(_xNil&& other) noexcept { return *this; }
    };
    struct _xCons {
        T1 p1_;

        const T1 &p1() const { return p1_; }


        _xCons(const T1 &p1 )
            :p1_(p1)
        {}
        _xCons(const _xCons& other)
            :p1_(other.p1_)
        {}
        _xCons(_xCons&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _xCons &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _xCons& operator=(const _xCons& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
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


        _xConss(const xlist<T1> &p1, const T1 &p2 )
            :p1_(std::make_shared<xlist<T1>>(p1))
            ,p2_(p2)
        {}
        _xConss(const _xConss& other)
            :p1_(std::make_shared<xlist<T1>>(*other.p1_))
            ,p2_(other.p2_)
        {}
        _xConss(_xConss&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _xConss &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
        _xConss& operator=(const _xConss& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<xlist<T1>>(*other.p1_);
                p2_ = other.p2_; 
            } 
            return *this; 
        } 
        _xConss& operator=(_xConss&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_xNil, _xCons, _xConss> value_;

  public:

    xlist(const std::variant<_xNil, _xCons, _xConss> &value) : value_(value) {}
    //深拷贝构造函数
    xlist(const xlist<T1>& other) { 
        if(std::holds_alternative<_xNil>(other.value_)){ 
            const _xNil& other_node = std::get<_xNil>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_xCons>(other.value_)){ 
            const _xCons& other_node = std::get<_xCons>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_xConss>(other.value_)){ 
            const _xConss& other_node = std::get<_xConss>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    xlist(xlist<T1>&& other){
        if(std::holds_alternative<_xNil>(other.value_)){ 
            _xNil& other_node = std::get<_xNil>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_xCons>(other.value_)){ 
            _xCons& other_node = std::get<_xCons>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_xConss>(other.value_)){ 
            _xConss& other_node = std::get<_xConss>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static xlist<T1> new_xNil() {
        return xlist<T1> ( _xNil (  ) );
    }
    static xlist<T1> new_xCons(const T1 &p1) {
        return xlist<T1> ( _xCons ( p1 ) );
    }
    static xlist<T1> new_xConss(const xlist<T1> &p1, const T1 &p2) {
        return xlist<T1> ( _xConss ( p1, p2 ) );
    }

    bool is_xNil() const { return std::holds_alternative<_xNil>(value_); }
    bool is_xCons() const { return std::holds_alternative<_xCons>(value_); }
    bool is_xConss() const { return std::holds_alternative<_xConss>(value_); }
    const _xCons &as_xCons() const { return std::get<_xCons>(value_); }
    const _xConss &as_xConss() const { return std::get<_xConss>(value_); }

    bool operator<(const xlist<T1> &rhs) const { return value_ < rhs.value_; }
    xlist<T1>& operator=(xlist<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_xNil>(other)){
                _xNil& other_value = std::get<_xNil>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_xCons>(other)){
                _xCons& other_value = std::get<_xCons>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_xConss>(other)){
                _xConss& other_value = std::get<_xConss>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    xlist<T1>& operator=(const xlist<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_xNil>(other.value_)){ 
                const _xNil& other_node = std::get<_xNil>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_xCons>(other.value_)){ 
                const _xCons& other_node = std::get<_xCons>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_xConss>(other.value_)){ 
                const _xConss& other_node = std::get<_xConss>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

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
    struct _aNode1 {
        std::shared_ptr<atree<T1>> p1_;
        T1 p2_;
        std::shared_ptr<atree<T1>> p3_;

        atree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }
        atree<T1> p3() const { return *p3_; }


        _aNode1(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3 )
            :p1_(std::make_shared<atree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<atree<T1>>(p3))
        {}
        _aNode1(const _aNode1& other)
            :p1_(std::make_shared<atree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<atree<T1>>(*other.p3_))
        {}
        _aNode1(_aNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _aNode1 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _aNode1& operator=(const _aNode1& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<atree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<atree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _aNode1& operator=(_aNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_aLeaf, _aNode1> value_;

  public:

    atree(const std::variant<_aLeaf, _aNode1> &value) : value_(value) {}
    //深拷贝构造函数
    atree(const atree<T1>& other) { 
        if(std::holds_alternative<_aLeaf>(other.value_)){ 
            const _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_aNode1>(other.value_)){ 
            const _aNode1& other_node = std::get<_aNode1>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    atree(atree<T1>&& other){
        if(std::holds_alternative<_aLeaf>(other.value_)){ 
            _aLeaf& other_node = std::get<_aLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_aNode1>(other.value_)){ 
            _aNode1& other_node = std::get<_aNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static atree<T1> new_aLeaf() {
        return atree<T1> ( _aLeaf (  ) );
    }
    static atree<T1> new_aNode1(const atree<T1> &p1, const T1 &p2, const atree<T1> &p3) {
        return atree<T1> ( _aNode1 ( p1, p2, p3 ) );
    }

    bool is_aLeaf() const { return std::holds_alternative<_aLeaf>(value_); }
    bool is_aNode1() const { return std::holds_alternative<_aNode1>(value_); }
    const _aNode1 &as_aNode1() const { return std::get<_aNode1>(value_); }

    bool operator<(const atree<T1> &rhs) const { return value_ < rhs.value_; }
    atree<T1>& operator=(atree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_aLeaf>(other)){
                _aLeaf& other_value = std::get<_aLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_aNode1>(other)){
                _aNode1& other_value = std::get<_aNode1>(other.value_);
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
            if(std::holds_alternative<_aNode1>(other.value_)){ 
                const _aNode1& other_node = std::get<_aNode1>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class btree {
    struct _bLeaf {
        _bLeaf() {}
        _bLeaf(const _bLeaf& other){ }
        _bLeaf(_bLeaf&& other) noexcept{ }
        bool operator<(const _bLeaf &) const { return false; }
        _bLeaf& operator=(const _bLeaf& other) { return *this; }
        _bLeaf& operator=(_bLeaf&& other) noexcept { return *this; }
    };
    struct _bNode1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }


        _bNode1(const T1 &p1 )
            :p1_(p1)
        {}
        _bNode1(const _bNode1& other)
            :p1_(other.p1_)
        {}
        _bNode1(_bNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _bNode1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _bNode1& operator=(const _bNode1& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
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


        _bNode2(const btree<T1> &p1, const T1 &p2, const btree<T1> &p3 )
            :p1_(std::make_shared<btree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<btree<T1>>(p3))
        {}
        _bNode2(const _bNode2& other)
            :p1_(std::make_shared<btree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<btree<T1>>(*other.p3_))
        {}
        _bNode2(_bNode2&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _bNode2 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _bNode2& operator=(const _bNode2& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<btree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<btree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _bNode2& operator=(_bNode2&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_bLeaf, _bNode1, _bNode2> value_;

  public:

    btree(const std::variant<_bLeaf, _bNode1, _bNode2> &value) : value_(value) {}
    //深拷贝构造函数
    btree(const btree<T1>& other) { 
        if(std::holds_alternative<_bLeaf>(other.value_)){ 
            const _bLeaf& other_node = std::get<_bLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_bNode1>(other.value_)){ 
            const _bNode1& other_node = std::get<_bNode1>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_bNode2>(other.value_)){ 
            const _bNode2& other_node = std::get<_bNode2>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    btree(btree<T1>&& other){
        if(std::holds_alternative<_bLeaf>(other.value_)){ 
            _bLeaf& other_node = std::get<_bLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_bNode1>(other.value_)){ 
            _bNode1& other_node = std::get<_bNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_bNode2>(other.value_)){ 
            _bNode2& other_node = std::get<_bNode2>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static btree<T1> new_bLeaf() {
        return btree<T1> ( _bLeaf (  ) );
    }
    static btree<T1> new_bNode1(const T1 &p1) {
        return btree<T1> ( _bNode1 ( p1 ) );
    }
    static btree<T1> new_bNode2(const btree<T1> &p1, const T1 &p2, const btree<T1> &p3) {
        return btree<T1> ( _bNode2 ( p1, p2, p3 ) );
    }

    bool is_bLeaf() const { return std::holds_alternative<_bLeaf>(value_); }
    bool is_bNode1() const { return std::holds_alternative<_bNode1>(value_); }
    bool is_bNode2() const { return std::holds_alternative<_bNode2>(value_); }
    const _bNode1 &as_bNode1() const { return std::get<_bNode1>(value_); }
    const _bNode2 &as_bNode2() const { return std::get<_bNode2>(value_); }

    bool operator<(const btree<T1> &rhs) const { return value_ < rhs.value_; }
    btree<T1>& operator=(btree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_bLeaf>(other)){
                _bLeaf& other_value = std::get<_bLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_bNode1>(other)){
                _bNode1& other_value = std::get<_bNode1>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_bNode2>(other)){
                _bNode2& other_value = std::get<_bNode2>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    btree<T1>& operator=(const btree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_bLeaf>(other.value_)){ 
                const _bLeaf& other_node = std::get<_bLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_bNode1>(other.value_)){ 
                const _bNode1& other_node = std::get<_bNode1>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_bNode2>(other.value_)){ 
                const _bNode2& other_node = std::get<_bNode2>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class dtree {
    struct _dLeaf {
        _dLeaf() {}
        _dLeaf(const _dLeaf& other){ }
        _dLeaf(_dLeaf&& other) noexcept{ }
        bool operator<(const _dLeaf &) const { return false; }
        _dLeaf& operator=(const _dLeaf& other) { return *this; }
        _dLeaf& operator=(_dLeaf&& other) noexcept { return *this; }
    };
    struct _dNode1 {
        T1 p1_;

        const T1 &p1() const { return p1_; }


        _dNode1(const T1 &p1 )
            :p1_(p1)
        {}
        _dNode1(const _dNode1& other)
            :p1_(other.p1_)
        {}
        _dNode1(_dNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
        { }

        bool operator<(const _dNode1 &rhs) const {
            return std::tie(p1_) < std::tie(rhs.p1_);
        }
        _dNode1& operator=(const _dNode1& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
            } 
            return *this; 
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


        _dNode2(const T1 &p1, const dtree<T1> &p2 )
            :p1_(p1)
            ,p2_(std::make_shared<dtree<T1>>(p2))
        {}
        _dNode2(const _dNode2& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<dtree<T1>>(*other.p2_))
        {}
        _dNode2(_dNode2&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _dNode2 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _dNode2& operator=(const _dNode2& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<dtree<T1>>(*other.p2_);
            } 
            return *this; 
        } 
        _dNode2& operator=(_dNode2&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };

    std::variant<_dLeaf, _dNode1, _dNode2> value_;

  public:

    dtree(const std::variant<_dLeaf, _dNode1, _dNode2> &value) : value_(value) {}
    //深拷贝构造函数
    dtree(const dtree<T1>& other) { 
        if(std::holds_alternative<_dLeaf>(other.value_)){ 
            const _dLeaf& other_node = std::get<_dLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_dNode1>(other.value_)){ 
            const _dNode1& other_node = std::get<_dNode1>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_dNode2>(other.value_)){ 
            const _dNode2& other_node = std::get<_dNode2>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    dtree(dtree<T1>&& other){
        if(std::holds_alternative<_dLeaf>(other.value_)){ 
            _dLeaf& other_node = std::get<_dLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_dNode1>(other.value_)){ 
            _dNode1& other_node = std::get<_dNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_dNode2>(other.value_)){ 
            _dNode2& other_node = std::get<_dNode2>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static dtree<T1> new_dLeaf() {
        return dtree<T1> ( _dLeaf (  ) );
    }
    static dtree<T1> new_dNode1(const T1 &p1) {
        return dtree<T1> ( _dNode1 ( p1 ) );
    }
    static dtree<T1> new_dNode2(const T1 &p1, const dtree<T1> &p2) {
        return dtree<T1> ( _dNode2 ( p1, p2 ) );
    }

    bool is_dLeaf() const { return std::holds_alternative<_dLeaf>(value_); }
    bool is_dNode1() const { return std::holds_alternative<_dNode1>(value_); }
    bool is_dNode2() const { return std::holds_alternative<_dNode2>(value_); }
    const _dNode1 &as_dNode1() const { return std::get<_dNode1>(value_); }
    const _dNode2 &as_dNode2() const { return std::get<_dNode2>(value_); }

    bool operator<(const dtree<T1> &rhs) const { return value_ < rhs.value_; }
    dtree<T1>& operator=(dtree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_dLeaf>(other)){
                _dLeaf& other_value = std::get<_dLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_dNode1>(other)){
                _dNode1& other_value = std::get<_dNode1>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_dNode2>(other)){
                _dNode2& other_value = std::get<_dNode2>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    dtree<T1>& operator=(const dtree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_dLeaf>(other.value_)){ 
                const _dLeaf& other_node = std::get<_dLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_dNode1>(other.value_)){ 
                const _dNode1& other_node = std::get<_dNode1>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_dNode2>(other.value_)){ 
                const _dNode2& other_node = std::get<_dNode2>(other.value_); 
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
    struct _eNode1 {
        T1 p1_;
        std::shared_ptr<etree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        etree<T1> p2() const { return *p2_; }


        _eNode1(const T1 &p1, const etree<T1> &p2 )
            :p1_(p1)
            ,p2_(std::make_shared<etree<T1>>(p2))
        {}
        _eNode1(const _eNode1& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<etree<T1>>(*other.p2_))
        {}
        _eNode1(_eNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _eNode1 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _eNode1& operator=(const _eNode1& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<etree<T1>>(*other.p2_);
            } 
            return *this; 
        } 
        _eNode1& operator=(_eNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
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


        _eNode2(const etree<T1> &p1, const T1 &p2, const etree<T1> &p3 )
            :p1_(std::make_shared<etree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<etree<T1>>(p3))
        {}
        _eNode2(const _eNode2& other)
            :p1_(std::make_shared<etree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<etree<T1>>(*other.p3_))
        {}
        _eNode2(_eNode2&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _eNode2 &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _eNode2& operator=(const _eNode2& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<etree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<etree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _eNode2& operator=(_eNode2&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_eLeaf, _eNode1, _eNode2> value_;

  public:

    etree(const std::variant<_eLeaf, _eNode1, _eNode2> &value) : value_(value) {}
    //深拷贝构造函数
    etree(const etree<T1>& other) { 
        if(std::holds_alternative<_eLeaf>(other.value_)){ 
            const _eLeaf& other_node = std::get<_eLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_eNode1>(other.value_)){ 
            const _eNode1& other_node = std::get<_eNode1>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_eNode2>(other.value_)){ 
            const _eNode2& other_node = std::get<_eNode2>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    etree(etree<T1>&& other){
        if(std::holds_alternative<_eLeaf>(other.value_)){ 
            _eLeaf& other_node = std::get<_eLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_eNode1>(other.value_)){ 
            _eNode1& other_node = std::get<_eNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_eNode2>(other.value_)){ 
            _eNode2& other_node = std::get<_eNode2>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static etree<T1> new_eLeaf() {
        return etree<T1> ( _eLeaf (  ) );
    }
    static etree<T1> new_eNode1(const T1 &p1, const etree<T1> &p2) {
        return etree<T1> ( _eNode1 ( p1, p2 ) );
    }
    static etree<T1> new_eNode2(const etree<T1> &p1, const T1 &p2, const etree<T1> &p3) {
        return etree<T1> ( _eNode2 ( p1, p2, p3 ) );
    }

    bool is_eLeaf() const { return std::holds_alternative<_eLeaf>(value_); }
    bool is_eNode1() const { return std::holds_alternative<_eNode1>(value_); }
    bool is_eNode2() const { return std::holds_alternative<_eNode2>(value_); }
    const _eNode1 &as_eNode1() const { return std::get<_eNode1>(value_); }
    const _eNode2 &as_eNode2() const { return std::get<_eNode2>(value_); }

    bool operator<(const etree<T1> &rhs) const { return value_ < rhs.value_; }
    etree<T1>& operator=(etree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_eLeaf>(other)){
                _eLeaf& other_value = std::get<_eLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_eNode1>(other)){
                _eNode1& other_value = std::get<_eNode1>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_eNode2>(other)){
                _eNode2& other_value = std::get<_eNode2>(other.value_);
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
            if(std::holds_alternative<_eNode1>(other.value_)){ 
                const _eNode1& other_node = std::get<_eNode1>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_eNode2>(other.value_)){ 
                const _eNode2& other_node = std::get<_eNode2>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class ftree {
    struct _fLeaf {
        _fLeaf() {}
        _fLeaf(const _fLeaf& other){ }
        _fLeaf(_fLeaf&& other) noexcept{ }
        bool operator<(const _fLeaf &) const { return false; }
        _fLeaf& operator=(const _fLeaf& other) { return *this; }
        _fLeaf& operator=(_fLeaf&& other) noexcept { return *this; }
    };
    struct _fNode1 {
        T1 p1_;
        std::shared_ptr<ftree<T1>> p2_;

        const T1 &p1() const { return p1_; }
        ftree<T1> p2() const { return *p2_; }


        _fNode1(const T1 &p1, const ftree<T1> &p2 )
            :p1_(p1)
            ,p2_(std::make_shared<ftree<T1>>(p2))
        {}
        _fNode1(const _fNode1& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<ftree<T1>>(*other.p2_))
        {}
        _fNode1(_fNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _fNode1 &rhs) const {
            return std::tie(p1_, *p2_) < std::tie(rhs.p1_, *rhs.p2_);
        }
        _fNode1& operator=(const _fNode1& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<ftree<T1>>(*other.p2_);
            } 
            return *this; 
        } 
        _fNode1& operator=(_fNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
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


        _fNode2(const ftree<T1> &p1, const ftree<T1> &p2, const T1 &p3 )
            :p1_(std::make_shared<ftree<T1>>(p1))
            ,p2_(std::make_shared<ftree<T1>>(p2))
            ,p3_(p3)
        {}
        _fNode2(const _fNode2& other)
            :p1_(std::make_shared<ftree<T1>>(*other.p1_))
            ,p2_(std::make_shared<ftree<T1>>(*other.p2_))
            ,p3_(other.p3_)
        {}
        _fNode2(_fNode2&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _fNode2 &rhs) const {
            return std::tie(*p1_, *p2_, p3_) < std::tie(*rhs.p1_, *rhs.p2_, rhs.p3_);
        }
        _fNode2& operator=(const _fNode2& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<ftree<T1>>(*other.p1_);
                p2_.reset();
                p2_ = std::make_shared<ftree<T1>>(*other.p2_);
                p3_ = other.p3_; 
            } 
            return *this; 
        } 
        _fNode2& operator=(_fNode2&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_fLeaf, _fNode1, _fNode2> value_;

  public:

    ftree(const std::variant<_fLeaf, _fNode1, _fNode2> &value) : value_(value) {}
    //深拷贝构造函数
    ftree(const ftree<T1>& other) { 
        if(std::holds_alternative<_fLeaf>(other.value_)){ 
            const _fLeaf& other_node = std::get<_fLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_fNode1>(other.value_)){ 
            const _fNode1& other_node = std::get<_fNode1>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_fNode2>(other.value_)){ 
            const _fNode2& other_node = std::get<_fNode2>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    ftree(ftree<T1>&& other){
        if(std::holds_alternative<_fLeaf>(other.value_)){ 
            _fLeaf& other_node = std::get<_fLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_fNode1>(other.value_)){ 
            _fNode1& other_node = std::get<_fNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_fNode2>(other.value_)){ 
            _fNode2& other_node = std::get<_fNode2>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static ftree<T1> new_fLeaf() {
        return ftree<T1> ( _fLeaf (  ) );
    }
    static ftree<T1> new_fNode1(const T1 &p1, const ftree<T1> &p2) {
        return ftree<T1> ( _fNode1 ( p1, p2 ) );
    }
    static ftree<T1> new_fNode2(const ftree<T1> &p1, const ftree<T1> &p2, const T1 &p3) {
        return ftree<T1> ( _fNode2 ( p1, p2, p3 ) );
    }

    bool is_fLeaf() const { return std::holds_alternative<_fLeaf>(value_); }
    bool is_fNode1() const { return std::holds_alternative<_fNode1>(value_); }
    bool is_fNode2() const { return std::holds_alternative<_fNode2>(value_); }
    const _fNode1 &as_fNode1() const { return std::get<_fNode1>(value_); }
    const _fNode2 &as_fNode2() const { return std::get<_fNode2>(value_); }

    bool operator<(const ftree<T1> &rhs) const { return value_ < rhs.value_; }
    ftree<T1>& operator=(ftree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_fLeaf>(other)){
                _fLeaf& other_value = std::get<_fLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_fNode1>(other)){
                _fNode1& other_value = std::get<_fNode1>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_fNode2>(other)){
                _fNode2& other_value = std::get<_fNode2>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    ftree<T1>& operator=(const ftree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_fLeaf>(other.value_)){ 
                const _fLeaf& other_node = std::get<_fLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_fNode1>(other.value_)){ 
                const _fNode1& other_node = std::get<_fNode1>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_fNode2>(other.value_)){ 
                const _fNode2& other_node = std::get<_fNode2>(other.value_); 
                value_ = other.value_; 
            } 
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


        _gNode1(const T1 &p1, const gtree<T1> &p2, const gtree<T1> &p3 )
            :p1_(p1)
            ,p2_(std::make_shared<gtree<T1>>(p2))
            ,p3_(std::make_shared<gtree<T1>>(p3))
        {}
        _gNode1(const _gNode1& other)
            :p1_(other.p1_)
            ,p2_(std::make_shared<gtree<T1>>(*other.p2_))
            ,p3_(std::make_shared<gtree<T1>>(*other.p3_))
        {}
        _gNode1(_gNode1&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _gNode1 &rhs) const {
            return std::tie(p1_, *p2_, *p3_) < std::tie(rhs.p1_, *rhs.p2_, *rhs.p3_);
        }
        _gNode1& operator=(const _gNode1& other){ 
            if(this != &other){ 
                p1_ = other.p1_; 
                p2_.reset();
                p2_ = std::make_shared<gtree<T1>>(*other.p2_);
                p3_.reset();
                p3_ = std::make_shared<gtree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _gNode1& operator=(_gNode1&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };
    struct _gNode2 {
        std::shared_ptr<gtree<T1>> p1_;
        T1 p2_;

        gtree<T1> p1() const { return *p1_; }
        const T1 &p2() const { return p2_; }


        _gNode2(const gtree<T1> &p1, const T1 &p2 )
            :p1_(std::make_shared<gtree<T1>>(p1))
            ,p2_(p2)
        {}
        _gNode2(const _gNode2& other)
            :p1_(std::make_shared<gtree<T1>>(*other.p1_))
            ,p2_(other.p2_)
        {}
        _gNode2(_gNode2&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
        { }

        bool operator<(const _gNode2 &rhs) const {
            return std::tie(*p1_, p2_) < std::tie(*rhs.p1_, rhs.p2_);
        }
        _gNode2& operator=(const _gNode2& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<gtree<T1>>(*other.p1_);
                p2_ = other.p2_; 
            } 
            return *this; 
        } 
        _gNode2& operator=(_gNode2&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
            }
            return *this;
        }
    };
    struct _gLeaf {
        _gLeaf() {}
        _gLeaf(const _gLeaf& other){ }
        _gLeaf(_gLeaf&& other) noexcept{ }
        bool operator<(const _gLeaf &) const { return false; }
        _gLeaf& operator=(const _gLeaf& other) { return *this; }
        _gLeaf& operator=(_gLeaf&& other) noexcept { return *this; }
    };

    std::variant<_gNode1, _gNode2, _gLeaf> value_;

  public:

    gtree(const std::variant<_gNode1, _gNode2, _gLeaf> &value) : value_(value) {}
    //深拷贝构造函数
    gtree(const gtree<T1>& other) { 
        if(std::holds_alternative<_gNode1>(other.value_)){ 
            const _gNode1& other_node = std::get<_gNode1>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_gNode2>(other.value_)){ 
            const _gNode2& other_node = std::get<_gNode2>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_gLeaf>(other.value_)){ 
            const _gLeaf& other_node = std::get<_gLeaf>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    gtree(gtree<T1>&& other){
        if(std::holds_alternative<_gNode1>(other.value_)){ 
            _gNode1& other_node = std::get<_gNode1>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_gNode2>(other.value_)){ 
            _gNode2& other_node = std::get<_gNode2>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_gLeaf>(other.value_)){ 
            _gLeaf& other_node = std::get<_gLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static gtree<T1> new_gNode1(const T1 &p1, const gtree<T1> &p2, const gtree<T1> &p3) {
        return gtree<T1> ( _gNode1 ( p1, p2, p3 ) );
    }
    static gtree<T1> new_gNode2(const gtree<T1> &p1, const T1 &p2) {
        return gtree<T1> ( _gNode2 ( p1, p2 ) );
    }
    static gtree<T1> new_gLeaf() {
        return gtree<T1> ( _gLeaf (  ) );
    }

    bool is_gNode1() const { return std::holds_alternative<_gNode1>(value_); }
    bool is_gNode2() const { return std::holds_alternative<_gNode2>(value_); }
    bool is_gLeaf() const { return std::holds_alternative<_gLeaf>(value_); }
    const _gNode1 &as_gNode1() const { return std::get<_gNode1>(value_); }
    const _gNode2 &as_gNode2() const { return std::get<_gNode2>(value_); }

    bool operator<(const gtree<T1> &rhs) const { return value_ < rhs.value_; }
    gtree<T1>& operator=(gtree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_gNode1>(other)){
                _gNode1& other_value = std::get<_gNode1>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_gNode2>(other)){
                _gNode2& other_value = std::get<_gNode2>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_gLeaf>(other)){
                _gLeaf& other_value = std::get<_gLeaf>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    gtree<T1>& operator=(const gtree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_gNode1>(other.value_)){ 
                const _gNode1& other_node = std::get<_gNode1>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_gNode2>(other.value_)){ 
                const _gNode2& other_node = std::get<_gNode2>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_gLeaf>(other.value_)){ 
                const _gLeaf& other_node = std::get<_gLeaf>(other.value_); 
                value_ = other.value_; 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class newTree {
    struct _newLeaf {
        _newLeaf() {}
        _newLeaf(const _newLeaf& other){ }
        _newLeaf(_newLeaf&& other) noexcept{ }
        bool operator<(const _newLeaf &) const { return false; }
        _newLeaf& operator=(const _newLeaf& other) { return *this; }
        _newLeaf& operator=(_newLeaf&& other) noexcept { return *this; }
    };
    struct _newNode {
        std::shared_ptr<newTree<T1>> p1_;
        list<T1> p2_;
        std::shared_ptr<newTree<T1>> p3_;

        newTree<T1> p1() const { return *p1_; }
        const list<T1> &p2() const { return p2_; }
        newTree<T1> p3() const { return *p3_; }


        _newNode(const newTree<T1> &p1, const list<T1> &p2, const newTree<T1> &p3 )
            :p1_(std::make_shared<newTree<T1>>(p1))
            ,p2_(p2)
            ,p3_(std::make_shared<newTree<T1>>(p3))
        {}
        _newNode(const _newNode& other)
            :p1_(std::make_shared<newTree<T1>>(*other.p1_))
            ,p2_(other.p2_)
            ,p3_(std::make_shared<newTree<T1>>(*other.p3_))
        {}
        _newNode(_newNode&& other) noexcept 
            :p1_(std::move(other.p1_))
            ,p2_(std::move(other.p2_))
            ,p3_(std::move(other.p3_))
        { }

        bool operator<(const _newNode &rhs) const {
            return std::tie(*p1_, p2_, *p3_) < std::tie(*rhs.p1_, rhs.p2_, *rhs.p3_);
        }
        _newNode& operator=(const _newNode& other){ 
            if(this != &other){ 
                p1_.reset();
                p1_ = std::make_shared<newTree<T1>>(*other.p1_);
                p2_ = other.p2_; 
                p3_.reset();
                p3_ = std::make_shared<newTree<T1>>(*other.p3_);
            } 
            return *this; 
        } 
        _newNode& operator=(_newNode&& other) noexcept {
            if(this != &other) {
                p1_ = std::move(other.p1_);
                p2_ = std::move(other.p2_);
                p3_ = std::move(other.p3_);
            }
            return *this;
        }
    };

    std::variant<_newLeaf, _newNode> value_;

  public:

    newTree(const std::variant<_newLeaf, _newNode> &value) : value_(value) {}
    //深拷贝构造函数
    newTree(const newTree<T1>& other) { 
        if(std::holds_alternative<_newLeaf>(other.value_)){ 
            const _newLeaf& other_node = std::get<_newLeaf>(other.value_); 
            value_ = other_node;
        } 
        if(std::holds_alternative<_newNode>(other.value_)){ 
            const _newNode& other_node = std::get<_newNode>(other.value_); 
            value_ = other_node;
        } 
    } 
    //移动构造函数
    newTree(newTree<T1>&& other){
        if(std::holds_alternative<_newLeaf>(other.value_)){ 
            _newLeaf& other_node = std::get<_newLeaf>(other.value_); 
            value_ = std::move(other_node);
        } 
        if(std::holds_alternative<_newNode>(other.value_)){ 
            _newNode& other_node = std::get<_newNode>(other.value_); 
            value_ = std::move(other_node);
        } 
    }

    static newTree<T1> new_newLeaf() {
        return newTree<T1> ( _newLeaf (  ) );
    }
    static newTree<T1> new_newNode(const newTree<T1> &p1, const list<T1> &p2, const newTree<T1> &p3) {
        return newTree<T1> ( _newNode ( p1, p2, p3 ) );
    }

    bool is_newLeaf() const { return std::holds_alternative<_newLeaf>(value_); }
    bool is_newNode() const { return std::holds_alternative<_newNode>(value_); }
    const _newNode &as_newNode() const { return std::get<_newNode>(value_); }

    bool operator<(const newTree<T1> &rhs) const { return value_ < rhs.value_; }
    newTree<T1>& operator=(newTree<T1>&& other) noexcept {
        if(this != &other){
            if(std::holds_alternative<_newLeaf>(other)){
                _newLeaf& other_value = std::get<_newLeaf>(other.value_);
                value_ = std::move(other_value);
            }
            if(std::holds_alternative<_newNode>(other)){
                _newNode& other_value = std::get<_newNode>(other.value_);
                value_ = std::move(other_value);
            }
        }
        return *this;
    }
    //拷贝赋值运算符
    newTree<T1>& operator=(const newTree<T1>& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_newLeaf>(other.value_)){ 
                const _newLeaf& other_node = std::get<_newLeaf>(other.value_); 
                value_ = other.value_; 
            } 
            if(std::holds_alternative<_newNode>(other.value_)){ 
                const _newNode& other_node = std::get<_newNode>(other.value_); 
                value_ = other.value_; 
            } 
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
