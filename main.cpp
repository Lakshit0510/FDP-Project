#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>



//unsigned long tim=millis();
class Stack { 
    int top; 
    int len;
public: 
    int a[20]; 
  
    Stack() { 
      top = -1; 
     len=0;
     } 
    void push(int x); 
    int pop(); 
    int le();
}; 
 
void Stack::push(int x) 
{ 
    
        a[++top] = x; 
        len++; 
   
} 
  
int Stack::pop() 
{ 
    
        int x = a[top--]; 
        
        return x; 
    
} 
int Stack::le(){

  return len;
  
  }


const char *ssid = "Aditya A71";
const char *password = "mcrh7563";

ESP8266WebServer server(80);

const int motorAPin1 = D1; // GPIO5
const int motorAPin2 = D2; // GPIO4
const int motorBPin1 = D3; // GPIO0
const int motorBPin2 = D4; // GPIO2

Stack in;
 Stack ti; 
void setup() {
 Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize motor control pins
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(motorBPin1, OUTPUT);
  pinMode(motorBPin2, OUTPUT);

  // Set up web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/forward", HTTP_GET, handleForward);
  server.on("/backward", HTTP_GET, handleBackward);
  server.on("/left", HTTP_GET, handleLeft);
  server.on("/right", HTTP_GET, handleRight);
  server.on("/stop",  HTTP_GET, handleStop);
  server.on("/reverse", HTTP_GET,handleReverse);
  server.on("/submit", HTTP_GET, handleSubmit);

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {

  String webpage = R"(
 <html>
<head>
  <script>
    var isWKeyPressed = false;
    var isSKeyPressed = false;
    var isDKeyPressed = false;
    var isAKeyPressed = false;
    var isRKeyPressed = false;
    var keyPressStartTime = 0;

    document.addEventListener('keydown', function(event) {
      if (event.key === 'w' && !isWKeyPressed) {
        isWKeyPressed = true;
        keyPressStartTime = Date.now();
        moveForward();
      } else if (event.key === 's' && !isSKeyPressed) {
        isSKeyPressed = true;
        keyPressStartTime = Date.now();
        moveBackward();
      } else if (event.key === 'a' && !isAKeyPressed) {
        isAKeyPressed = true;
        keyPressStartTime = Date.now();
        moveLeft();
      } else if (event.key === 'd' && !isDKeyPressed) {
        isDKeyPressed = true;
        keyPressStartTime = Date.now();
        moveRight();
      } else if (event.key === 'r' && !isRKeyPressed) {
        isRKeyPressed = true;
        keyPressStartTime = Date.now();
        reverse();
      }
    });

    document.addEventListener('keyup', function(event) {
      if (event.key === 'w') {
        isWKeyPressed = false;
        stopMoving();
        var keyPressDuration = Date.now() - keyPressStartTime;
        sendKeyPressData(event.key, keyPressDuration);
      } else if (event.key === 's') {
        isSKeyPressed = false;
        stopMoving();
        var keyPressDuration = Date.now() - keyPressStartTime;
      sendKeyPressData(event.key, keyPressDuration);
      } else if (event.key === 'a') {
        isAKeyPressed = false;
        stopMoving();
        var keyPressDuration = Date.now() - keyPressStartTime;
      sendKeyPressData(event.key, keyPressDuration);
      } else if (event.key === 'd') {
        isDKeyPressed = false;
        stopMoving();
        var keyPressDuration = Date.now() - keyPressStartTime;
      sendKeyPressData(event.key, keyPressDuration);
      } else if (event.key === 'r') {
        isRKeyPressed = false;
      }
      // Calculate key press duration and send it to the server
      
    });

    function sendKeyPressData(key, duration) {
      fetch('/submit?key=' + key + '&duration=' + duration);
    }

    function moveForward() {
      fetch('/forward');
    }

    function stopMoving() {
      fetch('/stop');
    }

    function moveBackward() {
      fetch('/backward');
    }

    function moveLeft() {
      fetch('/left');
    }

    function moveRight() {
      fetch('/right');
    }

    function reverse() {
      fetch('/reverse');
    }
  </script>
</head>
<body>
  <h1>WiFi Car Control</h1>
  <ul>
    <li>W: forward</li>
    <li>A: left</li>
    <li>S: backward</li>
    <li>D: right</li>
    <li>R: reroute</li>
  </ul>
</body>
</html>
  )";


  server.send(200, "text/html", webpage);
}

void handleForward() {
  moveForward();
  server.send(200, "text/plain", "Moving forward");
  Serial.println("Going Forward");
}

void handleBackward() {
  moveBackward();
  server.send(200, "text/plain", "Moving backward");
    Serial.println("Going Backward");
}

void handleLeft() {
  moveLeft();
  server.send(200, "text/plain", "Turning left");
    Serial.println("Going Left");
}

void handleRight() {
  moveRight();
  server.send(200, "text/plain", "Turning right");
    Serial.println("Going Right");
}


void handleStop() {
  stopCar();
  server.send(200, "text/plain", "Stopped");
}

void handleReverse(){
  reverse();  
  server.send(200, "text/plain", "Reversing");
     Serial.println("Reversing");
}

void handleSubmit() {
  String key = server.arg("key");
  String durationStr = server.arg("duration");

  if (key.length() > 0 && durationStr.length() > 0) {
    char keyCode = key.charAt(0);  // Extract the first character as the key code
    int duration = durationStr.toInt();  // Convert duration string to integer
    if(keyCode=='w'){
    in.push(0);
    ti.push(duration);}
    if(keyCode=='s'){
    in.push(1);
    ti.push(duration);}
    if(keyCode=='a'){
    in.push(2);
    ti.push(duration);}
    if(keyCode=='d'){
    in.push(3);
    ti.push(duration);}
    // Use the key code and duration as needed
    Serial.print("Key: ");
    Serial.println(keyCode);
    Serial.print("Duration: ");
    Serial.println(duration);
  }

  server.send(200, "text/plain", "Data received");
}

void moveForward() {
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, HIGH);
}

void moveBackward() {
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, HIGH);
  digitalWrite(motorBPin2, LOW);
}

void moveLeft() {
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin1, HIGH);
  digitalWrite(motorBPin2, LOW);
}

void moveRight() {
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, HIGH);
}

void stopCar() {
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin1, LOW);
  digitalWrite(motorBPin2, LOW);
}

void reverse(){
  
  uturn();
  for(int i=0;i<;i++){
    long tim3 = millis();
    long tim4=millis();
    int x = in.pop();
    long duration = ti.pop();
    if (x==0){
      
      while((tim4-tim3)<duration){
      moveForward();
      tim4=millis();
      }
      stopCar();
      delay(20);
      }
    else if (x==1){
      while((tim4-tim3)<duration){
      moveBackward();
      tim4=millis();
      }
      stopCar();
      delay(20);
      }
     else if (x==2){
      while((tim4-tim3)<duration){ 
      moveRight();
      tim4=millis();
      }
      stopCar();
      delay(20);
      }
     else if (x==3){
      while((tim4-tim3)<ti.pop()){
      moveLeft();
      tim4=millis();
      }
      stopCar();
      delay(20);
      }
    
    }
  stopCar();
}

void uturn(){
  long tim1=millis();
  long tim2 = millis();
  while((tim2-tim1)<1000){
    moveRight();
    tim2=millis();
    }
   stopCar();
  }
