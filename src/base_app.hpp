#ifndef BASE_APP_HPP
#define BASE_APP_HPP

class BaseApp {
 protected:
  BaseApp(int argc, const char* const* argv) : argc_{argc}, argv_{argv} {
  }

 protected:
  int argc_;
  const char* const* argv_;
};

#endif
