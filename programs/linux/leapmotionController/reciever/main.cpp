//****************************************************
//****************************************************
//
//              YARP: Streaming Control
//
//              Tipo: Receptor Leap Motion
//
//              Autor: David Velasco García
//		       @davidvelascogarcia
//
//****************************************************
//****************************************************

// Librerias
#include <iostream>
#include <string>
#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/Time.h>
#include <stdlib.h>
#include <stdio.h>

// Espacios de nombres
using namespace std;
using namespace yarp::os;

// Funcion primcipal
int main(){

// Iniciamos red yarp
cout<<"Iniciando red de Yarp..."<<endl;
Network yarp;

// Abrimos puerto
cout<<"Iniciando puerto receptor..."<<endl;
Port puerto_receptor;
puerto_receptor.open("/leapmotionController/data:i");
cout<<"Puerto /leapmotionController/data:i abierto correctamente."<<endl;
Bottle datos;

// Conectamos con el emisor
cout<<"Conectando con puerto emisor..."<<endl;
Network::connect("/leapmotionController/data:o","/leapmotionController/data:i");
cout<<"Conexion establecida correctamente"<<endl;

while(true){

// Leemos datos recibidos
puerto_receptor.read(datos);
string datos_recibidos="";
datos_recibidos=datos.toString();
cout<<datos_recibidos<<endl;

int bucle_control=0;
int x=0;
int y=0;
int z=0;
double garra=0;
string x_str="";
string y_str="";
string z_str="";
string garra_str="";

// Separamos el string en los ints de los valores
for(int i=0; i<datos_recibidos.length(); i++){

if(bucle_control==0){
if(datos_recibidos[i]==','){
bucle_control++;}
else if(datos_recibidos[i]!='"'){
x_str=x_str+datos_recibidos[i];}
}

else if(bucle_control==1){
if(datos_recibidos[i]==','){
bucle_control++;}
else if(datos_recibidos[i]!='"'){
y_str=y_str+datos_recibidos[i];}
}

else if(bucle_control==2){
if(datos_recibidos[i]==','){
bucle_control++;}
else if(datos_recibidos[i]!='"'){
z_str=z_str+datos_recibidos[i];}
}


else if(bucle_control==3){
if(datos_recibidos[i]==';'){
bucle_control++;}                                                            
else if(datos_recibidos[i]!='"'){
garra_str=garra_str+datos_recibidos[i];}
}

}
// Convertimos a int
x=atoi(x_str.c_str());
y=atoi(y_str.c_str());
z=atoi(z_str.c_str());
garra=atoi(garra_str.c_str());

// Mostramos valores
cout<<"Coordenadas X: "<<x_str<<endl;
cout<<"Coordenadas Y: "<<y_str<<endl;
cout<<"Coordenadas Z: "<<z_str<<endl;
cout<<"Coordenadas Garra: "<<garra_str<<endl;

}
}
