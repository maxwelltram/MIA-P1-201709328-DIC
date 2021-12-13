#include <iostream>
#include <stdlib.h>
#include <string>
#include "Entrada.h"
#include "LogDisco.h"
#include <vector>
#include <algorithm>

using namespace std;

//Variables a utilizar en toda la clase LogDisco
int inicio;
entrada entradita;
bool Err= false;
//Variables para fit,unit y Dir en ComandoMkdisk
string fit;
string unit;
string Dir;

LogDiscos::LogDiscos(){}

void LogDiscos::ComandoMkdisk(vector<string> parametros){
    string Auxpara;
    string tamanio;
    fit ="";
    unit  ="";
    Dir = "";

    for (string parametro: parametros) {
        Auxpara = parametro.substr(0,parametro.find(":"));
        parametro.erase(0,Auxpara.length()+2);
        Auxpara = Auxpara.substr(0,Auxpara.find("~"));

        if (entradita.Equals(Auxpara,"f")){
            if (fit.empty()){
                fit = parametro;
            }else{
                entradita.AlertaError( "ERROR EN  MKDISK", "Sucedio un problema con el fit \"F\" en la linea:"+ Auxpara);
            }
        }else if (entradita.Equals(Auxpara, "u")){
            if (unit.empty()){
                unit = parametro;
            }else{
                entradita.AlertaError( "ERROR EN  MKDISK", "Sucedio un problema con la unit \"U\" en la linea:"+ Auxpara);
            }
        }else if (entradita.Equals(Auxpara, "size")){
            if (tamanio.empty()){
                tamanio = parametro;
            }else{
                entradita.AlertaError( "ERROR EN  MKDISK", "Sucedio un problema con el tamanio \"SIZE\" en la linea:"+ Auxpara);
            }
        }else if (entradita.Equals(Auxpara, "path")){
            if (Dir.empty()){
                Dir = parametro;
                Dir = BorrarEspacio(Dir);
            }else{
                entradita.AlertaError( "ERROR EN  MKDISK", "Sucedio un problema con la direccion \"PATH\" en la linea:"+ Auxpara);
            }
        }else{
            entradita.AlertaError("ERROR EN MKDISK", "Se encontro un parametro indefinido o que no se esperaba, en la linea: "+ Auxpara);
            Err = false;
            break;
        }
    }
    fit = CorrigeParametrosFit(fit);
    unit = CorrigeParametrosUnit(unit);
    if (Err){
        return;
    }
    if (Dir.empty() && tamanio.empty()){
        entradita.AlertaError("ERROR EN MKDISK", "Se encontraron parametros vacios en el PATH y SIZE");
    }else if (tamanio.empty()){
        entradita.AlertaError("ERROR EN MKDISK", "Se encontro un valor indefinido o vacio para el parametro SIZE");
    }else if (Dir.empty()){
        entradita.AlertaError("ERROR EN MKDISK", "Se encontro un valor indefinido o vacio para el parametro PATH");
    }else if (!entradita.Equals(unit,"k")&& !entradita.Equals(unit, "m")) {
        entradita.AlertaError("ERROR EN MKDISK", "Se encontro un valor incorrecto, para el parametro unit \"U\"");
    }else if (!entradita.Equals(fit,"BF")&& !entradita.Equals(fit, "FF") && !entradita.Equals(fit, "WF")) {
        entradita.AlertaError("ERROR EN MKDISK", "Se encontro un valor incorrecto, para el parametro fit \"F\"");
    }else{
        //Si todo sale bien, se hara la particion
        CrearDisco(fit,unit, tamanio,Dir);
    }
}

string LogDiscos::CorrigeParametrosFit(string fit) {
    if (fit.empty())
    {
        fit = "BF";
    }
    return fit;
}
string LogDiscos::CorrigeParametrosUnit(string unit){
    if (unit.empty())
    {
        unit = "M";
    }
    return unit;
}

