//----------------------------------------------------------------------------//
//                     *** ФОРМАТ TLE И АЛГОРИТМ SGP4 ***                     //
//                                                                            //
// Файл TLE_SGP4.h                                                            //
//                                                                            //
//                      *** TLE_SGP4 РАСЧЕТНЫЙ КЛАСС ***                      //
//                                                                            //
// Авторы ГЛУЩЕНКО Сергей и VALLADO David                                     //
//                                                                            //
//                                                           Москва, 2022 год //
//----------------------------------------------------------------------------//


#ifndef TLE_SGP4H
#define TLE_SGP4H

#include <ctime>
#include <string>
#include <string.h>
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <math.h>


class TLE_SGP4
{
  private:
    //Тип описывает дату и время
    struct TimeTLE
    {
      TimeTLE() : Year(0), Month(0), Day(0), Hour(0), Min(0), Sec(0), DSec(0.0) {}
      int Year;     //Год
      int Month;    //Месяц
      int Day;      //День
      int Hour;     //Часы
      int Min;      //Минуты
      int Sec;      //Секунды
      double DSec;  //Доли секунды (меньше нуля)
    };
    typedef struct TimeTLE TimeTLE;

    //Тип описывает данные используемые алгоритмом SGP4 в процессе расчета
    struct ElsetRec
    {
      int whichconst;
      char satid[6];
      int epochyr, epochtynumrev, error;
      char operationmode, init, method;
      double a, altp, alta, epochdays, jdsatepoch, jdsatepochF, nddot, ndot, bstar, rcse, inclo, nodeo, ecco, argpo, mo, no_kozai;

      //sgp4fix add new variables from tle
      char classification, intldesg[12];
      int ephtype;
      long elnum, revnum;

      //sgp4fix add unkozai'd variable
      double no_unkozai;

      //sgp4fix add singly averaged variables
      double am, em, im, Om, om, mm, nm, t;

      //sgp4fix add constant parameters to eliminate mutliple calls during execution
      double tumin, mu, radiusearthkm, xke, j2, j3, j4, j3oj2;

      //Additional elements to capture relevant TLE and object information:
      long dia_mm;        //RSO dia in mm
      double period_sec;  //Period in seconds
      char active;        //"Active S/C" flag (0=n, 1=y)
      char not_orbital;   //"Orbiting S/C" flag (0=n, 1=y)
      double rcs_m2;      //"RCS (m^2)" storage

      //temporary variables because the original authors call the same method with different variables
      double ep, inclp, nodep, argpp, mp;

      int isimp;
      double aycof, con41, cc1, cc4, cc5, d2, d3, d4, delmo, eta, argpdot, omgcof, sinmao, t2cof, t3cof, t4cof, t5cof, x1mth2, x7thm1,
             mdot, nodedot, xlcof, xmcof, nodecf;

      //deep space
      int irez;
      double d2201, d2211, d3210, d3222, d4410, d4422, d5220, d5232, d5421, d5433, dedt, del1, del2, del3, didt, dmdt, dnodt, domdt, e3,
             ee2, peo, pgho, pho, pinco, plo, se2, se3, sgh2, sgh3, sgh4, sh2, sh3, si2, si3, sl2, sl3, sl4, gsto, xfact, xgh2, xgh3,
             xgh4, xh2, xh3, xi2, xi3, xl2, xl3, xl4, xlamo, zmol, zmos, atime, xli, xni, snodm, cnodm, sinim, cosim, sinomm, cosomm,
             day, emsq, gam, rtemsq, s1, s2, s3, s4, s5, s6, s7, ss1, ss2, ss3, ss4, ss5, ss6, ss7, sz1, sz2, sz3, sz11, sz12, sz13,
             sz21, sz22, sz23, sz31, sz32, sz33, z1, z2, z3, z11, z12, z13, z21, z22, z23, z31, z32, z33, argpm, inclm, nodem, dndt, eccsq;

      //for initl
      double ainv, ao, con42, cosio, cosio2, omeosq, posq, rp, rteosq, sinio;
    };
    typedef struct ElsetRec ElsetRec;

    void dpper(double e3, double ee2, double peo, double pgho, double pho,
               double pinco, double plo, double se2, double se3, double sgh2,
               double sgh3, double sgh4, double sh2, double sh3, double si2,
               double si3, double sl2, double sl3, double sl4, double t,
               double xgh2, double xgh3, double xgh4, double xh2, double xh3,
               double xi2, double xi3, double xl2, double xl3, double xl4,
               double zmol, double zmos, char init, ElsetRec *rec, char opsmode);

    void dscom(double epoch, double ep, double argpp, double tc, double inclp, double nodep, double np, ElsetRec *rec);

    void dsinit(double tc, double xpidot, ElsetRec *rec);

    void dspace(double tc, ElsetRec *rec);

    void initl(double epoch, ElsetRec *rec);

    bool sgp4init(char opsmode,ElsetRec *satrec);

    bool sgp4(ElsetRec *satrec, double tsince, double *r, double *v);

    void getgravconst(int whichconst, ElsetRec *rec);

    double gstime(double jdut1);

    void jday(int year, int mon, int day, int hr, int minute, double sec, double *jd, double *jdfrac);

    double fmod(double numer, double denom);

    long parseEpoch(char *str);

    //Parse the double
    double gd(char *str, int ind1, int ind2);

    //Parse the double with implied decimal
    double gdi(char *str, int ind1, int ind2);

    bool isLeap(int year);

    ElsetRec rec;  //Данные для расчета и данные в процессе расчета

    TimeTLE TLE_Time;  //Дата и время TLE (эпоха) в UTC

  public:
    TLE_SGP4(void);
    virtual ~TLE_SGP4(void);

    //Читает TLE (двухстрочный формат данных, представляющий собой набор элементов орбиты спутника Земли)
    // На вход подаются:
    //  - первая строка TLE;
    //  - вторая строка TLE.
    // На выходе можно получить:
    //  - true в случае успешного чтения формата TLE
    bool ReadTLE(std::string Str1, std::string Str2);

    //Получение вектора состояния на заданный сдвиг по времени после эпохи TLE в Абсолютной СК (эпоха TLE в UTC).
    // Метод возвращает код ошибки (0 - ошибок нет)
    // На вход подаются:
    //  - количество минут, прошедшее после начала эпохи TLE (эпоха TLE в UTC).
    // На выходе можно получить:
    //  - статический массив, содержащий компоненты радиус-вектора спутника в Абсолютной СК [м];
    //  - статический массив, содержащий компоненты вектора скорости спутника в Абсолютной СК [м/сек]
    int GetRV(double MinutesAfterEpochTLE, double R[3], double V[3]);

    //Выход на заданное Декретное московское время в Абсолютной СК. В случае ошибки R и V возвращаются нулевые
    // На вход подается Декретное московское время:
    //  - год;
    //  - месяц;
    //  - день;
    //  - часы;
    //  - минуты;
    //  - секунды;
    //  - доли секунды (меньше нуля).
    // На выходе можно получить:
    //  - статический массив, содержащий компоненты радиус-вектора спутника в Абсолютной СК [м];
    //  - статический массив, содержащий компоненты вектора скорости спутника в Абсолютной СК [м/сек]
    void VixTZ(int Year, int Month, int Day, int Hour, int Min, int Sec, double DSec, double R[3], double V[3]);

    //Возвращает текущее системное время
    void GetCurrentTime(int *Year, int *Month, int *Day, int *Hour, int *Min, int *Sec);
};


#endif
