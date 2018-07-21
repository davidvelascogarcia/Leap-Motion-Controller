
// Librerias

#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>
#include <stdio.h>
#include <fstream>
#include <string>

// Espacio de nombres

using namespace std;
using namespace yarp::os;

// Función principal

int main() {

// Red de yarp

Network yarp;

// Abrir puerto

Port leap_sender;
leap_sender.open("/leap_motion_datos");

// Bottle de almacenamiento

Bottle datos;
datos.clear();
string datos_fichero="";
string datos_fichero_2="";

// Bucle de envíonce

while(true){

// Leo fichero

ifstream fichero_datos;
fichero_datos.open("../../Leap_Motion_Controller_Basic/datos/datos.txt");
getline(fichero_datos, datos_fichero);
fichero_datos.close();

// Envío si son distintos

if(datos_fichero!=datos_fichero_2){

// Envío por yarp

datos.addString(datos_fichero);
leap_sender.write(datos);
datos_fichero_2=datos_fichero;
}
}

// Cerrar puerto

leap_sender.close();

    return 0;
}
