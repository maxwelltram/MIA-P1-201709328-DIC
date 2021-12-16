#include "iostream"
#include "stdlib.h"
#include <locale>
#include "string"



#include "Mount.h"

using namespace std;

Mount::Mount() {
}

void Mount::Desmontar(vector<string> entrada) {
    vector<string> datos = {"id"};
    string ide;

    for (int i = 0; i < entrada.size(); i++) {
        string posi = entrada.at(i);
        string idAux = posi.substr(0, posi.find("~"));
        posi.erase(0,idAux.length()+3);

        if(auxx.Equals(idAux,"id")){
            auto itr = find(datos.begin(),datos.end(),idAux);
            datos.erase(itr);
            ide = posi;
        }
    }
    if(datos.size()!=0){
        auxx.Alerta("COMANDO UNMOUNT", "Ocurrio un error, revise todos los"
                                       "parametros ingresados");
        return;
    }
    DesmontarAgain(ide);
}

void Mount::DesmontarAgain(string ide) {
    try{
        string p=ide;
        if(!(ide[0]=='v' && ide[1] == 'd')){
            throw runtime_error("Identificador no valido o no existe");
        }
        char letra=ide[ide.length()-2];
        ide.erase(0,2);
        ide.erase(0,1);
        int i = stoi(ide)-1;
        if(i<0){
            throw runtime_error("El identificador es invalido");
        }
        for (int j = 0; j < 26; j++) {
            if(ListaMontar[i].MontParticiones[j].esado=='1'){
                if(ListaMontar[i].MontParticiones[i].id== letra){
                    ParticionMontada Parti= ParticionMontada();
                    ListaMontar[i].MontParticiones[j]=Parti;
                    auxx.Respuesta("COMANDO UNMOUNT", "Se completo la desmontacion exitosamente");
                    return;
                }
            }
        }
        throw runtime_error("No se ha encontrado el id especificado, verifique que exista");
    }
    catch (invalid_argument &e){
        auxx.Alerta("COMANDO UNMOUNT", "El indentificador es invalido");
        return;
    }
    catch (exception &e){
        auxx.Alerta("COMANDO UNMOUNT", e.what());
    }
}

void Mount::Montar(vector<string> entrada){
        if(entrada.empty()){
            ListaMontaciones();
            return;
        }
        vector<string> datos = {"path","name"};
        string nombre;
        string direccion;

    for (auto posi: entrada) {
        string dato=auxx.Minus(posi.substr(0,posi.find('~')));
        posi.erase(0,dato.length()+3);
        if(posi.substr(0,1)=="\""){
            posi = posi.substr(1,posi.length()-2);
        }
        if(auxx.Equals(dato,"name")){
            if(count(datos.begin(),datos.end(),dato)){
                auto itr = find(datos.begin(),datos.end(),dato);
                datos.erase(itr);
                nombre = posi;
            }
        }else if(auxx.Equals(dato, "path")){
            if(count(datos.begin(),datos.end(),dato)){
                auto itr = find(datos.begin(),datos.end(), dato);
                datos.erase(itr);
                direccion = posi;
            }
        }
    }
    if(datos.size() !=0){
        auxx.Respuesta("COMANDO MOUNT", "No se cumplen todos los parametros necesarios, "
                                        "verifique los datos");
    }
    MontarAgain(direccion,nombre);
}

void Mount::MontarAgain(string path, string name) {
    try{
        FILE *archivo= fopen(path.c_str(),"r");
        if(archivo==NULL){
            throw runtime_error("No se ha encontrado el disco, verifique el path");
        }
        Structs::StructMBR DiscoAct;
        rewind(archivo);
        fread(&DiscoAct, sizeof(Structs::StructMBR),1,archivo);
        fclose(archivo);

        Structs::StructParticion particionAct = MetoDisco.Busqueda(DiscoAct,name,path);
        if(particionAct.Tipo == 'E'){
            vector<Structs::EBRStruct> EBR= MetoDisco.ConsLog(particionAct,path);
            if(!EBR.empty()){
                Structs::EBRStruct ebrAux = EBR.at(0);
                name=ebrAux.ParteNombre;

            }else{
                throw runtime_error("Ocurrio un error al intentar montar la"
                                    "particion extendida");
            }
        }
        for (int i = 0; i < 99; i++) {
            if(ListaMontar[i].path == path){
                for (int j = 0; j < 26; j++) {
                    if(Mount::ListaMontar[i].MontParticiones[j].esado == '0'){
                        ListaMontar[i].MontParticiones[j].esado='1';
                        string let= Diccionario.at(j)+ to_string(i+1);
                        strcpy(ListaMontar[i].MontParticiones[j].nombre, name.c_str());
                        ListaMontar[i].MontParticiones[j].id= Diccionario.at(j);

                        auxx.Respuesta("COMANDO MOUNT", "Mount realizado exitosamente en: vd"+let);
                        return;
                    }
                }
            }
        }
        for (int i = 0; i < 99; i++) {
            if(ListaMontar[i].estado == '0'){
                ListaMontar[i].estado = '1';
                strcpy(ListaMontar[i].path,path.c_str());
                for (int j = 0; j < 26; j++) {
                    if(Mount::ListaMontar[i].MontParticiones[j].esado == '0'){
                        ListaMontar[i].MontParticiones[j].id = Diccionario.at(j);
                        ListaMontar[i].MontParticiones[j].esado = '1';
                        string let = Diccionario.at(j)+ to_string(i+1);
                        strcpy(ListaMontar[i].MontParticiones[j].nombre, name.c_str());
                        auxx.Respuesta("COMANDO MOUNT", "Mount realizado exitosamente en: vd"+let);
                        return;
                    }
                }
            }
        }
    }
    catch (exception &e){
        auxx.Alerta("COMANDO MOUNT", e.what());
        return;
    }
}

Structs::StructParticion Mount::BusquedaMontar(string ide, string *path) {
    if(!(ide[0] == 'v' && ide[1]== 'd')){
        throw runtime_error("El id no es valido, debe empezar con vd");
    }
    string ant=ide;
    char letra = ide[ide.length()-2];
    ide.erase(0,2);
    ide.erase(0,1);
    int i = stoi(ide);
    if(i<0){
        throw runtime_error("El id no es valido, verifiquelo");
    }

    for (int j = 0; j < 26; j++) {
        if(ListaMontar[i].MontParticiones[j].esado == '1') {
            if (ListaMontar[i].MontParticiones[j].id == letra) {
                FILE *archivo = fopen(ListaMontar[i].path, "r");
                if (archivo == NULL) {
                    throw ("El disco solicitado, no existe");
                }

                Structs::StructMBR DiscoBus;
                rewind(archivo);
                fread(&DiscoBus, sizeof(Structs::StructMBR), 1, archivo);
                fclose(archivo);
                *path = ListaMontar[i].path;
                return MetoDisco.Busqueda(DiscoBus, ListaMontar[i].MontParticiones[j].nombre, ListaMontar[i].path);
            }
        }
    }
    throw runtime_error("La partición que se ingreso, no se encuentra");
}

void Mount::ListaMontaciones(){
    for (int i = 0; i < 99; i++) {
        for (int j = 0; j < 26;  j++) {
            if(ListaMontar[i].MontParticiones[j].esado == '1'){
                cout << "> 87" << i + 1 << Diccionario.at(j) << ", " << ListaMontar[i].MontParticiones[j].nombre << endl;
            }
        }
    }
}