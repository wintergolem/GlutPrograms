#include "CurvesHeader.h"


float wht[] = {1,1,1}, blk[] = {0,0,0}, red[] = {1,0,0}, yel[] = {1,1,0};
//Slider tSlider (AppSize/2,AppSize/2, 155, "T Slider", 0, 1, .5);
//Slider cSlider(AppSize/2, AppSize/2, 155, "C Slider", 0, 1, 0.5);

void Line(float x1, float y1, float x2, float y2)//, bool round = true)
{
    // draw line from (x1,y1) to (x2,y2), optionally round endpoints
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    if (true) {
        glBegin(GL_POINTS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}
void Line(int x1, int y1, int x2, int y2)//, bool round = true)
{
    Line(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2));//, round);
}

//Points Class
Point::Point(float x, float y){
	pos[0] = static_cast<float>(x);
	pos[1] = static_cast<float>(y);
	set = false;
}
void Point::draw(){
	glColor3f(1,0,0);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex2f(pos[0],pos[1]);
	glEnd();
}

//Widget Class
Widget::Widget(){}
Widget::Widget(int x, int y, int w, int h, string name) {
    x=x;
	y=y;
	w=w;
	h=h;
	name=name;
}
bool Widget::Hit(int xx, int yy)
{
	return xx >= x-h && xx <= x+w+h && yy >= y-h && yy <= y+h;
}

//Slider Class
Slider::Slider(){init_ed =false;}
void Slider::Init(int xx, int yx, int wx, string namex, float minx, float maxx, float initx)
{
	x=xx;
	y=yx;
	w=wx;
	name = namex;
	min = minx;
	max = maxx;
	init = initx;
	loc = x+(float)((init-min)/(max-min))*w;
	init_ed = true;
}
void Slider::Draw()
{
	glLineWidth(2.f);
	glPointSize(2.f);
    glColor3fv(yel);
	Line(x, y, x+w, y);
    glLineWidth(2.5f);
	Line(loc, y-7.f, loc, y+7.f);
	char buf[100];
	sprintf(buf, "%s: %f", name.c_str(), GetValue());
	glRasterPos2i(x, y-15);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*) buf);
}
float Slider::GetValue()
{
	 float v = min+((loc-x)/w)*(max-min);
     return v < min? min : v > max? max : v;
}
bool Slider::Hit(int x, int y)
{
	 // called upon mouse-down or mouse-drag
        bool hit = Widget::Hit(x, y);
	    if (hit)
            loc = static_cast<float>(x);
	    return hit;
}

