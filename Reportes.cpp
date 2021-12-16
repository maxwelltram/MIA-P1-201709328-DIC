#include <iostream>
#include <stdlib.h>
#include <string>
#include "Reportes.h"
#include <locale>
using namespace std;

Reportes::Reportes(){
}



void Reportes::generaReporte(vector<string> entradita, Mount monta) {
    Montar = monta;

    vector<string> parametros = {"path", "name","id"};
    string path,id, name;

    for (string posi: entradita) {
        string Auxid = auxMet.Minus(posi.substr(0,posi.find(':')));
        posi.erase(0,Auxid.length()+2);
        Auxid = Auxid.substr(0,Auxid.find("~"));
        if(posi.substr(0,1)=="\""){
            posi = posi.substr(1,posi.length()-2);
        }
        if(auxMet.Equals(Auxid, "name")){
            if(count(parametros.begin(), parametros.end(), Auxid)) {
                auto itr = find(parametros.begin(), parametros.end(), Auxid);
                parametros.erase(itr);
                name = posi;
            }
        }else if(auxMet.Equals(Auxid,"id")){
            if(count(parametros.begin(), parametros.end(), Auxid)){
                auto itr = find(parametros.begin(), parametros.end(), Auxid);
                parametros.erase(itr);
                id = posi;
            }
        }else if(auxMet.Equals(Auxid,"path")){
            if(count(parametros.begin(), parametros.end(), Auxid)){
                auto itr = find(parametros.begin(), parametros.end(), Auxid);
                parametros.erase(itr);
                path = posi;
            }
        }
    }
    if(parametros.size()!=0){
        auxMet.Alerta("COMANDO REPORT", "No se encontraron algunos parametros, verifique los datos");
        return;
    }

    if(auxMet.Equals(name,"MBR")) {
        ReporteMBR(path, id);
    }else if(auxMet.Equals(name,"DISK")){
        ReporteDisk(path,id);
    }else{
        auxMet.Alerta("COMANDO REPORTE", "El reporte ingresado, no es valido o no existe");
    }
}


