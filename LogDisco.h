
#ifndef UNTITLED1_LOGDISCO_H
#define UNTITLED1_LOGDISCO_H
#include <vector>
#include <string>
#include <algorithm>
#include "./StructsGlobal.h"
#include "./LogDisco.h"
#include "./Entrada.h"
#include "./AuxMet.h"

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


    void ComandoRmdisk(vector <string> comando);
    void Particion(string size, string unit, string path, string type, string fit, string nam, string ad);
    void AgregarParti(string add, string unit, string name, string path);
    void BorraParti(string d,string path, string unit);
    string CorrigeParametrosFit(string fit);
    string CorrigeParametrosUnit(string unit);
    string BorrarEspacio(string var);
    void CrearDisco(string fit, string unit, string tamanio, string path);
    void ComandoFdisk(vector<string> comando);
    void Analisis(Structs::StructParticion particion, Structs::StructParticion auxi, string pa);
    vector<Structs::StructParticion> JalaParticiones(Structs:: StructMBR disco);


    Structs::StructMBR Ajuste(Structs::StructMBR mbr, Structs::StructParticion parti, vector<Auxiliar> aux, vector<Structs::StructParticion> particiAux, int unit);


    vector<Structs::StructEBR> ConsLog(Structs::StructParticion particion, string path);

    Structs::StructParticion Busqueda(Structs::MBRStruct mbr, string nombre, string path);
private:
    aux Auxx;
};

#endif