//Hermite Class
int HermiteClass::selected = -1;
HermiteClass::HermiteClass(){}
void HermiteClass::Line(float x1, float y1, float x2, float y2)//, bool round = true)
{
    // draw line from (x1,y1) to (x2,y2), optionally round endpoints
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    if (true) {
        glBegin(GL_POINTS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}
void HermiteClass::Line(int x1, int y1, int x2, int y2)//, bool round = true)
{
    Line(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2));//, true);
}
void HermiteClass::DrawDots()
{
	for(vector<Point>::iterator it = points.begin();it != points.end(); it++){
		it->draw();
	}
}
void HermiteClass::CalCurve(float p1[],float p2[], float v1[], float v2[], float *result, float t){
	float a[2],b[2],c[2],d[2];
	float t2 = t*t, t3 =t*t*t;
	for( int i=0; i<2; i++){
		a[i] = -2 * p2[i] + 2 * p1[i] + v1[i] + v2[i];
		b[i] = -3 * p1[i] + 3 * p2[i] - 2 *v1[i] - v2[i];
		c[i] = v1[i];
		d[i] = p1[i];
		
		result[i] = a[i] * t3 + b[i] * t2 + c[i] * t + d[i];
	}
}
void HermiteClass::DrawCurve(){
	for(vector<Point>::iterator it = points.begin(); it != points.end(); it++)
		if(it->set == false) return;
	for(vector<Point>::iterator it2 = points.begin(); it2 != points.end()-1; it2++){
		vector<Point>::iterator it3 = it2+1;

		float result[2];
		glColor3f(0,0,1);
		glBegin(GL_LINE_STRIP);
		for(float t =0;t<1;t += 0.01f){
			CalCurve(it2->pos,it3->pos,it2->tang,it3->tang,result,t);
			glVertex2fv(result);
		}
		glEnd();
	}
}
void HermiteClass::DrawLines(){
	if(points.size() >1){
			/*glEnable(GL_LINE_STIPPLE);
			glLineStipple(1,0x00FF);*/
			glBegin(GL_LINE_STRIP);
			glColor3f(1,0,0);
			glVertex2f(points[0].pos[0], points[0].pos[1] );
			for(vector<Point>::iterator it = points.begin()+1;it != points.end(); it++)
				glVertex2f(it->pos[0],it->pos[1]);
			glEnd();
			//glDisable(GL_LINE_STIPPLE);
		}
}
void HermiteClass::AllocDot(int a, int b){
	float x = static_cast<float>(a);
	float y = static_cast<float>(b);
	points.push_back( Point(x,y) );
}
void HermiteClass::AllocTangent(Slider &cSlider){
	float c = cSlider.GetValue();

	/*
	vector<Point>::iterator it
	vector<Point>::iterator itn = it +1;
	vector<Point>::iterator itp = it -1;
	*/
	if(points.size() == 0 || points.size() == 1)
		return;
	if(points.size() == 2){//two points
		vector<Point>::iterator it = points.begin();
		//set first
		it->tang[0] = c*( (it+1)->pos[0] - it->pos[0] );
		it->tang[1] = c*( (it+1)->pos[1] - it->pos[1] );
		it->set = true;
		//set last
		it = points.end()-1 ;
		it->tang[0] = c*( it->pos[0] - (it-1)->pos[0] );
		it->tang[1] = c*( (it)->pos[1] - (it-1)->pos[1] );
		it->set = true;
	}
	if(points.size() >= 3){//three plus
		vector<Point>::iterator it = points.begin();
		//set first
		it->tang[0] = c*( (it+1)->pos[0] - it->pos[0] );
		it->tang[1] = c*( (it+1)->pos[1] - it->pos[1] );
		it->set = true;
		//set middle
		for(vector<Point>::iterator it = points.begin() +1; it != points.end() -1; it++){
			it->tang[0] = c*( (it+1)->pos[0] - (it-1)->pos[0] );
			it->tang[1] = c*( (it+1)->pos[1] - (it-1)->pos[1] );
			it->set = true;
		}
		//set last
		it =  points.end() -1;
		it->tang[0] = c*( (it)->pos[0] - (it-1)->pos[0] );
		it->tang[1] = c*( (it)->pos[1] - (it-1)->pos[1] );
		it->set = true;
	}
}
void HermiteClass::Draw(Slider &cSlider){
	ClearScreen(0,0,0);
	cSlider.Draw();
	DrawDots();
	//drawlines();
	if(points.size() > 2)
		DrawCurve();
}
void HermiteClass::Idle(Slider &cSlider)
{
	if(cSlider.init_ed == false){
		cSlider.Init(50,50,155, "C Slider", 0,1,0.5);
	}
	AllocTangent(cSlider);
}
void HermiteClass::MouseButton(int button, int state, int x, int y, AppState &appState, Slider &cSlider){
	selected = -1;
	if(cSlider.Hit(x,y) || (x < 230 && y <65 ) ) return;

	if(button == GLUT_LEFT_BUTTON && state == 0){
			AllocDot(x,y);
		}
	if(button == GLUT_RIGHT_BUTTON && state ==0){
		
		for(unsigned int i =0;i<points.size() ;i++)
				if (abs(points[i].pos[0]-x) < 10 && abs(points[i].pos[1]-y) < 10)
					selected = i;
	}
	//which curves with center button
	if(button == GLUT_MIDDLE_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			appState = BezierState;
		}
	}
}
void HermiteClass::MouseDrag(int x, int y,Slider &cSlider){
	//slider
	if(cSlider.Hit(x,y)) return;
	if(selected != -1) {
		points[selected].pos[0] = static_cast<float>(x);
		points[selected].pos[1] = static_cast<float>(y);
	}
	glutPostRedisplay();
}

//Bezier Class
BezierClass::BezierClass(){}
float BezierClass::points[4][2] = {{50,50},{50,350},{550,350},{550,50} };
int BezierClass::selected = -1;
void BezierClass::Line(float x1, float y1, float x2, float y2)//, bool round = true)
{
    // draw line from (x1,y1) to (x2,y2), optionally round endpoints
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    if (true) {
        glBegin(GL_POINTS);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }
}
void BezierClass::Line(int x1, int y1, int x2, int y2)//, bool round = true)
{
    Line(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2));//, round);
}
void BezierClass::Lerp(float *result, float *p0, float *p1, float t) {
	for (int i = 0; i < 2; i++)
		result[i] = p0[i]+t*(p1[i]-p0[i]);
}
void BezierClass::DrawPolygon(){
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0x00FF);
	glColor3f(.5f,.5f,.5f);
	for(int i =0;i<3;i++){
		glBegin(GL_LINES);
		glVertex2f(points[i][0],points[i][1]);
		glVertex2f(points[i+1][0],points[i+1][1]);
		glEnd();
	}
	glDisable(GL_LINE_STIPPLE);
}
void BezierClass::DrawDot(float x, float y){
		glColor3f(1,1,1);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2d(x,y);
		glEnd();
	}
