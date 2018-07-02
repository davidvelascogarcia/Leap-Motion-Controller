/*
*     Leap Motion: Controller
*
*     Autor: David Velasco García
*            @davidvelascogarcia
*
*/

// Librerias

#include <iostream>
#include <cstring>
#include "Leap.h"

// Espacio de nombres

using namespace Leap;
using namespace std;

// Clase escucha

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

const string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

// Función inicialización

void SampleListener::onInit(const Controller& controller) {
  cout << "Conexión inicializada" << endl;
}

// Función conexión

void SampleListener::onConnect(const Controller& controller) {
  cout << "Conexión con el dispositivo Leap Motion establecida correctamente" << endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

// Función desconexión

void SampleListener::onDisconnect(const Controller& controller) {
  cout << "Dispositivo Leap Motion desconectado" << endl;
}

// Función salida

void SampleListener::onExit(const Controller& controller) {
  cout << "Salida del sistema correcta" << endl;
}

// Función On Frame

void SampleListener::onFrame(const Controller& controller) {
  const Frame frame = controller.frame();
  cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << endl;

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {

    // Primera mano

    const Hand hand = *hl;
    string handType = hand.isLeft() ? "Left hand" : "Right hand";
    cout << string(2, ' ') << handType << ", id: " << hand.id()
              << ", palm position: " << hand.palmPosition() << endl;

    // Vector normal y dirección

    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Posición coordenadas cartesianas

   Leap::Vector coordenadas=hand.palmPosition();
   cout<<"Coordenadas cartesianas X, Y, Z:"<<endl;
   cout<<coordenadas.toString()<<endl;
   cout<<"Coordenadas X: "<<coordenadas[0]<<endl;
   cout<<"Coordenadas Y: "<<coordenadas[1]<<endl;
   cout<<"Coordenadas Z: "<<coordenadas[2]<<endl;

    // Calculo yaw, pitch y roll

    cout << string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << endl;

    // Get the Arm bone
    Arm arm = hand.arm();
    cout << string(2, ' ') <<  "Arm direction: " << arm.direction()
              << " wrist position: " << arm.wristPosition()
              << " elbow position: " << arm.elbowPosition() << endl;

    // Conseguir dedos

    const FingerList fingers = hand.fingers();
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
      const Finger finger = *fl;
      cout << string(4, ' ') <<  fingerNames[finger.type()]
                << " finger, id: " << finger.id()
                << ", length: " << finger.length()
                << "mm, width: " << finger.width() << endl;

      // Conseguir huesos de los dedos

      for (int b = 0; b < 4; ++b) {
        Bone::Type boneType = static_cast<Bone::Type>(b);
        Bone bone = finger.bone(boneType);
        cout << string(6, ' ') <<  boneNames[boneType]
                  << " bone, start: " << bone.prevJoint()
                  << ", end: " << bone.nextJoint()
                  << ", direction: " << bone.direction() << endl;
      }
    }
  }

  // Conseguir herramientas

  const ToolList tools = frame.tools();
  for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
    const Tool tool = *tl;
    cout << string(2, ' ') <<  "Tool, id: " << tool.id()
              << ", position: " << tool.tipPosition()
              << ", direction: " << tool.direction() << endl;
  }

  // Detectar gestos

  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        cout << string(2, ' ')
                  << "Circle id: " << gesture.id()
                  << ", state: " << stateNames[gesture.state()]
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness << endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        cout << string(2, ' ')
          << "Swipe id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() << endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        cout << string(2, ' ')
          << "Key Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<< endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        cout << string(2, ' ')
          << "Screen Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << screentap.position()
          << ", direction: " << screentap.direction()<< endl;
        break;
      }
      default:
        cout << string(2, ' ')  << "Gesto detectado desconocido" << endl;
        break;
    }
  }

  if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    cout << endl;
  }

}

// Función foco encontrado

void SampleListener::onFocusGained(const Controller& controller) {
  cout << "Foco encontrado" << endl;
}

// Función foco perdido

void SampleListener::onFocusLost(const Controller& controller) {
  cout << "Foco perdido..." << endl;
}

// Función cambio de Dispositivo

void SampleListener::onDeviceChange(const Controller& controller) {
  cout << "Device Changed" << endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    cout << "id: " << devices[i].toString() << endl;
    cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << endl;
  }
}

// Función servicio conectado

void SampleListener::onServiceConnect(const Controller& controller) {
  cout << "Servicio conectado correctamente" << endl;
  cout<<endl;
}

// Función servicio Desconectado

void SampleListener::onServiceDisconnect(const Controller& controller) {
  cout << "Servicio desconectado..." << endl;
  cout<<endl;
}

// Función main

int main(int argc, char** argv) {

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
