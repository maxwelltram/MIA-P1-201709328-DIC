#include "Decision.h"
#include "cstring"
#include "iostream"
#include "stdlib.h"
#include "./Entrada.h"
using namespace std;

entrada entra;

decision::decision(){

};
bool decision::Respuesta(string mensaje){
    std::cout << mensaje << "Escriba \"y\" para confirmar, si escribe cualquier otra letra sera tomada como cancelar\n";
    string respuesta;
    getline(cin,respuesta);
    if (entra.Equals(respuesta, "y"))
    {
        return true;
    }
    return false;

}

void decision::Mensaje(string comando, string mensaje){
    cout << "\033[0;42m(" + comando + ")~~> \033[0m"<< mensaje << endl;
}