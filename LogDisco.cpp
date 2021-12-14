#include <iostream>
#include <stdlib.h>
#include <string>
#include "Entrada.h"
#include "./LogDisco.h"
#include "./StructsGlobal.h"
#include <vector>
#include <algorithm>
#include "Decision.h"
#include "AuxMet.h"

using namespace std;

//Variables a utilizar en toda la clase LogDisco
int inicio;
entrada entradita;
decision decisiones;
aux Auxx;

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

void LogDiscos::ComandoRmdisk(vector<string> entrada){
    string dir;
    string AuxTokFor;
    //Verificamos que no este vacia la cadena
    if (entrada.size() ==0){
        entradita.AlertaError("COMANDO RMDISK", "No se encontro ninguna dirección valida, vuelva a intentarlo");

    }
    //Revisamos la dirección y eliminamos los parametros ~:~ para poder usarla
    for (string tokAux: entrada) {
        AuxTokFor = tokAux.substr(0,tokAux.find(":"));
        tokAux.erase(0,AuxTokFor.length()+2);
        AuxTokFor = AuxTokFor.substr(0,AuxTokFor.find("~"));
        if (entradita.Equals(AuxTokFor, "path")){
            //Si es correcto, copiamos el valor hacia la variable dir
            dir = tokAux;
        }else{
            //Error en la direccion
            dir="";
            entradita.AlertaError("COMANDO RMDISK","Se encontro un error en la direccion, en la linea: "+ AuxTokFor);
            break;
        }
    }
    if (!dir.empty()){
        //Revisamos si trae alguna comilla
        if (dir.substr(0,1)=="\"") {
            dir = dir.substr(1, dir.length() - 2);
        }
        //Intentamos ejecutar el comando RMDISK
        try{
            FILE *archivo = fopen(dir.c_str(), "r");
            if (archivo !=NULL){
                //Validamos la extension del disco
                if (!entradita.Equals(dir.substr(dir.find_last_of(".")+1),"disk")){
                    entradita.AlertaError("COMANDO RMDISK", "La extension ingresada no es valida, debe de ser .disk");
                    return;
                }
                fclose(archivo);
                if (decisiones.Respuesta("¿Estas seguro de eliminar este archivo?")){
                    if (remove(dir.c_str())==0){
                        decisiones.Mensaje("COMANDO RMDISK", "El disco se ha eliminado exitosamente");
                        return;
                    }
                }else{
                    decisiones.Mensaje("COMANDO RMDISK","Se ha negado el proceso de eliminacion del disco");
                    return;
                }
            }
            entradita.AlertaError("COMANDO RMDISK", "No se ha encontrado el disco especificado en la ruta");
        }
        catch (const exception& e){
            entradita.AlertaError("COMANDO RMDISK", "Se ha producido un error y no se logro eliminar el disco");
        }
    }

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

void LogDiscos::ComandoFdisk(vector<string> entrada){
    bool banderaBorrar= false;
    string id;
    for(string pivo : entrada){
        id = Auxx.Minus(pivo.substr(0, pivo.find(':')));
        pivo.erase(0, id.length()+2);
        if(pivo.substr(0,1)=="\""){
            pivo = pivo.substr(1, pivo.length()-2);
        }
        if(Auxx.Equals(id, "delete")){
            banderaBorrar = true;
        }
    }
    if(!banderaBorrar){
        vector<string> parametro={"size","path","name"};
        string unit= "k";
        string fit="WF";
        string Size;
        string Type ="P";
        string Name;
        string Add;
        string Path ;

        for ( auto pivo: entrada){
            string Auxid = Auxx.Minus(pivo.substr(0,pivo.find(':')));
            pivo.erase(0, Auxid.length()+2);
            if(pivo.substr(0,1)=="\""){
                pivo = pivo.substr(1,pivo.length()-2);
            }


            if(Auxx.Equals(Auxid,"u")){
                unit = pivo;
            }else if(Auxx.Equals(Auxid,"size")){
                if(count(parametro.begin(), parametro.end(),Auxid)){
                    auto itr = find(parametro.begin(),parametro.end(), Auxid);
                    parametro.erase(itr);
                    Size = pivo;
                }
            }else if(Auxx.Equals(Auxid,"type")){
                Type = pivo;
            }else if(Auxx.Equals(Auxid,"f")){
                fit = pivo;
            }else if(Auxx.Equals(Auxid,"path")){
                if(count(parametro.begin(),parametro.end(),Auxid)){
                    auto itr = find(parametro.begin(), parametro.end(), Auxid);
                    parametro.erase(itr);
                    Path=pivo;
                }
            }else if(Auxx.Equals(Auxid,"name")){
                if (count(parametro.begin(), parametro.end(), id)) {
                    auto itr = find(parametro.begin(), parametro.end(), id);
                    parametro.erase(itr);
                    Name = pivo;
                }
            }else if(Auxx.Equals(Auxid,"add")){
                Add = pivo;
                if (count(parametro.begin(), parametro.end(), "size")) {
                    auto itr = find(parametro.begin(), parametro.end(), "size");
                    parametro.erase(itr);
                    Size = pivo;
                }
            }
        }
        if(!parametro.empty()){
            Auxx.Alerta("COMANDO FDISK", "El parametro Create necesita  algunos parametros obligatorios, que no estan");
            return;
        }
        if(Add.empty()){
            Particion(Size,unit,Path,Type,fit,Name,Add);
        }else{
            AgregarParti(Add,unit,Name,Path);
        }
    }else{
        vector<string> parametro = {"delete","name","path"};
        string path;
        string name;
        string borrar;

        for (auto pivo: entrada) {
            string Auxid = Auxx.Minus(pivo.substr(0, pivo.find(':')));
            pivo.erase(0, Auxid.length()+2);
            if(pivo.substr(0,1)=="\""){
                pivo = pivo.substr(1, pivo.length()-2);
            }

            if(Auxx.Equals(Auxid, "name")){
                if(count(parametro.begin(), parametro.end(),Auxid)){
                    auto itr = find(parametro.begin(), parametro.end(),Auxid);
                    parametro.erase(itr);
                    name = pivo;
                }
            }else if(Auxx.Equals(Auxid, "delete")){
                if (count(parametro.begin(), parametro.end(), Auxid)) {

                    auto itr = find(parametro.begin(), parametro.end(), Auxid);
                    parametro.erase(itr);
                    borrar = pivo;
                }
            }else if(Auxx.Equals(Auxid, "path")){
                if (count(parametro.begin(), parametro.end(), Auxid)) {
                    auto itr = find(parametro.begin(), parametro.end(), Auxid);
                    parametro.erase(itr);
                    path = pivo;
                }
            }
        }
        if(parametro.size()!=0){
            Auxx.Alerta("COMANDO FDISK", "El parametro Create necesita  algunos parametros obligatorios, que no estan\n");
            return;
        }
        BorraParti(borrar, path, name);
    }

}

void LogDiscos::Particion(string size, string unit, string path, string type, string fit, string nam, string ad) {
    try{
        int estado = 0;
        int i = stoi(size);
        if(i<=0){
            throw runtime_error("El valor de size, no es mayor a 0");
        }
        if(path.substr(0,1)=="\""){
            path = path.substr(1, path.length()-2);
        }
        if(!(Auxx.Equals(type,"p") || Auxx.Equals(type,"e") || Auxx.Equals(type,"l"))){
            throw runtime_error("El parametro type \"T\" tiene valores que no esta definidos.");
        }
        if(Auxx.Equals(unit,"b") || Auxx.Equals(unit,"k") || Auxx.Equals(unit,"m")){
            if(Auxx.Equals(unit,"b")){
                i *=  (Auxx.Equals(unit,"k"))? 1024: 1024*1024;
            }
        }else{
            throw runtime_error("El parametro unit \"U\" tiene valores que no estan definidos.");
        }
        if(!(Auxx.Equals(fit,"bf") || Auxx.Equals(fit,"ff") || Auxx.Equals(fit,"wf"))){
            throw runtime_error("El parametro fit \"F\" tiene valores que no estan definidos.");
        }

        FILE *archivo = fopen(path.c_str(), "rb+");
        Structs::MBRStruct DiscoPart;
        //Se valida que el disco especificado se encuentre o exista en esa direccion
        if(archivo!=NULL){
            rewind(archivo);
            fread(&DiscoPart, sizeof(DiscoPart),1,archivo);
        }else{
            Auxx.Alerta("COMANDO FDISK", "El disco seleccionado, no exite o no se encuentra. "+path);
            return;
        }
        fclose(archivo);
        vector<Structs::StructParticion> particiones = JalaParticiones(DiscoPart);
        vector<Auxiliar> prueba;

        int Disp =0;
        int tama= sizeof(DiscoPart);
        int co=1;
        int e = 0;

        Structs::StructParticion StruExt;

        for(Structs::StructParticion patron: particiones){
            if(patron.DisponibilidadParte == '1'){
                Auxiliar aux;
                aux.termina = patron.IniciaParte + patron.TamanioParte;
                aux.particion = co;
                aux.principio = patron.IniciaParte;

                aux.pre = aux.principio - tama;
                tama = aux.termina;

                if(Disp!=0){
                    prueba.at(Disp-1).post = patron.IniciaParte - (prueba.at(Disp - 1).termina);
                }
                prueba.push_back(aux);
                Disp++;
                if(patron.Tipo =='E'|| patron.Tipo == 'e'){
                    e++;
                    StruExt = patron;
                }
            }
            if(Disp == 4 && !(Auxx.Equals(type,"l"))){
                throw runtime_error("Se han acabado las particiones, ya se han hecho 4");
            }else if(e==1 && Auxx.Equals(type,"e")){
                throw runtime_error("No se puede crear mas particiones logicas, unicamente particiones extendidas");
            }
            co++;
        }
        if(e==0 && Auxx.Equals(type,"l")) {
            throw runtime_error("No se puede realizar la particion logica, quiza no existe una particion extendida");
        }
        if(Disp !=0){
            prueba.at(prueba.size()-1.).post = DiscoPart.mbrTamanio - prueba.at(prueba.size()-1).termina;
        }
        try {
            Busqueda(DiscoPart, nam,path);
            Auxx.Alerta("COMANDO FDISK", "El nombre escrito ya existe, por favor proporcione otro");
            return;
        }catch (exception &e){
        }

        Structs::StructParticion Tra;

        Tra.TamanioParte=i;
        Tra.Tipo = toupper(type[0]);
        Tra.DisponibilidadParte='1';
        Tra.FitP= toupper(fit[0]);
        strcpy(Tra.NombreParte,nam.c_str());

        if(Auxx.Equals(type,"l")){
            Analisis(Tra, StruExt,path);
            return;
        }
        DiscoPart = Ajuste(DiscoPart, Tra, prueba, particiones, Disp);

        FILE *AuxFile= fopen(path.c_str(), "rb+");
            if(AuxFile!=NULL){
                fseek(AuxFile, 0 , SEEK_SET);
                fwrite(&DiscoPart,sizeof(Structs::StructMBR), 1, AuxFile);
                if(Auxx.Equals(type,"e")) {
                    Structs::StructEBR ebrr;
                    ebrr.ParteInicio = estado;
                    fseek(AuxFile, estado, SEEK_SET);
                    fwrite(&ebrr, sizeof(Structs::StructEBR),1,AuxFile);
                }
                fclose(AuxFile);
                Auxx.Respuesta("COMANDO FDISK", "La pinche particion fue creada con exito xD");
            }
        }
        catch (invalid_argument &e){
            Auxx.Alerta("COMANDO FDISK","El tamanio que trae el size no es valido, verifique que sea entero o mayor a 0");
            return;
    }
        catch (exception &e){
            Auxx.Alerta("COMANDO FDISK", e.what());
            return;
        }
}

void LogDiscos::Analisis(Structs::StructParticion particion, Structs::StructParticion auxi, string pa) {

}

Structs::MBRStruct
LogDiscos::Ajuste(Structs::StructMBR mbr, Structs::StructParticion parti, vector<Auxiliar> aux,vector<Structs::StructParticion> particiAux, int unit) {

}


Structs::StructParticion LogDiscos::Busqueda(Structs::MBRStruct mbr, string nombre, string path){
    Structs::StructParticion particiones[4];
    bool extendida= false;
    Structs::StructParticion PartExtend;


    particiones[0]= mbr.mbrParticion1;
    particiones[1]= mbr.mbrParticion2;
    particiones[2]= mbr.mbrParticion3;
    particiones[3]= mbr.mbrParticion4;

    for(auto &particion : particiones){
        if(particion.DisponibilidadParte =='1'){
            if(Auxx.Equals(particion.NombreParte, nombre)){
                return particion;
            }else if(particion.Tipo == 'E'){
                extendida=true;
                PartExtend = particion;
            }
        }
    }
    if(extendida){
        vector<Structs::StructEBR> EBR = ConsLog(PartExtend, path);
        for (Structs::StructEBR  eb: EBR) {
            if(eb.ParteEstado == '1'){
                if(Auxx.Equals(eb.ParteNombre, nombre)){
                    Structs::StructParticion AuxPart;
                    AuxPart.DisponibilidadParte = '1';
                    AuxPart.FitP = eb.ParteFit;
                    AuxPart.TamanioParte = eb.ParteTamanio;
                    AuxPart.Tipo = 'L';
                    AuxPart.IniciaParte = eb.ParteInicio;
                    strcpy(AuxPart.NombreParte, eb.ParteNombre);
                    return AuxPart;
                }
            }
        }
    }
    throw runtime_error("Verifique que la particion solicitada existe.");
}


vector<Structs::StructEBR> LogDiscos::ConsLog(Structs::StructParticion particion, string path) {

}



void LogDiscos::AgregarParti(string add, string unit, string name, string path) {

}

vector <Structs::StructParticion> LogDiscos::JalaParticiones(Structs::MBRStruct discoB){
    vector<Structs::StructParticion> ParVe;
    ParVe.push_back(discoB.mbrParticion1);
    ParVe.push_back(discoB.mbrParticion2);
    ParVe.push_back(discoB.mbrParticion3);
    ParVe.push_back(discoB.mbrParticion4);
    return ParVe;
}

void LogDiscos::BorraParti(string d, string path, string unit) {

}

string LogDiscos::BorrarEspacio(string Variable) {


    remove_if(Variable.begin(), Variable.end(), ::isspace);


    return Variable;
}


