#include <cstdint>
#include <cstdlib>
#include <list>
#include <memory>
#include <variant>

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

std::list<std::uint64_t> merge(const std::list<std::uint64_t> &arg1, const std::list<std::uint64_t> &arg2);

std::list<std::uint64_t> merge_sort(const std::list<std::uint64_t> &arg1);

template<typename T1>
std::uint64_t slength(const slist<T1> &arg1);

template<typename T1>
slist<T1> stake(const std::uint64_t &arg1, const slist<T1> &arg2);

template<typename T1>
slist<T1> sdrop(const std::uint64_t &arg1, const slist<T1> &arg2);

slist<std::uint64_t> smerge(const slist<std::uint64_t> &arg1, const slist<std::uint64_t> &arg2);

slist<std::uint64_t> smerge_sort(const slist<std::uint64_t> &arg1);

