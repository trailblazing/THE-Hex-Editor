#ifndef _SOURCE_H_
#define _SOURCE_H_

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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Progress.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Input_Choice.H>

#include <vector>
#include <map>
#include "util.h"
#include "delegate.h"
#include <bitset>
#include <iomanip>

#include"asd.h"
//#include"TIME.H"

#include"GPS.H"
#include"Specio.h"


struct ValueInfo;
typedef bool (*FT)(std::ostringstream *prepared_header_buffer, ValueInfo *value_info);
typedef bool (*WL)(ValueInfo *value_info);

extern ValueInfo *value_info;

struct ValueDel;
struct Delegate
{
    template<typename T, typename Name, typename Word, typename WL, typename FT, typename GV>
    Delegate(T *o, Name n, Word w, WL _wl, FT _vf, GV _value)
    {
        this->_name = new sd::AgencyRead<const string>(*o, n);
        this->_word_length = new sd::AgencyRead<const unsigned int>(*o, w);
        this->_word_length_restrict = new sd::AgencyReturn<bool>(*o, _wl);
        this->_value_find = new sd::AgencyReturn<string>(*o, _vf);

    }
    sd::AgencyRead<const string> *_name;
    sd::AgencyRead<const unsigned int> *_word_length;

    sd::AgencyReturn<bool> *_word_length_restrict;
    sd::AgencyReturn<string> *_value_find;

    const string name()
    {
        return (*_name)();
    }
    const unsigned int word_length()
    {
        return (*_word_length)();
    }
    bool word_length_restrict(ValueInfo *_value_info)
    {
        return (*_word_length_restrict)( _value_info);
    }
    string value_find(ValueInfo *_value_info)
    {
        return (*_value_find)( _value_info);
    }


};


/****** Main Window ******/
//extern Fl_Window *window;
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *file_browser;
extern Fl_Double_Window *window_main;

/****** Widget For General Window Stuff *****/
extern Fl_Box *main_output;
extern Fl_Box *main_mode;
extern Fl_Box *main_status;
extern Fl_Progress *progress_bar;
extern Fl_Scrollbar *hex_scroll;

extern string current_file;
extern string mode;
extern string start, _end;
extern bool partial, stat_change;
extern const int words_per_line;
extern int page_word_count; // = words_per_line * lines; // 6 * 28 = 168
extern int scroll_position, current_file_size;

extern string favorite_path;
extern char * const default_file;

extern vector <int> partial_file;
extern vector <string> hex_log;
extern vector <string> type_log;
extern vector <Delegate*> delegates;
extern vector <string> hex_log_unsigned;
extern vector <string> oct_log;
extern vector <string> ascii_log;
extern vector <string> hex_text;
extern map <string, int> hex_table;

extern FT ft[];

extern WL wl[];

extern map <string, FT> type_table;
extern map <string, WL> type_word_length;
extern map <string, Delegate*> name_to_delegate;
extern map <string, int> oct_table;
extern map <string, int> ascii_table;

/****** Widgets for Hex Editor ******/
extern Fl_Group *hex_group;
extern Fl_Text_Editor *hex_edit;
extern Fl_Text_Buffer *hex_buffer;
extern Fl_Text_Buffer *header_buffer;
extern Fl_Text_Display *view_header;

/****** Widgets for Data Analyzer ******/
extern Fl_Output *hex_out;
extern Fl_Output *oct_out;
extern Fl_Output *bin_out;
extern Fl_Output *ascii_out;
extern Fl_Output *dec_out;

extern Fl_Output *etb_out;
extern Fl_Output *stb_out;

extern Fl_Check_Button *sign_change;

extern Fl_Input *offset_hex;
extern Fl_Input *offset_dec;
extern Fl_Input *_offset_hex;
extern Fl_Input *_offset_dec;

extern Fl_Input *bytes_number;
extern Fl_Input_Choice *field_type;


/****** Widgets For Main Menu ******/
extern Fl_Menu_Bar  *main_menu;

extern Fl_Menu_Item menu_main_menu[];

extern Fl_Menu_Item main_item[];


int get_field(const int file_offset, const int field);
string find_bytes(vector<unsigned char> * const constbytes, const string &bytes_hex_address_in_file);
void refine_hex(char * const destination, string const &source);
void open_hex(Fl_Menu_*, void*);
void open_partial(Fl_Menu_*, void*);
void save_hex(Fl_Menu_*, void*);
void save_as_hex(Fl_Menu_*, void*);
void save_partial(Fl_Menu_*, void*);
void file_export(Fl_Menu_*, void*);
void quit_hex(Fl_Menu_*, void*);
void clear_hex(Fl_Menu_*, void*);
void view_hex(Fl_Menu_*, void*);
void view_octal(Fl_Menu_*, void*);
void show_help(Fl_Menu_*, void*);
void parse_asd(Fl_Menu_*, void*);

void goto_offset(Fl_Button*, void*);
void close_all(Fl_Return_Button*, void*);
void get_value(Fl_Button*, void*);
void save_value(Fl_Button*, void*);
void refresh_input(Fl_Text_Display*, void*);
void refresh_offset(Fl_Group*, void*);


/****** Callback Function - Scroll Text ******/
void scroll(Fl_Widget *w);
/****** Callback Function - Change Between Signed & Unsigned Data Analysis ******/
void flip(Fl_Widget *w);

/****** Callback Function - Change Save Status ******/
void update_save(int, int nInserted, int nDeleted,int, const char*, void* v);

struct ValueInfo
{
protected:
    int    _offset;
    int    _bytes_number;
    Delegate *      _type_delegate;

    Fl_Input *      ___offset_hex;
    Fl_Input *      ___offset_dec;
    Fl_Input *      __offset_hex;
    Fl_Input *      __offset_dec;
    Fl_Input *      _bytes_number_str;
    Fl_Input_Choice * _field_type;
    Fl_Text_Buffer *    _header_buffer;