void LogDiscos::CrearDisco(string fi, string un,string tam, string pat){
    //Llamamos el struct para crear el Disco c:
    Structs::MBRStruct CreaDisco;
    string AuxPath;
    string com;
    string com2;
    int tamanioDisk;
    try{
        tamanioDisk = stoi(tam);
        if (tamanioDisk<=0){
            entradita.AlertaError("ERROR EN MKDISK", "El tamaño ingresado no es mayor de 0");

        }
        if (entradita.Equals(un,"K")){
            //Tamanio del disco si es en Kilobytes
            tamanioDisk = 1024*tamanioDisk;
        }else if (entradita.Equals(un,"M")){
            //Tamanio del disco si es en Megabytes
            tamanioDisk = 1024*1024*tamanioDisk;
        }
        //Enviando datos si son validos
        fi = fi.substr(0,1);
        CreaDisco.mbrFechaC = time(nullptr);
        CreaDisco.mbrTamanio = tamanioDisk;
        CreaDisco.mbrPropiedad = rand() % 9999 +100;
        CreaDisco.mbrDiscoFit = toupper(fi[0]);
        //Creando el disco y validando si existe o no
        FILE *Comprobar = fopen(pat.c_str(), "r");
        //If comprueba la existencia del disco
        if (Comprobar ==NULL){
            entradita.AlertaMensaje("COMANDO MKDISK", "Creando Disco :D");
        }else if (Comprobar!= NULL){
            entradita.AlertaError("ERROR EN MKDISK", "Disco no se puede crear porque ya existe");
            fclose(Comprobar);
            return;
        }
        CreaDisco.mbrParticion1 = Structs::StructParticion();
        CreaDisco.mbrParticion2 = Structs::StructParticion();
        CreaDisco.mbrParticion3 = Structs::StructParticion();
        CreaDisco.mbrParticion4 = Structs::StructParticion();

        AuxPath = pat;

        if (pat.substr(0, 1)=="\""){
            pat= pat.substr(1,pat.length()-2);
        }
        if (!entradita.Equals(pat.substr(pat.find_last_of(".")+1),"disk")){
            entradita.AlertaError("ERROR EN MKDISK","La extension en el archivo es incorrecta");
            return;
        }

        try {
            FILE  *archivo = fopen(pat.c_str(),"w+b");
            if (archivo!=NULL){
                fwrite("\0",1,1,archivo);
                fseek(archivo,tamanioDisk-1,SEEK_SET);
                fwrite("\0",1,1,archivo);
                rewind(archivo);
                fwrite(&CreaDisco,sizeof(Structs::MBRStruct),1,archivo);
                fclose(archivo);
            } else{
                string com="mkdir -p \""+  pat + "\"";
                string com2="rmdir  \""+  pat + "\"";

                system(com.c_str());
                system(com2.c_str());

                FILE *archivo = fopen(pat.c_str(),"w+b");
                fwrite("\0",1,1,archivo);
                fseek(archivo,tamanioDisk-1,SEEK_SET);
                fwrite("\0",1,1,archivo);
                rewind(archivo);
                fwrite(&CreaDisco,sizeof(Structs::MBRStruct),1,archivo);
                fclose(archivo);
            }
            FILE *escribir = fopen(pat.c_str(),"r");
            if (escribir!=NULL){
                Structs::MBRStruct PrimerDisco;
                fseek(escribir, 0,SEEK_SET);
                fread(&PrimerDisco, sizeof(Structs::MBRStruct),1,escribir);
                struct  tm*tm;
                tm = localtime(&PrimerDisco.mbrFechaC);
                char FechaCreado[20];
                strftime(FechaCreado,20,"%Y/%m/%d %H:%M:%S", tm);
                cout<< "DISCO CREADO DE MANERA EXITOSA EN MKDISK\n";
                cout << "------------DETALLES DEL NUEVO DISCO------------\n";
                cout << "Date: "<< FechaCreado << "\n";
                cout << "Size: "<< PrimerDisco.mbrTamanio << "\n";
                cout << "Fit: "<< PrimerDisco.mbrDiscoFit << "\n";
                cout << "Disk Signature: "<< PrimerDisco.mbrPropiedad << "\n";
                cout << "Path: "<< AuxPath << "\n";
            }
            fclose(escribir);
        }
        catch (const exception& e){
            entradita.AlertaError("ERROR EN EL MKDISK", "Se encontro un error al intentar crear el disco D:");
        }
    }
    catch(invalid_argument &e){
        entradita.AlertaError("ERROR EN EL MKDSIK","Revise los parametros, el tamanio puede estar mal");
    }
}

string LogDiscos::BorrarEspacio(string Variable) {


    remove_if(Variable.begin(), Variable.end(), ::isspace);


    return Variable;
}


