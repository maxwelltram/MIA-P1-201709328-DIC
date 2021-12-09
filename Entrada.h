
#ifndef SCANNER_H
#define SCANNER_H
#include <vector>

using namespace std;

class entrada{
public:
    entrada();
    void MenuInicio();
    //string Minuscula(string comando);
    string busquedaToken(string comando);
    bool Equals(string Uno, string Dos);
    string CamMayusculas(string comandoCambio);
    vector<string> splitTok(string entrada);

};

#endif
