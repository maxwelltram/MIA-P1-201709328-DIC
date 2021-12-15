#include <iostream>
#include <string>
#include "string"
#include <stdlib.h>
#include <locale>
#include <bits/stdc++.h>
#include "AuxMet.h"

using namespace std;

aux::aux(){

}

bool aux::Equals(string primero, string segundo){
    primero = Mayus(primero);
    segundo = Mayus(segundo);

    if(primero==segundo){
        return true;
    }
    return false;
}

void aux::Alerta(string comando, string accion) {
    cout << "\033[1;31m Error: \033" << "\033[0;31m(" + comando + ")~> \033[0m"
         << accion <<"\n";
}

string aux::convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

string aux::Minus(string entrada){
    string Aux;
    locale l;

    int n = entrada.length();
    char cadena[n];

    strcpy(cadena, entrada.c_str());

    for ( int i = 0; i < strlen(cadena); i++ )  {
        cadena[i] = tolower( cadena[i] );
    }
    Aux = convertToString(cadena,(sizeof(cadena)/sizeof(char)));
    return Aux;
}




string aux::Mayus(string entrada){
    string Aux;
    locale l;

    int n = entrada.length();
    char cadena[n];

    strcpy(cadena, entrada.c_str());

    for ( int i = 0; i < strlen(cadena); i++ )  {
        cadena[i] = toupper( cadena[i] );
    }
    Aux = convertToString(cadena,(sizeof(cadena)/sizeof(char)));
    return Aux;
}

bool aux::Opcion(string comando, string accion) {
    string dec;
    cout << "\033[1;36m Desea confirmar la siguiente acción: \033"
         << "\033[0;36m(" + comando + ")~> \033[0m"
         << "¿" + accion + "? Si/No : ";

    getline(cin, dec);

    if (Equals(dec, "s") || Equals(dec, "Si"))
    {
        return true;
    }
    return false;
}

void aux::Respuesta(string comando, string dato){
    cout << "\033[0;32m (" + comando + "): \033[0m"
         << dato <<"\n";
}
