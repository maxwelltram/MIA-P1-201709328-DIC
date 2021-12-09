#include <iostream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include "Entrada.h"

using namespace std;
string tok;
//Vector que esta dentro del metodo
vector<string> tokens;
//Vector que recibe el retorno del metodo
vector<string> tokRet;
//Variable para split de tokens
string token = "";

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
         tok= busquedaToken(comando);

        //Borramos el token que acaba de registrar
        comando.erase(0,tok.length()+1);

        //Enviamos el resto de la cadena para que se hagan los splits
        tokRet = splitTok(comando);


        cout << "\n**********Programa en pausa**********\nPresione enter, para poder continuar:" << endl;
        getline(cin,comando);
        LimpiarPantalla();
        cout << "------------------------------BIENVENIDO A LA APLICACIÓN DE COMANDOS ---------------------------\n" << endl;
        cout << "-------------------------------ESCRIBA EXIT PARA SALIR DE LA APP--------------------------------\n" << endl;
        cout << "*";

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

vector<string> entrada::splitTok(string tokensCad){

    if (tokensCad.empty())
    {
        return tokens;
    }
    tokensCad.push_back(' ');

    int estado = 0;
    for(char &pivote: tokensCad){
        if (estado ==0 && pivote == '-')
        {
            estado = 1;

        }else if(estado==0 && pivote == '#'){
            continue;
        }else if(estado!=0){
            if (estado == 1)
            {
                if(pivote == '='){
                    estado = 2;
                }else if(pivote == ' '){
                    continue;
                }
            }else if(estado == 2){
                if (pivote == '\"')
                {
                    estado = 3;
                }else{
                    estado = 4;
                }

            }else if(estado == 3){
                if (pivote == '\"')
                {
                    estado = 4;
                }
            }else if (estado==4 && pivote == '\"')
            {
                //Vaciamos el array de tokens
                tokens.clear();
                continue;
            }else if (estado ==4 && pivote == ' ')
            {
                estado = 0;
                tokens.push_back(token);
                //Limpiamos la variable tokens
                token = "";
                continue;
            }
            token+=pivote;
        }
    }
    return tokens;
}