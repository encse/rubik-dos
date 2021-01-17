SYMANTEC LOCALISATION GROUP IRELAND

README.TXT--
Copyright 1998 Symantec Corp. All Rights Reserved.

NORTON COMMANDER
Version 5.5

Welcome to Norton Commander Version 5.5 !
------------------------------------------
Please read this document carefully, it contains important information not
included in the documentation.

     Topics covered in this note include:

     * Files that come with Norton Commander 5.5
     * Memory Requirements
	<> Commander Memory Requirements
     	<> Launching Commander with Small and Tiny memory configurations.
     * Compressed File Support
     	<> Compressing Hidden Files
     	<> Password protected Compressed files
	<> Limitations in Compressed Files
     * Viewer Support
	<> Internal Viewer
	<> Defining Viewers
     * Filters
     * Commander File Transfer
	<> Cloning using SHARE.EXE
	<> Pin layout of Commander cables
		- Pin layout of the serial cable
		- Pin layout of the parallel cable
     * Special Characters in the Commander Editor
     * Norton Commander Environment Variables
     * Limitations in Changing Drives
     * Accented characters in file and directory names
     * Installing Commander under Windows 3.1
     * Datex.p File
     * Windows 95 and OS/2 Compatibility
     * Copy Diskette Utility
     * Playwave Utility
     * Telephone Book Conversion Utility (DIR2DIR)
     * NC_EXIT Utility
     * Novell NDS
     * Useful advice when running Commander on a 1MB or less system
     * Running Norton Commander from a shared Network Drive
     * Creating a user defined tools menu
     * Using screen savers under Windows


Norton Commander 5.5 File List
------------------------------

Diskette 1 : Installation Diskette
	install.cfg 		Installation Configuration File
	install.exe		Installation Program
	installd.exe		Installation Program
	nc.cfg      		Configuration program for Viewers
	nc.exe      		NC Loader
	nc.fil      		NC Filters Database
	nc.hlp      		NC Main Help File
	nc.msg      		NC Message File
	ncclean.exe 		NC Cleanup Utility
	ncclean.ini 		NC Cleanup Utility INI File
	ncedit.exe  		NC Internal editor
	ncedit.msg  		NC Internal editor Message File
	ncff.exe    		NC File Finder
	ncff.msg    		NC File Finder Message File
	ncmain.exe  		NC Main Shell
	nczip.exe   		NC Compression Engine
	nczip.msg   		NC Compression Engine Message File
	packer.set  		NC Compression Configuration File
	readme.txt  		Readme File
        whats.new               List of what's new in Commander 5.5

Diskette 2 : Program Diskette 2

	123view.ex! 		Spreadsheet Viewer
	4372ansi.set		Conversion Table CP437->ANSI
	8502ansi.set		Conversion Table CP850->ANSI
	8632ansi.set		Conversion Table CP832->ANSI
	8652ansi.set		Conversion Table CP852->ANSI
	8662ansi.set		Conversion Table CP866->ANSI
	ansi2437.set		Conversion Table
	ansi2850.set		Conversion Table
	ansi2863.set		Conversion Table
	ansi2865.set		Conversion Table
	ansi2866.set		Conversion Table
	arcview.ex! 		Compress File Viewer
	bitmap.ex!  		Bitmap Viewer
	bug.ns!     		Screen Saver
	bungee.ns!  		Screen Saver
	clp2dib.ex! 		Viewer Conversion File
	dbview.ex!  		Database Viewer
	draw2wmf.ex!		Viewer Conversion File
	drw2wmf.ex! 		Viewer Conversion File
	evileye.ns! 		Screen Saver
	faces.ns!   		Screen Saver
	flip.ns!    		Screen Saver
	ico2dib.ex! 		Viewer Conversion File
	message.ms! 		General Message File
	mouse.ns!   		Screen Saver
	msp2dib.ex! 		Viewer Conversion File
        nc.ic!                  Icon File for NC 5.5
	norton.in!  		INI File for NCSF, NCDD
	packer.ex!  		Compression Converter Engine
	pct2dib.ex! 		Viewer Conversion File
	q&aview.ex! 		Q&A Database Viewer
	refview.ex! 		Reflex Viewer
	saver.ex!   		Screen Save Utility
	term95.gt!  		Terminal Emulation File
	term95.hl!  		Terminal Emulation Help File
	term95.in!  		Terminal Emulation INI File
	term95.re!  		Terminal Emulation Message File
	tif2dib.ex! 		Viewer Conversion File
	tornado.ns! 		Screen Saver
	vector.ex!  		Viewer Conversion File
	wpb2dib.ex! 		Viewer Conversion File
	wpv2wmf.ex! 		Viewer Conversion File
	wpview.ex!  		Wordprocessor Viewer

