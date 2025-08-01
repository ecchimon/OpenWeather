//====================================================================================
//                                  Libraries
//====================================================================================

// Time library:
// https://github.com/PaulStoffregen/Time
#include <Time.h>

// Time zone correction library:
// https://github.com/JChristensen/Timezone
#include <Timezone.h>

// Libraries built into IDE
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <WiFiUdp.h>

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

//====================================================================================
//                                  Settings
//====================================================================================

#ifdef ESP32 // Temporary fix, ESP8266 fails to communicate with some servers...
// Try to use pool url instead so the server IP address is looked up from those available
// (use a pool server in your own country to improve response time and reliability)
//const char* ntpServerName = "time.nist.gov";
//const char* ntpServerName = "pool.ntp.org";
const char* ntpServerName = "time.google.com";
#else
// Try to use pool url instead so the server IP address is looked up from those available
// (use a pool server in your own country to improve response time and reliability)
// const char* ntpServerName = "time.nist.gov";
const char* ntpServerName = "pool.ntp.org";
//const char* ntpServerName = "time.google.com";
#endif

// Try not to use hard-coded IP addresses which might change, you can if you want though...
//IPAddress timeServerIP(129, 6, 15, 30);   // time-c.nist.gov NTP server
//IPAddress timeServerIP(24, 56, 178, 140); // wwv.nist.gov NTP server
IPAddress timeServerIP;                     // Use server pool

// Example time zone and DST rules, see Timezone library documents to see how
// to add more time zones https://github.com/JChristensen/Timezone

// Zone reference "UK" United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        //British Summer (Daylight saving) Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         //Standard Time
Timezone UK(BST, GMT);

// Zone reference "euCET" Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule  CET = {"CET ", Last, Sun, Oct, 3, 60};      //Central European Standard Time
Timezone euCET(CEST, CET);

// Zone reference "ausET" Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    //UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    //UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Zone reference "usET US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  //Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   //Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// Zone reference "usCT" US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, dowSunday, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, dowSunday, Nov, 2, -360};
Timezone usCT(usCDT, usCST);

// Zone reference "usMT" US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, dowSunday, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, dowSunday, Nov, 2, -420};
Timezone usMT(usMDT, usMST);

// Zone reference "usAZ" Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST, usMST);

// Zone reference "usPT" US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, dowSunday, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, dowSunday, Nov, 2, -480};
Timezone usPT(usPDT, usPST);

// Zone reference "brPT" BR South America Time Zone (SãoPaulo, Brasília)
TimeChangeRule brEDT = {"EDT", Second, dowSunday, Mar, 2, -180};
TimeChangeRule brEST = {"EST", First, dowSunday, Nov, 2, -180};
Timezone brPT(brEDT, brEST);


//====================================================================================
//                                  Variables
//====================================================================================
TimeChangeRule *tz1_Code;   // Pointer to the time change rule, use to get the TZ abbrev, e.g. "GMT"

time_t utc = 0;

bool timeValid = false;

unsigned int localPort = 2390;      // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE ]; //buffer to hold incoming and outgoing packets

uint8_t lastMinute = 0;
uint8_t lastHour = 0;

uint32_t nextSendTime = 0;
uint32_t newRecvTime = 0;
uint32_t lastRecvTime = 0;

uint32_t newTickTime = 0;
uint32_t lastTickTime = 0;

bool rebooted = 1;

uint32_t no_packet_count = 0;


//====================================================================================
//                                    Function prototype
//====================================================================================

void syncTime(void);
void displayTime(void);
void printTime(time_t zone, char *tzCode);
void decodeNTP(void);
void sendNTPpacket(IPAddress& address);

//====================================================================================
//                                    Update Time
//====================================================================================
void syncTime(void)
{
  // Don't send too often so we don't trigger Denial of Service
  if (nextSendTime < millis()) {
    // Get a random server from the pool
    WiFi.hostByName(ntpServerName, timeServerIP);
    nextSendTime = millis() + 5000;

    // Flush old late packets
    while  (udp.parsePacket() > 0)  {                // Is a packet there?
      Serial.println("Reading delayed NTP packet."); // Yes
      udp.read(packetBuffer, NTP_PACKET_SIZE);       // read the packet into the buffer
    }

    sendNTPpacket(timeServerIP); // send an NTP packet to a time server
    decodeNTP();
  }
}

