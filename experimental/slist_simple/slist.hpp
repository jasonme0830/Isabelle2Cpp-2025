#include <memory>
#include <variant>

template<typename T1>
struct slist {
    struct _sNil {};
    struct _sCons {
        T1 p1_;
        std::shared_ptr<slist<T1>> p2_;

        T1 p1() const { return p1_; }
        slist<T1> p2() const { return *p2_; }
    };

    std::variant<_sNil, _sCons> value;

  public:
    static slist<T1> sNil() {
        return slist<T1> { _sNil {} };
    }

    static slist<T1> sCons(T1 p1, slist<T1> p2) {
        return slist<T1> { _sCons { p1, std::make_shared<slist<T1>>(p2) } };
    }

    bool is_sNil() const { return std::holds_alternative<_sNil>(value); }
    bool is_sCons() const { return std::holds_alternative<_sCons>(value); }

    const _sCons &as_sCons() const {
        return std::get<_sCons>(value);
    }
};