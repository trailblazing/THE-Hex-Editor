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


#include "source.h"
#include <iostream>

#include <string>
#include <sstream>
//#include <iostream>

template <class T>
bool from_string(T& t,
                 const std::string& s,
                 std::ios_base& (*f)(std::ios_base&))
{
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();
}


/****** Widget For General Window Stuff *****/


string current_file = "";
string mode = "Mode : Hexadecimal";
string start = "", _end = "";
bool partial = false, stat_change = false;
int scroll_position = -1, current_file_size = 0;

string favorite_path = ".";

char * const default_file = "RU 130427123802 Sample 00000.asd";


const string Byte::name =  "byte";
const string UnsignedShort::name =  "unsigned short int";
const string UnsignedInt::name =  "unsigned int";
const string UnsignedChar::name =  "unsigned char";
const string Char::name =  "char";
const string Int::name =  "int";
const string Float::name =  "float";
const string Double::name =  "double";
const string AppData::name =  "app data";
const string GPSData::name =  "gps data";
const string UnsignedLong::name =  "unsigned long int";
const string StructTm::name =  "struct tm";
const string Time_T::name =  "time_t";
const string ReverseInt::name =  "reverse int";
const int words_per_line = 8;

int page_word_count = 162; 

vector <int> partial_file;
vector <string> hex_log;
vector <string> type_log;
vector <Delegate*> delegates;
vector <string> oct_log;
vector <string> ascii_log;
vector <string> hex_text;
map <string, int> hex_table;
map <string, FT> type_table;
map <string, WL> type_word_length;
map <string, Delegate*> name_to_delegate;
map <string, int> oct_table;
map <string, int> ascii_table;


/****** Widgets for Hex Editor ******/

Fl_Text_Buffer *hex_buffer = new Fl_Text_Buffer(0);
Fl_Text_Buffer *header_buffer = new Fl_Text_Buffer(0);


ValueInfo *value_info = new ValueInfo();

/****** Callback Function - Open File Chooser ******/
void open_hex(Fl_Menu_*, void * launch_file)//void open_hex(Fl_Widget *w)
{
    Fl_File_Chooser *temp_fcho = (Fl_File_Chooser *)0;


    if((void *)0 == launch_file)
    {
		temp_fcho = new Fl_File_Chooser(favorite_path.c_str(), "*", Fl_File_Chooser::SINGLE, "Open File");
        if(temp_fcho != (Fl_File_Chooser *)0)
        {
            temp_fcho->show();
            /* While File Chooser Shown, Wait For File Selection */
            while(temp_fcho->shown())
            {
                Fl::wait();
            }
            current_file = temp_fcho->value();
        }

    }
    else
    {

        struct_name na = parse_file_name((char *)launch_file);

        cout << "origin launch_file =\t" << string((char*)launch_file )<< endl;
        cout << "file name =\t" << na.file_name << endl;
        current_file = na.file_name;

    }

    if("" != current_file)
    {
        ifstream file;
        int file_size = 0, b = 0, e = 0;
        unsigned char *mem;

        /* If User Did Not Click Cancel Proceed To Process */

        if(current_file.length() != 0)
        {
            file.open(current_file.c_str(), ios::binary);
            cout << "open file =\t" << current_file << endl;

            /* If File Can Be Opened */
            if(file.is_open())
            {

                string final = "", temp = "", offset = "";

                /* Clear Prior Cruft */
                hex_text.clear();

                hex_buffer->text("");
                hex_edit->buffer(hex_buffer);

                /* Get File Size */
                file.seekg(0, ios::beg);
                b = file.tellg();
                file.seekg(0, ios::end);
                e = file.tellg();
                file_size = e - b;
                current_file_size = file_size;

                file.clear();
                file.seekg(0, ios::beg);

                /* Initialize The Progress Bar */
                progress_bar->maximum(file_size);
                progress_bar->minimum(0);
                main_output->label("Loading File...");

                /* Load File Into Memory */
                mem = new unsigned char[file_size];
                file.read((char*)(mem), file_size);
                std::cout<< "file size = " << file_size << std::endl;

                cout << "fl_height 	()=\t" << fl_height() << endl;  // 16
                cout << "fl_font 	()=\t" << fl_font() << endl;    // 4
                cout << "fl_size 	()=\t" << fl_size() << endl;    // 14
                cout << "hex_edit->h() =\t"<< hex_edit->h() << endl;// 456
                cout << "window_main->h() =\t"<< window_main->h() << endl;
                //int words_per_line = 6;
				fl_height(0, 17);	// does not work
				int line_height = fl_height();
				if (line_height == 16) line_height = 17;
				int lines = hex_edit->h() / line_height;  // 28.5
                page_word_count = words_per_line * (lines - 2); // 6 * (28 - 1) = 162
                cout << "page_word_count =\t"<< page_word_count << endl;
                /* For File Size, Read Each Byte, Convert To Hex Value */
                for(int x = -1;  ++x < file_size;)
                {
                    temp = hex_log[(int)mem[x]];    // so, from unsigned char, use it as int as index in hex_log->map it to a char used in buffer

                    /* Split The Total Hex Values Into Screen Chunks - Reduces Memory Needed By Text Editor - Clear Final String */
                    //if(((x % page_word_count == 0)) && (x != 0) && (x % 6 == 0))    		// page_word_count=>126<=6*21->162 <= 6*27
                    if(((x % page_word_count == 0)) && (x != 0) && (x % words_per_line == 0))   // page_word_count=>126<=6*21->162 <= 6*27
                    {
                        hex_text.push_back(final);
                        final = "";
                    }

                    /* Add Offsets To Text Buffer @ Every 6th Entry */
                    if ((x  % words_per_line == 0) || (x == 0)) // if ((x  % 6 == 0) || (x == 0))
                    {
                        offset = umulti_base(x, 16);

                        while(offset.length() < 6)
                        {
                            offset = "0" + offset;
                        }

                        final += ("0x" + offset + "\t");
                    }

                    final += (temp + "\t");

                    /* Catch The Last Remaining Hex Values Into A Chunk - Or If Chunk Does Not Fill Screen Display All There Is */
                    if((x + 1) == file_size)
                    {
                        hex_text.push_back(final);
                        final = "";
                    }

                    /* Every Loaded Kilobyte Update Progress Bar */
                    if(x % 1024 == 0)
                    {
                        progress_bar->value(x);
                        Fl::wait(0.0000000000001);
                    }
                }

                /* Once All Elements Of File Have Been Turned To Hex, Delete Dynamic Memory, Close File, Reset Progress Bar */
                delete[] mem;
                file.close();
                progress_bar->value(0);

                /* Window Updates - Display 1st Screen Chunk */
                main_output->label("File Successfully Opened");
                hex_buffer->text(hex_text[0].c_str());
                hex_edit->buffer(hex_buffer);
                hex_edit->wrap_mode(-1, words_per_line * 10);    // hex_edit->wrap_mode(-1, 60);
                mode = "Mode : Hexadecimal";
                main_status->label("");
                main_mode->label(mode.c_str());
                partial = false;
                scroll_position = 0;
                hex_scroll->value(0, 0, 0, (hex_text.size() - 1));
                string label;
                
                struct_name na = parse_file_name(current_file);
                label = "The Hex Editor : " + na.name + "." + na.extend;
                window_main->label(label.c_str());

            }

            /* If Could Not Open, Print Error */
            else
            {
                main_output->label("Error Could Not Read File. Check Path or Permissions");
            }

        }
    }
}

