//----------------------------------------------------------------------------//
//                     *** ФОРМАТ TLE И АЛГОРИТМ SGP4 ***                     //
//                                                                            //
// Файл TestMain.cpp                                                          //
//                                                                            //
//                           *** ТЕСТОВЫЙ ПРОЕКТ ***                          //
//                                                                            //
// Автор ГЛУЩЕНКО Сергей                                                      //
//                                                                            //
//                                                           Москва, 2022 год //
//----------------------------------------------------------------------------//


#include <QCoreApplication>
#include <QTextStream>
#include <QTextCodec>
#include "../TLE_SGP4.h"

#define __TEST__

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QTextStream cout(stdout);
  TLE_SGP4 *Calc;
  bool res;
  std::string str1, str2;
  int Year, Month, Day, Hour, Min, Sec;

  #ifndef __unix__
  cout.setCodec(QTextCodec::codecForName("CP866"));
  #else
  cout.setCodec(QTextCodec::codecForName("UTF8"));
  #endif

  #ifdef __TEST__
  //Тестирование
  str1 = "1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753";
  str2 = "2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.82419157413667     0.00      4320.0        360.00";
  #else
  //Международная Космическая Станция
  str1 = "1 25544U 98067A   22021.11265549 -.00060239  00000-0 -10723-2 0  9997";
  str2 = "2 25544  51.6423 344.9290 0006791  43.3864  46.4901 15.49530317322374";
  #endif

  Calc = new TLE_SGP4();

  Calc->GetCurrentTime(&Year, &Month, &Day, &Hour, &Min, &Sec);
  cout << QString::fromUtf8("Текущее системное время: ") << QString::number(Day) << QString::fromUtf8(".") << QString::number(Month)
       << QString::fromUtf8(".") << QString::number(Year) << QString::fromUtf8("  ") << QString::number(Hour)
       << QString::fromUtf8(":") << QString::number(Min) << QString::fromUtf8(":") << QString::number(Sec) << QString::fromUtf8("\n");

  res = Calc->ReadTLE(str1, str2);

  if (res == true)
  {
    cout << QString::fromUtf8("Успешное чтение TLE\n");

    double r[3];
    double v[3];

    #ifdef __TEST__
    int rez = Calc->GetRV(750.0, r, v);
    if (rez == 0)
    {
      cout << QString::fromUtf8("\n");
      cout << QString::fromUtf8("Результат работы GetRV (задание минут, прошедших после начала эпохи TLE в UTC)\n");
      cout << QString::fromUtf8("r.x = ") << QString::number(r[0], 'g', 15) << QString::fromUtf8(" м\n");
      cout << QString::fromUtf8("r.y = ") << QString::number(r[1], 'g', 15) << QString::fromUtf8(" м\n");
      cout << QString::fromUtf8("r.z = ") << QString::number(r[2], 'g', 15) << QString::fromUtf8(" м\n");
      cout << QString::fromUtf8("\n");
      cout << QString::fromUtf8("v.x = ") << QString::number(v[0], 'g', 15) << QString::fromUtf8(" м/сек\n");
      cout << QString::fromUtf8("v.y = ") << QString::number(v[1], 'g', 15) << QString::fromUtf8(" м/сек\n");
      cout << QString::fromUtf8("v.z = ") << QString::number(v[2], 'g', 15) << QString::fromUtf8(" м/сек\n");
    }
    else
    {
      cout << QString::fromUtf8("Ошибка GetRV (задание минут, прошедших после начала эпохи TLE в UTC)\n");
    }

    r[0] = -1.0;
    r[1] = -1.0;
    r[2] = -1.0;
    #endif

    #ifdef __TEST__
    Calc->VixTZ(2000, 6, 28, 10, 20, 19, 0.733568, r, v);
    #else
    Calc->VixTZ(Year, Month, Day, Hour, Min, Sec, 0.0, r, v);
    #endif

    cout << QString::fromUtf8("\n");
    #ifdef __TEST__
    cout << QString::fromUtf8("Результат работы VixTZ (задание даты и времени декретного московского)\n");
    #else
    cout << QString::fromUtf8("Текущее положение МКС в Абсолютной СК\n");
    #endif
    cout << QString::fromUtf8("r.x = ") << QString::number(r[0], 'g', 15) << QString::fromUtf8(" м\n");
    cout << QString::fromUtf8("r.y = ") << QString::number(r[1], 'g', 15) << QString::fromUtf8(" м\n");
    cout << QString::fromUtf8("r.z = ") << QString::number(r[2], 'g', 15) << QString::fromUtf8(" м\n");
    cout << QString::fromUtf8("\n");
    cout << QString::fromUtf8("v.x = ") << QString::number(v[0], 'g', 15) << QString::fromUtf8(" м/сек\n");
    cout << QString::fromUtf8("v.y = ") << QString::number(v[1], 'g', 15) << QString::fromUtf8(" м/сек\n");
    cout << QString::fromUtf8("v.z = ") << QString::number(v[2], 'g', 15) << QString::fromUtf8(" м/сек\n");
  }
  else
  {
    cout << QString::fromUtf8("Ошибка чтения TLE\n");
  }

  delete Calc;
  return 0;
}
