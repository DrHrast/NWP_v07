#include "main.h"
#include "rc.h"
#include <filesystem>
#include <string>
#include <windows.h>

using namespace Gdiplus;
TCHAR* main_window::imageName = nullptr;

void findImageName(std::wstring path) 
{
	//std::wstring imgp = path;
	//size_t pos = imgp.find_first_of(L'\\');
	//std::wstring temp = imgp.substr(pos + 1);
	std::wstring temp = std::filesystem::path(path).filename().c_str();
	size_t nameSize = temp.length() + 1;
	TCHAR* temp2 = new TCHAR[nameSize];
	main_window::imageName = temp2;
}

void drawStrings(HDC hdc, RECT rc)
{
	Graphics graphics(hdc);
	Font mainFont(L"Arial", 16);
	Font shadowFont(L"Arial", 16);
	RectF mainLayout(0.2f, 0.2f, 200.0f, 50.0f);
	RectF shadowLayout(0.0f, 0.0f, 200.0f, 50.0f);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	SolidBrush mainBrush(Color(255, 255, 0, 0));
	SolidBrush shadowBrush(Color(255, 255, 255, 0.5));
	int length = lstrlen(main_window::imageName);

	graphics.DrawString
	(
		main_window::imageName,
		length,
		&shadowFont,
		shadowLayout,
		&format,
		&shadowBrush
	);
	
	graphics.DrawString
	(
		main_window::imageName,
		length,
		&mainFont,
		mainLayout,
		&format,
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
	drawStrings(hdc, rc);
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
				findImageName(path);
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
	delete[] main_window::imageName;
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