Diskette 3 : Program Diskette 3

	dir2dir.ex! 		Telephone Book Conversion Utility
	fish.ns!    		Screen Saver
	ncdd.ex!    		NC Disk Copier
	ncff.hl!    		NC File Find Help File
	nclabel.ex! 		NC Label Utility
	ncnet.ex!   		NC Network Utility
	ncpscrip.hd!		NC Postscript Header File
	ncsf.ex!    		NC Diskette Format
	ncsi.ex!    		NC System Information
	nc_exit.co! 		NC_EXIT Utility
	nc_exit.do! 		NC_EXIT Documentation
	paraview.ex!		Paradox Viewer
	playwave.ex!		WAV Sound File
	rbview.ex!  		Database Viewer
	term95.ex!  		Terminal Emulation Program
	term95.sc!  		Term95 Scripts




Memory Requirements
--------------------
	<> Memory requirements for Commander.

	Norton Commander needs at least 512K conventional memory in order to
	run all its external viewers. Some of the viewers need additional
	memory to run in quick view mode.

	<> Launching Commander with Small and Tiny memory configurations

	In some low memory situations, you will not be able to quickview
	large files using the Commander built-in viewers. In order to help
        you get around this problem, Norton Commander 5.5 comes with two
	additional memory configurations that can be selected from the
	command line. Each memory configuration limits the number of files
	that can be stored in each Commander File Panel. The command line
	syntax of these memory configurations are :-

	Command Line		Description
	-------------------	--------------------------------------------
	nc /s			Launch Commander with SMALL memory
				configuration. Number of files per panel
				is 700.

	nc /t			Launch Commander with TINY memory
				configuration. 	Number of files per panel
				is 300.

Compressed File Support
-----------------------
	<> Compressing Hidden Files

	If a hidden file is compressed using Norton Commander Compression,
	it will loose its hidden file attribute. This is to keep the
	Commander Compression File format compatible with PKZIPs. PKZIP at
	this time does not support hidden compressed files.

	<> Password protected Compressed files

	While the Norton Commander Compression Engine is compatible with
	PKZIP 1.x and PKZIP 2.x compress file formats, Commander only
	allows a single password for each compressed file to be active.

	<> Limitations in Compressed Files

	If the current panel contains an open compressed file, then it is
	not possible to change the drive of that panel. This compressed file
	should be closed first and then the drive selected in the normal
	manner.

	Due to the wide range of different compression utilities on the
	market today and the major differences in behaviour, you may find
	some features of Norton Commander impossible to implement using your
	compression utility. This is especially true of some compression
	utilities that do not support the decompressing of files into a
	destination location other than the current directory. In addition
	to this not all compression utilities support the storing of full
	pathnames and directories.

	Consult the instructions at the top of the Packer.Set file for ways
	to program Commander's compressed file support.

Viewer Support
--------------
	<> Internal Viewer

	To launch the internal viewer contained within NCMAIN.EXE,
	type ALT+F3.

	<> Defining Viewers

        All viewers that are supported by Norton Commander v5.5 are defined
	in the file NC.CFG. You can modify the list of files that are
	associated with viewers. For example, if your word processor
	documents have a special file extension such as .DCC, then you can
	change the list of files that the wpview.exe file handles by simply
	adding this file specification to the list of files that are defined
	underneath WPVIEW.EXE. For example, the WPVIEW section of NC.CFG
	would look similar to :-

				~wpview.exe
				$*.exe,p
				$*.com,p
				$*.dll,p
				$*.dcc,p     <----- Line added here to support
				$*.*		    *.dcc files
