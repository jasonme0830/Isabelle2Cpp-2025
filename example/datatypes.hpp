#include <cstdint>
#include <cstdlib>
#include <memory>
#include <variant>

enum snatCons {
    sZero,
    sSucc,
};

struct snatElem;
using snat = std::shared_ptr<snatElem>;

struct snatElem {
    struct c2 {
        snat p1;
    };

    snatElem(snatCons cons) : cons(cons) {}

    c2 &get_c2() {
        return std::get<c2>(value);
    };
    void set_c2(snat _p1) {
        value = c2{_p1};
    }

    snatCons cons;
    std::variant<c2> value;
};

enum slistCons {
    sNil,
    sCons,
};

template<typename T1>
struct slistElem;
template<typename T1>
using slist = std::shared_ptr<slistElem<T1>>;

template<typename T1>
struct slistElem {
    struct c2 {
        T1 p1;
        slist<T1> p2;
    };

    slistElem(slistCons cons) : cons(cons) {}

    c2 &get_c2() {
        return std::get<c2>(value);
    };
    void set_c2(T1 _p1, slist<T1> _p2) {
        value = c2{_p1, _p2};
    }

    slistCons cons;
    std::variant<c2> value;
};

enum sboolCons {
    sTrue,
    sFalse,
};

struct sbool {

    sbool(sboolCons cons) : cons(cons) {}


    sboolCons cons;
    std::variant value;
};

snat add(const snat &arg1, const snat &arg2);

snat fib(const snat &arg1);

std::uint64_t natofsnat(const snat &arg1);

snat snatofnat(const std::uint64_t &arg1);

template<typename T1>
slist<T1> app(const slist<T1> &arg1, const slist<T1> &arg2);

template<typename T1>
slist<T1> rev(const slist<T1> &arg1);

slist<snat> snat2slist(const snat &arg1);

template<typename T1>
snat len(const slist<T1> &arg1);

slist<std::uint64_t> listwithlen(const std::uint64_t &arg1);

template<typename T1>
std::uint64_t leninnat(const slist<T1> &arg1);

template<typename T1>
slist<T1> dblist(const slist<T1> &arg1);

sbool snot(const sbool &arg1);

