#include <iostream>
#include <cstring>
#include <stdio.h>
#include "Entrada.h"

using namespace std;


entrada::entrada() {
}

void LimpiarPantalla(){
    cout << "\x1B[2J\x1B[H";
}

void entrada::MenuInicio(){
    cout << "------------------------------BIENVENIDO A LA APLICACIÓN DE COMANDOS ---------------------------\n" << endl;
    cout << "-------------------------------ESCRIBA EXIT PARA SALIR DE LA APP--------------------------------\n" << endl;
    cout << "*";

    while (true){
        //Recibe comando
        string comando;
        getline(cin, comando);
        //Limpiamos la pantalla
        LimpiarPantalla();
        //string AuxCom = Minuscula(comando);
        //Validamos si es o no exit, para terminar o no el programa
        if (Equals(comando, "exit"))
        {
            exit(-1);
        }
        //Validamos el comando y buscamos los tokens
        string tok= busquedaToken(comando);
        comando.erase(0,tok.length()+1);
    }
}

bool entrada::Equals(string Uno, string Dos){
    if (CamMayusculas(Uno) == CamMayusculas(Dos))
    {
        return true;
    }
    return false;
}

string entrada::CamMayusculas(string comandoEntrada){
    string auxComando="";
    for(char &a: comandoEntrada){
        auxComando+=toupper(a);
    }
    return auxComando;
}

/*string entrada::Minuscula(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}*/

string entrada::busquedaToken(string comando)
{
    bool banderaFin = false;
    //Creamos la variable auxToken para reconocer el tokem
    string auxToken = "";
    for (char &c : comando){
        //Si la bandera es true, entramos dentro del If
        if (banderaFin)
        {
            //Se valuan si son espacios o guiones, para poder ignorarlos
            if (c == ' ' || c == '-'){
                //Si encontramos alguno de estos, termina la concatenacion
                break;
            }
            //Concatenamos si es algo distinto a espacios o guiones
            auxToken += c;
        }
        else if ((c != ' ' && !banderaFin))
        {
            //Analizamos si se trata de un comenatario
            if (c=='#'){
                auxToken=comando;
                break;
            }else{
                //Concatenamos para guardar el comando
                auxToken += c;
                banderaFin = true;
            }
        }
    }
    //Retornamos el token encontrado
    return auxToken;
}