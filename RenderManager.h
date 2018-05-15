/*credits by ibanned*/

#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();


namespace Render
{

	void Initialise();

	void Clear(int x, int y, int w, int h, Color color);
	void TexturedPolygon(int n, Vertex_t* vertice, Color col);
	void FilledCircle(Vector2D position, float points, float radius, Color color);
	void Outline(int x, int y, int w, int h, Color color);

	void DrawRect(int x, int y, int w, int h, Color col);

	bool TransformScreen(const Vector & in, Vector & out);

	void Text2(int x, int y, const char * _Input, int font, Color color);
	
	void rect(int x, int y, int w, int h, Color color);
	void text(int x, int y, const char * _Input, int font, Color color);
	void DrawTexturedPoly(int n, Vertex_t * vertice, Color col);
	void DrawFilledCircle(Vector2D center, Color color, float radius, float points);
	void Line(int x, int y, int x2, int y2, Color color);
	void PolyLine(int *x, int *y, int count, Color color);
	void CircleFilledx(int x, int y, float points, float radius, Color color);
	void CircleFilled(int x, int y, float points, float radius, Color color);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void outlined_rect(int x, int y, int w, int h, Color color_out, Color color_in);
	void outline(int x, int y, int w, int h, Color color);
	void color_spectrum(int x, int y, int w, int h);
	Color color_spectrum_pen(int x, int y, int w, int h, Vector stx);
	void DrawLine(int x0, int y0, int x1, int y1, Color col);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);
	void Rect2(int x, int y, int w, int h, int r, int g, int b, int a);
	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void DrawCircle(float x, float y, float r, float segments, Color color);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);

	namespace Fonts
	{
		extern DWORD Default;
		extern DWORD Menu;
		extern DWORD MenuBold;
		extern DWORD ESP;
		extern DWORD MenuText;
		extern DWORD Icon;
		extern DWORD MenuTabs;
		extern DWORD Text;
		extern DWORD Slider;
		extern DWORD Clock;
		extern DWORD Tabs;
		extern DWORD Watermark;
	    extern DWORD watermarkusfontus;
		extern DWORD aaindicator;
		extern DWORD LBY;
		extern DWORD LBY2;
	extern 	DWORD MenuSymbols;
		extern 	DWORD legitbot;
		extern 	DWORD supremacy;
	};

	void Text(int x, int y, Color color, DWORD font, const char* text);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	RECT GetTextSize(DWORD font, const char* text);

	bool WorldToScreen(Vector &in, Vector &out);
	RECT GetViewport();
};

void DrawEmptyRect(int x1, int y1, int x2, int y2, Color color, unsigned char ignore_flags);
