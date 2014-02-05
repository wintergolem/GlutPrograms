// BezierAndHermiteCurves.cpp : Defines the entry point for the console application.
//
//--INCLUDE FILES--
#include "CurvesHeader.h"
int AppSize = 600;
Slider cSlider;
Slider tSlider;

//Enums
//enum AppState { MenuState, BezierState, HermiteState};

//Globals
AppState appState;
HermiteClass Hermite;
BezierClass Bezier;
MenuClass Menu;

//Functions
void Draw(){
	ClearScreen(0,0,0);
	if(appState == HermiteState)
		Hermite.Draw(cSlider);
	else if(appState == BezierState)
		Bezier.Draw(tSlider);
	else
		Menu.Draw();
	Text(3000,100, "Which Curves with Center Mouse Buttonn");
	glFlush();
}
void Idle()
{
	if(appState == HermiteState)
		Hermite.Idle(cSlider);
	else if(appState == BezierState)
		Bezier.Idle(tSlider);
	else
		Menu.Idle();
	glutPostRedisplay();
}
void MouseButton(int button, int state, int x, int y)
{
	if(appState == HermiteState)
		Hermite.MouseButton(button,state, x, AppSize-y, appState, cSlider);
	else if(appState == BezierState)
		Bezier.MouseButton(button,state, x, AppSize-y, appState, tSlider);
	else
		Menu.MouseButton(button,state, x, AppSize-y, appState);
}
void MouseDrag(int x, int y)
{
	if(appState == HermiteState)
		Hermite.MouseDrag(x,AppSize-y, cSlider);
	else if(appState == BezierState)
		Bezier.MouseDrag(x, AppSize-y, tSlider);
	else
		Menu.MouseDrag(x, AppSize-y);
}
void main()
{
	appState = MenuState;
	
	int ac = 0;
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(AppSize, AppSize);
    glutInit(&ac, NULL);
    glutCreateWindow("Curves");

    // set default viewport and projection
    glViewport(0, 0, AppSize, AppSize);                            // map GL space into entire app window
    glMatrixMode(GL_PROJECTION);                       // camera view mode
    glLoadIdentity();                                  // no rotations
    glOrtho(0, AppSize, 0, AppSize, 0, -1);                        // GL space in pixels, no perspective
	//End making of window

	// set callbacks , run in loop
	glutDisplayFunc(Draw);                           // respond to redraw requests
	glutIdleFunc(Idle);  
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseDrag);
	// begin event handling, display, and updates
	glutMainLoop();
}

