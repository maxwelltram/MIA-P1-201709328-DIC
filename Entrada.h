
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
    void comandos(string comando, vector<string> tokens);
    void comandoScript(vector<string> parametros);
    void AlertaError(string comando, string alerta);
    void script(string dirc);
    void AlertaMensaje(string comando, string alerta);
};

#endif