/****** Callback Function - Partial Open File ******/
void open_partial(Fl_Menu_*, void*)//void open_partial(Fl_Widget *w)
{
    Fl_File_Chooser *temp_fcho = (Fl_File_Chooser *)0;
	temp_fcho = new Fl_File_Chooser(favorite_path.c_str(), "*", Fl_File_Chooser::SINGLE, "Partial Open File");
    if(temp_fcho != (Fl_File_Chooser *)0)
    {
        temp_fcho->show();

        /* While File Chooser Shown, Wait For File Selection */
        while(temp_fcho->shown())
        {
            Fl::wait();
        }

        const char *lower_bound = NULL;
        const char *upper_bound = NULL;

        /* Only Capture If No Cancels Were Pressed */
        if(temp_fcho->value() != NULL)
        {

            lower_bound = fl_input("Enter the starting offset - Offset must be a plain hexadecimal number", "Starting Offset");
            if(lower_bound != NULL)
            {
                start = lower_bound;

                upper_bound = fl_input("Enter the ending offset - Offset must be a plain hexadecimal number", "Ending Offset");
                if(upper_bound != NULL)
                {
                    _end = upper_bound;
                }
            }

        }

        ifstream file;
        int file_size = 0, b = 0, e = 0, z = 0, a = 0;
        unsigned char *mem;

        /* If User Did Not Click Cancel Proceed To Process */
        if((temp_fcho->value() != NULL) && (lower_bound != NULL) && (upper_bound != NULL))
        {
            current_file = temp_fcho->value();
            start = multi_base(start, "16");
            _end = multi_base(_end, "16");
            z = numparse(start) - 6;
            file.open(current_file.c_str(), ios::binary);

            /* If File Can Be Opened */
            if(file.is_open())
            {
                string final = "", temp = "", offset = "";
                partial_file.clear();
                hex_text.clear();

                /* Get File Size */
                file.seekg(0, ios::beg);
                b = file.tellg();
                file.seekg(0, ios::end);
                e = file.tellg();
                file_size = e - b;
                current_file_size = file_size;

                file.clear();
                file.seekg(0, ios::beg);

                /* Initialize The Progress Bar */
                progress_bar->maximum(file_size);
                progress_bar->minimum(0);
                main_output->label("Loading File...");

                /* Load File Into Memory */
                mem = new unsigned char[file_size];
                file.read((char*)(mem), file_size);

                /* Only Open Partially If Start & End Offsets Are Possible */
                if((numparse(start) >= 0) && (numparse(_end) >= 0) && (numparse(start) <= (file_size - 1)) && (numparse(_end) <= (file_size - 1)) && (numparse(_end) >= numparse(start)))
                {

                    /* For File Size, Read Each Byte, Convert To Hex Value */
                    for(int x = -1; ++x < file_size;)
                    {
                        partial_file.push_back((int)mem[x]);
						fl_height(0, 17);
						int line_height = fl_height();
						if (line_height == 16) line_height = 17;
						int lines = hex_edit->h() / line_height;  // 28.5 // int lines = hex_edit->h()/fl_height();  // 28.5
                        
                        page_word_count = words_per_line * (lines - 2); // 6 * (28 - 1) = 162
                        /* Only Read And Print Values In The Range */
                        if((x >= numparse(start)) && (x <= numparse(_end)))
                        {
                            temp = hex_log[(int)mem[x]];

                            /* Split The Total Hex Values Into Screen Chunks - Reduces Memory Needed By Text Editor - Clear Final String */
                            if(((a % page_word_count == 0)) && (a != 0))   // 126->162<=page_word_count
                            {
                                hex_text.push_back(final);
                                final = "";
                            }

                            /* Add Offsets To Text Buffer @ Every 6th Entry */
                            if ((a  % words_per_line == 0) || (a == 0))  // if ((a  % 6 == 0) || (a == 0)
                            {
                                z += 6;
                                offset = umulti_base(z, 16);

                                while(offset.length() < 6)
                                {
                                    offset = "0" + offset;
                                }

                                final += ("0x" + offset + "\t");
                            }

                            final += (temp + "\t");

                            /* Catch The Last Remaining Hex Values Into A Chunk - Or If Chunk Does Not Fill Screen Display All There Is */
                            if((x + 1) > numparse(_end))
                            {
                                hex_text.push_back(final);
                                final = "";
                            }

                            a++;
                        }


                        /* Every Loaded Kilobyte Update Progress Bar */
                        if(x % 1024 == 0)
                        {
                            progress_bar->value(x);
                            Fl::wait(0.0000000000001);
                        }
                    }

                    delete[] mem;
                    file.close();
                    progress_bar->value(0);

                    hex_buffer->text(hex_text[0].c_str());
                    hex_edit->buffer(hex_buffer);
                    hex_edit->wrap_mode(-1, words_per_line * 10);    // hex_edit->wrap_mode(-1, 60);
                    main_output->label("File Successfully Opened");
                    main_status->label("");
                    mode = "Mode : Hexadecimal";
                    main_mode->label(mode.c_str());
                    partial = true;
                    scroll_position = 0;
                    hex_scroll->value(0, 0, 0, (hex_text.size() - 1));
                }

                /* If Given Offsets Out Of Range For File */
                else
                {
                    main_output->label("Error Offsets Out Of Range. Offsets Do Not Exist In File");
                    current_file = "";
                }
            }

            /* If Could Not Open, Print Error */
            else
            {
                main_output->label("Error Could Not Read File. Check Path or Permissions");
            }

        }
    }
}

/****** Callback Function - Save Partial ******/
void save_partial(Fl_Menu_ *m, void *u)//void save_partial()
{
    string final = "", data = "", range = "", temp = "";
    int y = 0, z = 0, increment1 = 0, increment2 = 0, read_length = 0, part = numparse(start), put = 0;
    ofstream file;

    /* Update Current Scroll Position With Latest Data From Text Editor */
    hex_text[scroll_position] = hex_buffer->text();

    /* Form The String Variable That Is Combination Of All Elements From Hex Chunks */
    for(int x = -1; ++x < hex_text.size();)
    {
        data += hex_text[x];
    }

    /* If Mode Is Hex Save With These Settings */
    if(mode == "Mode : Hexadecimal")
    {
        y = 6;
        increment1 = 3;
        increment2 = 12;
        range = "16";
        read_length = 2;
    }

    /* If Mode Is Octal Save With These Settings */
    else if(mode == "Mode : Octal")
    {
        y = 5;
        increment1 = 4;
        increment2 = 13;
        range = "8";
        read_length = 3;
    }

    for(int x = 9; x < data.length();)
    {

        /* If 1st Or Not 6th Entry, Process Normally */
        if((z % words_per_line != 0) || (z == 0))    // if((z % 6 != 0) || (z == 0))
        {
            y += increment1;
            x += increment1;
        }

        /* If 6th Entry, Skip For Offsets */
        else
        {
            y += increment2;
            x += increment2;
        }

        /* Convert Data - Put In Partial Vector */
        temp = data.substr(y, read_length);

        /* Pull Data From Hex Table If Mode Is Hexadecimal */
        if(mode == "Mode : Hexadecimal")
        {
            partial_file[part] = hex_table[temp];
        }

        /* Pull Data From Oct Table If Mode Is Octal */
        else
        {
            partial_file[part] = oct_table[temp];
        }

        part++;

        z++;
    }

    file.open(current_file.c_str(), ios::binary);

    /* If File Can Be Opened */
    if(file.is_open())
    {

        for(int x = -1; ++x < partial_file.size();)
        {
            put = partial_file[x];
            file.write((char*)(&put), 1);
        }

        file.close();
        main_output->label("File Has Been Saved");
        main_status->label("");
    }

    else
    {
        main_output->label("Error : Could Not Write To File. Check Path or Permissions");
    }
}