//====================================================================================
// Send an NTP request to the time server at the given address
//====================================================================================
void sendNTPpacket(IPAddress& address)
{
  // Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;            // Stratum, or type of clock
  packetBuffer[2] = 6;            // Polling Interval
  packetBuffer[3] = 0xEC;         // Peer Clock Precision

  // 8 bytes of zero for Root Delay & Root Dispersion

  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

//====================================================================================
// Decode the NTP message and print status to serial port
//====================================================================================
void decodeNTP(void)
{
  timeValid = false;
  uint32_t waitTime = millis() + 500;
  while (millis() < waitTime && !timeValid)
  {
    yield();
    if (udp.parsePacket())
    {
      newRecvTime = millis();

      // We've received a packet, read the data from it
      udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

      Serial.print("\nNTP response time was  : ");
      Serial.print(500 - (waitTime - newRecvTime));
      Serial.println(" ms");

      Serial.print("Time since last sync is: ");
      Serial.print((newRecvTime - lastRecvTime) / 1000.0);
      Serial.println(" s");
      lastRecvTime = newRecvTime;

      // The timestamp starts at byte 40 of the received packet and is four bytes,
      // or two words, long. First, extract the two words:
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

      // Combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      unsigned long secsSince1900 = highWord << 16 | lowWord;

      // Now convert NTP Unix time (Seconds since Jan 1 1900) into everyday time:
      // UTC time starts on Jan 1 1970. In seconds the difference is 2208988800:
      utc = secsSince1900 - 2208988800UL;

      setTime(utc);      // Set system clock to utc time (not time zone compensated)

      timeValid = true;

      // Print the hour, minute and second:
      Serial.print("Received NTP UTC time  : ");

      uint8_t hh = hour(utc);
      Serial.print(hh); // print the hour (86400 equals secs per day)

      Serial.print(':');
      uint8_t mm = minute(utc);
      if (mm < 10 ) Serial.print('0');
      Serial.print(mm); // print the minute (3600 equals secs per minute)

      Serial.print(':');
      uint8_t ss = second(utc);
      if ( ss < 10 ) Serial.print('0');
      Serial.println(ss); // print the second
    }
  }

  // Keep a count of missing or bad NTP replies

  if ( timeValid ) {
    no_packet_count = 0;
  }
  else
  {
    Serial.println("\nNo NTP reply, trying again in 1 minute...");
    no_packet_count++;
  }

  if (no_packet_count >= 10) {
    no_packet_count = 0;
    // TODO: Flag the lack of sync on the display
    Serial.println("\nNo NTP packet in last 10 minutes");
  }
}

//====================================================================================
//                                  Debug use only
//====================================================================================
void printTime(time_t t, char *tzCode)
{
  String dateString = dayStr(weekday(t));
  dateString += " ";
  dateString += day(t);
  if (day(t) == 1 || day(t) == 21 || day(t) == 31) dateString += "st";
  else if (day(t) == 2 || day(t) == 22) dateString += "nd";
  else if (day(t) == 3 || day(t) == 23) dateString += "rd";
  else dateString += "th";

  dateString += " ";
  dateString += monthStr(month(t));
  dateString += " ";
  dateString += year(t);

  // Print time to serial port
  Serial.print(hour(t));
  Serial.print(":");
  Serial.print(minute(t));
  Serial.print(":");
  Serial.print(second(t));
  Serial.print(" ");
  // Print time t
  Serial.print(tzCode);
  Serial.print(" ");

  // Print date
  Serial.print(day(t));
  Serial.print("/");
  Serial.print(month(t));
  Serial.print("/");
  Serial.print(year(t));
  Serial.print("  ");

  // Now test some other functions that might be useful one day!
  Serial.print(dayStr(weekday(t)));
  Serial.print(" ");
  Serial.print(monthStr(month(t)));
  Serial.print(" ");
  Serial.print(dayShortStr(weekday(t)));
  Serial.print(" ");
  Serial.print(monthShortStr(month(t)));
  Serial.println();
}

//====================================================================================
