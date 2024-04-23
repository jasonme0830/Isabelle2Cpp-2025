#include <cstdlib>
#include <memory>
#include <variant>

class ybool {
    struct _yTrue {
        _yTrue() {}
        _yTrue(_yTrue&& other) noexcept{ }
        _yTrue(const _yTrue& other){ }
        bool operator<(const _yTrue &) const { return false; }
        _yTrue& operator=(_yTrue&& other) noexcept { return *this; }
        _yTrue& operator=(const _yTrue& other) { return *this; }
    };
    struct _yFalse {
        _yFalse() {}
        _yFalse(_yFalse&& other) noexcept{ }
        _yFalse(const _yFalse& other){ }
        bool operator<(const _yFalse &) const { return false; }
        _yFalse& operator=(_yFalse&& other) noexcept { return *this; }
        _yFalse& operator=(const _yFalse& other) { return *this; }
    };
    struct _yMid {
        _yMid() {}
        _yMid(_yMid&& other) noexcept{ }
        _yMid(const _yMid& other){ }
        bool operator<(const _yMid &) const { return false; }
        _yMid& operator=(_yMid&& other) noexcept { return *this; }
        _yMid& operator=(const _yMid& other) { return *this; }
    };

    std::variant<_yTrue, _yFalse, _yMid> value_;

  public:

    ybool() = default;
    ybool(const std::variant<_yTrue, _yFalse, _yMid> &value) : value_(value) {}
    ybool(ybool&& other) noexcept :value_(std::move(other.value_)){ }
    ybool(const ybool& other) : value_(other.value_){ 
        if(std::holds_alternative<_yTrue>(other.value_)){ 
            value_ = std::get<_yTrue>(other.value_); 
        } 
        if(std::holds_alternative<_yFalse>(other.value_)){ 
            value_ = std::get<_yFalse>(other.value_); 
        } 
        if(std::holds_alternative<_yMid>(other.value_)){ 
            value_ = std::get<_yMid>(other.value_); 
        } 
    } 

    static ybool yTrue() {
        return ybool { _yTrue {  } };
    }
    static ybool yFalse() {
        return ybool { _yFalse {  } };
    }
    static ybool yMid() {
        return ybool { _yMid {  } };
    }

    bool is_yTrue() const { return std::holds_alternative<_yTrue>(value_); }
    bool is_yFalse() const { return std::holds_alternative<_yFalse>(value_); }
    bool is_yMid() const { return std::holds_alternative<_yMid>(value_); }

    bool operator<(const ybool &rhs) const { return value_ < rhs.value_; }
    ybool& operator=(ybool&& other) noexcept {
        if(this != &other){
            value_ = std::move(other.value_);
        }
        return *this;
    }
    ybool& operator=(const ybool& other){ 
        if(this != &other){ 
            if(std::holds_alternative<_yTrue>(other.value_)){ 
                _yTrue& currentNode = std::get<_yTrue>(value_); 
                //deleteMethod(currentNode); 
                value_ = std::get<_yTrue>(other.value_); 
            } 
            if(std::holds_alternative<_yFalse>(other.value_)){ 
                _yFalse& currentNode = std::get<_yFalse>(value_); 
                //deleteMethod(currentNode); 
                value_ = std::get<_yFalse>(other.value_); 
            } 
            if(std::holds_alternative<_yMid>(other.value_)){ 
                _yMid& currentNode = std::get<_yMid>(value_); 
                //deleteMethod(currentNode); 
                value_ = std::get<_yMid>(other.value_); 
            } 
        } 
        return *this; 
    }

};

template<typename T1>
class newTree {
    struct _newLeaf {
        _newLeaf() {}
        _newLeaf(_newLeaf&& other) noexcept{ }
        _newLeaf(const _newLeaf& other){ }
        bool operator<(const _newLeaf &) const { return false; }
        _newLeaf& operator=(_newLeaf&& other) noexcept { return *this; }
        _newLeaf& operator=(const _newLeaf& other) { return *this; }
    };
    struct _newNode {
        std::shared_ptr<newTree<T1>> p1_;
        slist<T1> p2_;
        std::shared_ptr<newTree<T1>> p3_;

        newTree<T1> p1() const { return *p1_; }
        const slist<T1> &p2() const { return p2_; }
        newTree<T1> p3() const { return *p3_; }


