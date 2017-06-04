#include "register.h"

class Animal {
  public:
    virtual void say() = 0;
};

class Cat : public Animal {
  public:
    void say() {
        std::cout << "I'm a cat." << std::endl;
    }
};

REGISTER_CLASS(Cat)

class Dog : public Animal {
  public:
    void say() {
        std::cout << "I'm a dog." << std::endl;
    }
};

REGISTER_CLASS(Dog)

class AnimalFactory {
  public:
    Animal *create_animal(const std::string &animal_name) {
        return static_cast<Animal *>(ClassManager::get_new_instance(animal_name));
    }
};

int main() {
    AnimalFactory fac;
    Animal *a = fac.create_animal("Cat");
    a->say();

    Animal *b = fac.create_animal("Dog");
    b->say();

    return 0;
}