/****** Callback Function - Save File ******/
void save_hex(Fl_Menu_* m, void* u)//void save_hex(Fl_Widget *w)
{

    /* If File Was Not Partially Opened */
    if(partial == false)
    {
        string final = "", data = "", range = "", temp = "";
        int y = 0, z = 0, increment1 = 0, increment2 = 0, read_length = 0, put = 0;
        ofstream file;

        /* Update Current Scroll Position With Latest Data From Text Editor */
        hex_text[scroll_position] = hex_buffer->text();

        /* Form The String Variable That Is Combination Of All Elements From Hex Chunks */
        for(int x = -1; ++x < hex_text.size();)
        {
            data += hex_text[x];
        }

        /* If Mode Is Hex Save With These Settings */
        if(mode == "Mode : Hexadecimal")
        {
            y = 6;
            increment1 = 3;
            increment2 = 12;
            range = "16";
            read_length = 2;
        }

        /* If Mode Is Octal Save With These Settings */
        else if(mode == "Mode : Octal")
        {
            y = 5;
            increment1 = 4;
            increment2 = 13;
            range = "8";
            read_length = 3;
        }

        file.open(current_file.c_str(), ios::binary);

        /* If File Can Be Opened */
        if(file.is_open())
        {
            for(int x = 9; x < data.length();)
            {

                /* If 1st Or Not 6th Entry, Process Normally */
                if((z % words_per_line != 0) || (z == 0))    // if((z % 6 != 0) || (z == 0))
                {
                    y += increment1;
                    x += increment1;
                }

                /* If 6th Entry, Skip For Offsets */
                else
                {
                    y += increment2;
                    x += increment2;
                }

                /* Convert Data - If Not Partial */
                temp = data.substr(y, read_length);

                /* Pull Data From Hex Table If Mode Is Hexadecimal */
                if(mode == "Mode : Hexadecimal")
                {
                    put = hex_table[temp];
                }

                /* Pull Data From Oct Table If Mode Is Octal */
                else
                {
                    put = oct_table[temp];
                }

                file.write((char*)(&put), 1);

                z++;
            }

            file.close();
            main_output->label("File Has Been Saved");
            main_status->label("");
        }

        /* If Could Not Open, Print Error */
        else
        {
            main_output->label("Error : Could Not Write To File. Check Path or Permissions");
        }
    }

    /* If File Is Partially Opened Save Through Another Function */
    else
    {
        save_partial(m, u);
    }
}

/****** Callback Function - Save File As ******/
void save_as_hex(Fl_Menu_*, void*)//void save_as_hex(Fl_Widget *w)
{
	Fl_File_Chooser *temp_scho = new Fl_File_Chooser(favorite_path.c_str(), "*", Fl_File_Chooser::CREATE, "Save File As");
    temp_scho->show();

    /* While File Chooser Shown, Wait For File Selection */
    while(temp_scho->shown())
    {
        Fl::wait();
    }

    string final = "", data = "", range = "", temp = "";
    int y = 0, z = 0, increment1 = 0, increment2 = 0, read_length = 0, put = 0;
    ofstream file;

    /* If User Did Not Click Cancel Proceed To Process */
    if(temp_scho->value() != NULL)
    {
        current_file = temp_scho->value();
        partial = false;

        /* Update Current Scroll Position With Latest Data From Text Editor */
        hex_text[scroll_position] = hex_buffer->text();

        /* Form The String Variable That Is Combination Of All Elements From Hex Chunks */
        for(int x = -1; ++x < hex_text.size();)
        {
            data += hex_text[x];
        }

        /* If Mode Is Hex Save With These Settings */
        if(mode == "Mode : Hexadecimal")
        {
            y = 6;
            increment1 = 3;
            increment2 = 12;
            range = "16";
            read_length = 2;
        }

        /* If Mode Is Octal Save With These Settings */
        else if(mode == "Mode : Octal")
        {
            y = 5;
            increment1 = 4;
            increment2 = 13;
            range = "8";
            read_length = 3;
        }

        file.open(current_file.c_str(), ios::binary);

        /* If File Can Be Opened */
        if(file.is_open())
        {
            for(int x = 9; x < data.length();)
            {

                /* If 1st Or Not 6th Entry, Process Normally */
                if((z % words_per_line != 0) || (z == 0))    // if((z % 6 != 0) || (z == 0))
                {
                    y += increment1;
                    x += increment1;
                }

                /* If 6th Entry, Skip For Offsets */
                else
                {
                    y += increment2;
                    x += increment2;
                }

                /* Convert Data - If Not Partial */
                temp = data.substr(y, read_length);

                /* Pull Data From Hex Table If Mode Is Hexadecimal */
                if(mode == "Mode : Hexadecimal")
                {
                    put = hex_table[temp];
                }

                /* Pull Data From Oct Table If Mode Is Octal */
                else
                {
                    put = oct_table[temp];
                }

                file.write((char*)(&put), 1);

                z++;
            }

            file.close();
            main_output->label("File Has Been Saved");
            main_status->label("");
        }

        /* If Could Not Open, Print Error */
        else
        {
            main_output->label("Error : Could Not Write To File. Check Path or Permissions");
        }

    }
}

/****** Callback Function - Clear Hex Stuff ******/
void clear_hex(Fl_Menu_*, void*)//void clear_hex(Fl_Widget *w)
{
    partial_file.clear();
    hex_text.clear();
    start = "";
    _end = "";
    main_output->label("");
    hex_buffer->text("");
    hex_edit->buffer(hex_buffer);
    main_status->label("");
    current_file = "";
    current_file_size = 0;
    hex_out->value("");
    oct_out->value("");
    bin_out->value("");
    ascii_out->value("");
    dec_out->value("");
    etb_out->value("");
    stb_out->value("");
    scroll_position = -1;
}

/****** Callback Function - Convert Mode To Hexadecimal ******/
void view_hex(Fl_Menu_*, void*)//void view_hex(Fl_Widget *w)
{

    /* Don't Even Try To Do Anything If Mode Is Already Hex! */
    if(mode != "Mode : Hexadecimal")
    {
        string final = "", data = "", range = "", temp = "";
        int y = 5, z = 0;

        if(mode == "Mode : Octal")
        {
            range = "8";
        }

        /* Update Current Scroll Position With Latest Data From Text Editor */
        hex_text[scroll_position] = hex_buffer->text();

        /* Cycle Through All Hex Chunks Individually */
        for(int a = -1; ++a < hex_text.size();)
        {

            final = "";
            data = hex_text[a];
            final += data.substr(0, 9);
            y = 5;
            z = 0;

            for(int x = 9; x < data.length();)
            {

                if((z % words_per_line != 0) || (z == 0))    // if((z % 6 != 0) || (z == 0))
                {
                    y += 4;
                    x += 4;
                }

                else
                {
                    final += data.substr((y + 4), 9);
                    y += 13;
                    x += 13;
                }

                temp = data.substr(y, 3);
                temp = hex_log[oct_table[temp]];

                final += (temp + "\t");

                z++;
            }

            hex_text[a] = final;
        }

        stat_change = false;
        hex_buffer->text(hex_text[scroll_position].c_str());
        mode = "Mode : Hexadecimal";
        main_mode->label(mode.c_str());
    }
}

/****** Callback Function - Convert Mode To Octal ******/
void view_octal(Fl_Menu_*, void*)//void view_octal(Fl_Widget *w)
{

    /* Don't Even Try To Do Anything If Mode Is Already Octal! */
    if(mode != "Mode : Octal")
    {
        string final = "", data = "", range = "", offset = "", temp = "";
        int y = 6, z = 0;

        if(mode == "Mode : Hexadecimal")
        {
            range = "16";
        }

        /* Update Current Scroll Position With Latest Data From Text Editor */
        hex_text[scroll_position] = hex_buffer->text();

        /* Cycle Through All Hex Chunks Individually */
        for(int a = -1; ++a < hex_text.size();)
        {

            final = "";
            data = hex_text[a];
            final += data.substr(0, 9);
            y = 6;
            z = 0;

            for(int x = 9; x < data.length();)
            {

                if((z % words_per_line != 0) || (z == 0))    // if((z % 6 != 0) || (z == 0))
                {
                    y += 3;
                    x += 3;
                }

                else
                {
                    final += data.substr((y + 3), 9);
                    y += 12;
                    x += 12;
                }

                temp = data.substr(y, 2);
                temp = oct_log[hex_table[temp]];

                final += (temp + "\t");

                z++;
            }

            hex_text[a] = final;
        }

        stat_change = false;
        hex_buffer->text(hex_text[scroll_position].c_str());
        mode = "Mode : Octal";
        main_mode->label(mode.c_str());
    }
}
void close_all(Fl_Return_Button*, void*)
{
    exit(0);
}


