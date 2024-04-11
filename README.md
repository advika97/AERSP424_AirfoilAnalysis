# Airfoil Analysis

## Project description
Create any software program in C++ language and use as many as C++ features learnt in-class.

• The project size, in terms of functionality and amount of code, must be appropriate for the size of the team. Max team size is 4.

• It MUST compile and run without any significant problems.

• 3rd-party software/library is allowed, but it cannot do the majority of the work for your program.


## Project Proposal
Airfoil analysis for as many types of low speed 2D airfoils as possible by reverse engineering from existing test data and varying the input parameters like approach flow velocity, angle of attack, chord length, eccentricity, Reynolds number, etc. If possible, plot lift vs. drag ratios and pressure coefficient distribution, and streamlines around the foil. 
The project may satisfy most of the grading rubric:

•	Compilable - should build without errors and warnings on Windows, Linux and Mac.

•	Runtime errors - reduce/eliminate any runtime errors with unit testing and automation if time permits.

•	Readability - code comments, readme, and documentation

•	Math / Technical Knowledge from other areas - Airfoil analysis methods and math for calculations

•	Independency - cross platform, using CMake and any cross-platform GUI frameworks.

•	Code Organization - code headers and cpp files in respective directories, images and test data in separate directory, GUI code in different directory, etc.

•	Robustness - unit tested to handle corner and edge cases.

•	Resource Usages - no memory leaks, acceptable memory, and CPU usage footprint (use performance tools to monitor if time permits)

•	Write, compile, and include your own libraries - The analysis math code can be separated out as a library.

•	All Primitive Data Types - for variables, function parameters etc.

•	Global and Static variables - trying to avoid global and static variables, but can include if required.

•	Bitwise Operators - Not sure if bitwise operators will be used (can try to use for setting a flag by bits instead of boolean variables)

•	Conditional AND Iterative Statements – yes

•	Functions OR Recursions – yes

•	Preprocessor – yes

•	Pointers – yes

•	References – yes

•	Function Overloading – yes

•	Function Pointers OR Lambda Functions – yes

•	Template Functions OR Template Class – yes

•	Containers - yes (vectors, mostly, but can design data structures as required)

•	Struct / Class – yes

•	Objects – yes

•	Operator Overloading – yes

•	Inheritance – yes

•	Virtual Function / Overriding - yes

•	GUI - yes (cross platform, using wxWidgets or QT or terminal escape sequences)

•	File System Handling - yes, reading/writing test data files

•	Regular Expression - can be used for input validation, or any parsing task if required

•	Concurrent Programming - can try to run long calculations in parallel by splitting the workload, or run different methods of calculations concurrently for comparison


## Build Instructions
1. Update Visual Studio 2022:
	a.	C++ Development -> select CLang tools checkbox (let other selections remain as they are)

2.	Upgrade Visual Studio 2022 to latest updates (it may take time).

3.	Reboot laptop.

4.	Now, to setup wxWidgets:

	a.	Download the following from https://www.wxwidgets.org/downloads/

		i.	Latest Stable Release 3.2.4: Source Code -> Windows ZIP

		ii.	Extract this zip file into c:\sdks\wx324 (so that all files are inside the wx324 directory (not wxWidgets-3.2.4 kind)

	b.	Open the folder c:\sdks\wx324 in file explorer, and navigate to C:\sdks\wx324\build\msw directory

	c.	Double click on wx_vc17.sln file, which will open the project in Visual Studio (might take some time)

	d.	After it is open, click on Build menu, select Batch Build.

		i.	Here, click on the Select All button

		ii.	Then click on Build or Build All

	e.	This might take time.

	f.	If the build fails, repeate #d above. Do not click on Rebuild.

	g.	Usually, build is successful, but be patient. Do not anything else on the laptop when the build is running.

	h.	Close Visual Studio. Reboot laptop.


6.	Now setup a System Environment Variable named WXWIN with value c:\sdks\wx324 (this is the path for the wxWidgets binaries).

7.	To test if wxWidgets is setup, first you can build and run a minimal sample code.

	a. visit: https://docs.wxwidgets.org/latest/page_samples.html#page_samples_minimal

	b. download minimal sample code from -> https://github.com/wxWidgets/wxWidgets/tree/master/samples/minimal (download CMakeLists.txt and minimal.cpp only)

	c. copy these two files to a c:\test\wxMinimal directory

	d. In CMakeLists.txt, make the following changes:

		i. Change the find_package line to the following:

			find_package(wxWidgets REQUIRED core base gl OPTIONAL_COMPONENTS net propgrid aui adv)
  	
			include(${wxWidgets_USE_FILE})
  	
		ii. Change the target_link_libraries line to the following:

			target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
  	
		iii. Remove the if-endif block statements before add_executable and after target_link_libraries.

	
9.	Open Visual Studio -> and open folder c:\test\wxMinimal

	a.	this will automatically invoke cmake and create the intermediate files and out directory

	b.	If there are cmake errors (typically related to find_package(wxWidgets 3.3...), then close Visual Studio.

	c.	Wait for 5 minutes.

	d.	Open Visual Studio again, and open recent folder.

	e.	From Project menu, click "Delete Cache and Reconfigure" menu

	f.	This will run cmake again. Hopefully, this time there should not be any cmake errors.

	g.	Now click on Build All from Build menu. If you don't see Build All menu item, then "Select Startup Item" first (next to Configuration drop down)

	i. 	If there are any build errors, it means wxWidgets is not setup properly.


11.	Download wxFormBuilder (wxFormBuilder-4.1.0-x64.exe) from : https://github.com/wxFormBuilder/wxFormBuilder/releases

	a.	this is required to design the GUI form.

13. Clone this repository. Open folder in Visual Studio, CMake will run automatically. Build project and you are set!

### Let me know if you run into any problems.
