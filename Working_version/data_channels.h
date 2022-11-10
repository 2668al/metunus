enum DataChannels {
  chNone = 0, /* данные никуда не отправляются, нет канала связи */
  chSms,      /* данные отправляются в виде SMS */
  chMail      /* данные отправляются на почту */
};

void send_sensors_data(int data, int channel);