int get_field(const int file_offset, const int field)
{
    int insert = 0;
    int range = int(floor((file_offset/page_word_count)));  	//    int range = int(floor((file_offset/162)));//page_word_count=>126->162
    string temp = hex_buffer->text();           		//    string temp = hex_buffer->text();
    hex_text[scroll_position] = temp;           		//    hex_text[scroll_position] = temp;
    hex_buffer->text(hex_text[range].c_str());  		//    hex_buffer->text(hex_text[range].c_str());
    hex_edit->buffer(hex_buffer);               		//    hex_edit->buffer(hex_buffer);
    hex_edit->wrap_mode(-1, words_per_line * 10);               //    hex_edit->wrap_mode(-1, 60);
    scroll_position = range;                    		//    scroll_position = range;
    hex_scroll->value(scroll_position, 0, 0, (hex_text.size() - 1));		//    hex_scroll->value(scroll_position, 0, 0, (hex_text.size() - 1));

    int select_position = abs((range * page_word_count) - file_offset);         //page_word_count=>126->162//    int select_position = abs((range * 162) - file_offset);//126->162
    int offset_add = 1 + int(floor(select_position/words_per_line));            //    int offset_add = 1 + int(floor(select_position/6));

    if(select_position != 0) 
    {
        insert = (9 * offset_add) + ((field + 1) * select_position);    	//        insert = (9 * offset_add) + ((field + 1) * select_position);
    }

    else
    {
        insert = 9;//        insert = 9;
    }

    hex_buffer->select(insert, insert + field);

    return insert;
}

int my_strlen(unsigned char *str)
{
    int i;

    for (i = 0; str[i]; i++)
        ;
    return i;
}

string find_bytes( vector<unsigned char> *const bytes, const string & bytes_hex_address_in_file)    //const char *bytes_hex_address_in_file)
{
    int field_length = 3;
    if(mode == "Mode : Hexadecimal")
    {
        field_length = 2;
    }

    string result="";


    /* Do Only If Cancel Button Not Pressed */
    if(bytes_hex_address_in_file.length() != 0)
    {
        string where_to_get_byte = bytes_hex_address_in_file;

        int file_offset = numparse(multi_base(where_to_get_byte, "16")), insert = 0;

        string test="", test1="";

        for(int i = 0; i < bytes->size(); i ++)
        {
            /* Go To Offset - Full File Open */
            if((file_offset <= current_file_size) && (file_offset >= 0) && (current_file_size != 0) && (current_file != "") && (partial == false))
            {

                insert = get_field(file_offset, field_length);
                hex_edit->insert_position(insert);

            }

            /* Go To Offset - Partial File Open */
            else if((file_offset <= numparse(_end)) && ((file_offset - numparse(start)) >= 0) && (current_file_size != 0) && (current_file != "") && (partial == true))
            {
                file_offset -= numparse(start);

                insert = get_field(file_offset, field_length);
                hex_edit->insert_position(insert);
            }
            else
            {
                main_output->label("Error : Offsets Out Of File Range");
            }

            if(NULL != bytes)
            {

                string temp_chain = hex_buffer->selection_text();
                unsigned char temp_char;

                (*bytes)[i]=(unsigned char)(unsigned int)hex_table[temp_chain];

            }

            result += hex_buffer->selection_text(); // result get a char array here!
            file_offset ++;
        }

    }
    return result;

}

/****** Callback Function - Goes To Specified Offset ******/
void goto_offset(Fl_Button*, void * user_data)//void goto_offset(Fl_Widget *w)
{
    //const char *find_byte = (const char *)0;
    string find_byte = "";
    cout<< "offset_hex->value() =\t"<< hex << offset_hex->value() << endl;
    string where_to_get_byte = offset_hex->value();
    cout << "where_to_get_byte =\t" << where_to_get_byte << endl;
    int file_offset = numparse(multi_base(where_to_get_byte, "16"));
    cout << "file_offset =\t" << file_offset << endl;
    if(file_offset> current_file_size || atoi(offset_dec->value()) < 0)
    {
        find_byte = fl_input("Enter the specific offset to go to - Offset must be a plain hexadecimal number", "Offset");
        if(find_byte.length() == 0)
        {
            cout<< "in if find_type = "<< find_byte << endl;

        }
        else
            find_byte = '0';
    }
    else
    {
        find_byte = offset_hex->value();
        cout<< "in else find_type =\t"<< find_byte << endl;
    }

    vector<unsigned char> result(1);
    find_bytes(&result, find_byte);// 1 is fields number

}

/****** Callback Function - Scroll Text ******/
void scroll(Fl_Widget *w)
{
    bool temp_change = false;

    if(main_status->label() == "*")
    {
        temp_change = false;
    }

    else
    {
        temp_change = true;
    }

    if(current_file != "")
    {
        string temp = hex_buffer->text();
        hex_text[scroll_position] = temp;
        hex_buffer->text(hex_text[hex_scroll->value()].c_str());
        hex_edit->buffer(hex_buffer);
        hex_edit->wrap_mode(-1, words_per_line * 10);    // hex_edit->wrap_mode(-1, 60);
        scroll_position = hex_scroll->value();
    }

    if(temp_change == true)
    {
        main_status->label("");
    }

    stat_change = temp_change;
}

/****** Callback Function - Quit *****/
void quit_hex(Fl_Menu_*, void*)//void quit_hex(Fl_Widget *w)
{
    exit(0);
}

/****** Callback Function - Show About Window ******/
void show_help(Fl_Menu_*, void*)//void show_help(Fl_Widget *w)
{
    Fl_Window *help = new Fl_Window(500, 135, "About The Hex Editor");
    help->begin();
    Fl_Text_Display *info = new Fl_Text_Display(25, 20, 450, 100, "");

    Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
    buffer->text("THE - The Hex Editor\nDerived from \"The Hex Editor 0.0.7\" of (c) Dan Baxter 2009\nMade with FLTK\nVersion : 0.0.8, Sept, 30, 2014\nEmail : beunprepared\@gmail.com");
    info->buffer(buffer);
    info->box(FL_FLAT_BOX);
    info->color(FL_BACKGROUND_COLOR);
    info->align(Fl_Align(FL_ALIGN_LEFT));
    help->position((Fl::w()/2 - 100) , (Fl::h()/2 - 75));

    help->end();
    help->show();
}

/****** Callback Function - Change Between Signed & Unsigned Data Analysis ******/
void flip(Fl_Widget *w)
{
    string eight_bit = etb_out->value();
    string sixteen_bit = stb_out->value();
    string temp = "";

    /* Change to Signed */
    if(sign_change->value() == 1)
    {
        signed char bit_set = numparse(eight_bit);
        etb_out->value(numparse((int)(bit_set)).c_str());
        temp = numparse((signed short int)numparse(sixteen_bit));
        stb_out->value(temp.c_str());
    }

    /* Change to Unsigned */
    else
    {
        etb_out->value(dec_out->value());
        temp = numparse((unsigned short int)numparse(sixteen_bit));
        stb_out->value(temp.c_str());
    }

}

