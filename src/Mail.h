#include <ESP_Mail_Client.h>

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

/* The sign in credentials */
#define AUTHOR_EMAIL "your_emailt@gmail.com"
#define AUTHOR_PASSWORD "12ac12ac12ac"

/* Recipient's email*/
#define RECIPIENT_EMAIL "send_to@email.com.br"

/* Define variaveis de temperatura e umidade */
// 1 = Hora
// 2 = Temperatura Minima
// 3 = Temperatura Maxima
// 4 = Temperatura Atual
// 5 = Umidade 
int RegManha[5];
int RegTarde[5];
int RegNoite[5];
// Dia do registro das informações
int RegDia;
int DiaHoje; 
bool emailEnviado = false;


void enviaEmail(float umidade, float temp, int nErro){
/* Declare the global used SMTPSession object for SMTP transport */
 SMTPSession smtp;

  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);

  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  //smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = -3;
  config.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("System Administrator");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Controle de Temperatura");
  message.addRecipient(F("TI"), RECIPIENT_EMAIL);

  /*Send HTML message*/
  //String htmlMsg1 = '';
  //String htmlMsg2 = '';
  //String htmlMsg3 = '';
  //message.html.content = htmlMsg1.c_str() + htmlMsg2.c_str() + htmlMsg3.c_str();
  //message.html.content = htmlMsg.c_str();
  //message.text.charSet = "us-ascii";
  //message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

   
  //Send raw text message
  String textMsg0 = "-------------------------\n";
  String textMsg1 = "| TEMPERATURA | UMIDADE |\n";
  String textMsg2 = "-------------------------\n";
  String textMsg3 = "|  " + String(float(temp)) + "*C    |   " + String(int(umidade)) + "%   |\n";
  String textMsg4 = "-------------------------\n";
  String textMsg = textMsg0 + textMsg1 + textMsg2 + textMsg3 + textMsg4;
  if (nErro == 0){
    message.text.content = textMsg.c_str();
  }else{
    String textMsgHeader = "Erro ao enviar dados. Servidor Inacessivel.\nCodigo do erro: " + String(nErro) + "\n";
    String textMsg = textMsgHeader + textMsg;
    message.text.content = textMsg.c_str();
  }
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Connect to the server */
  if (!smtp.connect(&config)){
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn()){
    Serial.println("\nNot yet logged in.");
  }
  else{
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
  
}


/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);
