#include "main.h"
#include "rc.h"
#include <filesystem>
#include <windows.h>

void main_window::on_paint(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Image image();
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
		case ID_OPEN: {
			TCHAR path[MAX_PATH];
			ZeroMemory(path, sizeof(path));

			TCHAR filter[] = _T("Image Files (*.jpeg; *.png; *.bmp; *.gif; *.tiff; *.emf)\0*.jpeg;*.png;*.bmp;*.gif;*.tiff;*.emf\0All Files (*.*)\0*.*\0\0");
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = path;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = filter;
			if (GetOpenFileName(&ofn))
			{
				::Gdiplus::Image::FromFile;
				InvalidateRect(*this, nullptr, true);
			}
			break;
		}
		case ID_EXIT: {
			DestroyWindow(*this);
			break;
		}
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
