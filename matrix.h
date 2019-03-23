#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include <math.h>

class Matrice {
private:
    int * matrix;
    int linii, coloane;
    int readValues;
    bool isSameOrder(Matrice m);
    int Det(Matrice m, int n);



public:
    Matrice(int linii, int coloane);
    ~Matrice();
    int getReadValues();
    int* operator[](int x);
    void operator=(Matrice m);
    Matrice* operator+(Matrice m);
    Matrice* operator-(Matrice m);
    Matrice* operator*(Matrice m);
    Matrice* operator*(int scalar_factor);
    void operator<<(int m);
    int Determinant();
    void getCofactor(Matrice* mat, Matrice* temp, int p, int q, int n);
    bool isInversable();
    void eliminaLinia(int linie);
    void eliminaColoana(int coloana);
    void elimina(int linie, int coloana);
    int getColumns();
    int getLines();
    void ShowMatrix();
    void cpy(Matrice* m);
};

#endif // MATRIX_H_INCLUDED
