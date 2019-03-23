#include <iostream>
#include <fstream>
#include "matrix.h"
using namespace std;

Matrice::Matrice(int nr_linii, int nr_coloane) {
    readValues = 0;
    // Alocarea dinamica a memoriei, un total de celule de memorie  nr_linii * nr_coloane fiecare
    matrix = new int[nr_linii * nr_coloane];//(int *) malloc(nr_linii * nr_coloane * sizeof (*matrix));
    //stocarea numarului de linii si coloane
    linii = nr_linii;
    coloane = nr_coloane;
}

Matrice::~Matrice() {
    // eliberarea memoriei alocate;
    delete (matrix);
}

int* Matrice::operator[](int x) {
    // selectarea unei linii din matrice
    int tmp_j = 0;
    //functia returneaza un pointer catre o zona de memorie de lungimea unei linii in care se gasesc coloanele corespunzatoare selectiei
    int* tmp = new int[coloane]; //alocarea memoriei
    while (tmp_j < coloane) {
        //atribuire
        tmp[tmp_j] = matrix[x * coloane + tmp_j];
        tmp_j++;
    }
    return tmp;
};

Matrice* Matrice::operator+(Matrice m) {
    Matrice* new_m = new Matrice(linii, coloane);
    // adunarea se va face doar daca matricele au acelasi ordin in caz contrar null pointer va fi returnat
    if (isSameOrder(m)) {

        int index_linie = 0;
        int index_coloana = 0;
        // matricele vor fi parcurse si se va aduna element cu element
        while (index_linie < linii) {
            index_coloana = 0;
            while (index_coloana < coloane) {
               // printf("adding element %d with %d at %d,%d \n", matrix[index_linie * coloane + index_coloana], m.matrix[index_linie * coloane + index_coloana], index_linie, index_coloana);
                new_m->operator<<(matrix[index_linie * coloane + index_coloana] + m.matrix[index_linie * coloane + index_coloana]);
                index_coloana++;
            }
            index_linie++;

        }
        return new_m;
    } else {
       // return null;
       throw new exception();
    }

}
 //este nevoie de get-uri pentru linii si coloane deoarece acestea sunt private si nu pot fi accesate din exterior
int Matrice::getLines() {
    //returnarea liniilor
    return linii;
}

int Matrice::getColumns() {
    //returnarea liniilor
    return coloane;
}

bool Matrice::isSameOrder(Matrice m) {
    //verificarea ordinelor intre matricea in cauza si cea primita ca parametru
    int linie_b = m.getLines();
    int coloane_b = m.getColumns();
    return linii == linie_b && coloane == coloane_b;

}

void Matrice::operator<<(int element) {
    //  printf("reading \n");
    // injectarea elementelor se va face element cu element pana la ocuparea spatiului alocat
    // readValues reprezentand flag-ul de stop
    if (readValues < linii * coloane) {
        // printf("read element %d at position %d \n",element,readValues);
        matrix[readValues] = element;
        readValues++;
    }
}

Matrice* Matrice::operator-(Matrice m) {
    // Scaderea --- analog adunare
    Matrice* new_m = new Matrice(linii, coloane);
    if (isSameOrder(m)) {

        int index_linie = 0;
        int index_coloana = 0;

        while (index_linie < linii) {
            index_coloana = 0;
            while (index_coloana < coloane) {
                new_m -> operator<<(matrix[index_linie * coloane + index_coloana] - m.matrix[index_linie * coloane + index_coloana]);
                index_coloana++;
            }
            index_linie++;

        }
        return new_m;
    } else {
       // return nullptr;
       throw new exception();
    }
}

int Matrice::getReadValues() {
    return readValues;
}

void Matrice::operator=(Matrice m) {
    // eliberarea memoriei alocate
    delete(matrix);
    coloane = m.getColumns();
    linii = m.getLines();
    readValues = m.getReadValues();
    // alocarea unei noi zone de memorie pentru noul tablou de informatie
    matrix = new int[m.getLines() * m.getColumns() ];//(int *) malloc(m.getLines() * m.getColumns() * sizeof (*matrix));
    int index_linie = 0;
    int index_coloana;
    while (index_linie < m.getLines()) {
        int* linie = m[index_linie];
        index_coloana = 0;
        while (index_coloana < m.getColumns()) {
                //nucleul atribuirii
            matrix[index_linie * coloane + index_coloana] = linie[index_coloana];
            index_coloana++;
        }
        index_linie++;
    }
};

Matrice* Matrice::operator*(Matrice m) {
    // perform compatibility check
    if (coloane == m.getLines()) {

        Matrice* new_m = new Matrice(linii, m.coloane);
        int index_linie = 0;
        int index_coloana = 0;

        while (index_linie < linii) {
            index_coloana = 0;
            while (index_coloana < m.getColumns()) {
                int sum = 0;
               // printf(" multiplying  line  %d with column %d \n", index_linie, index_coloana);
                for (int i = 0; i < coloane; i++) {
                    sum += this->matrix[index_linie * coloane + i] * m.matrix[i * m.getColumns() + index_coloana];
                }
                new_m -> operator<<(sum);
                index_coloana++;
            }
            index_linie++;
        }
        return new_m;
    } else {
        cout <<"inmultire necompatibila"<< coloane <<" == "<<m.getLines()<<"\n";
       // return nullptr;
       throw new exception();
    }
}