void Reportes::ReporteMBR(string path, string ide) {
    try{
        string direccion;
        Structs::StructParticion  particion= Montar.BusquedaMontar(ide, &direccion);
        FILE *archivo = fopen(direccion.c_str(),"rb+");
        if(archivo==NULL){
            throw runtime_error("El disco ingresado, no se ha encontrado");
        }

        Structs::MBRStruct DiscoAct;
        rewind(archivo);
        fread(&DiscoAct, sizeof(Structs::MBRStruct),1,archivo);
        fclose(archivo);
        string auxp= path.substr(0,path.find('.'));
        auxp+= ".dot";
        FILE *rep= fopen(auxp.c_str(),"r");
        if(rep == NULL){
            string coman= "mkdir -p \""+auxp+"\"";
            string comann= "rmdir \""+auxp+"\"";
            system(coman.c_str());
            system(comann.c_str());
        }else{
            fclose(rep);
        }

        Structs::StructParticion particiones[4];
        particiones[0] = DiscoAct.mbrParticion1;
        particiones[1] = DiscoAct.mbrParticion2;
        particiones[2] = DiscoAct.mbrParticion3;
        particiones[3] = DiscoAct.mbrParticion4;

        struct tm *tm;

        tm= localtime(&DiscoAct.mbrFechaC);
        char Fechita[20];
        strftime(Fechita,20, "%Y/%m/%d %H:%M:%S",tm);
        string contenidoGraph;
        contenidoGraph = "digraph G{\n"
                         "rankdir=TB;\n"
                         "graph [ dpi = \"600\" ]; \n"
                         "forcelabels= true;\n"
                         "node [shape = plaintext];\n"
                         "general [label = <<table>\n"
                         "<tr><td COLSPAN = '2' BGCOLOR=\"#102027\"><font color=\"white\">MBR</font></td></tr>\n"
                         "<tr><td BGCOLOR=\"#ff6f00\">NOMBRE</td><td BGCOLOR=\"#ff6f00\" >VALOR</td></tr>\n"
                         "<tr>\n"
                         "<td>mbr_tamaño</td>\n"
                         "<td>" +
                to_string(DiscoAct.mbrTamanio)  + "</td>\n"
                                                  "</tr>\n"
                                                  "<tr>\n"
                                                  "<td>mbr_fecha_creación</td>\n"
                                                  "<td>" +
                                                  string(Fechita)+ "</td>\n"
                                                                   "</tr>\n"
                                                                   "<tr>\n"
                                                                   "<td>mbr_disk_signature</td>\n"
                                                                   "<td>" +
                to_string( DiscoAct.mbrPropiedad)+ "</td>\n"
                                                   "</tr>\n"
                                                   "<tr>\n"
                                                   "<td>Disk_fit</td>\n"
                                                   "<td>" +
                                                   string(1,DiscoAct.mbrDiscoFit)+ "</td>\n"
                                                                                   "</tr>\n";
        Structs::StructParticion  PartiExt;
        bool AuxExt= false;
        for (int i = 0; i < 4; ++i) {
            if(particiones[i].DisponibilidadParte == '1'){
                if(particiones[i].Tipo=='E'){
                    AuxExt=true;
                    PartiExt = particiones[i];
                }
                contenidoGraph+="<tr>\n"
                                "<td>part_status_" + to_string(i + 1) + "</td>\n"
                                                                        "<td>" +
                                                                        particiones[i].DisponibilidadParte+" </td >\n"
                                                                                                           "</tr>\n"
                                                                                                           "<tr>\n"
                                                                                                           "<td>part_type_" + to_string(i + 1) + "</td>\n"
                                                                                                                                                 "<td>" +
                        particiones[i].Tipo + "</td>\n"
                                                  "</tr>\n"
                                                  "<tr>\n"
                                                  "<td>part_fit_" + to_string(i + 1) + "</td>\n"
                                                                                       "<td>" +
                        particiones[i].FitP + "</td>\n"
                                                 "</tr>\n"
                                                 "<tr>\n"
                                                 "<td>part_start_" + to_string(i + 1) + "</td>\n"
                                                                                        "<td>" +
                        to_string(particiones[i].IniciaParte) + "</td>\n"
                                                              "</tr>\n"
                                                              "<tr>\n"
                                                              "<td>part_size_" + to_string(i + 1) + "</td>\n"
                                                                                                    "<td>" +
                        to_string(particiones[i].TamanioParte) + "</td>\n"
                                                             "</tr>\n"
                                                             "<tr>\n"
                                                             "<td>part_name_" + to_string(i + 1) + "</td>\n"
                                                                                                   "<td>" +
                        particiones[i].NombreParte + "</td>\n"
                                                  "</tr>\n";
            }
        }
        int contador=0;
        if(AuxExt){
            vector<Structs::StructEBR> EBR = DiscoMeto.ConsLog(PartiExt, direccion);
            for (Structs::StructEBR AuxEBR: EBR) {
                contenidoGraph+="<tr><td BORDER=\"0\"></td><td BORDER=\"0\"></td></tr>"
                                "<tr><td COLSPAN = '2' BGCOLOR=\"#102027\"><font color=\"white\">EBR_" +
                                to_string(contador + 1) +
                                "</font></td></tr>\n"
                                "<tr><td BGCOLOR=\"#ff6f00\">NOMBRE</td><td BGCOLOR=\"#ff6f00\" >VALOR</td></tr>\n"
                                "<tr>\n"
                                "<td>part_status_" + to_string(contador + 1) + "</td>\n"
                                                                            "<td>" +
                                AuxEBR.ParteEstado + "</td>\n"
                                                  "</tr>\n"
                                                  "<tr>\n"
                                                  "<td>part_fit_" + to_string(contador + 1) + "</td>\n"
                                                                                           "<td>" +
                                AuxEBR.ParteFit + "</td>\n"
                                               "</tr>\n"
                                               "<tr>\n"
                                               "<td>part_start_" + to_string(contador + 1) + "</td>\n"
                                                                                          "<td>" +
                                to_string(AuxEBR.ParteInicio) + "</td>\n"
                                                            "</tr>\n"
                                                            "<tr>\n"
                                                            "<td>part_size_" + to_string(contador + 1) + "</td>\n"
                                                                                                      "<td>" +
                                to_string(AuxEBR.ParteTamanio) + "</td>\n"
                                                           "</tr>\n"
                                                           "<tr>\n"
                                                           "<td>" + to_string(contador + 1) + "</td>\n"
                                                                                                    "<td>" +
                                to_string(AuxEBR.ParteSig) + "</td>\n"
                                                           "</tr>\n"
                                                           "<tr>\n"
                                                           "<td>part_name" + to_string(contador + 1) + "</td>\n"
                                                                                                    "<td>" +
                                AuxEBR.ParteNombre + "</td>\n"
                                                "</tr>\n";
                contador++;
            }
        }
        contenidoGraph+="</table>>];";
        contenidoGraph += "\n\n}\n";
        ofstream outfile(auxp);
        outfile << contenidoGraph.c_str()<<"\n";
        outfile.close();
        string funcion = "dot -Tjpg " + auxp + " -o " + path;
        system(funcion.c_str());

        auxMet.Respuesta("COMANDO REPORTE", "Se ha generado el reporte MBR correctamente");
    }catch (exception &e){
        auxMet.Alerta("COMANDO REPORTE", e.what());
    }
}

