void websocket_setup(){
  ws_server.begin();
}

void ws_send_periodically(){
  // Sending to websockets periodically
  static long last_ws_send_time;
  if(millis()- last_ws_send_time> WS_SEND_PERIOD){
    last_ws_send_time = millis();

    StaticJsonDocument<100> outbound_JSON_message; 
  
    outbound_JSON_message["RMS_current_phase_1"] = (String) phase_1_measurement.RMS_current;
    outbound_JSON_message["RMS_current_phase_2"] = (String) phase_2_measurement.RMS_current;
    
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));
    ws_server.broadcastTXT(JSONmessageBuffer);
    
  } 
}
