#include <iostream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "Entrada.h"
#include "Mount.h"
#include "Reportes.h"

LogDiscos DiscoLogica;
Mount ComMount;
Reportes Reporte;

using namespace std;
string tok;
//Vector que esta dentro del metodo
//Vector que recibe el retorno del metodo
vector<string> tokRet;
//Variable para split de tokens que sirve para ir formando los parametros
string token = "";
//Variale direccion para comando exec
string dir="";

entrada::entrada() {
}

void LimpiarPantalla(){
    cout << "\x1B[2J\x1B[H";
}

void entrada::MenuInicio(){
    cout << "------------------------------BIENVENIDO A LA APLICACIÓN DE COMANDOS\n" << endl;
    cout << "-------------------------------ESCRIBA EXIT PARA SALIR DE LA APP\n" << endl;
    cout << "Escriba acá su comando:    ";

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
        //Probaremos si funcionan los scripts
        comandos(tok,tokRet);

        cout << "\n**********Programa en pausa**********\nPresione enter, para poder continuar:" << endl;
        getline(cin,comando);
        LimpiarPantalla();
        cout << "------------------------------BIENVENIDO A LA APLICACIÓN DE COMANDOS\n" << endl;
        cout << "-------------------------------ESCRIBA EXIT PARA SALIR DE LA APP\n" << endl;
        cout << "Escriba acá su comando:    ";
    }
}



bool entrada::Equals(string Uno, string Dos){
    if (CamMayusculas(Uno) == CamMayusculas(Dos))
    {
        return true;
    }
    return false;
}

