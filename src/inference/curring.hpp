#include <type_traits>

template<typename Function, typename... CapturedArgs>
class curried
{
private:
  using CapturedArgsTuple = std::tuple<std::decay_t<CapturedArgs>...>;
  template<typename... Args>
  static auto capture_by_copy(Args&&... args)
  {
    return std::tuple<std::decay_t<Args>...>(std::forward<Args>(args)...);
  }

public:
  curried(Function function, CapturedArgs... args)
    : m_function(function)
    , m_captured(capture_by_copy(std::move(args)...))
  {
    //...
  }
  curried(Function function, std::tuple<CapturedArgs...> args)
    : m_function(function)
    , m_captured(std::move(args))
  {
    //...
  }

  template<typename... NewArgs>
  auto operator()(NewArgs&&... args) const
  {
    auto new_args = capture_by_copy(std::forward<NewArgs>(args)...);
    auto all_args = std::tuple_cat(m_captured, std::move(new_args));
    if constexpr (std::is_invocable_v<Function, CapturedArgs..., NewArgs...>) {
      return std::apply(m_function, all_args);
    } else {
      return curried<Function, CapturedArgs..., NewArgs...>(m_function,
                                                            all_args);
    }
  }

private:
  Function m_function;
  std::tuple<CapturedArgs...> m_captured;
};