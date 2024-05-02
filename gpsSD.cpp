#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5); 
const int signal = D4;
const char* ssid = "4G HAVELI";
const char* password = "manjulika";

float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString;


WiFiServer server(80);
void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Serial.println(signal);
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);
      }

      if (gps.date.isValid())
      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
        DateString = '0';
        DateString += String(date);

        DateString += " / ";

        if (month < 10)
        DateString += '0';
        DateString += String(month);
        DateString += " / ";

        if (year < 10)
        DateString += '0';
        DateString += String(year);
      }

      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour()+ 5; //adjust UTC
        minute = gps.time.minute();
        second = gps.time.second();
    
        if (hour < 10)
        TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
        TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
        TimeString += '0';
        TimeString += String(second);
      }

    }
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  //Response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n
  <!DOCTYPE html>
<html>
<head>
    <title>NEO-6M GPS Readings</title>
    <style>
        /* Global Styles */
        body {
            font-family: Arial, sans-serif;
            background-color: #71cca0;
        }
        
        /* Header Styles */
        h1 {
            text-align: center;
            font-size: 36px;
            font-weight: bold;
            margin-bottom: 20px;
            color: #333;
        }
        
        /* Table Styles */
        table {
            width: 50%;
            margin: 40px auto;
            border-collapse: collapse;
            border: 1px solid #7ce709;
        }
        
        th, td {
            padding: 10px;
            border: 1px solid #000000;
        }
        
        th {
            background-color: #f4e406;
            font-weight: bold;
        }
        
        /* Link Styles */
        a {
            text-decoration: none;
            color: #6688a5;
        }
        
        a:hover {
            color: #23527c;
        }
        
        /* Conditional Styles */
       .valid-location {
            color: #cc2e2e;
        }
        
       .invalid-location {
            color: #e74c3c;
        }
    </style>
</head>
<body>
    <h1 style="text-align: center;">Smart Dustbin GPS Readings</h1>
    <p style="text-align: center; font-size: 18px; font-weight: bold;">Location Details</p>
    <table style="width: 50%; margin: 40px auto;">
        <tr>
            <th>Latitude</th>
            <td id="Latitude"><?LatitudeString?> </td>
        </tr>
        <tr>
            <th>Longitude</th>
            <td id="Longitude"> <?LongitudeString?> </td>
        </tr>
        <tr>
            <th>Date</th>
            <td id="Date"> <?DateString?> </td>
        </tr>
        <tr>
            <th>Time</th>
            <td id="Time"> <?TimeString?> </td>
                 </td>
        </tr>
    </table>
    <? if (gps.location.isvalid()) {?>
        <p style="text-align: center; margin-top: 20px;">
            <a href="http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=<? LatitudeString?>+<? LongitudeString?>" target="_top" class="valid-location">Click here</a> to open the location in Google Maps.
        </p>
    <? } else {?>
        <p style="text-align: center; margin-top: 20px; color: #e74c3c;"></p>
    <? }?>
</body>
</html>