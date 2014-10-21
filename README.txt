THE - The Hex Editor		Version 0.0.7
=====================

Description
=====================
THE is an acronym that stands for The Hex Editor. It is a simple hex editor designed with FLTK that is meant to be used across all major computing platforms. It is meant to be an open source solution to having a basic functioning hex editor that performs well on almost any system.

This program is released under the GNU General Public License (GPL) version 2. Please see the included file 'COPYING' for more information.

Installation
=====================
Refer to the included file INSTALL

Features
=====================
	* Open any file and display in raw hexadecimal, octal values.
	* Save changes to a file from hexadecimal, octal values.
	* View current offset of hexadecimal, octal values.
	* Open any file partially from specified offsets.
	* Save changes of partially opened file to original file.
	* Data Analyzer gives details for Hexadecimal, Octal, Binary, ASCII, 8-bit and 16-bit values.
	* Data Analyzer provides viewing of signed and unsigned values.
	* Jump to any specified offset within a file. 

Keybindings
=====================
Control + O			Open File
Control + S			Save File
Control + Shift + S		Save File As
Control + Q			Quit
Alt + C				Clear Screen
Alt + G				Go To Offset
Alt + H				Switch View Mode To Hexadecimal
Alt + O				Switch View Mode To Octal

Using THE
=====================
There are a few things to note about THE, in that being such an early build of the program, it is slightly picky about things. DO NOT try to edit the offsets, they are not meant to be messed with in any way. Let them be whenever you open a file. THE will not parse the file correctly if you do something with them, which may end up damaging your file. Also, to ensure the integrity with which THE parses hex values, make sure that all hex values have a tab before and after them. If you're changing existing hex values, this shouldn't be a problem, however, if you are adding new hex values directly to a file, please be aware of this as well. Also note that for THE to properly parse the hexadecimal data, all values must be capitalized alphanumeric. Values such as "xx" will not be parsed as their equivalent "XX" representation. Aside from that, please be aware that THE, in its current beta, is not quite suitable for large files (25MB and above), simply because of the load and save time issues that are present within this current build. The problem is isolated to the delay between loading a large file and saving a large files, not actually with manipulating the file itself. Depending on user hardware and computer specs, the time delay due to processing may be acceptable. Future builds will seek to minimize loading and saving times.

THE is relatively easy to use. You simply open a file, and in the six last columns, you'll see the file's hexadecimal values which you are free to edit. You'll notice four bars down at the bottom. The first one is the program's status which will periodically display messages such as "File Saved Successfully" or will generate error messages based upon the conditions. The second bar shows the current view/edit mode that THE is in. Currently, there is only support for hexadecimal and octal editing, but eventually THE will support binary, ASCII and other view/edit modes. The last one is the current file's save status. A little "*" will pop up any time you make a change to a file and have not committed a save of some sort to it. Once saved, it returns to being blank. The last bar is THE's progress bar that details the loading state of a file. Once it fills up completely, the file will be opened and displayed in hexadecimal notation.

Once a file is loaded use the View Menu to toggle back and forth between hexadecimal editing and octal editing. At any point you can save your file in either mode and the values will be parsed by THE automatically. For better efficiency, the offsets are always displayed as hexadecimal numbers, regardless of whatever the view mode is. Again be aware that THE will not correctly parse values like "xx", only values in the form of "XX". Also, ensure that the values are of the appropiate length for proper parsing. For example, if you have just "7" in hexadecimal, the correct form is "07" to convert from hexadecimal to octal. Likewise, the correct octal form for "7" is "007". Note that these rules apply to switching view modes as well as saving. THE won't be able to parse the values correctly if the view mode is switched when the values aren't set up in the correct forms.

THE also offers the ability to open a file and edit only a specific range of values from a given file. Under the File Menu, select "Partial Open..." and open the file. After that THE will ask for the starting offset and the ending offset. The offsets must be given as a plain hexadecimal number, such as "7A" or "32", or "F89". You must pick out offsets that are within range of the file. If the offsets cannot be found in the file, THE will not work on the file at all, so it is important to know the correct values when attempting to open a file partially. THE can also commit partial saves based upon a file that was partially opened. The only changes to the original file will be from the offsets specified when the file was first partially opened. In addition, it's possible to save the partial file as a seperate and distinct file using "Save As".

To engage the data analyzer in THE, once a file has been loaded simply double click on the value you would like to inspect. At the bottom of THE, the data will be displayed for each of the following : Hexadecimal, Octal, Binary, ASCII, 8-bit and 16-bit. At the lower left hand corner, there is a checkbox to toggle between viewing the data as either signed or unsigned. By default, it is set to signed, but clicking on this will switch this setting. Currently, THE does not support 32-bit and 64-bit analysis, only because at this time these features are not ready to be part of the project. Once considered stable, they will immediately be adopted.

THE now has the ability to go to any offset within a file. Under the Edit Menu, select "Go To Offset" and enter in the offset of your choice. Like partially opening a file, the offset muse be given as a plain hexadecimal number, "7A" or "32", or "F89". THE will be taken to the corresponding offset and select the given offset. The data analyzer will automatically be engaged, showing the relevant information to that value.

Feedback
=====================
THE is still a project very much in its infancy. Please let the author know of any bugs or unusual behavior you experience during its usage. You can contact the author via e-mail about your problem. Please be as specific as you can in describing what's wrong. Also, if you would like to request a feature, make a suggestion for the project or have any other comments, don't hesitate to drop a line. Talking about THE in a blog, podcast, magazine or other medium? Let us know!

THE Author
=====================
Daniel Baxter - 
Position : Main Programmer, Project Admin
Contact : shonumi@gmail.com
Contributed by Hugh Young
Contact : hughvonyoung@gmail.com
Random Fact : Wishes there were more corn dogs in the 'fridge.


Special Thanks
=====================
Sebastiani from the C++ Programming boards over at cprogramming.com (http://cboard.cprogramming.com/) for giving Dan the idea of using a lookup table. Mega speed increases instantly ensued! Many thanks Sebastiani!
