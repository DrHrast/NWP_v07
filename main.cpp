#include "main.h"
#include "rc.h"
#include <filesystem>
#include <string>
#include <windows.h>

using namespace Gdiplus;

tstring findImageName(std::wstring path) 
{
	//std::wstring imgp = path;
	//size_t pos = imgp.find_first_of(L'\\');
	//std::wstring temp = imgp.substr(pos + 1);
	tstring temp = std::filesystem::path(path).filename();
	return temp;
}

void drawStrings(HDC hdc, RECT rc, const TCHAR* imageName)
{
	Graphics graphics(hdc);
	int length = lstrlen(imageName);
	Font mainFont(L"Arial", 16);
	Font shadowFont(L"Arial", 16);
	RectF mainLayout(static_cast<float>(rc.left), static_cast<float>(rc.top), static_cast<float>(rc.right), static_cast<float>(rc.bottom - 10));
	//RectF shadowLayout(static_cast<float>(rc.left), static_cast<float>(rc.top), static_cast<float>(rc.right), static_cast<float>(rc.bottom - 10));
	StringFormat mainFormat;
	StringFormat shadowFormat;
	mainFormat.SetAlignment(StringAlignmentCenter); mainFormat.SetLineAlignment(StringAlignmentFar);
	shadowFormat.SetAlignment(StringAlignmentCenter); shadowFormat.SetLineAlignment(StringAlignmentFar);
	SolidBrush mainBrush(Color(255, 115, 115, 115));
	SolidBrush shadowBrush(Color(115, 255, 255, 255));

	graphics.DrawString
	(
		imageName,
		length,
		&shadowFont,
		mainLayout,
		&shadowFormat,
		&shadowBrush
	);
	
	mainLayout.Offset(2, -2);

	graphics.DrawString
	(
		imageName,
		length,
		&mainFont,
		mainLayout,
		&mainFormat,
		&mainBrush
	);
}

void main_window::on_paint(HDC hdc)
{
	RECT rc;
	GetClientRect(*this, &rc);
	Graphics graphics(hdc);
	Pen pen(Color(255, 255, 0, 0), 2);
	graphics.DrawImage(imagePath, 0, 0, rc.right, rc.bottom);
	drawStrings(hdc, rc, imageName.c_str());
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
				imagePath = Image::FromFile(path);
				imageName = findImageName(path);
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
	delete main_window::imagePath;
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