/****** Callback Function - Change Save Status ******/
void update_save(int, int nInserted, int nDeleted,int, const char*, void* v)
{
    if(stat_change == true)
    {
        main_status->label("*");
    }

    string temp = hex_buffer->selection_text();
    string worker = "";
    string data = "";
    string append = "";
    int pos = 0;
    int base = 0;
    int append_length = 0;

    int strlen = 3;
    string strbase = "8";

    if(mode == "Mode : Hexadecimal")
    {
        strlen = 2;
        strbase = "16";
    }

    /* Analyze Data From Hexadecimal Mode */
    if((temp.length() == strlen) && (int(temp.find("	")) == -1))
    {
        worker = umulti_base(multi_base(temp, strbase), "16");

        if(worker.length() == 1)
        {
            worker = "0" + worker;
        }

        /* Get Hex Data */
        hex_out->value(worker.c_str());
        worker = umulti_base(multi_base(temp, strbase), "8");

        while(worker.length() < 3)
        {
            worker = "0" + worker;
        }

        /* Get Oct Data */
        oct_out->value(worker.c_str());
        worker = umulti_base(multi_base(temp, strbase), "2");

        while(worker.length() < 8)
        {
            worker = "0" + worker;
        }

        /* Get Binary and Decimal Data */
        bin_out->value(worker.c_str());
        worker = umulti_base(multi_base(temp, strbase), "10");
        dec_out->value(worker.c_str());

        /*Get ASCII and 8 - Bit Data */
        ascii_out->value(ascii_log[numparse(worker)].c_str());

        if(sign_change->value() == 1)
        {
            signed char bit_set = numparse(worker);
            etb_out->value(numparse((int)(bit_set)).c_str());
        }

        else
        {
            unsigned char bit_set = numparse(worker);
            etb_out->value(numparse((int)(bit_set)).c_str());
        }

        /*Get 16 - Bit Data */
        pos = hex_edit->insert_position();
        data = hex_buffer->text();
        append_length = data.length();

        /* If Selected Value Is Adjacent - Do Nothing Special */
        for(int x = 0; ++x <= strlen;)
        {
            if((data.substr((pos + x), 1) == "	") && (pos < (append_length - (strlen + 1))) )
            {
                pos += (x + 1);

                /* If Selected Value Is Adjacent To Last File Value - Include Last Value */
                if((pos >= (append_length - (strlen + 1))) && ((scroll_position + 1) == hex_text.size()))
                {
                    data = parse_zero(umulti_base(multi_base(data.substr(pos, strlen), strbase), "16")) + parse_zero(umulti_base(multi_base(hex_buffer->selection_text(), strbase), "16"));

                    if(sign_change->value() == 1)
                    {
                        data = numparse((signed short int)numparse(multi_base(data, "16")));
                    }

                    else
                    {
                        data = numparse((unsigned short int)numparse(multi_base(data, "16")));
                    }

                    pos = -1;
                    stb_out->value(data.c_str());
                }

                break;
            }
        }

        /* If Selected Value Is At The End Of A Screen But Not The Last Value Of File - Skip To Next Screen For Next Value */
        for(int x = 0; ++x <= strlen;)
        {
            if((data.substr((pos + x), 1) == "	") && (pos >= (append_length - (strlen + 1))) && ((scroll_position + 1) != hex_text.size()))
            {
                pos = -1;
                data = hex_text[scroll_position + 1].substr(9, strlen);
                data = parse_zero(umulti_base(multi_base(data, strbase), "16")) + parse_zero(umulti_base(multi_base(hex_buffer->selection_text(), strbase), "16"));

                if(sign_change->value() == 1)
                {
                    data = numparse((signed short int)numparse(multi_base(data, "16")));
                }

                else
                {
                    data = numparse((unsigned short int)numparse(multi_base(data, "16")));
                }

                stb_out->value(data.c_str());
                break;
            }
        }

        /* If Selected Value Is At The End Of A Screen And Is The Last Value Of File - Don't Try To Grab Another Value */
        for(int x = 0; ++x <= strlen;)
        {
            if((data.substr((pos + x), 1) == "	") && (pos >= (append_length - (strlen + 1))) && ((scroll_position + 1) == hex_text.size()))
            {
                data = hex_buffer->selection_text();

                if(sign_change->value() == 1)
                {
                    data = numparse((signed short int)numparse(multi_base(data, strbase)));
                }

                else
                {
                    data = numparse((unsigned short int)numparse(multi_base(data, strbase)));
                }

                stb_out->value(data.c_str());
                pos = -1;
                break;
            }
        }

        /* If Selected Value Is At The End Of A Line - Jump To Next Line For Value */
        if(pos % (((strlen + 1) * words_per_line) + 9)  == 0)    // if(pos % (((strlen + 1) * 6) + 9)  == 0)
        {
            pos += 9;
            data = data.substr(pos, strlen);
            data = parse_zero(umulti_base(multi_base(data, strbase), "16")) + parse_zero(umulti_base(multi_base(hex_buffer->selection_text(), strbase), "16"));

            if(sign_change->value() == 1)
            {
                data = numparse((signed short int)numparse(multi_base(data, "16")));
            }

            else
            {
                data = numparse((unsigned short int)numparse(multi_base(data, "16")));
            }

            stb_out->value(data.c_str());
        }

        /* Handle Output For Values Not Requiring Special Processing */
        else if(pos != -1)
        {

            if(strlen == 2)
            {
                data = data.substr(pos, strlen);
                data = data + hex_buffer->selection_text();
            }

            else
            {
                data = multi_base(data.substr(pos, strlen), "8");
                data = umulti_base(data, "16");
                temp = hex_buffer->selection_text();
                temp = multi_base(temp, "8");
                temp = umulti_base(temp, "16");
                data = parse_zero(data) + parse_zero(temp);
            }

            if(sign_change->value() == 1)
            {
                data = numparse((signed short int)numparse(multi_base(data, "16")));
            }

            else
            {
                data = numparse((unsigned short int)numparse(multi_base(data, "16")));
            }

            stb_out->value(data.c_str());
        }
    }

    stat_change = true;
}

void parse_asd(Fl_Menu_*m, void* file)
{

    try
    {
        open_hex(m, default_file);
    }
    catch (exception e)
    {
        throw e;
    }

}



void get_value(Fl_Button*, void*v )
{

    ValueInfo * query_info = (ValueInfo*)v;

    string offset_info_hex, offset_info_dec;
    int index;
    string field_type_info;
    try
    {

        if(0 == query_info->offset_hex().length())query_info->offset_hex(fl_input("Input Offset address in Hexadecimal"));

        if(0 == query_info->bytes_number())
        {
            index = 1;
        }
        else
        {
            index = query_info->bytes_number();
        }
        cout << "index =" << index <<endl;

        field_type_info = query_info->field_type();

        cout << "query_info->offset_hex() =\t" << query_info->offset_hex() << endl;
        cout << "query_info->offset_dec() =\t" << query_info->offset_dec() << endl;
        cout << "query_info->bytes_number() =\t" << query_info->bytes_number() << endl;
        cout << "query_info->field_type() =\t" << query_info->field_type() << endl;

    }
    catch(exception e)
    {
        throw e;
    }

    std::ostringstream prepared_header_buffer;
    prepared_header_buffer << "Offset =\t" << query_info->offset_dec() <<"\tValue =\t";

    string _value = name_to_delegate[query_info->field_type()]->value_find(query_info);

    prepared_header_buffer <<_value << "("<< query_info->field_type() <<")\n"<<endl;


    string buffer(prepared_header_buffer.str());

	char buffer_chars[256];//char buffer_chars[buffer.length()+1];
    buffer_chars[buffer.length()] = '\0';
    for(int i=0; i< buffer.length(); i++)
    {
        buffer_chars[i]=buffer.at(i);

    }

    header_buffer->text(buffer_chars);

    offset_hex->hide();
    offset_dec->hide();
    offset_hex->show();
    offset_dec->show();
    bytes_number->hide();
    bytes_number->show();
    field_type->hide();
    field_type->show();

}

void save_value(Fl_Button*, void*)
{

}

void refine_hex(char * const destination, string const &source)
{
    int len = source.length();

    destination[8] = '\0';
    for(int i = 7; i >= 2; i --)
    {
        if(len  > 0)
        {
            destination[i] = toupper(source[--len]);

        }
        else
            destination[i] = '0';
    }



    destination[1] = 'X';
    destination[0] = '0';

}

void synchronize_offset_to_hex(Fl_Input* the, void*vi)
{
    ((ValueInfo *)vi)->offset_dec(the->value());

}

std::string remove_chars(const std::string& source, const std::string& chars)
{
    std::string result="";

    for (unsigned int i=0; i<source.length(); i++)
    {
        bool foundany=false;
        for (unsigned int j=0; j<chars.length() && !foundany; j++)
        {
            foundany=(source[i]==chars[j]);
        }
        if (!foundany)
        {
            result+=source[i];
        }
    }
    return result;
}

std::string keep_chars(const std::string& source, const std::string& chars)
{
    std::string result="";

    for (unsigned int i=0; i<source.length(); i++)
    {
        bool foundany=false;
        for (unsigned int j=0; j<chars.length() && !foundany; j++)
        {
            foundany=(source[i]==chars[j]);
        }
        if (foundany)
        {
            result+=source[i];
        }
    }
    return result;
}

void synchronize_offset_to_dec(Fl_Input* the, void*vi)
{
    ((ValueInfo *)vi)->offset_hex(the->value());
}


void restrict_word_length_dec(Fl_Input*the, void*vi)
{
    ((ValueInfo *)vi)->bytes_number_str(the->value());

}

