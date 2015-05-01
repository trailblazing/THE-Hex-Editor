#ifndef _UTIL_H_
#define _UTIL_H_

/* THE - The Hex Editor */
/* Copyright (C) 2009  Daniel Baxter */

/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License */
/* as published by the Free Software Foundation; either version 2 */
/* of the License, or (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with this program; if not, write to the Free Software */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/
#define WIN32
#define NDEBUG
#ifdef WIN32 && NDEBUG

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif

#include <fstream>
//#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
// C program for implementation of ftoa()
#include<stdio.h>
#include<math.h>
#include <iomanip>

#include <bitset>
#include <typeinfo>
using namespace std;

/****** Namespace For Commonly Used Variables ******/
/****** Define Them So Temporary Variables Are Not Initialized For Inline Functions Are Called A Ridiculous Amount Of Times. Saves Memory! ******/
namespace util {
extern string ans, bit;
extern int incr, range, value, count;
}

/****** Manual Integer to String Converter ******/
string numparse(long long int inp);

/****** Manual String to Integer Converter ******/
int numparse(string inp);

/****** Multi Base Conversion -From String To Decimal ******/
string multi_base(string input1, string from_base);		// base is to identify the number system

/****** Multi Base Unconversion - From Decimal Strings ******/
string umulti_base(string input1, string to_base);		// base is to identify the number system

/****** Multi Base Unconversion - From Decimal Integers ******/
inline string umulti_base(int input1, int to_base)		// base is to identify the number system
{

    util::ans = "";
    util::bit = "0123456789ABCDEF";
    util::incr = 1;
    util::value = input1;
    util::range = to_base;
    util::count = 0;

    while((util::incr * util::range) <= util::value) {
        util::incr *= util::range;
        ++util::count;
    }

    while(util::count >= 0) {

        for(int x = util::range; --x >= 0;) {

            if((util::value - (util::incr * x)) >= 0) {
                util::ans += util::bit[x];
                util::value -= (util::incr * x);
                break;
            }

        }

        util::incr /= util::range;
        --util::count;
    }

    return util::ans;
}

/****** Parse Zero - Adds 0 To Hex String ******/
string parse_zero(string input);

void reverse(char *str, int len);
int intToStr(int x, char str[], int d);
void ftoa(float n, char *res, int afterpoint);

char* internal_itoa(int value, char* result, int base);

template<typename S>struct _func_true {

    S  v;
    _func_true(S _v) {
        v = _v;
    }

};

template<typename Type>std::ostream & _media_func(std::ostream & out, void * f)
{

    out.setf ( std::ios::hex, std::ios::basefield );  // set hex as the basefield
    out.setf ( std::ios::showbase );                  // activate showbase

    out << showbase                 // show the 0x prefix
        << internal                 // fill between the prefix and the number
        << std::setfill('0');       // fill with 0s, cant be '\0'

    out <<std::hex << std::setw(18) << std::setfill('0') << (static_cast<_func_true<Type>*>(f))->v; //out << (Type)(*static_cast<Type*>(v)) << endl;

    out.unsetf ( std::ios::hex);
    out.unsetf ( std::ios::basefield );
    out.unsetf ( std::ios::showbase);

    return out;

}

template<typename Type>std::ostream & _media_true(std::ostream & out, void * v) // does not work, because of void * point to basic type for example long int lead data lost
{

    out.setf ( std::ios::hex, std::ios::basefield );  // set hex as the basefield
    out.setf ( std::ios::showbase );                  // activate showbase

    out << showbase             // show the 0x prefix
        << internal             // fill between the prefix and the number
        << std::setfill('0');   // fill with 0s, cant be '\0'

    out <<std::hex << std::setw(18) << std::setfill('0') << setprecision(16) << (Type)(*static_cast<Type*>(v)); //out << (Type)(*static_cast<Type*>(v)) << endl;

    out.unsetf ( std::ios::hex);
    out.unsetf ( std::ios::basefield );
    out.unsetf ( std::ios::showbase);

    return out;

}

template<typename T>void _del_func(void* f)
{
    delete static_cast<_func_true<T> *>(f);
}

struct cheat_hex {
protected:

    void * _func;
    std::ostream & (* _media)(std::ostream & out, void *);
    void (* _del)(void *);
public:

    ~cheat_hex() {
        if((void*)0 != _func)_del(_func);
    }

    template<typename Special>
    cheat_hex(Special value)
    {

        _func = (void *)new _func_true<Special>(value);
        _media = _media_func<Special>;
        _del = _del_func<Special>;
    }

    std::ostream & operator << (std::ostream & out) {
        return _media(out, _func);
    }

    friend inline std::ostream & operator << (std::ostream & out, const cheat_hex & c) {
        return c._media(out, c._func);
    }
};


template<typename Y>struct cheat {
    cheat(Y & y):value(y) {}
    Y value;
};

template<typename T>
std::ostream & operator << (std::ostream & out, const cheat<T> &c)
{

    out.setf ( std::ios::hex, std::ios::basefield );  // set hex as the basefield
    out.setf ( std::ios::showbase );                  // activate showbase

    out << showbase             // show the 0x prefix
        << internal             // fill between the prefix and the number
        << std::setfill('0');   // fill with 0s, cant be '\0'
    out.width(18);
    out << std::hex << std::setw(18) << std::setfill('0') << setprecision(16) << c.value;

    out.unsetf ( std::ios::hex);
    out.unsetf ( std::ios::basefield );
    out.unsetf ( std::ios::showbase);

    return out;
}

template<typename T>
std::ostream & operator << (std::ostream & out, const vector<T>& v)
{
    out << (std::basic_ostream<char>)("[");
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << (std::basic_ostream<char>)(", ");
    }
    out << (std::basic_ostream<char>)("]");

    return out;

}


struct time_utility {
    long int real_year;
    long int cur_year_months_left;
    long int cur_month_days_left;
    long int cur_day_hours_left;
    long int cur_hour_minutes_left;
    long int cur_minute_seconds_left;
};


time_utility get_time_from_seconds(long int const s, long int year_base);

struct struct_name {
    string path;
    string file_name;
    string name;
    string extend;
};

struct_name parse_file_name(string const & t);

#endif // _UTIL_H_
