// ====================== Parse String ============================
String Json_parse_str(String key) {
  String intext = JSON.stringify(jsonObject[key]); intext.replace("\"","");
  return intext;
}

// ====================== Parse Integer ============================
int Json_parse_int(String key) {
  String intext = JSON.stringify(jsonObject[key]); intext.replace("\"","");
  return intext.toInt();
}