Filters
-------
The definitions for the Panel Filters used in Norton Commander Version 4.0
were stored in a file called NC.CFG. In the new Commander, Panel Filters
(and indeed File Operation Filters) are stored in a binary file called
NC.FIL. You will need to convert your existing Commander 4.0 filters to
this new filter file by creating the filters from within Commander 5.5.
Refer to your User's Guide for instructions on how to define Filters.

Commander File Transfer
-----------------------
	<> Cloning using SHARE.EXE

	A problem exists when you are trying to clone to a COM port on a
	machine that has SHARE.EXE installed. A conflict exists between the
	Clone software opening the device COMX and SHARE granting exclusive
	rights to this port to the Clone Bootstrap loader. This results in
	sharing violation errors occurring during a cloning session. To get
	around this problem use the CTTY AUX command instead of CTTY COM1:.

	<> Pin layout of the cables.

	You can order a special Commander Cable to link any two computers
	together. A special order form is included in the Norton
        Commander 5.5 box. Please refer to it for further details on this
	offer.

	<> Pin layout of the serial cables.

	    9 - 9 Serial           9 - 25 Serial         25 - 25 Serial

	    2 ======= 3             2 ======= 2	         2 ======== 3
	    3 ======= 2             3 ======= 3	         3 ======== 2
	    4 ======= 6             4 ======= 6	         4 ======== 5
	    5 ======= 5             5 ======= 7	         5 ======== 4
	    6 ======= 4             6 ======= 20         6 ======== 20
	    7 ======= 8             7 ======= 5	         7 ======== 7
	    8 ======= 7             8 ======= 4	        20 ======== 6

	<> Pin layout of the parallel cable.

			            1 ======= 7
				    2 ======= 15
				    3 ======= 13
				    4 ======= 12
				    5 ======= 10
				    6 ======= 11
				    7 ======= 1
				    8 ======= 14
				    9 ======= 16
				   10 ======= 5
				   11 ======= 6
				   12 ======= 4
				   13 ======= 3
				   14 ======= 8
				   15 ======= 2
				   16 ======= 9
				   17 ======= 17
				   18 ======= 18
				   19 ======= 19
				   20 ======= 20
				   21 ======= 21
				   22 ======= 22
				   23 ======= 23
				   24 ======= 24
				   25 ======= 25

Special Characters in the Commander Editor
-------------------------------------------
To enter special European characters, it is sometimes necessary to use the
CTRL+Q key first followed by the character you wish to enter. For example, to
enter the German paragraph character, type CTRL+Q followed by SHIFT+3.

Norton Commander Environment Variables
---------------------------------------
Norton Commander searches your environment for the following three
environment variables in order to determine where to locate and store
temporary work files :- TEMP, TMP, NC.

Accented characters in file and directory names
-----------------------------------------------

On a Novell network, using NDOS, it is possible to create different files
or directories using upper case or lower case accented characters.
For example :

MD „Ž
MD Ž„

creates two new directories. For this reason, Commander does not convert
accented characters in file names or directories to upper or lower case.


Installing commander under Windows 3.1
--------------------------------------

Go to the section headed "Installing Commander under Windows" of
the user manual. Perform steps 1 to 7.

Select the Change Icon button.

Type in the full path name of the NC.ICO file, e.g. C:\NC\NC.ICO, or use
the browse button if you are not sure of the path.

Select the OK button.

The Norton Commander is now installed on your desktop.

Datex.p File
------------
The file DATEX.P contains a list of telephone numbers that can be used
when running the script file CMPSRV.SCX. This file is used by the Terminal
Emulation Program. The format of this file is a follows :-

Position      1	        2	  3	    4	      5		6
1             5	        5	  5	    5	      5		5
<City Name   ><Prefix  ><300     ><1200/75 ><1200    ><2400    ><9600    >

