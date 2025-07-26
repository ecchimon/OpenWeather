# Raspberry Pico W, RP2040 Nano Connect, ESP8266 and ESP32 OpenWeather client

// Disclaimer

Este é um clone do OpenWeather (https://github.com/openweathermap) modificado para usar sensor de temperatura local em conjunto com os dados fornecidos pela plataforma OpenWeather.
A versão original utiliza e LittleFS que não encontrei como usar nos ambientes de desenvolvimento atuais. Por isso anexei os arquivos da pasta Data diretamente no fonte do programa.
Esta baseado no microcontrolador esp32 do tipo CYD (cheap Yellow Device) com tela integrada, com o sensor de temperatura e pressão DHT11, mas pode ser usado outros sensores.
A razão desta adaptação é não apenas mostrar os dados do sensor, mas registrá-los em um banco de dados.
Para isso, os dados dos sensores são lidos e enviados em uma determinada frequencia (1 hora a princípio) a um servidor web, por meio de uma api escrita em PHP que alimentará um banco de dados.
Estes dados podem então serem consultados, permitindo a geração de relatórios.
Ainda foi implementado notificação através do envio de e-mail se pressão ou temperatura fiquem acima ou abaixo dos limites pré-estabelecidos

//-------------------------------------------------------------------------------------------------

// Original README.md -----------------------------------------------------------------------------

Arduino client library for https://openweathermap.org/

Collects current weather plus daily forecasts.

Requires the JSON parse library here:
https://github.com/Bodmer/JSON_Decoder

The OpenWeather_Forecast_Test example sketch sends collected data to the Serial port for API test. It does not not require a TFT screen and works with the Raspberry Pico W, RP2040 Nano Connect, ESP32 and ESP8266 processor boards. This example provides access to the weather data via a ser of variables, so could be adapted for use in weather related projects.

The TFT_eSPI_OpenWeather_LittleFS example works with the RP2040 Pico W, RP2040 Nano Connect, ESP32 and ESP8266. It uses LittleFS and displays the weather data on a TFT screen. This example uses the TFT_eSPI library.

The above examples will work with a free subscription to the OpenWeather service. The examples in the Onecall folder however require a subscription account (See OpenWeatherMap website for details).

The Raspberry Pico W and RP2040 Nano Connect must be used with Earle Philhower's board package:
https://github.com/earlephilhower/arduino-pico

These examples use anti-aliased fonts and newly created icons:

![Weather isons](https://i.imgur.com/luK7Vcj.jpg)

Latest screen grabs:

![TFT screenshot 1](https://i.imgur.com/ORovwNY.png)

