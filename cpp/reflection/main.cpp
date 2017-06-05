#include "register.h"

class AnimalBase {
  public:
    virtual void say() = 0;
};

template <typename DerivedT>
class Animal : public AnimalBase {
  public:
    void say() {
        static_cast<DerivedT*>(this)->say();
        return;
    }
};

class Cat : public Animal<Cat> {
  public:
    void say() {
        std::cout << "I'm a cat." << std::endl;
    }
};

REGISTER_CLASS(Cat)

class Dog : public Animal<Dog> {
  public:
    void say() {
        std::cout << "I'm a dog." << std::endl;
    }
};

REGISTER_CLASS(Dog)

class AnimalFactory {
  public:
    AnimalBase *create_animal(const std::string &animal_name) {
        return static_cast<AnimalBase *>(ClassManager::get_new_instance(animal_name));
    }
};

int main() {
    AnimalFactory fac;
    AnimalBase *a = fac.create_animal("Cat");
    a->say();

    AnimalBase *b = fac.create_animal("Dog");
    b->say();

    return 0;
}