void entrada::AlertaError(string comando, string alerta){

    cout << "\033[1;41m Error\033"<< "\033[0;31m(" + comando + ")-----> \033[0m"<< alerta << endl;

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

void entrada::AlertaMensaje(string comando, string alerta){

    cout << "\033[0;42m(" + comando + ")----->  \033[0m" << alerta << endl;
}


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

bool  entrada::ComparaCadena(string var){
    char prueba[]="PATH~:~";
    int n = var.length();
    char cadena[n];
    strcpy(cadena,var.c_str());
    for ( int i = 0; i < strlen(cadena); i++ )  {
        cadena[i] = toupper( cadena[i] );
    }
    if (strcmp(cadena,prueba) == 0){
        return true;
    }
    return false;
}

vector<string> entrada::splitTok(string tokensCad){
    vector<string> tokens;
    if (tokensCad.empty())
    {
        return tokens;
    }
    tokensCad.push_back(' ');

    int estado = 0;
    /*La variable pivote, es la que ira tomando el valor de cada una de las letras o signos
      que tokensCad traiga*/
    for(char &pivote: tokensCad){

      //si encuentra un - signica que es el inicio de algun parametro y cambiamos al estado 1
        if (estado ==0 && pivote == '-')
        {
            estado = 1;
            //Verificamos si sera o no un comentario entonces utilizamos el # para validarlo
        }else if(estado==0 && pivote == '#'){
            continue;
            //Si el estado sera diferente de 0, tenemos diferentes instrucciones para cada estado
        }/*else if(estado ==0 && pivote == '#' && ){

        }*/

        else if (estado==0 && pivote =='/') {
            estado = 4;
            token+=pivote;
        }else if(estado!=0){
            if (estado == 1)
            {
                if(pivote == '=' || pivote == '~'){
                    estado = 2;
                }else if(pivote == ' '){
                    continue;
                }
            }else if(estado == 2){
                if (pivote ==':'){
                    estado = 2;
                }else if (pivote == '~'){
                    estado = 4;
                }else if (pivote == '\"')
                {
                    estado = 3;
                }else{
                    estado = 4;
                }
            //Guardamos los path que han sido insertados
            }else if(estado == 3){
                if (pivote == '\"')
                {
                    estado = 4;
                }

            }else if (estado==4 && pivote == '\"')
            {
                //Vaciamos el array de tokens

                continue;
                //Guardamos el final de los parametros
            }else if (estado ==4 && pivote == ' ' && ComparaCadena(token) == true){
                estado= 4;
                continue;
            }//ACA TENGO QUE PROBAR LOS ESPACIOS PARA EL PATH
            else if (estado ==4 && pivote == ' ')
            {
                estado = 0;
                //Guardamos en el vector tokens al final el ultimo parametro
                tokens.push_back(token);
                cout<< token<<"\n";
                //Limpiamos la variable token
                token = "";
                continue;
            }
            token+=pivote;
        }
    }
    return tokens;
}

void entrada::comandoScript(vector<string> parametros){
        string tok="";
    for (string token: parametros) {
        tok=token.substr(0, token.find(":"));
        token.erase(0, tok.length()+2);
        tok = tok.substr(0,tok.find("~"));
        if (Equals(tok,"PATH")){
            dir = token;
        }
    }
    if (dir.empty()){
        AlertaError("COMANDO EXEC - SCRIPT","Sucedio un error, revise la dirección del comando");
        return;
    }else{
        cout<<"NO SE ENCONTRO NINGUN ERROR EN EL COMANDO SCRIPT\n";
    }
    script(dir);
}


void entrada::script(string direc){
    cout<<"PROBANDO SI FUNCIONA ESTA ONDA\n";
    vector<string> vecL;
    string auxLin;
    string tokenSc;
    string nombreArc(direc);
    string escritura;
    string p;


    ifstream archEn(nombreArc);
    //Error si no se llega a encontrar la direccion
    if (!archEn.is_open()){
        cerr <<"°°°°°°°°°°ERROR: El archivo no se abrio correctamente°°°°°°°°°°";
        return;
    }

    //Podemos leer en el archivo
    while(getline(archEn,auxLin)){
        vecL.push_back(auxLin);
    }
    for (const auto &i: vecL) {
         escritura=i;
        tokenSc= busquedaToken(escritura);
        if (Equals(escritura,"PAUSE")){
            cout << "\n**********Programa en pausa**********\n";
            AlertaMensaje("PAUSA","Presione la letra enter.......");
            getline(cin,p);
            continue;
        }
        escritura.erase(0,tokenSc.length()+1);
        vector <string> toks= splitTok(escritura);
        comandos(tokenSc,toks);
    }
    archEn.close();
    return;
}

void entrada::comandos(string tokenC, vector<string> tks)
{
    if (Equals(tokenC, "MKDISK"))
    {
        cout << "*****EJECUTANDO COMANDO MKDISK*****" << endl;
        DiscoLogica.ComandoMkdisk(tks);
    }else if(Equals(tokenC, "RMDISK")) {
        cout << "*****EJECUTANDO COMANDO RMDISK*****\n";
        DiscoLogica.ComandoRmdisk(tks);
    }else if(Equals(tokenC, "FDISK")) {
        cout << "*****EJECUTANDO COMANDO FDISK*****\n";
        DiscoLogica.ComandoFdisk(tks);
    }else if(Equals(tokenC,"MOUNT")) {
        cout << "*****EJECUTANDO COMANDO MOUNT*****\n";
        ComMount.Montar(tks);
    }else if(Equals(tokenC,"UNMOUNT")){
        cout << "*****EJECUTANDO COMANDO unMOUNT*****\n";
        ComMount.Desmontar(tks);
    }else if(Equals(tokenC, "EXEC")) {
        cout << "*****EJECUTANDO COMANDO EXEC*****\n";
        comandoScript(tks);
    }else if(Equals(tokenC,"REP")){
        Reporte.generaReporte(tks, ComMount);
    }else if(Equals(tokenC,"PAUSE")){
        cout<< "*****EJECUTANDO COMANDO PAUSE*****\n";
        cout << "\n**********Programa en pausa**********\nPresione enter, para poder continuar:" << endl;
        string comando;
        getline(cin,comando);
    }
    else if (Equals(tokenC.substr(0,1), "#")){
        cout<<"*****EJECUTANDO COMANDO COMENTARIO*****\n";
        AlertaMensaje("COMENTARIO",tokenC);
    }
}


