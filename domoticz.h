
/// domoticz
char* host = "192.168.1.20";
int httpPort = 8080;
//// Domoticz IDX 
String linky_IDX = "28";
String solar_IDX = "29";
String ballon_IDX = "61";
String linky;
String solar;
String linky_today;
String solar_today;
String ballon;

String batterie_IDX = "83";
String batterie;

const int wash = 45;
const int disher = 550 ; 

// un décallage visuel du texte rendu peut apparaitre, il est du à la police présente sur le projet 
/// liste des valeurs à afficher : {{"nom","IDX","codeJson"},{"Prod","28","CounterToday"}, {} }  // {} --> zone vide

// nombre de valeurs max affichable
const int max_val=12;
const String domoticz_IDX[max_val][3] = {{"Ballon","61","Data"},{"Batterie","83","Data"},{},{"Linky","28","Usage"},{"Today","28","CounterToday"},{},{"Solaire","29","Usage"},{"Today","29","CounterToday"},{},{"Gazpar","55","CounterToday"}} ; 
String domoticz_result[max_val]; 

String json_domoticz(char* host, int httpPort , String idx){
WiFiClient client;

if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return "connection failed";
  }
  
   String url = "/json.htm?type=devices&rid="+idx;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('OK');
  Serial.println("=======DATA ===");
  return line;
}

/////////////////// get "data" in json


void get_data_domoticz() {
  String json_data;
  const size_t capacity = 2*JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(16) + JSON_OBJECT_SIZE(50) + 1270;
  DynamicJsonDocument doc(capacity);

  //récupération des données dans domoticz
   for (int i = 0; i <= max_val ; i++) {

    if (domoticz_IDX[i][0] != NULL ) {
      json_data = json_domoticz(host, httpPort, domoticz_IDX[i][1]);
      DeserializationError error = deserializeJson(doc, json_data);
      JsonObject root = doc.as<JsonObject>();
      JsonObject result0 = root["result"][0];
      const char* clean_data = result0[domoticz_IDX[i][2]];
      domoticz_result[i] = clean_data ;
    //  Serial.println(domoticz_result[i]);
      Serial.println(clean_data);
    }
  }

  
}