Matrice* Matrice::operator*(int scalar_factor) {

    int index_linie = 0;
    int index_coloana = 0;
    // matricea este parcursa element cu element, fiecare fiind inmultit cu factorul scalar
    while (index_linie < linii) {
        index_coloana = 0;
        while (index_coloana < coloane) {
            matrix[index_linie * coloane + index_coloana] *= scalar_factor;
            index_coloana++;
        }
        index_linie++;

    }
    return this;
};

// Functia Det reprezinta o metoda recursiva de a calcula determinant-ul
// spargand recursiv determinantul in structuri din ce in ce mai mici pana cand pot fi calculate prin inmultirea in cruce
int Matrice::Det(Matrice m, int n) {
    int D = 0; // initializare rezultat
    // Cel mai bun caz cand matricea e formata dintr-un singur element
    if (n == 1)
        // capatul recursivitatii
        return m[0][0];

    if (n == 2) {
        // capatul recursivitatii cand structura a fost adusa la o forma calculabila
        int det = 0;
        det = (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
        return det;
    }

    int semn = 1; // To store sign multiplier
    // Iterate prin fiecare element de pe primul rand
    for (int f = 0; f < linii; f++) {
        Matrice* temp = new Matrice(n - 1, n - 1);
        //calcularea structurii de ordin inferior
        getCofactor(&m, temp, 0, f, n);
        //adaugarea la determinant
        D += semn * m[0][f] * Det(*temp, n - 1);
        delete (temp);
        // termenii sunt adaugati la determinant cu semn alternativ
        semn = -semn;
    }
    return D;

};

void Matrice::eliminaColoana(int coloana) {
    Matrice* mtx = new Matrice(linii, coloane - 1);
    int index_linie = 0;
    int index_coloana = 0;
    // se va clona matricea in cauza si se va sari coloana specificata
    // pentru ca in final originalul sa fie inlocuit
    while (index_linie < linii) {
        index_coloana = 0;
        while (index_coloana < coloane) {
            if (index_coloana != coloana) {
                *mtx << matrix[index_linie * (coloane) + index_coloana];
            }
            index_coloana++;
        }
        index_linie++;
    }
    *this = *mtx;
    //return mtx;

};

void Matrice::eliminaLinia(int linie) {
    // analog --- eliminare linie
    Matrice* mtx = new Matrice(linii - 1, coloane);
    int index_linie = 0;

    int index_coloana = 0;

    while (index_linie < linii) {
        index_coloana = 0;
        if (index_linie != linie) {
            while (index_coloana < coloane) {
                *mtx << matrix[index_linie * (coloane) + index_coloana];
                index_coloana++;
            }

        }
        index_linie++;
    }
    *this = *mtx;
    // return mtx;

};

bool Matrice::isInversable() {
    // verificarea conditiei de inversabilitate
    return linii == coloane && Det(*this, linii) > 0;
};

void Matrice::ShowMatrix() {
    //secventa de cod ce afiseaza matricea sub o forma lizibila
    std::cout<<("------\n");
    int index_linie = 0;
    int index_coloana = 0;
    // parcurgerea matricei element cu element + line-break-uri la finalul liniilor
    while (index_linie < linii) {
        index_coloana = 0;
        while (index_coloana < coloane) {
            cout<<(matrix[index_linie * (coloane) + index_coloana])<<" ";
            index_coloana++;
        }
        cout<<("\n");
        index_linie++;

    }

    cout<<("------\n");
};

void Matrice::getCofactor(Matrice* mat, Matrice* temp, int p, int q, int n) {

    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp->operator<<(mat->operator[](row)[col]);
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int Matrice::Determinant() {
    Matrice cpy(linii,coloane);
    cpy.cpy(this);
    // verificarea conditiei de existenta si calcularea determinantului
    if (cpy.getLines() == cpy.getColumns()) {
        return Det(cpy, cpy.getLines());
    } else {
        throw new exception();
    }
}

void Matrice::cpy(Matrice* m){
    int index_line = 0,index_coloana;
    while(index_line < m->getLines()){
        int* line = m->operator[](index_line);
        index_coloana = 0;
        while(index_coloana<m->getColumns()){
            *this<< line[index_coloana];
             index_coloana++;
        }
       index_line++;
    }
}


int main() {
     std::fstream demofile("data.txt");
     int linii,coloane,buffer;
     demofile>>linii;
     demofile>>coloane;
     Matrice m(linii, coloane);

     // initializarea matricei element cu element
     for(int i=0;i<linii*coloane;i++){
          demofile>>buffer;
          m<<buffer;
     }
     demofile>>linii;
     demofile>>coloane;

    Matrice m2(linii, coloane);
    Matrice* m3;

    // afisarea matricei citite
    m.ShowMatrix();
    for(int i=0;i<linii*coloane;i++){
          demofile>>buffer;
          m2<<buffer;
     }
    m2.ShowMatrix();
    //m2 = m;
   // m2.ShowMatrix();
    //calcularea si afisarea determinantului
    cout<<"det m = "<<m.Determinant()<<" \n";
    m.ShowMatrix();
    // inmultirea a doua matrice
    m3 = m*m2;
    // apelarea metodei se face cu -> deoarece inmultirea returneaza un pointer catre un obiect de tipul matrice
    m3->ShowMatrix();
    return 0;

}

