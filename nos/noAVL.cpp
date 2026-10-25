#ifndef NO_AVL_CPP
#define NO_AVL_CPP

#include "./no.cpp"

using namespace std;

template <typename T>
class NoAVL : public No<T> {
private:
    int altura;

public:
    NoAVL(T id) : No<T>(id), altura(1) {}

    virtual ~NoAVL() {}

    int getAltura() const { return altura; }
    void setAltura(int alt) { altura = alt; }
};

#endif