#ifndef UNTITLED1_AUXMET_H
#define UNTITLED1_AUXMET_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

class aux{
public:
  aux();
  string Minus(string entrada);
  string Mayus(string entrada);
  string convertToString(char* a, int size);
  bool Equals(string primero, string segundo);
  void Respuesta(string comando, string dato);
  void Alerta(string comando, string accion);
  bool Opcion(string comando, string accion);
};
#endif
