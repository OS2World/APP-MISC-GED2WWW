***************GED2WWW READ_ME.TXT**************
GED2WWW is a freeware program designed to convert GEDCOM (genealogical data)
files to HTML with an emphasis on minimizing the amount of HTML produced
while not sacrificing vital information.

For more information about GED2WWW; check out the GED2WWW web page
at http://www.lesandchris.com/ged2www/

******************DISTRIBUTION******************
The binary distribution contains 5 files:

read_me.txt     - this file
ged2www.exe     - the GED2WWW executable
history.txt     - history of GED2WWW
COPYING         - the GNU Public Copyright that covers GED2WWW

******************USAGE NOTES*******************
The binary distribution of GED2WWW is a 32bit Windows 
console progrem.
It will run under Windows 3.1, 95 or NT.

The source code distribution of GED2WWW is available and should 
compile fairly easily on any machine with an ANSI C compiler.

GED2WWW does not have a fancy user interface (what do you expect for 
free).  The user interface is utilitarian at best (I plan on improving 
it some in future releases).  Since you should not need to run GED2WWW a 
lot this should not be a problem.  

When you run GED2WWW you are asked a series of questions.  A default value
is listed for most questions, and you can accept the default value by
hitting <ENTER>.  In almost all cases accepting the default values  
will work just fine.  The first group 
of question (in the Technical Database Information section) ask you 
technical questions about your database.  This information is used
in determining how the HTML pages will be formatted The next series of 
questions (database owner information section) asks questions about the  
database owner.  Information provided in these questions will be used to
provide contact information on the base page of the database (index.htm).

After answering all the questions GED2WWW will go to work.  It will give 
you status updates as it is processing.  On my 33MHz486 it takes less 
than 70 seconds to process a 6500 person GEDCOM file.  The output HTML 
files are placed in the HTML directory under the directory in which 
GED2WWW.EXE is located.  These files can be view with Netscape (or your 
favorite web browser) before uploading to your Web Host.  I can not go 
into the specifics of how to get your files to your Web Host (or ISP) 
because it varies too much from one service to another.


Note to Windows 95 users:

Windows 95 converts the first letter of filenames to upper case 
(in order to make them easier to read).  This option can not be turned
off with any of Windows 95's setup options.  This will not cause a problem
when viewing files locally (Windows 95 is smart enough to compensate for
itself).  But because of this "Malicious Capatilization", you should make 
sure that the files are transmitted to your Web Host with LOWER CASE FILE 
NAMES.  If you do not, the links between files may not work properly.

*****************RELEASE NOTES******************
see history.txt file

*****************SPECIAL THANKS*****************  
Special thanks to Dean Adams for the year extraction code used
in determining if individuals are alive or not for purposes
of hiding dates on living individuals.

************COPYRIGHT and MISC LEGAL************
GED2WWW - GEDCOM to HTML translator.
Copyright (C) 1996-1999 Leslie Howard

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program in the COPYING file; if not, write to the 
Free Software Foundation, Inc., 675 Mass Ave, Cambridge, 
MA 02139, USA.
