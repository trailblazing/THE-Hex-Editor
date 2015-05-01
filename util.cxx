#include "util.h"

namespace util
{
string ans = "", bit = "0123456789ABCDEF";
int incr = 0, range = 0, value = 0, count = 0;
}

/****** Manual Integer to String Converter ******/
string numparse(long long int inp)
{

    string temp = "", numericals = "0123456789";
    int pow = 1, x = 1, y = 0;

    if(inp < 0)
    {
        temp = "-" + temp;
        inp *= -1;
    }

    while((pow * 10) <= inp)
    {
        pow *= 10;
        ++x;
    }

    while(--x > -1)
    {

        for(y = -1; ++y < 11;)
        {
            if((y * pow) > inp)
            {
                inp-= ((y - 1) * pow);
                pow /= 10;
                temp += numericals[y - 1];
                y = 11;
            }
        }

    }

    return temp;
}

/****** Manual String to Integer Converter ******/
int numparse(string inp)
{

    int pow = 1, x = 0, y = 0, temp = 0;
    string numericals = "0123456789";
    bool negative = false;

    if(inp.substr(0, 1) == "-")
    {
        negative = true;
        inp = inp.substr(1, (inp.length() - 1));
    }

    for(x = 0; ++x < inp.length();)
    {
        pow *= 10;
    }

    for(x = -1; ++x < inp.length();)
    {

        for(y = -1; ++y < 10;)
        {
            if(inp[x] == numericals[y])
            {
                temp += (pow * y);
            }
        }

        pow /= 10;
    }

    if(negative == true)
    {
        temp *= -1;
    }

    return temp;
}

/****** Multi Base Conversion - From String To Decimal ******/
string multi_base(string input1, string base)
{

    int range = numparse(base);

    int ans = 0;
    int incr = 1;
    string bit1 = "0123456789ABCDEF";
    string bit2 = "0123456789abcdef";

    for(int x = input1.length(); --x >= 0;)
    {

        for(int y = -1; ++y < bit1.length();)
        {

            if(((bit1[y] == input1[x]) || (bit2[y] == input1[x])) && (y < range))
            {
                ans += (incr * y);
                incr *= range;
            }

            else if(((bit1[y] == input1[x]) || (bit2[y] == input1[x])) && (y >= range))
            {
                return "Error : Input Out Of Base Range";
            }

        }

    }

    return numparse(ans);
}

/****** Multi Base Unconversion - From Decimal Strings ******/
string umulti_base(string input1, string base)
{

    string ans = "";
    string bit = "0123456789ABCDEF";
    int incr = 1;
    int range = numparse(base);
    int value = numparse(input1);
    int count = 0;

    while((incr * range) <= value)
    {
        incr *= range;
        count++;
    }

    while(count >= 0)
    {

        for(int x = range; --x >= 0;)
        {

            if((value - (incr * x)) >= 0)
            {
                ans += bit[x];
                value -= (incr * x);
                x = -1;
            }

        }

        incr /= range;
        count--;
    }

    return ans;
}



/****** Parse Zero - Adds 0 To Hex String ******/
string parse_zero(string input)
{
    string temp = "";

    if(input.length() < 2)
    {
        temp = "0" + input;
        return temp;
    }

    else
    {
        return input;
    }
}




// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)//do not work! from: http://www.geeksforgeeks.org/convert-floating-point-number-string/
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}


/**

 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C"

 * with slight modification to optimize for specific architecture:

 * http://www.jb.man.ac.uk/~slowe/cpp/itoa.html

 */


/**
 * C++ version 0.4 std::string style "itoa":
 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
 * and Brian Hunt
 * http://www.jb.man.ac.uk/~slowe/cpp/itoa.html#newest
 */


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * http://www.jb.man.ac.uk/~slowe/cpp/itoa.html#newest
 */
char* internal_itoa(int value, char* result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    }
    while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}



// Depreciated, just for learn & test, new implementation use std::localtime(), hughvonyoung@gmail.com
time_utility get_time_from_seconds(long int const s, long int year_base)
{
    time_utility u;
    long int all_seconds = s ;
    long int all_minutes = s/60;
    long int all_hours = all_minutes/60;
    long int all_days = all_hours/24;
    long int cur_years = all_days/365;

    long int cur_year_days_left = all_days - (cur_years/4 + cur_years*365);
    long int cur_year_months_left = cur_year_days_left/30;
    long int mounths_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    long int cur_year_days_gone = 0;
    for(int i = 1; i <= cur_year_months_left; i ++)cur_year_days_gone += mounths_days[i];
    long int cur_month_days_left = cur_year_days_left - cur_year_days_gone;
    long int cur_day_hours_left = all_hours - all_days*24;
    long int cur_hour_minutes_left = all_minutes - all_hours*60;
    long int cur_minute_seconds_left = all_seconds - all_minutes*60;
    long int real_year =  cur_years + year_base;
    u.real_year = real_year;
    u.cur_year_months_left = cur_year_months_left;
    u.cur_month_days_left = cur_month_days_left;
    u.cur_day_hours_left = cur_day_hours_left;
    u.cur_hour_minutes_left = cur_hour_minutes_left;
    u.cur_minute_seconds_left = cur_minute_seconds_left;

    return u;
}



struct_name parse_file_name(string const & t)
{
    struct_name a;
    string file_name = "", path = "", name = "", extend = "";

    int len = t.length();

    int index = -1;
    if(len > 0)
    {
        for(int i = len - 1; i >= 0; i --)
        {
            if(t.at(i) == '\/' || t.at(i) == '\\')
            {
                cout << "in circle found i = \t" << i << endl;
                index  = i;
                break;
            }
        }

        if(0 < index && index == len - 1)
        {
            path = t;
            file_name = "";
        }
        else if(0 <= index && index < (len -1))
        {
            path = t.substr(0, index + 1);
            file_name = t.substr(index + 1, (len -1)- index);
        }
        else
        {
            path = "";
            file_name = t;
        }
    }
    len = file_name.length();

    index = -1;

    if(len > 0)
    {
        for(int i = len - 1; i >= 0; i --)
        {
            if(file_name.at(i) == '.')
            {
                index = i;
                break;
            }
        }
        if(0 < index && index == (len -1))
        {
            name = file_name.substr(0, index);
            extend = "";
        }
        else if(0 < index && index < len - 1)
        {
            name = file_name.substr(0, index);
            extend = file_name.substr(index + 1, (len - 1)- index);
        }
        else if(0 == index)
        {
            name = "";
            extend = file_name.substr(1, len - 1);
        }
        else
        {
            extend = "";

        }

    }
    else
    {
        name = "";
        extend = "";
    }
    cout << "path =\t[" << path << "]\tname =\t[" << name << "]\textand =\t[" << extend << "]" << endl;
    a.path = path;
    a.file_name = file_name;
    a.name = name;
    a.extend = extend;
    return a;
}


