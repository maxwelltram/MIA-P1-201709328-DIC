
#ifndef UNTITLED1_LOGDISCO_H
#define UNTITLED1_LOGDISCO_H
#include <vector>
#include <string>
#include "./StructsGlobal.h"
#include "./LogDisco.h"
#include "./Entrada.h"

using namespace std;

class LogDiscos {
    //struct a utilizar con su estructura
public:
    LogDiscos();

    void ComandoMkdisk(vector<string> parametros);

    typedef struct Aux {
        int principio;
        int termina;
        int pre;
        int post;
        int particion;
    } Auxiliar;

    void CorrigeParametros(string fit, string unit);

    void CrearDisco(string fit, string unit, string tamanio, string path);

};

#endif
