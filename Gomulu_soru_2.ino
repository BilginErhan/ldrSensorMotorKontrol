#include <Servo.h>  //servo kütüphanesi tanımlandı
#include <avr/io.h> //avr/io.h kütüphanesi tanımlandı
#include <LiquidCrystal.h> // LiquidCrystal.h kütüphanesi tanımlandı

LiquidCrystal lcd(12,11,5,4,3,2); // lcd pinleri tanımlandı
Servo servo1; //servod1 nesnesi tanımlandı
Servo servo2; //servod2 nesnesi tanımlandı

void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
    
    // ADC aktif edilir ve gerekli bit değerleri 1 lenir
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
 
// adc okuyucu fonkstion
uint32_t adc_read(uint8_t ch)
{
    //gelen bit değeri and operatörüyle and lenir
    ch &= 0b00000111;  
    ADMUX = (ADMUX & 0xF8)|ch;     // son 3 bit sıfırlanır
 

    //gelen sinyali çevirme işlemimden önce ADSC 1 atanır.
    ADCSRA |= (1<<ADSC);
 
    //ADSC 0 olur tekrar 1 olana kadar beklenir.
    while(ADCSRA & (1<<ADSC));
    //okunan adc değeri geri döndürülür
    return (ADC);
}
 
int main(void)
{  
  cli();//kesmeler durdurulur
  DDRB=B001100;// servo motor pinleri output olarak ayarlanır
  DDRC =B000000; // ldr pinleri input olarak ayarlandı
  adc_init();//adc init fonksiyonu çağırılır.
  int i=0;// lcd temizleme için kullanılan değişken
  uint32_t adc_result0,adc_result1; // okunan adc değerleri bu değişkenlere atılır
  int servod1,servod2; // servoların derecelerini tutan değişken
  servo1.attach(10); // servo pinleri
  servo2.attach(9); //servo pinleri
  lcd.begin(16,2);  //lcd 16x2 lik olduğu tanımlanır
  sei();  // kesmeleri devam ettirir.
  while(1)
  {//döngü başlatılır
    adc_result0 = adc_read(2);      // analog2 deki okunan değer
    adc_result1 = adc_read(3);      // analog3 deki okunan değer
    
    
    servod1=(adc_result0*180.0)/975.0;
    servod2=(adc_result1*180.0)/975.0;
    //okunan adc değeri 0 ile 180 arasına alınır
    //çünkü servo motorlar 0-180 derece arası döner
    
    
    if(adc_result0>adc_result1)
    {//soldaki ldr nin direnci büyük ise 
      //servolar sola doğru döner ve referans noktasını
      //180 derecen başlar
      servod1=180-servod1;
      servod2=180-servod2;
      servo1.write(servod1);//servolar haraket ettirilir
      servo2.write(servod2);
    }
    if(adc_result0<adc_result1)
    {//eğer sağdaki ldr büyük ise 
      //servolar sağa doğru döner
      //referans noktası 0. dereceden başlar
      servo1.write(servod1);
      servo2.write(servod2);
    }
    if(adc_result0==adc_result1)
    {// eğer ldr lere eşit ışık düşerse
     // soldaki servo ışık şiddeti kadar sola döner
     // sağdaki servo ışık şiddeti kadar sağa döner
      servod1=180-servod1;
      servo1.write(servod1);
      servo2.write(servod2);
    }

    lcd.setCursor(0,0);//ilk sutun secilir
    lcd.print("Servo 1 :");//servo 1 yazdırılır
    lcd.print(servod1);//ve servo 1 in derecesi yazdırılır.
    lcd.setCursor(0,1);//ikinci sutun seçilir
    lcd.print("Servo 2 :"); // servo 2 yazdırılır
    lcd.print(servod2); // ve servo 2 nin derecesi yazdırılır.
    i=i+1;// 1 değeri her döngüde 1 arttırılır
    if(i==20)
    {//her 20 adımda bir i sıfırlanır ve 
      //lcd temizlenir
      i=0;
      lcd.clear();
    }
    
  }
}


