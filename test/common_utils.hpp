#include <filesystem>

template <class T>
std::uintmax_t remove_all(T&& name) {
  return std::filesystem::remove_all(std::forward<T>(name));
}

template <class T, class... Ts>
std::uintmax_t remove_all(T&& name, Ts&&... names) {
  return std::filesystem::remove_all(std::forward<T>(name)) +
         remove_all<Ts...>(std::forward<Ts>(names)...);
}

template <class Rng1, class Rng2>
bool eq(const Rng1& rng1, const Rng2& rng2) {
  return rng1.size() == rng2.size() &&
         std::equal(rng1.begin(), rng1.end(), rng2.begin());
}
