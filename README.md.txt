Este é um clone do OpenWeather (https://github.com/openweathermap) modificado para usar sensor de temperatura local em conjunto com os dados fornecidos pela plataforma OpenWeather.
Esta baseado no microcontrolador esp32 com o sensor de temperatura e pressão DHT11, mas pode ser usado outros sensores.
A razão desta adaptação é não apenas mostrar os dados do sensor, mas registrá-los em um banco de dados.
Para isso, os dados dos sensores são lidos e enviados em uma determinada frequencia (1 hora a princípio) a um servidor web, por meio de uma api escrita em PHP que alimentará um banco de dados.
Estes dados podem então serem consultados, permitindo a geração de relatórios.
Ainda foi implementado notificação através do envio de e-mail se pressão ou temperatura fiquem acima ou abaixo dos limites pré-estabelecidos
