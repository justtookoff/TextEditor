//EditApp.cpp
#include "EditApp.h"
#include "Edit.h"

BOOL EditApp::InitInstance() {
	Edit *edit = new Edit;
	edit->Create(NULL, "Text");
	edit->ShowWindow(this->m_nCmdShow);
	edit->UpdateWindow();
	this->m_pMainWnd = edit;

	return TRUE;
}

EditApp editApp;