The telephone numbers that are stored in this file correspond to the numbers
that are used to connect to the CompuServe Bulletin Board System. The numbers
specified after position 25 are the different numbers to be used depending on
the baud rate that you wish to connect at. You can add in you own telephone
numbers easily by modifying this file using the Commander Editor.

Windows 95 and OS/2 Compatibility
---------------------------------

Under Windows '95 and OS/2 2.x and 3.x you may experience difficulty in
running some of the Disk Utilities. This is especially true of Disk Format
and System Information.

In addition to this, copying files with long filename on a Windows '95 machine
using Commander will result in the long filename associated with the file
being lost. This is also true for Long Filename under OS/2.

Copy Diskette Utility
---------------------

The Option to Skip Bad Sectors is never saved in the Commander INI file. It
defaults to OFF automatically when the utility is launched. All other options
in Copy Diskette will be saved.

Playwave Utility
----------------

Norton Commander 5.5 comes with a viewer for playing WAV files. Since this
viewer does not rely on additional sound hardware on your machine, the
quality of play back may be poor on some machines.

Telephone Book Conversion Utility (DIR2DIR)
-------------------------------------------
The DIR2DIR.EXE utility can be used to convert your phonebook files from
NC 4.0 to the new format in NC 5.5. Type DIR2DIR at the command prompt
for more details on this utility.

NC_EXIT Utility
---------------
This useful utility unloads the Norton Commander kernel without the use of
a TSR program. It can also be used to optionally stuff up to an 11 character
command for processing after. See the NC_EXIT.DOC file for more information
on this utility.

Novell NDS
----------
The Norton Network Utilities will operate on Netware 3.x and 4.x. The login
component will have problems attaching to Netware 4.x servers running Novell's
Directory Services(NDS).

Useful advice when running Commander on a 1MB or less system
-------------------------------------------------------------
The memory management system that Commander uses to run its external utilities
is to swap the Kernel (NCMAIN) into extended or expanded memory. If your system
has less then 1 megabyte of memory, Commander will swap NCMAIN to a disk file.
This Commander SWAP file is stored in a temporary directory which is located by
searching for the following environment variables TEMP, TMP, NC, NORTON.

If you are running Commander from a floppy disk on a 1 Megabyte system, to
ensure maximum performance, you should set one of these variables to a
temporary directory on your hard disk, otherwise Commander will attempt to
create its SWAP file on your floppy diskette.

Running Norton Commander from a shared Network Drive
----------------------------------------------------
Norton Commander can be run from a shared network drive as follows :-

1. Installation on shared network drive.
	- Log into your server as the Supervisor.
	- Create a directory called NC5NET.
	- Install the Norton Commander software into this account using
	  the standard installation diskettes.
	- Create an User account called NC5NETUSER.
	- Ensure that this account has only Read/File Scan rights to the
	  NC5NET directory.

2. Installation of NC 5.5 on User machine.
	- Copy only the INI files from the NC5NET account to the NC directory
	  on the users machine.
	- Modify the autoexec.bat file and set the following environment
	  variables :-
		SET NC=c:\NC5
		SET NORTON=c:\NC5
	- Login into the network as NC5NETUSER and ensure that the NC5NET
	  directory is in the user's DOS PATH.
	- Type NC at the command prompt.

Creating a tools menu
---------------------
It is possible to add a user defined Tools Menu to the Norton Commander's
main menu. To define a Tools Menu create a file called TOOLS.MNU and store
it in the same directory as the NC.INI file.

Each line within the TOOLS.MNU file corresponds to a single menu item. To
define a Menu item use the following format :-

	Menu_Item_Name;Command_to_be invoked_when_selected

For example a simple Tools Menu may look like :-

	File Date;fd.exe
	Norton Control Centre;ncc.exe
	Norton Disk Doctor;ndd.exe
	Norton Disk Editor;de.exe
	Text Search;ts.exe

The number of menu items that can be defined in the TOOLS.MNU file is
limited only by the amount of free memory.

Using screen savers under Windows 3.1
-------------------------------------
When Commander is running in a DOS box under MS-Windows, its graphical screen
savers will be disabled even if one is selected from the configuration panel.
In circumstances like this, Commander's default screen saver will be
invoked instead.





