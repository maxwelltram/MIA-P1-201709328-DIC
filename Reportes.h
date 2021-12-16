#ifndef UNTITLED1_REPORTES_H
#define UNTITLED1_REPORTES_H
#include <string>
#include <bits/stdc++.h>
#include "Mount.h"
#include "LogDisco.h"
#include "StructsGlobal.h"
#include "AuxMet.h"
using namespace std;

class Reportes{
public:
    aux auxMet;
    LogDiscos DiscoMeto;
    Mount Montar;
    Reportes();
    void generaReporte(vector<string> context, Mount monta);
    void ReporteMBR(string path, string ide);
};


#endif