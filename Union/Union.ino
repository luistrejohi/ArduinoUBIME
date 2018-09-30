#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

char* latitud[]={"22.783152", "22.7800266", "22.780825",
"22.779597", "22.777862","22.776091","22.775788","22.775919","22.775130","22.774913"};

char* longitud[]={"-102.615858", "-102.619258", "-102.620071",
"-102.619681", "-102.618523","-102.619290","-102.619207","-102.616228","-102.614303","-102.612147"};
int aux=0;
    String p_ut="_method=PUT";
    String lat="&latitud=";
    String lon="&longitud=";

    String del="_method=DELETE";

const char *host = "192.168.137.1:8000"; 
const char *ssid =  "localtest";     // replace with your wifi ssid and wpa2 key
const char *pass =  "192837465";
int cont = 0;
char Inicio = '|';
char Leer;
String Nombre; 
String ID[50];
int N;

String MOD_ID = "1";
String Saldo = "8.50";


WiFiClient client;

void setup() 
{       
       Serial.begin(9600);
         WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
          delay(1000);
          WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
       
       delay(10);
             
       WiFi.begin(ssid, pass); 
        Serial.print("Connecting");
       while (WiFi.status() != WL_CONNECTED){
            delay(500);
            Serial.print(".");
            cont=cont+1;
            }
          
          if(cont<10){
          Serial.println("");
          Serial.println("WiFi connected"); 
        
          }

    
}
 
void loop() {      
         HTTPClient http;
         String postData=""; 

          
         
          
          

          while(!Serial.available()){
            //String con los parámetros para modificar la longitud y latitud del módulo de pago
            
          http.begin("http://192.168.137.1:8000/api/pay_module/"+MOD_ID);//Va a actualizar el modulo con MOD_ID
          postData = p_ut+lat+latitud[aux]+lon+longitud[aux];
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //headers

          int httpCode = http.POST(postData);
          Serial.println(httpCode);   //Print HTTP return code
          Serial.println(postData);
          String payload = http.getString();    //Get the response payload
          Serial.println(payload);    //Print request response payload
          Serial.flush(); //Quitar si no jala
          http.end();  //Close 
         //////////////////////////////  
             if(aux<9){
                aux+=1;
              }
              else{
                aux=0;
              }
              delay(2000);  
          }


        
         Leer= Serial.read();
         if(Inicio == Leer){
            String IDx = Serial.readString();
            N = IDx.toInt();
                if(IDx == ID[N]){
                      
                     /* http.begin("http://192.168.137.1:8000/api/user_transport_ubication/"+N);//Va a actualizar el modulo con MOD_ID
                      postData = del;
                      http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //headers
            
                      int httpCode = http.POST(postData);
                      Serial.println(httpCode);   //Print HTTP return code
                      Serial.println(postData);
                      String payload = http.getString();    //Get the response payload
                      Serial.println(payload);    //Print request response payload
                      Serial.flush(); //Quitar si no jala
                      http.end();  //Close */
                      
                      ID[N]="";
                      Serial.println("Usuario Eliminado");
                }
                else{
                      ID[N]=IDx;  
                      Serial.println("Uusario Agregado");
                  
                       postData = "module_id="+MOD_ID+"&user_id="+ID[N]+"&saldo="+Saldo;
                       http.begin("http://192.168.137.1:8000/api/user_payment");              //Specify request destination
                       http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
              
                       int httpCode = http.POST(postData);   //Send the request
                       String payload = http.getString();    //Get the response payload
                       
                       Serial.println(httpCode);   //Print HTTP return code
                       Serial.println(payload);    //Print request response payload
               
                       http.end();  //Close 
                        ///////////////////////////////
                       postData = "module_id="+MOD_ID+"&user_id="+ID[N];
                       http.begin("http://192.168.137.1:8000/api/user_transport_ubication");              //Specify request destination
                       http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
              
                       httpCode = http.POST(postData);   //Send the request
                       payload = http.getString();    //Get the response payload
                       
                       Serial.println(httpCode);   //Print HTTP return code
                       Serial.println(payload);    //Print request response payload
               
                       http.end();  //Close 
                       Leer = ' ';
                }//fin del else
           }//fin  if principal
         

           ////////////////////////////////////////////////////////
          /////////////////////////////////////////////////////////
}
