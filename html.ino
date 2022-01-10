// Head partias
// Style
const String styles = "<style>"
  ".main{padding: 15px 50px; display: flex; flex-direction:column; align-items: stretch; text-align:center}"
  "#power_container{height: 50vh; border: 3px solid DimGrey; padding: 2px; display: flex; flex-direction: column-reverse;}"
  ".phase_power_container{width: 100%; display: flex; justify-content: center; align-items: center; color: white}"
  "#phase_1_power_container{background-color: #4286f4;overflow:hidden;}"
  "#phase_2_power_container{background-color: #12ba77;overflow:hidden;}"
  ".graph_container{height:20vh;}"
  "</style>";


// Body partials
const String header = "<div class='header'>Power monitor</div>";

const String nav = "<div class='nav'>"
  "<a href='/'>Status</a>"
  "<a href='/plot'>Plot</a>"
  "<a href='/update_form'>Firmware update</a>"
  "</div>";
  
const String footer= "<div class='footer'>"
  "<img src='https://cdn.maximemoreillon.com/logo/logo.png'>"
  "  <div>"
  "    <div>Power monitor v2</div>"
  "    <div>Maxime Moreillon</div>"
  "  </div>"
  "</div>";

// Update form
String update_form = "<h2>Firmware update</h2>"
  "<form method='POST' action='/update' enctype='multipart/form-data'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>";
  
// Combinations
String head ="<head>"
  "<title>Power monitor</title>"
  + styles +
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<link rel='stylesheet' type='text/css' href='https://cdn.maximemoreillon.com/master_css/master.css'>"
  "</head>";

String pre_main = "<html>"
  + head + 
  "<body>"
  "<div class='wrapper'>"
  + header + nav +
  "<div class='main'>";
  
String post_main = "</div>"
  + footer +
  "</div>"
  "</body>"
  "</html>";
// Root
String root_main = ""
  "<div id='power_container'>"
  "  <div class='phase_power_container' id='phase_1_power_container'>"
  "  </div>"
  "  <div class='phase_power_container' id='phase_2_power_container'>"
  "  </div>"
  "</div>"
  "<script>"
  "websock = new WebSocket('ws://' + window.location.hostname + ':8087/');"
  "websock.onopen = function(evt) { console.log('websocket open'); };"
  "websock.onclose = function(evt) { console.log('websocket close'); };"
  "websock.onerror = function(evt) { console.log(evt); };"
  "websock.onmessage = function(evt) {"
  "  var max_current = 30.00;"
  "  var payload = JSON.parse(evt.data);"
  "  var phase_1_power_container = document.getElementById('phase_1_power_container');"
  "  phase_1_power_container.innerHTML = 'PHASE 1<br>' + payload.RMS_current_phase_1 + ' A';"
  "  phase_1_power_container.style.height = (payload.RMS_current_phase_1/max_current)*100 + '%';"
  "  var phase_2_power_container = document.getElementById('phase_2_power_container');"
  "  phase_2_power_container.innerHTML = 'PHASE 2<br>' + payload.RMS_current_phase_2 + ' A';"
  "  phase_2_power_container.style.height = (payload.RMS_current_phase_2/max_current)*100 + '%';"
  "};"
  "</script>";

// Root
String plot_main = ""
  "<h2>Phase 1</h2>"
  "<div class='graph_container' id='phase_1_graph'></div>"
  "<h2>Phase 2</h2>"
  "<div class='graph_container' id='phase_2_graph'></div>"
  "<script src='https://cdn.maximemoreillon.com/graph/graph.js'></script>"
  "<script>"
  "var phase_1_graph = new Graph('phase_1_graph');"
  "var phase_2_graph = new Graph('phase_2_graph');"
  "phase_1_graph.axes.y.min = 0;"
  "phase_1_graph.axes.y.max = 30;"
  "phase_2_graph.axes.y.min = phase_1_graph.axes.y.min;"
  "phase_2_graph.axes.y.max = phase_1_graph.axes.y.max;"
  "phase_2_graph.line_color = '#0000ce';"
  "var RMS_current_phase_1 = [];"
  "var RMS_current_phase_2 = [];"
  "var length = 200;"
  "for(var i = 0; i < length; i++) {"
  "  RMS_current_phase_1.push(0);"
  "  RMS_current_phase_2.push(0);"
  "}"
  ""
  "websock = new WebSocket('ws://' + window.location.hostname + ':8087/');"
  "websock.onopen = function(evt) { console.log('websocket open'); };"
  "websock.onclose = function(evt) { console.log('websocket close'); };"
  "websock.onerror = function(evt) { console.log(evt); };"
  "websock.onmessage = function(evt) {"
  "  var payload = JSON.parse(evt.data);"
  "  RMS_current_phase_1.shift();"
  "  RMS_current_phase_1.push(payload.RMS_current_phase_1);"
  "  RMS_current_phase_2.shift();"
  "  RMS_current_phase_2.push(payload.RMS_current_phase_2);"
  "  phase_1_graph.plot(RMS_current_phase_1);"
  "  phase_2_graph.plot(RMS_current_phase_2);"
  "};"
  "</script>";

// Not found
String not_found = "<h2>Not found</h2>";
