//****** App

// global
Button startButton(275, 10, 90, 25);

// in MouseButton
if (startButton.Hit(x, y))
	restartGame = true;

// in DrawProc
float blk[] = {0, 0, 0};
startButton.Draw("Start", blk);


//****** Buttons

class Button
{
public:
	bool round;
    int x, y, w, h, radius, winW, winH; // in pixels
	Button();
	Button(int x, int y, int radius);
    Button(int x, int y, int w, int h);
	void Init(int x, int y, int w, int h);
	void Init(int x, int y, int radius);
    void Draw(char *name, float *color);
    bool Hit(int x, int y);
};

Button::Button() {
	Init(0, 0, 0, 0);
}

Button::Button(int x, int y, int w, int h) {
	Init(x, y, w, h);
}

Button::Button(int x, int y, int radius) {
	Init(x, y, radius);
}

void Button::Init(int x, int y, int radius) {
	this->x = x;
	this->y = y;
	this->radius = radius;
	round = true;
	winW = -1;
}

void Button::Init(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	round = false;
	winW = -1;
}

void Button::Draw(char *name, float *color) {
	if (winW < 0) {
		winW = glutGet(GLUT_WINDOW_WIDTH);
		winH = glutGet(GLUT_WINDOW_HEIGHT);
	}
	if (round) {
		glLineWidth(2.f*radius);
		Line(x-radius, y, x+radius, y, color, color);
		float xPos = (2*(x+13))/(float)winW-1.f;
		float yPos = (2*(y-5))/(float)winH-1.f;
		glRasterPos2f(xPos, yPos);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*) (name));
	}
	else {
		Rect(x, y, w, h, color);
		float xPos = (2*(x+5))/(float)winW-1.f;
		float yPos = (2*(y+6))/(float)winH-1.f;
		glRasterPos2f(xPos, yPos);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*) (name));
	}
}

bool Button::Hit(int x, int y) {

	return	round?
		x >= this->x-radius && x < this->x+radius &&
		y >= this->y-radius && y < this->y+radius :
	    x >= this->x && x <= this->x+w &&
		y >= this->y && y <= this->y+h;
}

