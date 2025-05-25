//  Use the OpenWeather library: https://github.com/Bodmer/OpenWeather

//  The weather icons and fonts are in the sketch data folder, press Ctrl+K
//  to view.

// The ESP32 board support package 2.0.0 or later must be loaded in the
// Arduino boards manager to provide LittleFS support.

//            >>>       IMPORTANT TO PREVENT CRASHES      <<<
//>>>>>>  Set LittleFS to at least 1.5Mbytes before uploading files  <<<<<<

//            >>>           DON'T FORGET THIS             <<<
//  Upload the fonts and icons to LittleFS using the "Tools" menu option.

// You can change the "User_Setup.h" file inside the OpenWeather
// to shows the data stream from the server.

//////////////////////////////
// Settings defined below

#define WIFI_SSID      "YOUR_WIFI"
#define WIFI_PASSWORD  "123456789"


#define TIMEZONE brPT // See NTP_Time.h tab for other "Zone references", UK, usMT etc

// Update every 15 minutes, up to 1000 request per day are free (viz average of ~40 per hour)
const int UPDATE_INTERVAL_SECS = 15UL * 60UL; // 15 minutes

// Pins for the TFT interface are defined in the User_Config.h file inside the TFT_eSPI library

// For units use "metric" or "imperial"
const String units = "metric";

// Sign up for a key and read API configuration info here:
// https://openweathermap.org/, change x's to your API key
const String api_key = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Set the forecast longitude and latitude to at least 4 decimal places
const String latitude =  "-19.3256"; // 90.0000 to -90.0000 negative for Southern hemisphere
const String longitude = "-41.2553"; // 180.000 to -180.000 negative for West

// For language codes see https://openweathermap.org/current#multi
const String language = "pt_br"; // Default language = en = English

// Short day of week abbreviations used in 4 day forecast (change to your language)
const String shortDOW [8] = {"???", "DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"};

// Change the labels to your language here:
const char sunStr[]        = "Sol";
const char cloudStr[]      = "Nublado";
const char humidityStr[]   = "Umidade";
const String moonPhase [8] = {"   Nova   ", "Crescente ", "Qrt Cresc ", "Gibosa Cr ", "  Cheia   ", "Gibosa Min", "Qrt Mingu ", "Minguante "};

// End of user settings
//////////////////////////////
