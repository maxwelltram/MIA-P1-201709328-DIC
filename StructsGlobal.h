#ifndef STRUCTSGLOBAL_H
#define STRUCTSGLOBAL_H

class   Structs{
public:
    Structs();

    //Struct de particion con sus campos requeridos
    typedef struct ParticionesStruct{
        int  TamanioParte = 0;
        int  IniciaParte=-1;
        char NombreParte[16];
        char DisponibilidadParte = 0;
        char FitP;
        char Tipo;
    }StructParticion;

    typedef  struct EBRStruct{
        int ParteInicio;
        int ParteTamanio = 0;
        int ParteSig=-1;
        char ParteFit;
        char ParteEstado ='0';
        char ParteNombre[16];

    }StructEBR;


    /*Struct del MBR con cada uno de sus campos y llamando
    al struct de particiones */
    typedef struct MBRStruct{
        time_t mbrFechaC;
        int    mbrPropiedad;
        int    mbrTamanio;
        char   mbrDiscoFit;
        StructParticion mbrParticion1;
        StructParticion  mbrParticion2;
        StructParticion mbrParticion3;
        StructParticion mbrParticion4;
    }StructMBR;



};

#endif