        _newNode(){
            p1_ = nullptr;
            // slist<T1> p2_;
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
        _newNode(const _newNode& value) { 
            p1_ = value.p1_;
            p2_ = value.p2_; 
            p3_ = value.p3_;
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
        _newNode& operator=(const _newNode& other){ 
            if(this != &other){ 
                delete p1_;
                p1_ = other.p1_;
                p2_ = other.p2_; 
                delete p3_;
                p3_ = other.p3_;
            } 
            return *this; 
        } 
    };

    std::variant<_newLeaf, _newNode> value_;

  public:

    newTree() = default;
    newTree(const std::variant<_newLeaf, _newNode> &value) : value_(value) {}
    newTree(newTree<T1>&& other) noexcept :value_(std::move(other.value_)){ }
    newTree(const newTree<T1>& other) { 
        if(std::holds_alternative<_newLeaf>(other.value_)){ 
            value_ = _newLeaf{};
        } 
        if(std::holds_alternative<_newNode>(other.value_)){ 
            _newNode& other_node = std::get<_newNode>(other.value_);
            //new 这个对象给value_的赋值过程是浅拷贝，是正确的，也可以改成move
            value_ = _newNode{
                //使用大括号直接进行聚合初始化，避免复制
                std::make_shared<newTree<T1>>(*other_node.p1_),
                other_node.p2_,
                std::make_shared<newTree<T1>>(*other_node.p3_)
            };
        } 
    } 
    // 拷贝构造函数  
    // newTree(const newTree& other) {  
    //     std::visit([&](const auto& arg) {  
    //         using T = std::decay_t<decltype(arg)>;  
    //         if constexpr (std::is_same_v<T, _newLeaf>) {  
    //             value_ = _newLeaf{};  
    //         } else if constexpr (std::is_same_v<T, _newNode>) {  
    //             value_ = _newNode{  
    //                 std::make_shared<newTree<T1>>(*arg.p1_), // 递归拷贝子树  
    //                 arg.p2_                                   // 直接拷贝列表  
    //             };  
    //         }  
    //     }, other.value_);  
    // } 


    static newTree<T1> newLeaf() {
        return newTree<T1> { _newLeaf {  } };
    }
    static newTree<T1> newNode(const newTree<T1> &p1, const slist<T1> &p2, const newTree<T1> &p3) {
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
    newTree<T1>& operator=(const newTree<T1>& other){ 
        //指向同一个地址，直接返回
        if(this == &other) return *this;
        //递归删除this原有的数据
        if(std::holds_alternative<_newLeaf>(value_)){

        }
        if(std::holds_alternative<_newNode>(value_)){
            _newNode& currentNode = std::get<_newNode>(value_);
            //reset函数虽然不是递归的操作，但是可以递归消除所有节点
            currentNode.p1_.reset();
            currentNode.p3_.reset();
        }
        //向空变量中拷贝数据
        if(std::holds_alternative<_newLeaf>(other.value_)){ 
            value_ = _newLeaf(){};
        } 
        if(std::holds_alternative<_newNode>(other.value_)){ 
            _newNode& other_node = std::get<_newNode>(other.value_);
            value_ = _newNode(
                std::make_shared<newTree<T1>>(*other_node.p1_),
                other_node.p2_,
                std::make_shared<newTree<T1>>(*other_node.p3_)
            );
        } 

        return *this; 
    }
    // 拷贝赋值运算符  
    // newTree& operator=(const newTree& other) {  
    //     if (this != &other) {  
    //         std::visit([&](const auto& arg) {  
    //             using T = std::decay_t<decltype(arg)>;  
    //             if constexpr (std::is_same_v<T, _newLeaf>) {  
    //                 value_ = _newLeaf{};  
    //             } else if constexpr (std::is_same_v<T, _newNode>) {  
    //                 if (std::holds_alternative<_newNode>(value_)) {  
    //                     // 如果当前是节点，且已有一个子树，则先释放它  
    //                     auto& current_node = std::get<_newNode>(value_);  
    //                     if (current_node.p1_.use_count() == 1) {  
    //                         current_node.p1_.reset();  
    //                     }  
    //                 }  
    //                 value_ = _newNode{  
    //                     arg.p1_ ? std::make_shared<newTree<T1>>(*arg.p1_) : nullptr, // 拷贝或保持nullptr  
    //                     arg.p2_                                                     // 直接拷贝列表  
    //                 };  
    //             }  
    //         }, other.value_);  
    //     }  
    //     return *this;  
    // } 


};



// generated by HOL2Cpp
