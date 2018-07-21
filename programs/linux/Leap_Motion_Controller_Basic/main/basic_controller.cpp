// Librerias

#include <iostream>
#include <string.h>
#include "Leap.h"
#include <fstream>

// Espacio de nombres

using namespace Leap;
using namespace std;

// Clase escucha

class SampleListener : public Listener {

public:
  virtual void onConnect(const Controller&);
  virtual void onDisconnect(const Controller&);
  virtual void onFrame(const Controller&);

};

// Función conexión

void SampleListener::onConnect(const Controller& controller) {

cout << "Conectado al dispositivo Leap Motion" << endl;
// Enable gestures, set Config values:
controller.enableGesture(Gesture::TYPE_SWIPE);
controller.config().setFloat("Gesture.Swipe.MinLength", 200.0);
controller.config().save();

}

// Función desconexión

void SampleListener::onDisconnect(const Controller& controller) {
    cout << "Desconectado del dispositivo Leap Motion" << endl;
}

// Función On Frame

void SampleListener::onFrame(const Controller& controller) {
  const Frame frame = controller.frame();

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {

    // Primera mano

    const Hand hand = *hl;
    string handType = hand.isLeft() ? "Left hand" : "Right hand";

    // Vector normal y dirección

    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Posición coordenadas cartesianas

   Leap::Vector coordenadas=hand.palmPosition();
   cout<<endl;
   cout<<"Coordenadas cartesianas X, Y, Z:"<<endl;
   cout<<coordenadas.toString()<<endl;
   cout<<endl;
   cout<<"Coordenadas X: "<<coordenadas[0]<<endl;
   cout<<"Coordenadas Y: "<<coordenadas[1]<<endl;
   cout<<"Coordenadas Z: "<<coordenadas[2]<<endl;
   cout<<endl;
   cout<<"Apertura / Cerrado de la mano:"<<endl;
   cout<<hand.grabStrength()<<endl;
   cout<<endl;
   ofstream fichero_datos;
   fichero_datos.open("../datos/datos.txt");
   fichero_datos<<coordenadas[0]<<","<<coordenadas[1]<<","<<coordenadas[2]<<","<<hand.grabStrength()<<";";
   fichero_datos.close();

    }
  }

int main(int argc, char** argv){

  // Instanciación de controlador y objeto escucha

  SampleListener listener;
  Controller controller;

  // Mensaje de bienvenida

  cout<<endl;
  cout<<"*********************************"<<endl;
  cout<<"      Leap Motion: Controller    "<<endl;
  cout<<"*********************************"<<endl;
  cout<<endl;
  cout<<"Bienvenido al controlador Leap Motion"<<endl;

  // Añadir escucha al controlador

  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Espera

  cout<<endl;
  cout << "Presione una tecla para salir..." << endl;
  cin.get();

  // Quitar escucha al controlador

  controller.removeListener(listener);

  // Despedida

  cout<<endl;
  cout<<"Apagando controlador..."<<endl;
  cout<<endl;

  return 0;
}