void restrict_type_string(Fl_Input*, void*)
{

}

template<typename T>string value_find_reverse_template(T *the, ValueInfo *v)
{

    cout << "calling data type parse function : \t" << the->name() << endl;

    const int _word_length = the->word_length();
    cout << "word_length =\t" << _word_length << endl;
    vector<unsigned char> bytes(_word_length);

    string _value_string = find_bytes(&bytes, ((ValueInfo*)v)->offset_hex()); // just get one word

    const size_t union_length = sizeof(typename T::value_type);
    cout << "union_length =\t" << union_length << endl;
    union
    {
        unsigned char origin[union_length];
        typename T::value_type f;
    } switcheroo;

    if(_word_length < union_length)
    {
        for(int i = 0; i < _word_length; i ++)
        {
		switcheroo.origin[i] = bytes[i];
        }

        for(int i = _word_length; i < union_length; i ++)
        {
            switcheroo.origin[i] = '\0';
        }
    }
    else if(_word_length == union_length)
    {
        for(int i = 0; i < _word_length; i ++)
        {
		switcheroo.origin[i] = bytes[i];
        }

    }

    typename T::value_type _value = *reinterpret_cast<typename T::value_type*>(switcheroo.origin);

    cout << "union version in hex from cheat_hex =\t" << cheat_hex(switcheroo.f ) << endl;
    cout << "union version in hex from cheat<long int> =\t" << cheat<typename T::value_type>(switcheroo.f ) << endl;
    cout << "union version in dec =\t" << switcheroo.f << endl;

    cout << "reinterpret_cast value in hex from cheat_hex =\t" << cheat_hex(_value ) << endl;
    cout << "reinterpret_cast value in hex from cheat<long int> =\t" << cheat<typename T::value_type>(_value ) << endl;
    cout << "reinterpret_cast value in dec =\t" << _value << endl;

    std::ostringstream ss;
    ss << setprecision(16) << _value;
    std::string s(ss.str());

    return s;
}

template<typename T>string value_find_template(T *the, ValueInfo *v)
{

    cout << "calling data type parse function : \t" << the->name() << endl;

    const int _word_length = the->word_length();
    cout << "word_length =\t" << _word_length << endl;
    vector<unsigned char> bytes(_word_length);

    string _value_string = find_bytes(&bytes, ((ValueInfo*)v)->offset_hex()); // just get one word

    const size_t union_length = sizeof(typename T::value_type);
    cout << "union_length =\t" << union_length << endl;
    union
    {
        unsigned char origin[union_length];
        typename T::value_type f;
    } switcheroo;

    if(_word_length < union_length)
    {
        for(int i = 0; i < _word_length; i ++)
        {
		switcheroo.origin[i] = bytes[i];
        }
        
        for(int i = _word_length; i < union_length; i ++)
        {

            switcheroo.origin[i] = '\0'; 

        }
    }
    else if(_word_length == union_length)
    {
        for(int i = 0; i < _word_length; i ++)
        {
		switcheroo.origin[i] = bytes[i];
        }

    }
    
    typename T::value_type _value = *reinterpret_cast<typename T::value_type*>(switcheroo.origin);
    
    cout << "union version in hex from cheat_hex =\t" << cheat_hex(switcheroo.f ) << endl;
    cout << "union version in hex from cheat<long int> =\t" << cheat<typename T::value_type>(switcheroo.f ) << endl;
    cout << "union version in dec =\t" << switcheroo.f << endl;

    cout << "reinterpret_cast value in hex from cheat_hex =\t" << cheat_hex(_value ) << endl;
    cout << "reinterpret_cast value in hex from cheat<long int> =\t" << cheat<typename T::value_type>(_value ) << endl;
    cout << "reinterpret_cast value in dec =\t" << _value << endl;

    std::ostringstream ss;
    ss << setprecision(16) << _value;
    std::string s(ss.str());

    return s;
}

template<>string TypeBase<Byte>::value_find(ValueInfo *v)
{
    return value_find_template(this, v);
}


template<>string TypeBase<UnsignedShort>::value_find(ValueInfo *v)
{
    return value_find_template(this, v);
    
}

template<>string TypeBase<UnsignedInt>::value_find(ValueInfo *v)
{

    return value_find_template(this, v);

}



template<>string TypeBase<UnsignedChar>::value_find(ValueInfo *v)
{
    return value_find_template(this, v);
}


template<>string TypeBase<Char>::value_find(ValueInfo *v)
{
    return value_find_template(this, v);
}


template<>string TypeBase<Int>::value_find(ValueInfo *v)
{
    return value_find_template(this, v);
}


template<>string TypeBase<Float>::value_find(ValueInfo *v)
{

    return value_find_template(this, v);

}



template<>string TypeBase<Double>::value_find(ValueInfo *v)
{

    return value_find_template(this, v);

}




template<>string TypeBase<AppData>::value_find(ValueInfo *)
{
    cout << "calling value_find<AppData>" << endl;
    return "";
}




template<>string TypeBase<GPSData>::value_find(ValueInfo *)
{
    cout << "calling value_find<GPSData>" << endl;
    return "";
}


template<>string TypeBase<UnsignedLong>::value_find(ValueInfo *v)
{
    return value_find_template(this, v);
}


template<>string TypeBase<StructTm>::value_find(ValueInfo *v)
{
    _tm time_cur;
    int int_chain[9];
    string result = "";
    int  offset_dec = atoi(v->offset_dec().c_str());
    string parse_type = "unsigned int";
    for(int i = 0; i < 9; i ++)
    {


        char buffer[17];
        buffer[16] = '\0';
        internal_itoa(offset_dec, buffer, 10);
        v->offset_dec(string(buffer));

        Delegate * del = name_to_delegate[parse_type];
        string temp  = del->value_find(v);
        int_chain[i] =  atoi(temp.c_str());//chain[i] =  atoi((name_to_delegate[string("unsigned int")]->value_find(v)).c_str());

        result += temp + ":";
        offset_dec += name_to_delegate[parse_type]->word_length();
    }
    cout << "int chain =\t ";
    for(int i = 0; i < 9; i ++)cout <<  int_chain[i] << " ";
    cout << endl;
    /*
        int tm_sec;       // seconds [0,61]
        int tm_min;       // minutes [0,59]
        int tm_hour;      // hour [0,23]
        int tm_mday;      // day of month [1,31]
        int tm_mon;       // month of year [0,11]
        int tm_year;      // years since 1900
        int tm_wday;      // day of week [0,6] (Sunday = 0)
        int tm_yday;      // day of year [0,365]
        int tm_isdst;     // daylight savings flag
    */

    time_cur.tm_sec =  int_chain[0];
    time_cur.tm_min =  int_chain[1];
    time_cur.tm_hour =  int_chain[2];
    time_cur.tm_mday =  int_chain[3];
    time_cur.tm_mon =  int_chain[4];
    time_cur.tm_year =  int_chain[5];
    time_cur.tm_wday =  int_chain[6];
    time_cur.tm_yday =  int_chain[7];
    time_cur.tm_isdst =  int_chain[8];

    cout << "v->offset_dec() =\t" << v->offset_dec() << endl;
    cout << "v->offset_hex() =\t" << v->offset_hex() << "\nresult =\t" << result << endl;
    ostringstream ss;
    ss << time_cur;
    string r = ss.str();
    cout << time_cur << endl;

    return r;
}

template<>string TypeBase<ReverseInt>::value_find(ValueInfo *v)
{
    return value_find_reverse_template(this, v);
}

