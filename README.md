## comboBoxSearch
A header library to easily create a search suggestions system for Win32 comboBoxes.
### Tutorial
I created this library to be super easy to use.

1. Put the comboBoxSearch.h file in the same directory as your cpp file, do #include "comboBoxSearch.h"
2. Create a Win32 comboBox control w/ the style CBS_DROPDOWN (DO NOT use CBS_DROPDOWNLIST as obviously the text box is static w/ that style).
3. Add all the strings to the comboBox. *** YOU MUST DO THIS BEFORE THE NEXT STEP, OR IT WON'T WORK.  **ALL** THE STRINGS NEED TO BE ADDED IN THIS STEP.
4. Declare an instance of the comboBoxSearch struct, and call makeSearchable(HWND cbTemp, bool allLowerCaseStrs [optional parameter, default is false] ) passing the handle of your comboBox that you want to associate w/ the instance of the comboBoxSearch struct, and true to allLowerCaseStrs if there is not a single uppercase letter in your dropdown.

That's it, now your comboBox is like a hybrid between Google search and a dropdown! No need to call the trie functions or handle any notifications to its parent, b/c I subclassed the parent's window procedure.
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
*Q: Why isn't there much customization?*
A: Ease-of-use was the first priority when I created this library. If I add more parameters and things that the library user has to learn, it would reduce the chance of ppl using it. If you want to modify the code to achieve your specific needs, feel free to do so.

*Q: Why did you create this?*
A: The reason I created this was b/c in developing my app BrightType, I wanted users to be able to search fonts in a combobox. I looked on stackoverflow, and ppl also were trying to figure this out. You can use IAutoComplete, but there exist drawbacks (Windows 10 required, very manual, etc.) so I learned about trie and implemented it into BrightType. So it isn't like I spent much time in this, as I implemented it into my app before creating this. All I needed to do was make some minor adjustments and testing. 

*Q: Can you have multiple comboBoxes in one app that use this library?*
A: Yes, I tested it. The comboBox Parent window procedure that I subclassed is global, but it doesn't cause issues between handling notifications from multiple comboBoxes, as I call DefWindowProc for any messages not related to the CBN_EDITCHANGE message notification. I tried putting the handler inside the comboBoxSearch struct, but it would just cause errors and nothing would work.

### Extras/Optimization/Things I learned
I tested this for efficiency w/ a file containing 10,000 words.
At first, I would send the CB_DELETESTRING message for all the strings each time CBN_EDITCHANGE occurred. I thought Win32 comboBoxes had a std::vector-like O(1) pop_back operation, so I did:
```
for (int i = SendMessageW(cb, CB_GETCOUNT, 0, 0) - 1; i >= 0; i--) SendMessageW(cb, CB_DELETESTRING, i, 0);
```
I was SHOCKED that this took over a second to delete all the 10k strs in this manner, especially b/c sending CB_ADDSTRING was super fast and instant for the 10k strs, like a std::vector push_back. 
Then I thought that perhaps Win32 comboBoxes are implemented like a queue, where CB_DELETESTRING is only O(1) if deleting the first item in the comboBox... so I did:
```
while (SendMessageW(cb, CB_GETCOUNT, 0, 0)) SendMessageW(cb, CB_DELETESTRING, 0, 0);
```
But this was about the same, taking a whole second. So this puzzled me, as comboBoxes aren't implemented like a queue or vector. I ended up doing:
```
SendMessageW(cb, CB_RESETCONTENT, 0, 0);
```
And this was instant for 10k strs, only taking ~2 ms. It's probably a memset or something, so very fast.
So the culprit was CB_DELETESTRING, or moreso the implementation of the Win32 comboBox. CB_DELETESTRING requires index as WPARAM, while CB_ADDSTRING adds to the end, so this prob was a hint... but still.

Even if I had gotten the combobox info & text, destroyed the combobox, the created it w/ the info, then set the text... it would still be so much faster, like nearly instant. 
***tldr; Always do extensive testing, as things may not be as they seem***
