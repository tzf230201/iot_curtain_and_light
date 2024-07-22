#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
#define WIFI_SSID "pa_afis"//"circle"
#define WIFI_PASSWORD "qwerty123"//"Gr34tw4ll"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6080xxxxxx:AAEnxDxxxxxx_ziTF9s0-GOHD19Moxxxxxx"

const unsigned long BOT_MTBS = 500; // mean time between scan messages

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done



#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;

char t[32];

#define RED_PIN D8
#define GREEN_PIN D4
#define BLUE_PIN D3

String color = "red";

void RGBLedInit()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(BLUE_PIN, 0);
}

void RGBLedWrite(int r, int g, int b)
{
  analogWrite(RED_PIN, map(r,0,100,0,255));
  analogWrite(GREEN_PIN, map(g,0,100,0,255));
  analogWrite(BLUE_PIN, map(b,0,100,0,255));
}

void RGBLedTesting()
{
  RGBLedWrite(100, 100, 100);
  delay(100);
  RGBLedWrite(100, 0, 0);
  delay(100);
  RGBLedWrite(0, 100, 0);
  delay(100);
  RGBLedWrite(0, 0, 100);
  delay(100);
  RGBLedWrite(100, 100, 0);
  delay(100);
  RGBLedWrite(0, 100, 100);
  delay(100);
  RGBLedWrite(100, 0, 100);
  delay(100);
}

int jam_mulai = 17;
int menit_mulai = 30;
int detik_mulai = 00;

int jam_selesai = 6;
int menit_selesai = 30;
int detik_selesai = 00;

