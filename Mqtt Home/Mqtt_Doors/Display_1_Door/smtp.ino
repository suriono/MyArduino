void smtp_init() {
   
  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = F("127.0.0.1");
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 6;
  config.time.day_light_offset = 0;
}

// =============================================================

void smtp_send() {
  //delay(1000);    // to prevent crashing
  //smtp.debug(1);
  /* Set the callback function to get the sending results */
  //smtp.callback(smtpCallback);
  /* Declare the Session_Config for user defined session credentials */
  //Session_Config config;
  /* Set the session config */
//  config.server.host_name = SMTP_HOST;
//  config.server.port = SMTP_PORT;
//  config.login.email = AUTHOR_EMAIL;
//  config.login.password = AUTHOR_PASSWORD;
//  config.login.user_domain = F("127.0.0.1");
//  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
 // config.time.gmt_offset = 3;
 // config.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;
  /* Set the message headers */
  message.sender.name = F("Me Uz");
  message.sender.email = AUTHOR_EMAIL;
  String subject = "ALERT: Freezer door is open !!!";
  message.subject = subject;
  message.addRecipient(F("Uz or myself"), RECIPIENT_EMAIL);
  String textMsg = "\nATTENTION: your freezer door is open.\n";
  
  message.text.content = textMsg;
  message.text.transfer_encoding = "base64"; // recommend for non-ASCII words in message.
  message.text.charSet = F("utf-8"); // recommend for non-ASCII words in message.
  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
   */
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
  message.addHeader(F("Message-ID: <suriono@gmail.com>"));
  /* Connect to the server */
  if (!smtp.connect(&config))
  {
    MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }
  if (!smtp.isLoggedIn())
  {
    Serial.println("Not yet logged in.");
  }
  else
  {
    if (smtp.isAuthenticated())
      Serial.println("Successfully logged in.");
    else
      Serial.println("Connected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

  delay(5000);    // to prevent crashing, could be reduced in the future once it is stable
}

// ==============================================================

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    // MailClient.printf used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    MailClient.printf("Message sent success: %d\n", status.completedCount());
    MailClient.printf("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)

      MailClient.printf("Message No: %d\n", i + 1);
      MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
      MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      MailClient.printf("Recipient: %s\n", result.recipients.c_str());
      MailClient.printf("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}
