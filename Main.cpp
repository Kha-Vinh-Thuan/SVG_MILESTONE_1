#include "ReadFile.h"
string filename = "sample.svg";


float zoomScale = 1.0f;
float offsetX = 0.0f;
float offsetY = 0.0f;


void ZoomGraphics(Graphics& graphics, float scale_)
{
        graphics.ResetTransform();
        graphics.TranslateTransform(offsetX, offsetY);
        graphics.ScaleTransform(scale_, scale_);
}

string GetClassName(Shape* element)
{
    if (dynamic_cast<Circle*>(element) != NULL)
    {
        return "Circle";
    }
    else if (dynamic_cast<Rect_*>(element) != NULL)
    {
        return "Rect";
    }
    else if (dynamic_cast<Line*>(element) != NULL)
    {
        return "Line";
    }
    else if (dynamic_cast<Text*>(element) != NULL)
    {
        return "Text";
    }
    else if (dynamic_cast<Polyline_*>(element) != NULL)
    {
        return "Polyline";
    }
    else if (dynamic_cast<Polygon_*>(element) != NULL)
    {
        return "Polygon";
    }
    else if (dynamic_cast<Ellipse_*>(element) != NULL)
    {
        return "Ellipse";
    }
    else if (dynamic_cast<ClassPath*>(element) != NULL)
    {
        return "Path";
    }
    if (dynamic_cast<Group*>(element) != NULL)
    {
        return "Group";
    }
    return "Shape";
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    vector<Shape*> elements;
    vector<Defs*> elements_defs;

    parseAndRenderSVG(filename, elements, elements_defs);
        ZoomGraphics(graphics, zoomScale);

    for (const auto& element : elements)
    {
        string className = GetClassName(element);
        if (className == "Circle")
        {
            Circle* circle = dynamic_cast<Circle*>(element);
            circle->Draw(graphics, elements_defs);
        }
        else if (className == "Rect")
        {
            Rect_* rect = dynamic_cast<Rect_*>(element);
            rect->Draw(graphics, elements_defs);
        }
        else if (className == "Line")
        {
            Line* line = dynamic_cast<Line*>(element);
            line->Draw(graphics, elements_defs);
        }
        else if (className == "Text")
        {
            Text* text = dynamic_cast<Text*>(element);
            text->Draw(graphics, elements_defs);
        }
        else if (className == "Polyline")
        {
            Polyline_* polyline = dynamic_cast<Polyline_*>(element);
            polyline->Draw(graphics, elements_defs);
        }
        else if (className == "Polygon")
        {
            Polygon_* polygon = dynamic_cast<Polygon_*>(element);
            polygon->Draw(graphics, elements_defs);
        }
        else if (className == "Ellipse")
        {
            Ellipse_* ellipse = dynamic_cast<Ellipse_*>(element);
            ellipse->Draw(graphics, elements_defs);
        }
        else if (className == "Path")
        {
            ClassPath* path = dynamic_cast<ClassPath*>(element);
            path->Draw(graphics, elements_defs);
        }
        else if (className == "Group")
        {
            Group* group = dynamic_cast<Group*>(element);
            group->Draw(graphics, elements_defs);
        }
    }
    graphics.ResetTransform();
}
int lastMouseX = 0;
int lastMouseY = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_MOUSEWHEEL:
    {
        short delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (delta > 0)
            zoomScale += 0.1f;
        else
            zoomScale -= 0.1f;

        if (zoomScale < 0.1f)
            zoomScale = 0.1f;

        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (wParam & MK_LBUTTON)
        {
            int mouseX = GET_X_LPARAM(lParam);
            int mouseY = GET_Y_LPARAM(lParam);

            float deltaX = static_cast<float>(mouseX - lastMouseX);
            float deltaY = static_cast<float>(mouseY - lastMouseY);

            offsetX += deltaX;
            offsetY += deltaY;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        lastMouseX = GET_X_LPARAM(lParam);
        lastMouseY = GET_Y_LPARAM(lParam);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, INT iCmdShow)
{
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    wstring wide_filename;
    if (argc > 1)
    {
        wide_filename = argv[1];
        filename = string(wide_filename.begin(), wide_filename.end());
    }
    LocalFree(argv);

    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);
    hWnd = CreateWindow(
        TEXT("GettingStarted"), // window class name
        TEXT("SVG Demo"),       // window caption
        WS_OVERLAPPEDWINDOW,    // window style
        CW_USEDEFAULT,          // initial x position
        CW_USEDEFAULT,          // initial y position
        CW_USEDEFAULT,          // initial x size
        CW_USEDEFAULT,          // initial y size
        NULL,                   // parent window handle
        NULL,                   // window menu handle
        hInstance,              // program instance handle
        NULL);                  // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
} // WinMain