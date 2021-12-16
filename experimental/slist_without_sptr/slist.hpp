#include <variant>

template<typename T1>
class slist {
    struct Elem;

    struct _sNil {};
    struct _sCons {
        T1 p1_;
        Elem *p2_;

        T1 p1() const { return p1_; }
        slist p2() const { return p2_; }
    };

    struct Elem {
        int ref;
        std::variant<_sNil, _sCons> value;
    };

    Elem *elem;

public:
    // enable construct slist from Elem*
    // slist.as_sCons().p2 can be passed as slist
    slist(Elem *elem) : elem(elem) {
        elem->ref++;
    }

    slist(const slist &other) : elem(other.elem) {
        elem->ref++;
    }

    static slist<T1> sNil() {
        slist<T1> res(new Elem { 0, _sNil{} });
        return res;
    }
    static slist<T1> sCons(const T1 &p1, const slist &p2) {
        slist<T1> res(new Elem { 0, _sCons{ p1, p2.elem } });
        p2.elem->ref++;
        // p3.elem->ref++;
        // ...
        return res;
    }

    bool is_sNil() const {
        return std::holds_alternative<_sNil>(elem->value);
    }

    bool is_sCons() const {
        return std::holds_alternative<_sCons>(elem->value);
    }

    const _sCons &as_sCons() const {
        return std::get<_sCons>(elem->value);
    }

    // use bfs here
    ~slist() {
        auto p = elem;
        while (p) {
            --p->ref;
            if (p->ref == 0) {

            } else {
                break;
            }
        }
    }
};