template<>string TypeBase<Time_T>::value_find(ValueInfo *v)
{
    cout << "calling data type parse function : \t" << this->name() << endl;

    const int _word_length = name_to_delegate[((ValueInfo*)v)->field_type()]->word_length();
    const int real_hex_count = _word_length *2;   // ?
    cout << "_word_length =\t" << _word_length << endl;
    cout << "real_hex_count =\t" << real_hex_count << endl;
    vector<unsigned char> bytes(_word_length);//vector<unsigned char> bytes(atoi(value_info->bytes_number->value())); // just get one word

    string _value_string = find_bytes(&bytes, ((ValueInfo*)v)->offset_hex()); // just get one word

    const size_t union_length = sizeof(unsigned long int);
    union
    {
        unsigned char origin[union_length];
        unsigned long int f;
    } switcheroo;	// worked, good way

    if(_word_length < union_length)
    {
        for(int i = 0; i < _word_length; i ++)
        {
		switcheroo.origin[i] = bytes[i];
        }
        cout << "sizeof(long int) =\t" << cheat_hex(union_length) << endl;
        for(int i = _word_length; i < union_length; i ++)
        {

		switcheroo.origin[i] = '\0';

        }
    }
	else if (_word_length == union_length)
	{
		for (int i = 0; i < _word_length; i++)
		{
			switcheroo.origin[i] = bytes[i];
		}

	}
    
    unsigned long int _value = *reinterpret_cast<unsigned long int*>(switcheroo.origin);

    
    cout << "union version in hex from cheat_hex =\t" << cheat_hex(switcheroo.f ) << endl;
    
    cout << "union version in dec =\t" << switcheroo.f << endl;

    cout << "reinterpret_cast value in hex from cheat_hex =\t" << cheat_hex(_value ) << endl;
    
    cout << "reinterpret_cast value in dec =\t" << _value << endl;

    time_utility tu  = get_time_from_seconds(switcheroo.f, 1970);

    cout << "record time is: \t" << dec << tu.real_year << ":"<< std::setfill('0') << std::setw(2) << tu.cur_year_months_left<<":"<< tu.cur_month_days_left<<":"<< tu.cur_day_hours_left<<":"<< tu.cur_hour_minutes_left<<":"<< tu.cur_minute_seconds_left<< endl;


    std::ostringstream ss;
    ss << tu.real_year << ":"<< std::setw(2) << std::setfill('0')<< tu.cur_year_months_left<<":" << tu.cur_month_days_left<<":"<< tu.cur_day_hours_left<<":"<< tu.cur_hour_minutes_left<<":"<< tu.cur_minute_seconds_left;
    std::string s(ss.str());

    return s;

}

void bind_query_type(Fl_Input_Choice*field_type, void* )
{

    try
    {
        name_to_delegate[string(field_type->value())]->word_length_restrict((ValueInfo*)value_info);
    }
    catch(exception e)
    {
        throw e;
    }
}


void export_single_file(Fl_Menu_*m, string & input_file_name)
{

    struct_name full_file_name = parse_file_name(input_file_name);
    const char *input_file_chain = input_file_name.c_str();
    

    string output_file_name = full_file_name.path + full_file_name.name + ".csv";
    cout << "output_file_name =\t" << output_file_name << endl;

    auto len = output_file_name.length();

    const char *output_file_chain = output_file_name.c_str();
    cout << "output_file_name.length() =\t" << len << endl;

    cout << "before call open_hex input_file_chain =\t" << input_file_chain << endl;
    open_hex(m, (void*)input_file_chain);

    std::ofstream file_output(output_file_name.c_str(), ios::out);  // default use the same name and path;
    if(file_output.is_open())
    {
        field_type->value("double");
        Delegate * del=name_to_delegate[value_info->field_type()];

        char buffer[17];
        buffer[16] = '\0';
        internal_itoa(del->word_length(),buffer, 10);
        value_info->bytes_number(del->word_length());
        value_info->field_type("double");
        char buffer0[17];
        buffer0[16] = '\0';
		internal_itoa(484, buffer0, 10);
        value_info->offset_dec(buffer0);
        
        vector<output_tri> contents(1451);
        contents.reserve(1451);
        float index = 350.00;
        int account = 1;
        for(int i = 484; i < 484 + 1451 * 8; i += 8)   //current_file_size-484
        {
            value_info->offset(i);
            output_tri o; 
            std::ostringstream buffer_index;
            buffer_index << index;

            o.x = buffer_index.str();

            o.y1 = name_to_delegate[value_info->field_type()]->value_find(value_info);

            value_info->offset(i + 11628);  // 11628 <= 12112 - 484

            cout << "value_info->offset_dec() =\t" << value_info->offset_dec() << endl;
            cout << "value_info->offset_hex() =\t" << value_info->offset_hex() << endl;

            o.y2 = name_to_delegate[value_info->field_type()]->value_find(value_info);

            contents.push_back(o);

            cout<< "contents[" << account << "].x =\t" << contents.back().x << "\tcontents[" << account << "].y1 =\t" << contents.back().y1 << "\tcontents[" << account << "].y2 =\t" << contents.back().y2 << endl;
            file_output << contents.back().x << "," << contents.back().y1 << "," << contents.back().y2 << "\n";

            index ++;
            account ++;
        }

    }
    file_output.close();

}

void file_export(Fl_Menu_*m, void*)
{


    Fl_File_Chooser *temp_fcho = (Fl_File_Chooser *)0;

    if(current_file_size <= 0)
    {

        temp_fcho = new Fl_File_Chooser(favorite_path.c_str(), "*", Fl_File_Chooser::MULTI, "Open Files For Export");
        if((Fl_File_Chooser *)0 != temp_fcho)
        {
            temp_fcho->show();

            while( temp_fcho->shown())
            {
                Fl::wait();
            }

            cout << "temp_fcho->count() = \t" << temp_fcho->count() << endl;

            vector<string> file_list;

            for ( int t = 1; t <= temp_fcho->count(); t ++ )
            {
                string input_file_name = temp_fcho->value(t);
                export_single_file(m, input_file_name);

            }
        }

    }
    else
    {
        export_single_file(m, current_file);
    }


}

void refresh_input(Fl_Text_Display*, void*)
{
    cout << "In Text_Desplay Refresh" << endl;

    offset_hex->position(830, 315);

    offset_dec->position(830, 357);
    offset_hex->show();
    offset_dec->show();
    bytes_number->hide();
    bytes_number->show();
    field_type->hide();
    field_type->show();

}

void refresh_offset(Fl_Group*, void*)
{
    cout << "In Group Refresh" << endl;

    offset_hex->position(830, 315);

    offset_dec->position(830, 357);
    offset_hex->show();
    offset_dec->show();
    bytes_number->hide();
    bytes_number->show();
    field_type->hide();
    field_type->show();

}

unsigned int ValueInfo::offset()
{
    if(-1 == this->_offset)this->_offset = atoi(this->offset_dec().c_str());
    return this->_offset;
}
unsigned int ValueInfo::bytes_number()
{
    if(-1 == this->_bytes_number)this->_bytes_number = atoi(::bytes_number->value());
    return this->_bytes_number;
}

Delegate * const ValueInfo::type_delegate()
{
    if((Delegate *)0 == this->_type_delegate)this->_type_delegate = name_to_delegate[string(::field_type->value())];
    return this->_type_delegate;
}

string ValueInfo::_offset_hex()
{
    this->___offset_hex = ::_offset_hex;
    return this->___offset_hex->value();
}
string ValueInfo::_offset_dec()
{
    this->___offset_dec = ::_offset_dec;
    return this->___offset_dec->value();
}
string ValueInfo::offset_hex()
{
    this->__offset_hex = ::offset_hex;
    return this->__offset_hex->value();
}
string ValueInfo::offset_dec()
{
    this->__offset_dec = ::offset_dec;
    return this->__offset_dec->value();
}
string ValueInfo::bytes_number_str()
{
    this->_bytes_number_str = ::bytes_number;
    return this->_bytes_number_str->value();
}
string ValueInfo::field_type()
{
    this->_field_type = ::field_type;
    return this->_field_type->value();
}
string ValueInfo::header_buffer()
{
    this->_header_buffer = ::header_buffer;
    return this->_header_buffer->text();
}

int const restrict_bytes_number(ValueInfo *vi, Fl_Input *const the, int const _value)
{

    int word_length = name_to_delegate[vi->field_type()]->word_length();
    int input_length;

    input_length = _value;
    if(input_length == 0)input_length = 1;

    if(input_length < 0)input_length = -input_length;

    int offset_length = atoi(vi->offset_dec().c_str());
    int left = current_file_size - offset_length;

    if (0 == current_file_size|| left < 0)input_length = 0;
    else if(input_length > left)
    {
        input_length = left;
    }
    else if(input_length < word_length)
    {
        input_length = word_length;
    }
    else if(input_length%word_length != 0)
    {
        input_length += word_length - input_length%word_length;
    }
    cout << "input_length =\t" << input_length << endl;

    char file_offset_char[17];
    file_offset_char[16] = '\0';

	internal_itoa(input_length, file_offset_char, 10);

    the->value(file_offset_char);

    return input_length;
}

