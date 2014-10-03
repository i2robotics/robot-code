/*--------------------------------------------------
 *
 *		Color v. 1
 *			utility for interpreting color sensor
 *			values.
 *
 *------------------------------------------------*/

int color(int value) {
	int red = 0;
	int green = 0;
	int blue = 0;

	if (value==0) {
		HTCS2readRGB(HTCOLOR1, red, green, blue);
	}
	else if (value==1) {
		HTCS2readRGB(HTCOLOR2, red, green, blue);
	}
	if (red<27 && green<27 && blue < 23) {
		return 1;//gray
	}
	else if (red < 120 && red > 100 && green < 120 && green > 100 && blue > 90 && blue < 110) {
		return 2;//Gray White
	}
	else if (red > 170 && green > 175 && blue > 155) {
		return 3;//white
	}
	else if (red  < 25 && green > 25 && green < 40 && blue < 60 && blue > 50) {
		return 4;//gray blue
	}
	else if (red < 25 && green > 33 && green < 45 && blue > 70 && blue < 85) {
		return 5;//blue red
	}
	else if (red > 53 && red < 68 && green < 35 && green > 20 && blue > 30 && blue < 43) {
		return 6;//blue red
	}
	else if (red > 50 && red < 62 && green > 15 && green < 25 && blue <20) {
		return 7;//gray red
	}
	else if (red > 90 && red < 110 && green <25 && blue < 10) {
		return 8;//red
	}
	return 1;
}

bool onWhite(int value) {
	if (color(value) == csWhite) {
		return true;
	}
	return false;
}

bool onGray(int value) {
	if (color(value) == csGray) {
		return true;
	}
	return false;
}

bool onGrayWhite(int value) {
	if (color(value) == csGrayWhite) {
		return true;
	}
	return false;
}
