## comboBoxSearch
A header library to easily give search suggestions to Win32 comboBoxes.
### Tutorial
I created this library to be super easy to use.

1. Create a Win32 comboBox control w/ the style CBS_DROPDOWN (DO NOT use CBS_DROPDOWNLIST as obviously the text box is static w/ that style).
2. Add all the strings to the comboBox. *** YOU MUST DO THIS BEFORE THE NEXT STEP, OR IT WON'T WORK.  ** ALL ** THE STRINGS NEED TO BE ADDED IN THIS STEP.
3. Declare an instance of the comboBoxSearch struct, and call makeSearchable(HWND cbTemp, bool allLowerCaseStrs [optional parameter, default is false] ) passing the handle of your comboBox that you want to associate w/ the instance of the comboBoxSearch struct, and true to allLowerCaseStrs if there is not a single uppercase letter in your dropdown.

That's it, now your comboBox is like a hybrid between Google search and a dropdown! No need to handle any notifications to its parent, b/c I subclassed the procedure. 