    ValueInfo(int _offset
              , int _bytes_number
              , Delegate * _type_delegate
              , Fl_Input * _offset_hex
              , Fl_Input * _offset_dec
              , Fl_Input * offset_hex
              , Fl_Input * offset_dec
              , Fl_Input * bytes_number
              , Fl_Input_Choice * _field_type
              , Fl_Text_Buffer * header_buffer)
        : _offset(_offset)
        , _bytes_number(_bytes_number)
        , _type_delegate(_type_delegate)
        , ___offset_hex(_offset_hex)
        , ___offset_dec(_offset_dec)
        , __offset_hex(offset_hex)
        , __offset_dec(offset_dec)
        , _bytes_number_str(bytes_number)
        , _field_type(_field_type)
        , _header_buffer(header_buffer)
    {

    }

public:
    ValueInfo()
    {
        this->_offset = -1;
        this->_bytes_number = -1;
        _type_delegate = (Delegate *)0;

        ___offset_hex = (Fl_Input *)0;
        ___offset_dec =  (Fl_Input *)0;
        __offset_hex =  (Fl_Input *)0;
        __offset_dec =  (Fl_Input *)0;
        _bytes_number_str =  (Fl_Input *)0;
        _field_type = (Fl_Input_Choice *)0;
        _header_buffer = (Fl_Text_Buffer *)0;

    }


    int offset();
    int bytes_number();
    Delegate * const type_delegate();
    string _offset_hex();
    string _offset_dec();
    string offset_hex();
    string offset_dec();
    string bytes_number_str();
    string field_type();

    string header_buffer();

    int offset(int );
    int bytes_number(int );
    Delegate * const type_delegate(Delegate * const);
    string _offset_hex(string const & _value);
    string _offset_dec(string const & _value);
    string offset_hex(string const & _value);
    string offset_dec(string const & _value);
    string bytes_number_str(string const & _value);
    string field_type(string const & _value);
    string header_buffer(string const & _value);

};




extern void synchronize_offset_to_hex(Fl_Input*, void*);
extern void synchronize_offset_to_dec(Fl_Input*, void*);

extern void restrict_word_length_dec(Fl_Input* the, void*);
extern void restrict_type_string(Fl_Input*, void*);

extern void bind_query_type(Fl_Input_Choice*, void*);

template<typename T>bool restict_word_length(T *_this, ValueInfo *)
{
    cout << "calling type "<< _this->name()<< " word_length_restrict." << endl;
    char buffer[17];buffer[16] = '\0';

	internal_itoa(_this->word_length(), buffer, 10);
    cout << "value_info->bytes_number()=\t"<< value_info->bytes_number() << endl;
    cout << "_this->word_length() =\t"<< _this->word_length() << endl;

    if(value_info->bytes_number()!= _this->word_length())
        value_info->bytes_number(_this->word_length());

    return true;
}



template<typename T>
struct TypeBase
{
    typedef typename T::value_type value_type;
protected:
    value_type *_value;

public:
    TypeBase()
    {
        _value = (value_type*)0;
    }
    ~TypeBase()
    {
        if (_value != (value_type*)0)delete _value;
    }
    const string name()
    {
        return T::name;
    }
    const unsigned int word_length()
    {
        return T::word_length;
    }
    value_type value()
    {
        return *_value;
    }
    bool word_length_restrict(ValueInfo *_value_info)
    {
        return restict_word_length(this, value_info);
    }
    string value_find(ValueInfo *_value_info);
};

struct Byte
{
    typedef unsigned int value_type;
    const static string name;
    const static unsigned int word_length = 1;

};

struct UnsignedShort
{
    typedef unsigned short int value_type;
    const static string name;
    const static unsigned int word_length = 2;
};

struct UnsignedInt
{
    typedef unsigned int value_type;
    const static string name;
    const static unsigned int word_length = 2;

};


struct UnsignedChar
{
    typedef unsigned char value_type;
    const static string name;
    const static unsigned int word_length = 1;

};

struct Char
{
    typedef char value_type;
    const static string name;
    const static unsigned int word_length = 1;
};

struct Int
{
    typedef int value_type;
    const static string name;
    const static unsigned int word_length = 2;
};

struct Float
{
    typedef float value_type;
    const static string name;
    const static unsigned int word_length = 4;
};

struct Double
{
    typedef double value_type;
    const static string name;
    const static unsigned int word_length = 8;
};


struct AppData
{
    typedef APP_DATA value_type;
    const static string name;
    const static unsigned int word_length = 128;

};

struct GPSData
{
    typedef GPS_DATA value_type;
    const static string name;
    const static unsigned int word_length = 56;
};

struct UnsignedLong
{
    typedef unsigned long int value_type;
    const static string name;
    const static unsigned int word_length = 4;
};


struct StructTm
{
    typedef _tm value_type;
    const static string name;
    const static unsigned int word_length = 18;
};

struct Time_T
{
    typedef time_t value_type;
    const static string name;
    const static unsigned int word_length = 4;
};

struct ReverseInt
{
    typedef int value_type;
    const static string name;
    const static unsigned int word_length = 2;
};

struct output_tri
{
    string x;
    string y1;
    string y2;
};

template<typename T>string value_find_template(T *the, ValueInfo *v);

#ifndef define generate_delegates(NAME)
#define generate_delegates(NAME)\
  delegates.push_back(new Delegate(new NAME()\
                                   , &NAME::name\
                                   , &NAME::word_length\
                                   , &NAME::word_length_restrict\
                                   , &NAME::value_find\
                                   , &NAME::value\
                                   )\
                      );
#endif


#endif // _SOURCE_H_
