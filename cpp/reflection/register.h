#include <iostream>
#include <map>
#include <string>

typedef void *(*ConstructFunc)();

class ClassManager {
  public:
    static void *get_new_instance(const std::string &class_name) {
        std::map<std::string, ConstructFunc>::iterator itor = _register_map.find(class_name);
        if (itor == _register_map.end()) {
            return nullptr;
        } else {
            return (itor->second)();
        }
    }

    static void register_new_class(const std::string &class_name, ConstructFunc func) {
        _register_map[class_name] = func;
        std::cout << "register class:" << class_name << std::endl;
        return;
    }

  private:
    static std::map<std::string, ConstructFunc> _register_map;
};

std::map<std::string, ConstructFunc> ClassManager::_register_map;

class Register {
  public:
    Register(const std::string &class_name, ConstructFunc func) {
        ClassManager::register_new_class(class_name, func);
    }
};

#define REGISTER_CLASS(class_name)    \
    class class_name##Register {      \
      private:                        \
        static void *new_instance() { \
            return new class_name;    \
        }                             \
        const static Register reg;    \
    };                                \
    const Register class_name##Register::reg(#class_name, class_name##Register::new_instance);
