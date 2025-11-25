#include <FastLED.h>
CRGB monPixel;

#include <Arduino.h>
#define MA_BROCHE_ANGLE 32

#define MA_BROCHE_BOUTON 39

#include <MicroOscSlip.h>
// Le nombre 128 entre les < > ci-dessous est le nombre maximal d'octets réservés pour les messages entrants.
// Les messages sortants sont écrits directement sur la sortie et ne nécessitent pas de réservation d'octets supplémentaires.
MicroOscSlip<128> monOsc(&Serial);

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812, 27, GRB>(&monPixel, 1);
  monPixel = CRGB(255, 150, 0);
  FastLED.show();
}

void loop()
{
  int maLectureAnalogique;
  maLectureAnalogique = analogRead(MA_BROCHE_ANGLE);
  monOsc.sendInt("/pot", maLectureAnalogique);

  int maLectureBouton;
  maLectureBouton = digitalRead( MA_BROCHE_BOUTON );
 
  monOsc.sendInt( "/but" , maLectureBouton );
 
  delay(200);

  /*if (maLectureAnalogique < 2000)
  {
    monPixel = CRGB (0,0,255);
    FastLED.show();
  }

else if (maLectureAnalogique > 2000)
{
  monPixel = CRGB(255,150,0);
  FastLED.show();
};*/
}





























#include <Arduino.h>
#include <M5_PbHub.h>   
#include <MicroOscSlip.h>
#include <FastLED.h>

MicroOscSlip<128> monOsc(&Serial); 

#define CANAL_KEY_UNIT 1   
#define KEY_CHANNEL_ANGLE 0

unsigned long monChronoDepart ; // À DÉPLACER au début du code avec les autres variables globales
CRGB keyPixel;

M5_PbHub myPbHub; 

void myOscMessageParser(MicroOscMessage & receivedOscMessage) {
  // Ici, un if et receivedOscMessage.checkOscAddress() est utilisé pour traiter les différents messages
  if (receivedOscMessage.checkOscAddress("/color")) {  // MODIFIER /pixel pour l'adresse qui sera reçue
       int premierArgument = receivedOscMessage.nextAsInt(); // Récupérer le premier argument du message en tant que int
       int deuxiemerArgument = receivedOscMessage.nextAsInt(); // SI NÉCESSAIRE, récupérer un autre int
       int troisiemerArgument = receivedOscMessage.nextAsInt(); // SI NÉCESSAIRE, récupérer un autre int

       // UTILISER ici les arguments récupérés
       myPbHub.setPixelColor( CANAL_KEY_UNIT , 0 , premierArgument,deuxiemerArgument,troisiemerArgument );
       FastLED.show();
       

   // SI NÉCESSAIRE, ajouter d'autres if pour recevoir des messages avec d'autres adresses
   } else if (receivedOscMessage.checkOscAddress("/")) {  // MODIFIER /autre une autre adresse qui sera reçue
       // ...
   }
}

void setup() {
  Wire.begin();
  myPbHub.begin();
  Serial.begin(115200);
  
  myPbHub.setPixelCount(CANAL_KEY_UNIT, 1);
}

void loop() {
  monOsc.onOscMessageReceived(myOscMessageParser);

  if ( millis() - monChronoDepart >= 20 ) { 
    monChronoDepart = millis(); 
    
    int maLectureAnalogique = myPbHub.analogRead(KEY_CHANNEL_ANGLE);
    monOsc.sendInt("/angle", maLectureAnalogique);
  
    int maLectureKey = myPbHub.digitalRead(CANAL_KEY_UNIT);
    monOsc.sendInt("/but", maLectureKey); 
    
  }
} 
















































































using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using extOSC;

public class Player : MonoBehaviour
{
    public float torqueForce = 1f;
    public float jumpForce = 5f;

    public float checkRadius = 0.1f;
    public LayerMask groundLayer;

    private Rigidbody2D rb;

    public extOSC.OSCReceiver oscReceiver;
    public extOSC.OSCTransmitter oscTransmitter;



