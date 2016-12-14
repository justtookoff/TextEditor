/*
 * Name: Donghwee Kwon
 * Date: Dec.11.2016
 * File: EditApp.cpp
 *
 * This is cpp file for invoking window
 */


#include "EditApp.h"
#include "Edit.h"


/**
 * Name: EditApp
 * Purpose: This is to make Edit class, and
 *          create and show the window of this program.
 *
 */
BOOL EditApp::InitInstance() {
	Edit *edit = new Edit;
	edit->Create(NULL, "Text");
	edit->ShowWindow(this->m_nCmdShow);
	edit->UpdateWindow();
	this->m_pMainWnd = edit;

	return TRUE;
}

EditApp editApp;