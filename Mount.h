#ifndef UNTITLED1_MOUNT_H
#define UNTITLED1_MOUNT_H
#include <iostream>
#include <string>
#include "AuxMet.h"
#include <bits/stdc++.h>
#include "StructsGlobal.h"
#include "LogDisco.h"

using namespace std;
class Mount{
public:
    Mount();
    aux auxx;
    LogDiscos MetoDisco;

    typedef struct MountParticion{
        char esado = '0';
        char nombre[20];
        char id;
    }ParticionMontada;

    void Montar(vector<string> entrada);
    void Desmontar(vector<string> entrada);
    typedef struct MountDisco{
        char estado = '0';
        char path[150];
        ParticionMontada  MontParticiones[26];
    }DiscoMontado;
    DiscoMontado ListaMontar[99];
    vector<char> Diccionario={'a','b','c','d','e','f','g','h','i','j','k'
    ,'l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    void ListaMontaciones();
    void MontarAgain(string path, string name);
    void DesmontarAgain(string ide);

};

#endif

