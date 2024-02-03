# terr
Simple windows MessageBox generator/explorer

Because a friend (hello Henter) complained that all those he could find on the net seemed slightly shady.
Written in win32 C.

Buttons = are all buttons combinations accepted by MessageBox. Help is special (it can be added to existing ones) so it has its own checkbox

Icon is a bit misleading, represents type of MessageBox as well as informational icon. However, many "icons" have the same representation, while some might affect the sound being played when the MessageBox is displayed.

Having Pelles C installed the program can be either opened as a project and built from there or with bld.bat from a "Pelles C Command Prompt"

In addition, bld_4.bat allows building from NT4 with Pelles 4 installed. bld_4.bat calls povars32.bat from Pelles C default install directory.
