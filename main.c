/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic dialog based Win32 application.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

/* 
 * Either define WIN32_LEAN_AND_MEAN, or one or more of NOCRYPT,
 * NOSERVICE, NOMCX and NOIME, to decrease compile time (if you
 * don't need these defines -- see windows.h).
 */

#define WIN32_LEAN_AND_MEAN
/* #define NOCRYPT */
/* #define NOSERVICE */
/* #define NOMCX */
/* #define NOIME */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <wchar.h>
#include <stdlib.h>
#include "main.h"

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Prototypes **************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/

static HANDLE ghInstance;

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, WCHAR *pszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    WNDCLASSEX wcx;

    ghInstance = hInstance;

    /* Initialize common controls. Also needed for MANIFEST's */
    /*
     * TODO: set the ICC_???_CLASSES that you need.
     */
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES /*|ICC_COOL_CLASSES|ICC_DATE_CLASSES|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES|... */;
    InitCommonControlsEx(&icc);

    /* Load Rich Edit control support */
    /*
     * TODO: uncomment one of the lines below, if you are using a Rich Edit control.
     */
    // LoadLibrary(L"riched32.dll");  /* Rich Edit v1.0 */
    // LoadLibrary(L"riched20.dll");  /* Rich Edit v2.0, v3.0 */

    /*
     * TODO: uncomment line below, if you are using the Network Address control (Windows Vista+).
     */
    // InitNetworkAddressControl();

    /* Get system dialog information */
    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
        return 0;

    /* Add our own stuff */
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wcx.lpszClassName = L"terrClass";
    if (!RegisterClassEx(&wcx))
        return 0;

    /* The user interface is a modal dialog box */
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}

void doit(HWND hDlg)
{
	wchar_t *msg;
	wchar_t *title;

	UINT miarr[]={0,MB_ICONEXCLAMATION,MB_ICONWARNING,MB_ICONINFORMATION,MB_ICONASTERISK,MB_ICONQUESTION,MB_ICONSTOP,MB_ICONERROR,MB_ICONHAND};
	UINT mbarr[]={MB_ABORTRETRYIGNORE,MB_CANCELTRYCONTINUE,MB_OK,MB_OKCANCEL,MB_RETRYCANCEL,MB_YESNO,MB_YESNOCANCEL};

	UINT mbflags=0;

	int si,sb;

	UINT tl,ml;

	int tlen,mlen;

	tl=SendDlgItemMessage(hDlg,ED_TITLU,WM_GETTEXTLENGTH,0,0);
	ml=SendDlgItemMessage(hDlg,ED_MSG,WM_GETTEXTLENGTH,0,0);

	/* sanity check */
	if(tl>2048 || ml>4096) return;

	tlen=2*tl+4; mlen=2*ml+4;
	title=malloc(tlen);
	msg=malloc(mlen);

	/* alloc check */
	if(msg==NULL || title==NULL) return;

	/* get title and message */
	GetDlgItemText(hDlg,ED_TITLU,title,tlen);
	GetDlgItemText(hDlg,ED_MSG,msg,mlen);

	/* get buttons and icon */
	si=SendDlgItemMessage(hDlg,SL_ICON,LB_GETCURSEL,0,0);
	sb=SendDlgItemMessage(hDlg,SL_BTNS,LB_GETCURSEL,0,0);

	if(si>=0 && si<(int)NELEMS(miarr)) mbflags|=miarr[si];
	if(sb>=0 && sb<(int)NELEMS(mbarr)) mbflags|=mbarr[sb];

	/* help button is separate - get checkbox */
	if(SendDlgItemMessage(hDlg,CHK_HELP,BM_GETCHECK,0,0)==BST_CHECKED)
		mbflags|=MB_HELP;

	MessageBox(hDlg,msg,title,mbflags);

}

void init_lb(HWND hDlg)
{
	HWND lbh=GetDlgItem(hDlg,SL_ICON);
	ListBox_AddString(lbh,L" --- None ---");
	ListBox_AddString(lbh,L"Exclamation");
	ListBox_AddString(lbh,L"Warning");
	ListBox_AddString(lbh,L"Information");
	ListBox_AddString(lbh,L"Asterisk");
	ListBox_AddString(lbh,L"Question");
	ListBox_AddString(lbh,L"Stop");
	ListBox_AddString(lbh,L"Error");
	ListBox_AddString(lbh,L"Hand");

	/* select stop by default */
	ListBox_SetCurSel(lbh,5);

	lbh=GetDlgItem(hDlg,SL_BTNS);
	ListBox_AddString(lbh,L"ABORT RETRY IGNORE");
	ListBox_AddString(lbh,L"CANCEL RETRY CONTINUE");
//	ListBox_AddString(lbh,L"HELP");
	ListBox_AddString(lbh,L"OK");
	ListBox_AddString(lbh,L"OK CANCEL");
	ListBox_AddString(lbh,L"RETRY CANCEL");
	ListBox_AddString(lbh,L"YES NO");
	ListBox_AddString(lbh,L"YES NO CANCEL");

	/* selecct OK by default */
	ListBox_SetCurSel(lbh,3);
}

/****************************************************************************
 *                                                                          *
 * Function: MainDlgProc                                                    *
 *                                                                          *
 * Purpose : Process messages for the Main dialog.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
			init_lb(hwndDlg);
            return TRUE;

        case WM_SIZE:
            /*
             * TODO: Add code to process resizing, when needed.
             */
            return TRUE;

        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
                /*
                 * TODO: Add more control ID's, when needed.
                 */
                case IDOK:
					doit(hwndDlg);
                    return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        /*
         * TODO: Add more messages, when needed.
         */
    }

    return FALSE;
}