string hex_restrict_to_dec(Fl_Input *const the, string const & _value)
{
    char field_range[]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};

    string range = field_range;

    string temp = _value;

    if(temp == "")temp = "0X000000";

    string where_to_get_byte="";

    if (temp.substr(0,1)== "0X")temp = temp.substr(2, temp.length()-1);
    cout << "as the->value() =\t" << temp << endl;
    where_to_get_byte = keep_chars(temp, field_range);


    int i = 0;
    while(i < where_to_get_byte.length() && ('0' == where_to_get_byte.at(i++))) {}	// remove '0' before effective numbers
    where_to_get_byte = where_to_get_byte.substr(i-1, where_to_get_byte.length()-1);

    cout << "before process where_to_get_byte =\t" << where_to_get_byte << endl;
    if(where_to_get_byte.length() > 6)where_to_get_byte = where_to_get_byte.substr(0, 5);


    int file_offset = numparse(multi_base(where_to_get_byte, "16"));

    char buffer [17];
    buffer[16] = '\0';	// no "0X" mark

    if(file_offset > current_file_size)
    {
        file_offset = current_file_size;
        internal_itoa(file_offset, buffer, 16);
        cout << "over trunk::buffer[6] =\t" << string(buffer) << endl;
    }
    else
    {

        for(int i = 0; i < where_to_get_byte.length(); i ++)
        {
            buffer[i] = where_to_get_byte.at(i);
        }
        buffer[where_to_get_byte.length()] = '\0';
        cout << "direct::buffer[6] =\t" << string(buffer)<<endl;
    }
    cout << "file_offset =\t" << file_offset << endl;

    char refined[17];
    refined[16] = '\0';
    refine_hex(refined, buffer);

    the->value(refined);

    char buffer_dec[17];
    buffer_dec[16] = '\0';

    return internal_itoa(file_offset, buffer_dec, 10);
}


string dec_restrict_to_hex(Fl_Input *const the, string const & _value)
{

    int real_value = atoi(_value.c_str());

    if(real_value < 0)real_value = -real_value;

    int word_length = value_info->bytes_number();
    int left = current_file_size - word_length;
    if(0 ==current_file_size||left < 0)real_value = 0;
    else if(real_value > left)
    {
        real_value = left;
    }
    cout << "decimal_offset =\t" << real_value << endl;

    char file_offset_char[17];
    file_offset_char[16] = '\0';

    internal_itoa(real_value, file_offset_char, 10);
    the->value(file_offset_char);


    char refined[17];
    refined[16] = '\0';
    internal_itoa(real_value, file_offset_char, 16);
    refine_hex(refined, file_offset_char);

    return refined;
}

string ValueInfo::_offset_hex(string const & _value)
{
    if((Fl_Input *)0 == this->___offset_hex)this->___offset_hex = ::_offset_hex;
    if((Fl_Input *)0 == this->__offset_hex)this->__offset_hex = ::offset_hex;
    if((Fl_Input *)0 == this->___offset_dec)this->___offset_dec = ::_offset_dec;
    if((Fl_Input *)0 == this->__offset_dec)this->__offset_dec = ::offset_dec;

    string dec_str = hex_restrict_to_dec(this->___offset_hex, _value);

    this->__offset_hex->value(this->___offset_hex->value());
    this->___offset_dec->value(dec_str.c_str());
    this->__offset_dec->value(dec_str.c_str());

    this->_offset = atoi(dec_str.c_str());

    return this->___offset_hex->value();
}
string ValueInfo::_offset_dec(string const & _value)
{
    if((Fl_Input *)0 == this->___offset_hex)this->___offset_hex = ::_offset_hex;
    if((Fl_Input *)0 == this->__offset_hex)this->__offset_hex = ::offset_hex;
    if((Fl_Input *)0 == this->___offset_dec)this->___offset_dec = ::_offset_dec;
    if((Fl_Input *)0 == this->__offset_dec)this->__offset_dec = ::offset_dec;

    string hex_str = dec_restrict_to_hex(this->___offset_dec, _value);

    string dec_str = this->___offset_dec->value();
    this->__offset_dec->value(dec_str.c_str());
    this->___offset_hex->value(hex_str.c_str());
    this->__offset_hex->value(hex_str.c_str());

    this->_offset = atoi(dec_str.c_str());

    return this->___offset_dec->value();
}
string ValueInfo::offset_hex(string const & _value)
{
    if((Fl_Input *)0 == this->___offset_hex)this->___offset_hex = ::_offset_hex;
    if((Fl_Input *)0 == this->__offset_hex)this->__offset_hex = ::offset_hex;
    if((Fl_Input *)0 == this->___offset_dec)this->___offset_dec = ::_offset_dec;
    if((Fl_Input *)0 == this->__offset_dec)this->__offset_dec = ::offset_dec;

    string dec_str = hex_restrict_to_dec(this->__offset_hex, _value);

    this->___offset_hex->value(this->__offset_hex->value());
    this->___offset_dec->value(dec_str.c_str());
    this->__offset_dec->value(dec_str.c_str());

    this->_offset = atoi(dec_str.c_str());

    return this->__offset_hex->value();
}
string ValueInfo::offset_dec(string const & _value)
{
    if((Fl_Input *)0 == this->___offset_hex)this->___offset_hex = ::_offset_hex;
    if((Fl_Input *)0 == this->__offset_hex)this->__offset_hex = ::offset_hex;
    if((Fl_Input *)0 == this->___offset_dec)this->___offset_dec = ::_offset_dec;
    if((Fl_Input *)0 == this->__offset_dec)this->__offset_dec = ::offset_dec;

    string hex_str = dec_restrict_to_hex(this->__offset_dec, _value);
    string dec_str = this->__offset_dec->value();
    this->___offset_dec->value(dec_str.c_str());
    this->___offset_hex->value(hex_str.c_str());
    this->__offset_hex->value(hex_str.c_str());

    this->_offset = atoi(dec_str.c_str());

    return this->__offset_dec->value();
}
string ValueInfo::bytes_number_str(string const & _value)
{
    if((Fl_Input *)0 == this->_bytes_number_str)this->_bytes_number_str = ::bytes_number;
    this->_bytes_number = restrict_bytes_number(this, this->_bytes_number_str, atoi(_value.c_str()));

    return this->_bytes_number_str->value();
}
string ValueInfo::field_type(string const & _value)
{
    if((Fl_Input_Choice *)0 == this->_field_type)this->_field_type = ::field_type;
    this->_field_type->value(_value.c_str());
    return this->_field_type->value();
}
string ValueInfo::header_buffer(string const & _value)
{
    if((Fl_Text_Buffer *)0 == this->_header_buffer)this->_header_buffer = ::header_buffer;
    this->_header_buffer->text(_value.c_str());
    return this->_header_buffer->text();
}
Delegate * const ValueInfo::type_delegate(Delegate * const _type_delegate)
{
    if((Delegate *)0 == this->_type_delegate)this->_type_delegate = name_to_delegate[string(::field_type->value())];
    return this->_type_delegate = _type_delegate;
}

int ValueInfo::bytes_number(int const _bytes_number)
{
    if((Fl_Input *)0 == this->_bytes_number_str)this->_bytes_number_str = ::bytes_number;
    this->_bytes_number = restrict_bytes_number(this, this->_bytes_number_str, _bytes_number);

    return this->_bytes_number;
}


int ValueInfo::offset(int const offset)
{
    if(-1 == this->_offset)this->_offset = _offset;
    char buffer[17];
    buffer[16] = '\0';
    this->offset_dec(internal_itoa(offset, buffer, 10));
    cout << "after assignment, this->offset_dec() =\t" << this->offset_dec() << endl;
    return this->_offset;
}