    private int etatEnMemoire = 1;
    private int etatEnMemoire2 = 1;// Le code initalise l'état initial du bouton comme relâché

      private void maLectureKey(OSCMessage message)
    {

        // Si le message n'a pas d'argument ou l'argument n'est pas un Int on l'ignore
        if (message.Values.Count == 0)
        {
            Debug.Log("No value in OSC message");
            return;
        }

        if (message.Values[0].Type != OSCValueType.Int)
        {
            Debug.Log("Value in message is not an Int");
            return;
            
        }

           int nouveauEtat = message.Values[0].IntValue; // REMPLACER ici les ... par le code qui permet de récuérer la nouvelle donnée du flux
        if (etatEnMemoire != nouveauEtat)
        { // Le code compare le nouvel etat avec l'etat en mémoire
            etatEnMemoire = nouveauEtat; // Le code met à jour l'état mémorisé
            if (nouveauEtat == 1)
            {
                if (IsGrounded())
                {
                    rb.AddForce(Vector2.up * jumpForce, ForceMode2D.Impulse);

                        var oSCMessage = new OSCMessage("/color");  // CHANGER l'adresse /pixel pour l'adresse désirée
                        oSCMessage.AddValue( OSCValue.Int(0) ); // Ajoute l'entier 255
                        oSCMessage.AddValue( OSCValue.Int(0) ); // Ajoute un autre 255
                        oSCMessage.AddValue( OSCValue.Int(0) ); // Ajoute un troisième 255
                        oscTransmitter.Send(oSCMessage); 
                        
                }  
                
                
            }
            else if (nouveauEtat == 0)
            {
                     var oSCMessage = new OSCMessage("/color");  // CHANGER l'adresse /pixel pour l'adresse désirée
                        oSCMessage.AddValue( OSCValue.Int(250) ); // Ajoute l'entier 255
                        oSCMessage.AddValue( OSCValue.Int(0) ); // Ajoute un autre 255
                        oSCMessage.AddValue( OSCValue.Int(0) ); // Ajoute un troisième 255
                        oscTransmitter.Send(oSCMessage); 
            }
        }

    }

    private void maLectureAnalogique(OSCMessage message)
    {

        // Si le message n'a pas d'argument ou l'argument n'est pas un Int on l'ignore
        if (message.Values.Count == 0)
        {
            Debug.Log("No value in OSC message");
            return;
        }

        if (message.Values[0].Type != OSCValueType.Int)
        {
            Debug.Log("Value in message is not an Int");
            return;
        }


        int AngleValue = message.Values[0].IntValue; // REMPLACER ici les ... par le code qui permet de récuérer la nouvelle donnée du flux

            if (AngleValue > 0)
            {
                rb.AddTorque(-torqueForce); // clockwise
           
            }
            else if (AngleValue < 0)
            {
                rb.AddTorque(torqueForce); // counter-clockwise
         
            }

    }




    void Start()
    {
        rb = GetComponent<Rigidbody2D>();

        oscReceiver.Bind("/but", maLectureKey);
        oscReceiver.Bind("/angle", maLectureAnalogique);
        if (oscReceiver == null) return;
    }

    void FixedUpdate()
    {
        // Roll left/right
        if (Input.GetKey(KeyCode.RightArrow))
        {
            rb.AddTorque(-torqueForce); // clockwise
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            rb.AddTorque(torqueForce); // counter-clockwise
        }

       
    }

    void Update()
    {
         // Jump
         // GetKeyDown() does not work in FixedUpdate()
        if (Input.GetKeyDown(KeyCode.UpArrow)) 
        {
            if (IsGrounded())
            {
                rb.AddForce(Vector2.up * jumpForce, ForceMode2D.Impulse);
            }
        }
    }

    public bool IsGrounded()
    {
        float extraHeight = 0.1f;
        RaycastHit2D hit = Physics2D.Raycast(
            transform.position,
            Vector2.down,
            GetComponent<CircleCollider2D>().radius + extraHeight,
            groundLayer
        );
        return hit.collider != null;
    }
}