void Reportes::ReporteDisk(string path, string ide) {
    try {
        string direccion;
        Structs::StructParticion particiones = Montar.BusquedaMontar(ide, &direccion);

        FILE *archivo = fopen(direccion.c_str(), "rb+");
        if (archivo == NULL) {
            throw runtime_error("El disco no se encontro o no existe");
        }

        Structs::StructMBR DiscoAct;
        rewind(archivo);
        fread(&DiscoAct, sizeof(Structs::StructMBR), 1, archivo);
        fclose(archivo);

        string aux = path.substr(0, path.find('.'));
        aux += ".dot";
        FILE *conca = fopen(aux.c_str(), "r");
        if (conca == NULL) {
            string coman = "mkdir -p \"" + aux + "\"";
            string coman2 = "rmdir \"" + aux + "\"";
            system(coman.c_str());
            system(coman2.c_str());
        } else {
            fclose(conca);
        }
        Structs::StructParticion particion[4];
        particion[0] = DiscoAct.mbrParticion1;
        particion[1] = DiscoAct.mbrParticion2;
        particion[2] = DiscoAct.mbrParticion3;
        particion[3] = DiscoAct.mbrParticion4;
        Structs::StructParticion extendida;

        bool Auxext = false;
        for (int i = 0; i < 4; ++i) {
            if (particion[i].DisponibilidadParte == '1') {
                if (particion[i].Tipo == 'E') {
                    Auxext = true;
                    extendida = particion[i];
                }
            }
        }

        string content;

        content = "digraph G{\n"
                    "rankdir=TB;\n"
                    "forcelabels= true;\n"
                    "graph [ dpi = \"600\" ]; \n"
                    "node [shape = plaintext];\n";
        content += "nodo1 [label = <<table>\n";
        content += "<tr>\n";

        int posicion[5] = {0, 0, 0, 0, 0};
        int posicion2[5] = {0, 0, 0, 0, 0};
        posicion[0] = DiscoAct.mbrParticion1.IniciaParte - (1 + sizeof(Structs::StructMBR));
        posicion[1] =
                DiscoAct.mbrParticion2.IniciaParte -
                (DiscoAct.mbrParticion1.IniciaParte + DiscoAct.mbrParticion1.TamanioParte);
        posicion[2] =
                DiscoAct.mbrParticion3.IniciaParte -
                (DiscoAct.mbrParticion2.IniciaParte + DiscoAct.mbrParticion2.TamanioParte);
        posicion[3] =
                DiscoAct.mbrParticion4.IniciaParte -
                (DiscoAct.mbrParticion3.IniciaParte + DiscoAct.mbrParticion3.TamanioParte);
        posicion[4] =
                DiscoAct.mbrTamanio + 1 - (DiscoAct.mbrParticion4.IniciaParte + DiscoAct.mbrParticion4.TamanioParte);
        copy(posicion, posicion2, posicion2);
        for (size_t j = 0; j < 5; j++) {
            bool negative = false;
            for (size_t i = 0; i < 4; i++) {
                if (posicion[i] < 0) {
                    negative = true;
                }
                if (posicion[i] <= 0 && posicion2[i] <= 0 && negative && posicion[i + 1] > 0) {
                    posicion[i] = posicion[i] + posicion[i + 1];
                    posicion[i + 1] = 0;
                }
            }
            negative = false;
        }
        int a = 0;
        string tempa;
        if (Auxext) {
            tempa = "<tr>\n";
            Structs::StructEBR EBRAux;
            FILE *extendi = fopen(direccion.c_str(), "r+b");
            fseek(extendi, extendida.IniciaParte, SEEK_SET);
            fread(&EBRAux, sizeof(Structs::StructEBR), 1, extendi);
            fclose(extendi);
            while (EBRAux.ParteSig != -1) {
                float res = (float) EBRAux.ParteTamanio / (float) DiscoAct.mbrTamanio;
                res = round(res * 10000.00F) / 100.00F;
                tempa += "<td>EBR</td>";
                tempa += "<td>Logica\n" + to_string(res) + "% del disco</td>\n";
                float resta = (float) EBRAux.ParteSig - ((float) EBRAux.ParteInicio + (float) EBRAux.ParteTamanio);
                resta = resta / DiscoAct.mbrTamanio;
                resta = resta * 10000.00F;
                resta = round(resta) / 100.00F;
                if (resta != 0) {
                    tempa += "<td>Logica\n" + to_string(resta) + "% libre del disco</td>\n";
                    a++;
                }
                a += 2;
                FILE *ext2 = fopen(path.c_str(), "r+b");
                fseek(ext2, EBRAux.ParteSig, SEEK_SET);
                fread(&aux, sizeof(Structs::StructEBR), 1, ext2);
                fclose(ext2);
            }
            float re = (float) EBRAux.ParteTamanio / (float) DiscoAct.mbrTamanio;
            re = round(re * 10000.00F) / 100.00F;
            tempa += "<td>EBR</td>";
            tempa += "<td>Logica\n" + to_string(re) + "% del disco</td>\n";
            float resta = (float) extendida.TamanioParte -
                          ((float) EBRAux.ParteInicio + (float) EBRAux.ParteTamanio - (float) extendida.IniciaParte);
            resta = resta / DiscoAct.mbrTamanio;
            resta = resta * 10000.00F;
            resta = round(resta) / 100.00F;
            if (resta != 0) {
                tempa += "<td>Libre\n" + to_string(resta) + "% del disco</td>\n";
                a++;
            }
            tempa += "</tr>\n\n";
            a += 2;
        }
        for (int i = 0; i < 4; ++i) {
            if (particion[i].Tipo == 'E') {
                content += "<td COLSPAN='" + to_string(a) + "'> Extendida </td>\n";
            } else {
                if (posicion[i] != 0) {
                    float res = (float) posicion[i] / (float) DiscoAct.mbrTamanio;
                    res = round(res * 100.0F * 100.0F) / 100.0F;
                    content += "<td ROWSPAN='2'> Libre \n" + to_string(res) + "% del disco</td>";
                } else {
                    float res = ((float) particion[i].TamanioParte) / (float) DiscoAct.mbrTamanio;
                    res = round(res * 10000.00F) / 100.00F;
                    content += "<td ROWSPAN='2'> Primaria \n" + to_string(res) + "% del disco</td>";
                }
            }

        }
        if (posicion[4] != 0) {
            float res = (float) posicion[4] / (float) DiscoAct.mbrTamanio;
            res = round(res * 100.0F * 100.0F) / 100.0F;
            content += "<td ROWSPAN='2'> Libre \n" + to_string(res) + "% del disco</td>";
        }
        content += "</tr>\n\n";
        content += tempa;
        content += "</table>>];\n}\n";
        ofstream outfile(aux);
        outfile << content.c_str() << endl;
        outfile.close();
        string function = "dot -Tjpg " + aux + " -o " + path;
        system(function.c_str());

        auxMet.Respuesta("COMANDO REPORT","El reporte DISK se genero de manera exitosa");
    }catch (exception &e){
        auxMet.Alerta("COMANDO REPORT", e.what());
    }
}