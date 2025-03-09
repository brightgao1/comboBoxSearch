// Created by Bright Gao. Feel free to contact me for any reason: brightgao1234@gmail.com
// You may modify the code and use this library all you want :D
#pragma once
#include <windows.h>
#include <vector>
#include <array>
#include <map>
#include <string>
std::map<HWND, std::vector<std::wstring>> COMBOBOXSEARCHFullStrList_; std::map<HWND, bool> COMBOBOXSEARCHIsLowerOnly_;
// You Should NOT need to call any function from preTree or preTreeLowerOnly
// ONLY create the comboBoxSearch struct and call makeSearchable, which will create the trie and call the functions of the trie when the edit of the cb changes
// (The point of the library is to make it as EZ as possible for u to create a very standard prefix-search-suggestion combo box, w/o needing to implement a trie or handle the CB_EDITCHANGE notifs to its parent... ofc u may modify the code if u want more control/customization)
struct preTree {
    std::vector<std::array<int, 53>> trie; std::vector<std::array<bool, 53>> end;
    std::map<std::array<int, 2>, std::wstring> strAtRC; int nxt = 0; HWND cb;
    inline void init(int lenSum, HWND cbTemp) {
        wchar_t* buf = new wchar_t[10]; GetClassNameW(cbTemp, buf, 10);
        std::wstring wName = buf; delete[] buf;
        for (short i = 0; i < wName.length(); i++) wName[i] = std::tolower(wName[i]);
        if (wName != L"combobox") MessageBox(GetParent(cbTemp), L"Initialization failed:\nstruct preTree, function init: the HWND you passed to init is not a combobox", L"Error: comboBoxSearch.h", MB_OK);
        else {
            std::array<int, 53> tempi; std::array<bool, 53> tempb;
            for (short i = 0; i < 53; i++) tempi[i] = 0, tempb[i] = false;
            trie.assign(lenSum + 5, tempi), end.assign(lenSum + 5, tempb), cb = cbTemp;
        }
    }
    inline short getInd(short ascii) {
        if (ascii >= 97 && ascii <= 122) return ascii - 97;
        else if (ascii >= 65 && ascii <= 90) return ascii - 39;
        else return 52;
    }
    inline void dfs(int curr) {
        for (short i = 0; i < 53; i++) {
            if (end[curr][i]) SendMessageW(cb, CB_ADDSTRING, 0, (LPARAM)strAtRC[{curr, i}].c_str());
            if (trie[curr][i]) dfs(trie[curr][i]);
        }
    }
    inline void insert(std::wstring s) {
        int curr = 0;
        for (int i = 0; i < s.length(); i++) {
            short ind = getInd((short)s[i]);
            if (!trie[curr][ind]) nxt++, trie[curr][ind] = nxt;
            curr = trie[curr][ind];
        }
        end[curr][getInd(s.back())] = true, strAtRC[{curr, getInd(s.back())}] = s;
    }
    inline int search(std::wstring s) {
        int curr = 0;
        for (int i = 0; i < s.length(); i++) {
            short ind = getInd((short)s[i]);
            if (!trie[curr][ind]) return -1;
            curr = trie[curr][ind];
        }
        return curr;
    }
};
struct preTreeLowerOnly {// spaces are allowed too, ind 0-25 (inclusive) is a-z, 26 is space
    std::vector<std::array<int, 27>> trie; std::vector<std::array<bool, 27>> end;
    std::map<std::array<int, 2>, std::wstring> strAtRC; int nxt = 0; HWND cb;
    inline void init(int lenSum, HWND cbTemp) {
        wchar_t* buf = new wchar_t[10]; GetClassNameW(cbTemp, buf, 10);
        std::wstring wName = buf; delete[] buf;
        for (short i = 0; i < wName.length(); i++) wName[i] = std::tolower(wName[i]);
        if (wName != L"combobox") MessageBox(GetParent(cbTemp), L"Initialization failed:\nstruct preTreeLowerOnly, function init: the HWND you passed to init is not a combobox", L"Error: comboBoxSearch.h", MB_OK);
        else {
            std::array<int, 27> tempi; std::array<bool, 27> tempb;
            for (short i = 0; i < 27; i++) tempi[i] = 0, tempb[i] = false;
            trie.assign(lenSum + 5, tempi), end.assign(lenSum + 5, tempb), cb = cbTemp;
        }
    }
    inline void dfs(int curr) {
        for (short i = 0; i < 27; i++) {
            if (end[curr][i]) SendMessageW(cb, CB_ADDSTRING, 0, (LPARAM)strAtRC[{curr, i}].c_str());
            if (trie[curr][i]) dfs(trie[curr][i]);
        }
    }
    inline void insert(std::wstring s) {
        int curr = 0;
        for (int i = 0; i < s.length(); i++) s[i] = std::tolower(s[i]);
        for (int i = 0; i < s.length(); i++) {
            short ascii = s[i] - 'a';
            if (s[i] == ' ') ascii = 26;
            if (!trie[curr][ascii]) nxt++, trie[curr][ascii] = nxt;
            curr = trie[curr][ascii];
        }
        end[curr][s.back() - 'a'] = true, strAtRC[{curr, s.back() - 'a'}] = s;
    }
    inline int search(std::wstring s) {
        int curr = 0;
        for (int i = 0; i < s.length(); i++) {
            short ascii = s[i] - 'a';
            if (s[i] == ' ') ascii = 26;
            if (!trie[curr][ascii]) return -1;
            curr = trie[curr][ascii];
        }
        return curr;
    }
};
std::map<HWND, preTreeLowerOnly> COMBOBOXSEARCHPreTreeLowerOnly_; std::map<HWND, preTree> COMBOBOXSEARCHPreTree_;
inline LRESULT CALLBACK comboBoxSearchParentSubclass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_COMMAND) {
        HWND cb = (HWND)lParam;
        if (HIWORD(wParam) == CBN_EDITCHANGE && !GetWindowTextLengthW(cb)) {
            SendMessageW(cb, CB_RESETCONTENT, 0, 0), SetFocus(cb);
            for (int i = 0; i < COMBOBOXSEARCHFullStrList_[cb].size(); i++) SendMessageW(cb, CB_ADDSTRING, 0, (LPARAM)COMBOBOXSEARCHFullStrList_[cb][i].c_str());
            SendMessageW(cb, CB_SHOWDROPDOWN, TRUE, 0);
        }
        else if (HIWORD(wParam) == CBN_EDITCHANGE && GetWindowTextLengthW(cb)) {
            wchar_t* buf = new wchar_t[GetWindowTextLengthW(cb) + 1];
            GetWindowTextW(cb, buf, GetWindowTextLengthW(cb) + 1); std::wstring userIn = buf; delete[] buf;
            SendMessageW(cb, CB_RESETCONTENT, 0, 0), SetWindowTextW(cb, userIn.c_str()), SendMessageW(cb, CB_SETEDITSEL, 0, MAKELPARAM(userIn.length(), userIn.length()));
            if (COMBOBOXSEARCHIsLowerOnly_[cb]) {
                preTreeLowerOnly ptlo = COMBOBOXSEARCHPreTreeLowerOnly_[cb]; int afterPre = ptlo.search(userIn);
                if (afterPre != -1) ptlo.dfs(afterPre);
                SendMessageW(cb, CB_SHOWDROPDOWN, TRUE, 0);
            }
            else {
                preTree pt = COMBOBOXSEARCHPreTree_[cb]; int afterPre = pt.search(userIn);
                if (afterPre != -1) pt.dfs(afterPre);
                SendMessageW(cb, CB_SHOWDROPDOWN, TRUE, 0);
            }
        }
        else return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    else return DefWindowProcW(hwnd, message, wParam, lParam);
}
struct comboBoxSearch {
    preTree pt; preTreeLowerOnly ptlo; bool lo; HWND cb;
    inline void makeSearchable(HWND cbTemp, bool allLowerCaseStrs = false) {// *** ONLY CALL THIS WHEN YOUR COMBOBOX IS ALREADY FILLED W/ ALL THE STRINGS U WANT TO BE SEARCHABLE, OR ELSE THE PRETREE (TRIE) WON'T HAVE ALL OF THEM AND SEARCHING WON'T WORK
        lo = allLowerCaseStrs, cb = cbTemp; int lenSum = 0, entryCnt = SendMessageW(cb, CB_GETCOUNT, 0, 0);
        for (int i = 0; i < entryCnt; i++) lenSum += SendMessageW(cb, CB_GETLBTEXTLEN, i, 0);
        if (lo) {
            ptlo.init(lenSum, cbTemp);
            for (int i = 0; i < entryCnt; i++) {
                wchar_t* buf = new wchar_t[SendMessageW(cb, CB_GETLBTEXTLEN, i, 0) + 1];
                SendMessageW(cb, CB_GETLBTEXT, i, (LPARAM)buf); std::wstring ws = buf; delete[] buf;
                COMBOBOXSEARCHFullStrList_[cb].push_back(ws), ptlo.insert(ws);
            }
            COMBOBOXSEARCHPreTreeLowerOnly_[cb] = ptlo;
        }
        else {
            pt.init(lenSum, cb);
            for (int i = 0; i < entryCnt; i++) {
                wchar_t* buf = new wchar_t[SendMessageW(cb, CB_GETLBTEXTLEN, i, 0) + 1];
                SendMessageW(cb, CB_GETLBTEXT, i, (LPARAM)buf); std::wstring ws = buf; delete[] buf;
                COMBOBOXSEARCHFullStrList_[cb].push_back(ws), pt.insert(ws);
            }
            COMBOBOXSEARCHPreTree_[cb] = pt;
        }
        COMBOBOXSEARCHIsLowerOnly_[cb] = lo, SetWindowLongPtrW(GetParent(cbTemp), GWLP_WNDPROC, (LONG_PTR)comboBoxSearchParentSubclass);
    }
};
