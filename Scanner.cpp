#include <iostream>
#include <cstring>
#include <stdio.h>
#include "Scanner.h"

using namespace std;


scanner::scanner() {
}

void LimpiarPantalla(){
    cout << "\x1B[2J\x1B[H";
}

void scanner::MenuInicio(){
    cout << "------------------------------BIENVENIDO A LA APLICACIÓN DE COMANDOS ---------------------------\n" << endl;
    cout << "-------------------------------ESCRIBA EXIT PARA SALIR DE LA APP--------------------------------\n" << endl;
    cout << "*";

    while (true){
        string comando;
        getline(cin, comando);
        LimpiarPantalla();
        //string AuxCom = Minuscula(comando);
        if (Equals(comando, "exit"))
        {
            exit(-1);
        }

    }
}

bool scanner::Equals(string Uno, string Dos){
    if (CamMayusculas(Uno) == CamMayusculas(Dos))
    {
        return true;
    }
    return false;
}

string scanner::CamMayusculas(string comandoEntrada){
    string auxComando="";
    for(char &a: comandoEntrada){
        auxComando+=toupper(a);
    }
    return auxComando;
}

/*string scanner::Minuscula(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}*/