void RTCInit()
{
    Wire.begin(SDA, SCL);   //Setting wire (4 untuk SDA dan 5 untuk SCL)
 
    rtc.begin();
    // rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));  //Setting Time
  
  // Kalian dapat menambahkan bagian dibawah ini untuk set manual jam
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void RTCTesting()
{
  

  DateTime now = rtc.now();       //Menampilkan RTC pada variable now
 
  Serial.print("Tanggal : ");
  Serial.print(now.day());        //Menampilkan Tanggal
  Serial.print("/");
  Serial.print(now.month());      //Menampilkan Bulan
  Serial.print("/");
  Serial.print(now.year());       //Menampilkan Tahun
  Serial.print(" ");
  
  Serial.print("Jam : ");
  Serial.print(now.hour());       //Menampilkan Jam
  Serial.print(":");
  Serial.print(now.minute());     //Menampilkan Menit
  Serial.print(":");
  Serial.print(now.second());     //Menampilkan Detik
  Serial.println();
}

#define LDR_PIN PIN_A0
int prev_LDR = 0;

#define BTN_PIN D0
int prev_BTN = 0;

const int ledPin = LED_BUILTIN;
int ledStatus = 0;
int prev_ledStatus = 0;

#define NYALA 0
#define MATI 1

#define IN1_PIN D5
#define IN2_PIN D7

#define LAMP_PIN D6

// #define LS1_PIN RX
// #define LS2_PIN TX


unsigned long stopwatch = 0;




void actuatorController(int state)
{
  if(state == 1)
  {
    //motor
    digitalWrite(IN1_PIN, 1);
    digitalWrite(IN2_PIN, 0);
    //lampu
    digitalWrite(LAMP_PIN, NYALA); //active_low
  }
  else
  {
    //motor
    digitalWrite(IN1_PIN, 0);
    digitalWrite(IN2_PIN, 1);
    //lampu
    digitalWrite(LAMP_PIN, MATI);
  }
}

String chat_id;

enum class Mode {
  LDR,//Light Dependent Resistor
  RTC,//Real Time Clock
  BTN,//Button
  MSG//message 

};

Mode mode = Mode::LDR;

// Inisialisasi state mesin
enum class State {
  IDLE,       // State idle
  WELCOME,    // Menerima pesan baru
  CAHAYA,    // Memproses pesan
  TIMER,      // Mengirim balasan pesan
  TIMER2,
  BUTTON,
  TELEGRAM,
  RESET,
  ERROR       // Kesalahan
};

// Inisialisasi variabel state
State state = State::IDLE;

int kecerahan = 100;



void stateMachine(String chat_id,String text)
{

  String parsed0 = getValue(text, '%', 0);
  String parsed1 = getValue(text, '%', 1);


  if (text == "red" || text == "green" || text == "blue")
  {
    color = text;
    String msg = "warna led adalah: ";
    msg += text;
    msg += "\n\n";
    msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
    msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
    bot.sendMessage(chat_id, msg, "Markdown");
  }
  else if( (text.indexOf('%') >= 1) && parsed1 == "")
  {
    kecerahan = parsed0.toInt();

    String msg = "kecerahan : ";
    msg += text;
    msg += "\n\n";
    msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
    msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
    bot.sendMessage(chat_id, msg, "Markdown");
  }
  else
  {



  switch (state) {
    case State::IDLE: {
      // State idle
      // Tidak melakukan apa-apa
      if (text == "start")
      {
        String msg = "*Selamat Datang!*\n\n";
        msg += "Pilih salah satu input di bawah:\n";
        msg += "1 : Input cahaya (LDR)\n";
        msg += "2 : Input timer (RTC)\n";
        msg += "3 : Input button\n";
        msg += "4 : Input telegram\n\n";
        msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
        msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        state = State::WELCOME;
      }
      break;
    }
    case State::WELCOME: {
      if (text == "1")
      {
        String msg = "*Input cahaya telah aktif!*\n\n";
        msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n\n";
        msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        state = State::CAHAYA;
        mode = Mode::LDR;

      }
      else if (text == "2")
      {
        String msg = "*Input timer telah aktif!*\n\n";
        msg += "Silahkan masukkan waktu mulai dengan format #hh##mm#ss (misal #17#30#00)!\n\n";
        msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
        msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        state = State::TIMER;
        mode = Mode::RTC;

      }
      else if (text == "3")
      {
        String msg = "*Input button telah aktif!*\n\n";
        msg += "Silahkan gunakan button eksternal yang disediakan!\n\n";
        msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
        msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        state = State::BUTTON;
        mode = Mode::BTN;

      }
      else if (text == "4")
      {
        String msg = "*Input telegram telah aktif!*\n\n";
        msg += "Silahkan kirim pesan on atau off!\n\n";
        msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
        msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        state = State::TELEGRAM;
        mode = Mode::MSG;

      }
      else if (text == "reset")
      {
        state = State::RESET;
      }
      else
      {
        bot.sendMessage(chat_id, "perintah keliru\n", "");
      }

      break;
    }
    case State::CAHAYA: {

      

      if (text == "reset")
      {
        state = State::RESET;
      }
      else
      {
        bot.sendMessage(chat_id, "perintah keliru\n", "");
      }


      break;
    }
    case State::TIMER: {

      String sub_text0 = getValue(text, '#', 0);
      String sub_text1 = getValue(text, '#', 1);
      String sub_text2 = getValue(text, '#', 2);
      String sub_text3 = getValue(text, '#', 3);

      Serial.println("rtc");
      Serial.println(sub_text0);
      Serial.println(sub_text1);
      Serial.println(sub_text2);
      Serial.println(sub_text3);


      if (text == "reset")
      {
        state = State::RESET;
      }
      else if(sub_text0 == "")
      {
        String msg = "Data waktu mulai disimpan : ";
        msg += sub_text1 + ":";
        msg += sub_text2 + ":";
        msg += sub_text3 + "\n\n";
        msg += "Silahkan masukkan waktu akhir dengan format #hh##mm#ss (misal #6#30#00)!\n\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        jam_mulai = sub_text1.toInt();
        menit_mulai = sub_text2.toInt();
        detik_mulai = sub_text3.toInt();

        state = State::TIMER2;
      }
      else
      {
        bot.sendMessage(chat_id, "perintah keliru\n", "");
      }

      break;
    }
    case State::TIMER2: {

      String sub_text0 = getValue(text, '#', 0);
      String sub_text1 = getValue(text, '#', 1);
      String sub_text2 = getValue(text, '#', 2);
      String sub_text3 = getValue(text, '#', 3);

      Serial.println("rtc");
      Serial.println(sub_text0);
      Serial.println(sub_text1);
      Serial.println(sub_text2);
      Serial.println(sub_text3);


      if (text == "reset")
      {
        state = State::RESET;
      }
      else if(sub_text0 == "")
      {
        String msg = "Data waktu selesai disimpan : ";
        msg += sub_text1 + ":";
        msg += sub_text2 + ":";
        msg += sub_text3 + "\n\n";
        bot.sendMessage(chat_id, msg, "Markdown");

        jam_selesai = sub_text1.toInt();
        menit_selesai = sub_text2.toInt();
        detik_selesai = sub_text3.toInt();
      }
      else
      {
        bot.sendMessage(chat_id, "perintah keliru\n", "");
      }

      break;
    }
    case State::BUTTON: {

      

      if (text == "reset")
      {
        state = State::RESET;
      }
      else
      {
        bot.sendMessage(chat_id, "perintah keliru\n", "");
      }

      break;
    }
    case State::TELEGRAM: {

      if (text == "reset")
      {
        state = State::RESET;
      }
      else if (text == "on")
      {
        ledStatus = 1;

      }
      else if (text == "off")
      {
        ledStatus = 0;
      }
      else
      {
        bot.sendMessage(chat_id, "perintah keliru\n", "");
      }

      break;
    }
    case State::ERROR: {
      // State kesalahan
      // Menampilkan pesan kesalahan pada serial monitor
      Serial.println("Error");
      bot.sendMessage(chat_id, "ERROR! kirim //start untuk memulai ulang!\n", "");

      state = State::IDLE;
      break;
    }
  }

  }

  if (state == State::RESET)
  {
    String msg = "*Reset berhasil!*\n\n";
    msg += "Pilih salah satu input di bawah:\n";
    msg += "1. Input cahaya (LDR)\n";
    msg += "2. Input timer (RTC)\n";
    msg += "3. Input button\n";
    msg += "4. Input telegram\n\n";
    msg += "\\*Jika ingin mengganti warna tinggal kirim pesan warna yang dipilih\n";
    msg += "\\*Anda dapat mengganti tingkat kecerahan lampu dari 0%, 50% hingga 100%\n";
    bot.sendMessage(chat_id, msg, "Markdown");

    state = State::WELCOME;
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
 
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void routine()
{
  Serial.println("masuk rutin");
  switch (mode) 
  {
    case Mode::LDR: 
    {
      int read_LDR = analogRead(LDR_PIN) > 512? 0 : 1;
      // Serial.print("ldr = ");
      // Serial.println(read_LDR);
      if(read_LDR != prev_LDR)
      {
        if(read_LDR == 0)
        {
          bot.sendMessage(chat_id, "suasana terdeteksi gelap, menyalakan lampu\n", "");
        }
        else
        {
          bot.sendMessage(chat_id, "suasana terdeteksi terang, mematikan lampu\n", "");
        }
        actuatorController(!read_LDR);

        prev_LDR = read_LDR;
      }
      break;
    }
    case Mode::RTC: 
    {

      DateTime now = rtc.now();       //Menampilkan RTC pada variable now
      Serial.print("Jam : ");
      Serial.print(now.hour());       //Menampilkan Jam
      Serial.print(":");
      Serial.print(now.minute());     //Menampilkan Menit
      Serial.print(":");
      Serial.print(now.second());     //Menampilkan Detik
      Serial.println();

      int mulai = (now.hour() == jam_mulai) && (now.minute() == menit_mulai) && (now.second() >= detik_mulai);
      int selesai = (now.hour() == jam_selesai) && (now.minute() == menit_selesai) && (now.second() >= detik_selesai);

     
      if(selesai)
      {
        ledStatus = 0;
      }

      else if(mulai)
      {
        ledStatus = 1;
      }

      if(ledStatus != prev_ledStatus)
      {
        if(ledStatus == 1)
        {
          actuatorController(1);
          bot.sendMessage(chat_id, "memasuki waktu mulai , menyalakan lampu\n", "");
        }
        else
        {
          actuatorController(0);
          bot.sendMessage(chat_id, "memasuki waktu selesai , mematikan lampu\n", "");
        }
        actuatorController(ledStatus);

        prev_ledStatus = ledStatus;

      }

     
      break;
    }
    case Mode::BTN: 
    {
      int read_BTN = !digitalRead(BTN_PIN);

      if(read_BTN == 1)
      {
        ledStatus = !ledStatus;
        actuatorController(ledStatus);

        if(ledStatus == 1)
        {
          bot.sendMessage(chat_id, "button ditekan, menyalakan lampu\n", "");
        }
        else
        {
          bot.sendMessage(chat_id, "button ditekan, mematikan lampu\n", "");
        }
      }

      break;
    }
    case Mode::MSG: 
    {

      if(ledStatus != prev_ledStatus)
      {
        if(ledStatus == 1)
        {
          bot.sendMessage(chat_id, "lampu menyala\n", "");
        }
        else
        {
          bot.sendMessage(chat_id, "lampu mati\n", "");
        }
        actuatorController(ledStatus);

        prev_ledStatus = ledStatus;

      }
      
      break;
    }
  }
}



void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";


    stateMachine(chat_id, text);


  }
}




void setup()
{
  //inisialisasi serial
  Serial.begin(115200);
  Serial.println();

  // attempt to connect to Wifi network:
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  //menghubungkan ke jaringan wifi
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Check NTP/Time, usually it is instantaneous and you can delete the code below.
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  //tanda wifi sudah connect
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  for(int i = 0; i < 6; i++)
  {
    digitalWrite(ledPin, LOW); // initialize pin as off (active LOW)
    delay(50);
    digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)
    delay(50);
  }

  // inisialisasi RTC
  RTCInit();

  //inisialisasi led RGB
  RGBLedInit();
  pinMode(LDR_PIN, INPUT);
  pinMode(BTN_PIN, INPUT);

  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);

  digitalWrite(IN1_PIN, NYALA); //active low
  digitalWrite(IN2_PIN, MATI); //active low
  digitalWrite(LAMP_PIN, MATI); //active low
}

void loop()
{

  //jika waktu internal dikurangi waktu sebelumnya lebih dari 500 ms, maka check pesan masuk
  if (millis() - bot_lasttime > BOT_MTBS) 
  {
    // cek berapa jumlah pesan yang masuk dari telegram
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    // selama jumlah pesan masuk lebih dari 0, maka handle new message bakal dijalankan
    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    routine();

    bot_lasttime = millis();
  }

  int led_light = map(kecerahan,0,100,0,255);

  if (color == "red")
  {
    analogWrite(RED_PIN, led_light);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
  }
  else if (color == "green")
  {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, led_light);
    analogWrite(BLUE_PIN, 0);
  }
  else if (color == "blue")
  {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, led_light);
  }  
}
