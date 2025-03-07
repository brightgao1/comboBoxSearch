## comboBoxSearch
A header library to easily give a search suggestions system to Win32 comboBoxes.
### Tutorial
I created this library to be super easy to use.

1. Put the comboBoxSearch.h file in the same directory as your cpp file, do #include "comboBoxSearch.h"
2. Create a Win32 comboBox control w/ the style CBS_DROPDOWN (DO NOT use CBS_DROPDOWNLIST as obviously the text box is static w/ that style).
3. Add all the strings to the comboBox. *** YOU MUST DO THIS BEFORE THE NEXT STEP, OR IT WON'T WORK.  **ALL** THE STRINGS NEED TO BE ADDED IN THIS STEP.
4. Declare an instance of the comboBoxSearch struct, and call makeSearchable(HWND cbTemp, bool allLowerCaseStrs [optional parameter, default is false] ) passing the handle of your comboBox that you want to associate w/ the instance of the comboBoxSearch struct, and true to allLowerCaseStrs if there is not a single uppercase letter in your dropdown.

That's it, now your comboBox is like a hybrid between Google search and a dropdown! No need to handle any notifications to its parent, b/c I subclassed the parent's window procedure. 
### Example
This is a simple example based on the tutorial above:
```
HWND cb = CreateWindowW(L"ComboBox", L"", WS_CHILD | WS_VSCROLL | CBS_DROPDOWN, 20, 20, 200, 400, mainWindow, NULL, NULL, NULL);
std::vector<std::wstring> strings = {L"test", L"test test", L"testing", L"tessst", L"tested", L"tee", L"testt"};
for (int i = 0; i < strings.size(); i++) SendMessageW(cb, CB_ADDSTRING, 0, (LPARAM)strings[i].c_str());
comboBoxSearch cbSearch; cbSearch.makeSearchable(cb);
ShowWindow(cb, SW_SHOW), UpdateWindow(cb);
```
#### Result:

![bandicam2025-03-0711-06-30-041online-video-cutter com-ezgif com-video-to-gif-converter (1)](https://github.com/user-attachments/assets/54b9ac31-2401-4539-9bb6-0b85f8f30708)

### FAQ
Q: Why isn't there much customization?
A: Ease-of-use was the first priority when I created this library. If I add more parameters and things that the library user has to learn, it would reduce the chance of ppl using it. If you want to modify the code to achieve your specific needs, feel free to do so.

Q: Why did you create this? Win32 is dead.
A: It is still the most performant. Also, the whole reason I created this was b/c in developing my app BrightType, I wanted users to be able to search fonts in a combobox. I looked on stackoverflow, and ppl also were trying to figure this out. You can use IAutoComplete, but there exist drawbacks (Windows 10 required, very manual, etc.) so I learned about trie and implemented it into BrightType. So it isn't like I wasted much time in this, as I implemented it into my app before creating this. All I needed to do was make some minor adjustments and testing. 