void BezierClass::DrawDots(float a0[2],float a1[2],float a2[2],float b0[2],float b1[2],float c0[2]){
		glColor3f(1,1,1);
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex2d(a0[0],a0[1]);
		glVertex2d(a1[0],a1[1]);
		glVertex2d(a2[0],a2[1]);
		glVertex2d(b0[0],b0[1]);
		glVertex2d(b1[0],b1[1]);
		glVertex2d(c0[0],c0[1]);
		glEnd();
	}
void BezierClass::DrawBezierConstruct(float t){
		//setting values
		float a0[2],a1[2],a2[2];
		float b0[2],b1[2];
		float c0[2];
		Lerp(a0, points[0], points[1], t);
		Lerp(a1, points[1], points[2], t);
		Lerp(a2, points[2], points[3], t);
		Lerp(b0, a0, a1, t);
		Lerp(b1, a1, a2, t);
		Lerp(c0, b0, b1, t);
		//draw a
		glColor3f(1,0,0);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00FF);
		glBegin(GL_LINES);
		glVertex2f(a0[0],a0[1]);
		glVertex2f(a1[0],a1[1]);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		glColor3f(1,0,0);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1,0x00FF);
		glBegin(GL_LINES);
		glVertex2f(a1[0],a1[1]);
		glVertex2f(a2[0],a2[1]);
		glEnd();
		glDisable(GL_LINE_STIPPLE);

		//draw b
		glColor3f(0,1,0);
		//glLineStipple(1,0x00FF);
		//glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINES);
		glVertex2f(b0[0],b0[1]);
		glVertex2f(b1[0],b1[1]);
		glEnd();
		//glDisable(GL_LINE_STIPPLE);

		//draw c
		glColor3f(0,0,1);
		glPointSize(15);
		glBegin(GL_POINTS);
		glVertex2d(c0[0],c0[1]);
		glEnd();

		DrawDots(a0,a1,a2,b0,b1,c0);
	}
void BezierClass::MakeBezierCurve(float *result,float *a, float *b,float c[], float d[], float t){
		float dt = t*t;
		float dat = t*t*t;

		for(int i =0; i<2; i++){
			result[i] = ((-dat + 3*dt - 3*t +1)*a[i]+ ( 3*dat -6*dt + 3*t) * b[i]+ (-3*dat + 3*dt)*c[i]+dat*d[i]);
		}
	}
void BezierClass::DrawBezierCurve(){
		float result[2];
		glColor3f(1,0,0);
		glBegin(GL_LINE_STRIP);
		for (float t = 0; t < 1; t += 0.01f) {
	//	for(int i=0;i<100;i++){
			MakeBezierCurve(result,points[0],points[1],points[2],points[3], t);
			//glVertex2f(result[0],result[1]);
			glVertex2fv(result);
		}
		glEnd();
	}
void BezierClass::Draw(Slider &tSlider){
		ClearScreen(0,0,0);
		DrawPolygon();
		for(int i=0;i<4;i++){
			DrawDot(points[i][0],points[i][1]);
		}
		DrawBezierConstruct(tSlider.GetValue() );
		DrawBezierCurve();
		tSlider.Draw();
	}
void BezierClass::Idle(Slider &tSlider)
{
	//if t slider isn't initialised, do it now
		if(tSlider.init_ed == false){
			tSlider.Init(50,50,155, "T Slider", 0,1,0.5);
		}
	}
void BezierClass::MouseButton(int button, int state, int x, int y, AppState &appState,Slider &tSlider){
		//points on polygon
		selected = -1;
		if(state == GLUT_DOWN)
			for(int i =0;i<4;i++)
					if (abs(points[i][0]-x) < 15 && abs(points[i][1]-y) < 15)
						selected = i;

		//slider
		tSlider.Hit(x,y);
		//which curves with center button
		if(button == GLUT_MIDDLE_BUTTON)
		{
			if(state == GLUT_DOWN)
			{
				appState = HermiteState;
			}
		}
	}
void BezierClass::MouseDrag(int x, int y,Slider &tSlider){
		//slider
		tSlider.Hit(x,y);
		if(selected != -1) {
			points[selected][0] = static_cast<float>(x);
			points[selected][1] = static_cast<float>(y);
		}
		glutPostRedisplay();
	}

//Menu Class
	//entrance screen to lead to curves
MenuClass::MenuClass()
{
	chooseText = "Left mouse for Bezier /n Right mouse for Hermite";
}
void MenuClass::Draw()
{
	//tell user button options
	Text(2000,5000, chooseText);
}
void MenuClass::Idle()
{
	//just redisplay
	glutPostRedisplay();
}
void MenuClass::MouseButton(int button, int state, int x, int y, AppState &appState)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			appState = BezierState;
		}
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
		{
			appState = HermiteState;
		}
	}
	
}
void MenuClass::MouseDrag(int x, int y)
{
	// no drag use
}
