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

    if(auxMet.Equals(name,"MBR")){
        ReporteMBR(path,id);
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
                                                           "<td>part_next" + to_string(contador + 1) + "</td>\n"
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