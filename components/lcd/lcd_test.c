#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h> // time

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#if defined(IMAGE)
#include "esp_err.h"
#endif /* IMAGE */

#include "lcd.h"
#include "fontx.h"
#if defined(IMAGE)
#include "bmpfile.h"
#include "decode_jpeg.h"
#include "decode_png.h"
#include "pngle.h"
#endif /* IMAGE */

#define INTERVAL 200
#define WAIT vTaskDelay(INTERVAL)

// static const char *TAG = "LCD_Test";


TickType_t LineTestHV(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	color=RED;
	for(int32_t ypos=0;ypos<height;ypos=ypos+10) {
		lcdDrawHLine(dev, 0, ypos, width, color);
	}

	for(int32_t xpos=0;xpos<width;xpos=xpos+10) {
		lcdDrawVLine(dev, xpos, 0, height, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t LineTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, BLACK);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	for(int32_t i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565(red, green, blue);
		int32_t x0=rand()%width;
		int32_t y0=rand()%height;
		int32_t x1=rand()%width;
		int32_t y1=rand()%height;
		lcdDrawLine(dev, x0, y0, x1, y1, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t FillTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdFillScreen(dev, RED);
	lcdWriteFrame(dev);
	vTaskDelay(50);
	lcdFillScreen(dev, GREEN);
	lcdWriteFrame(dev);
	vTaskDelay(50);
	lcdFillScreen(dev, BLUE);
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ColorBarTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	if (width < height) {
		int32_t y1,y2;
		y1 = height/3;
		y2 = (height/3)*2;
		lcdFillRect(dev, 0, 0, width-1, y1-1, RED);
		lcdFillRect(dev, 0, y1, width-1, y2-1, GREEN);
		lcdFillRect(dev, 0, y2, width-1, height-1, BLUE);
		lcdWriteFrame(dev);
	} else {
		int32_t x1,x2;
		x1 = width/3;
		x2 = (width/3)*2;
		lcdFillRect(dev, 0, 0, x1-1, height-1, RED);
		lcdFillRect(dev, x1, 0, x2-1, height-1, GREEN);
		lcdFillRect(dev, x2, 0, width-1, height-1, BLUE);
		lcdWriteFrame(dev);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ColorBandTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	color = RED;
	int32_t delta = height/16;
	int32_t ypos = 0;
	for(int32_t i=0;i<16;i++) {
		//ESP_LOGI(__FUNCTION__, "color=0x%x",color);
		lcdFillRect(dev, 0, ypos, width-1, ypos+delta, color);
		color = color >> 1;
		ypos = ypos + delta;
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t FillRectTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, CYAN);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	for(int32_t i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565(red, green, blue);
		int32_t xpos=rand()%width;
		int32_t ypos=rand()%height;
		int32_t size=rand()%(width/5);
		lcdFillRect(dev, xpos, ypos, xpos+size, ypos+size, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t FillTriTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, CYAN);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	for(int32_t i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565(red, green, blue);
		int32_t x0=rand()%width;
		int32_t y0=rand()%height;
		int32_t x1=rand()%width;
		int32_t y1=rand()%height;
		int32_t x2=rand()%width;
		int32_t y2=rand()%height;
		lcdFillTri(dev, x0, y0, x1, y1, x2, y2, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t FillCircleTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, CYAN);

	uint16_t red;
	uint16_t green;
	uint16_t blue;
	srand( (unsigned int)time( NULL ) );
	for(int32_t i=1;i<100;i++) {
		red=rand()%255;
		green=rand()%255;
		blue=rand()%255;
		color=rgb565(red, green, blue);
		int32_t radius=rand()%(width/5);
		int32_t xpos=rand()%width;
		if (xpos < radius) xpos = radius; // clip
		else if (xpos > width-1-radius) xpos = width-1-radius;
		int32_t ypos=rand()%height;
		if (ypos < radius) ypos = radius; // clip
		else if (ypos > height-1-radius) ypos = height-1-radius;
		lcdFillCircle(dev, xpos, ypos, radius, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t CircleTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	int32_t limit = width;
	if (height < width) limit = height;
	limit /= 2;
	lcdFillScreen(dev, BLACK);
	color = CYAN;
	int32_t xpos = width/2;
	int32_t ypos = height/2;
	for(int32_t i=5;i<limit;i=i+5) {
		lcdDrawCircle(dev, xpos, ypos, i, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t RoundRectTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();


	uint16_t color;
	int32_t limit = width;
	if (width > height) limit = height;
	lcdFillScreen(dev, BLACK);
	color = BLUE;
	for(int32_t i=5;i<limit;i=i+5) {
		if (i > (limit-i-1) ) break;
		//ESP_LOGI(__FUNCTION__, "i=%d, width-i-1=%d",(int)i, (int)width-i-1);
		lcdDrawRoundRect(dev, i, i, (width-i-1), (height-i-1), 10, color);
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t ArrowTest(TFT_t *dev, FontxFile *fx, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%hhd fontHeight=%hhd",fontWidth,fontHeight);

	int32_t xpos;
	int32_t ypos;
	int32_t	stlen;
	uint8_t ascii[24];
	uint16_t color;

	lcdFillScreen(dev, BLACK);

	strcpy((char *)ascii, "LCD");
	if (width < height) {
		xpos = ((width - fontHeight) / 2) - 1;
		ypos = (height - (strlen((char *)ascii) * fontWidth)) / 2;
		lcdSetFontDirection(dev, DIRECTION90);
	} else {
		ypos = ((height - fontHeight) / 2) - 1;
		xpos = (width - (strlen((char *)ascii) * fontWidth)) / 2;
		lcdSetFontDirection(dev, DIRECTION0);
	}
	color = WHITE;
	lcdDrawString(dev, fx, xpos, ypos, ascii, color);

	lcdSetFontDirection(dev, 0);
	color = RED;
	lcdFillArrow(dev, 10, 10, 0, 0, 5, color);
	strcpy((char *)ascii, "0,0");
	lcdDrawString(dev, fx, 0, 30, ascii, color);

	color = GREEN;
	lcdFillArrow(dev, width-11, 10, width-1, 0, 5, color);
	//strcpy((char *)ascii, "79,0");
	sprintf((char *)ascii, "%d,0",(int)width-1);
	stlen = strlen((char *)ascii);
	xpos = (width-1) - (fontWidth*stlen);
	lcdDrawString(dev, fx, xpos, 30, ascii, color);

	color = GRAY;
	lcdFillArrow(dev, 10, height-11, 0, height-1, 5, color);
	//strcpy((char *)ascii, "0,159");
	sprintf((char *)ascii, "0,%d",(int)height-1);
	ypos = (height-11) - (fontHeight) + 5;
	lcdDrawString(dev, fx, 0, ypos, ascii, color);

	color = CYAN;
	lcdFillArrow(dev, width-11, height-11, width-1, height-1, 5, color);
	//strcpy((char *)ascii, "79,159");
	sprintf((char *)ascii, "%d,%d",(int)width-1, (int)height-1);
	stlen = strlen((char *)ascii);
	xpos = (width-1) - (fontWidth*stlen);
	lcdDrawString(dev, fx, xpos, ypos, ascii, color);
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t RectangleTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	color = CYAN;
	int32_t xpos = width/2;
	int32_t ypos = height/2;

	int32_t h = ((height < width) ? height : width) * 0.7;
	int32_t w = h * 0.5;
	int32_t angle;

	for(angle = 0; angle < (360*3); angle += 30) {
		lcdDrawRectangle(dev, xpos, ypos, w, h, angle, color);
		lcdDrawRectangle(dev, xpos, ypos, w, h, angle, BLACK);
	}

	for(angle = 0; angle < 180; angle += 30) {
		lcdDrawRectangle(dev, xpos, ypos, w, h, angle, color);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t TriangleTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	color = CYAN;
	int32_t xpos = width/2;
	int32_t ypos = height/2;

	int32_t h = ((height < width) ? height : width) * 0.7;
	int32_t w = h * 0.7;
	int32_t angle;

	for(angle = 0; angle < (360*3); angle += 30) {
		lcdDrawTriangle(dev, xpos, ypos, w, h, angle, color);
		lcdDrawTriangle(dev, xpos, ypos, w, h, angle, BLACK);
	}

	for(angle = 0; angle < 360; angle += 30) {
		lcdDrawTriangle(dev, xpos, ypos, w, h, angle, color);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t DirectionTest(TFT_t *dev, FontxFile *fx, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%hhd fontHeight=%hhd",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=0");
	lcdSetFontDirection(dev, 0);
	lcdDrawString(dev, fx, 0, fontHeight-1, ascii, color);

	color = BLUE;
	strcpy((char *)ascii, "Direction=2");
	lcdSetFontDirection(dev, 2);
	lcdDrawString(dev, fx, (width-1), (height-1)-(fontHeight*1), ascii, color);

	color = CYAN;
	strcpy((char *)ascii, "Direction=1");
	lcdSetFontDirection(dev, 1);
	lcdDrawString(dev, fx, (width-1)-fontHeight, 0, ascii, color);

	color = GREEN;
	strcpy((char *)ascii, "Direction=3");
	lcdSetFontDirection(dev, 3);
	lcdDrawString(dev, fx, (fontHeight-1), height-1, ascii, color);
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t HorizontalTest(TFT_t *dev, FontxFile *fx, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%hhd fontHeight=%hhd",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=0");
	lcdSetFontDirection(dev, 0);
	lcdDrawString(dev, fx, 0, fontHeight*1-1, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, 0, fontHeight*2-1, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, GREEN);
	lcdDrawString(dev, fx, 0, fontHeight*3-1, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, 0, fontHeight*4-1, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	color = BLUE;
	strcpy((char *)ascii, "Direction=2");
	lcdSetFontDirection(dev, 2);
	lcdDrawString(dev, fx, width, height-(fontHeight*1)-1, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, width, height-(fontHeight*2)-1, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, YELLOW);
	lcdDrawString(dev, fx, width, height-(fontHeight*3)-1, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, width, height-(fontHeight*4)-1, ascii, color);
	lcdWriteFrame(dev);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t VerticalTest(TFT_t *dev, FontxFile *fx, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%hhd fontHeight=%hhd",fontWidth,fontHeight);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t ascii[20];

	color = RED;
	strcpy((char *)ascii, "Direction=1");
	lcdSetFontDirection(dev, 1);
	lcdDrawString(dev, fx, width-(fontHeight*1), 0, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, width-(fontHeight*2), 0, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, GREEN);
	lcdDrawString(dev, fx, width-(fontHeight*3), 0, ascii, color);
	lcdSetFontUnderLine(dev, RED);
	lcdDrawString(dev, fx, width-(fontHeight*4), 0, ascii, color);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	color = BLUE;
	strcpy((char *)ascii, "Direction=3");
	lcdSetFontDirection(dev, 3);
	lcdDrawString(dev, fx, (fontHeight*1)-1, height, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, (fontHeight*2)-1, height, ascii, color);
	lcdUnsetFontUnderLine(dev);

	lcdSetFontFill(dev, YELLOW);
	lcdDrawString(dev, fx, (fontHeight*3)-1, height, ascii, color);
	lcdSetFontUnderLine(dev, BLUE);
	lcdDrawString(dev, fx, (fontHeight*4)-1, height, ascii, color);
	lcdWriteFrame(dev);
	lcdUnsetFontFill(dev);
	lcdUnsetFontUnderLine(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t CodeTest(TFT_t *dev, FontxFile *fx, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	// get font width & height
	uint8_t buffer[FontxGlyphBufSize];
	uint8_t fontWidth;
	uint8_t fontHeight;
	GetFontx(fx, 0, buffer, &fontWidth, &fontHeight);
	//ESP_LOGI(__FUNCTION__,"fontWidth=%hhd fontHeight=%hhd",fontWidth,fontHeight);
	uint8_t xmoji = width / fontWidth;
	uint8_t ymoji = height / fontHeight;
	//ESP_LOGI(__FUNCTION__,"xmoji=%hhd ymoji=%hhd",xmoji, ymoji);

	uint16_t color;
	lcdFillScreen(dev, BLACK);
	uint8_t code;

	color = CYAN;
	lcdSetFontDirection(dev, 0);
	code = 0xA0;
	for(int32_t y=0;y<ymoji;y++) {
		int32_t xpos = 0;
		int32_t ypos =  fontHeight*(y+1)-1;
		for(int32_t x=0;x<xmoji;x++) {
			xpos = lcdDrawCode(dev, fx, xpos, ypos, code, color);
			if (code == 0xFF) break;
			code++;
		}
	}
	lcdWriteFrame(dev);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

#if defined(IMAGE)
TickType_t BMPTest(TFT_t *dev, char * file, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);

	// open requested file
	esp_err_t ret;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	// read bmp header
	bmpfile_t *result = (bmpfile_t*)malloc(sizeof(bmpfile_t));
	ret = fread(result->header.magic, 1, 2, fp);
	assert(ret == 2);
	if (result->header.magic[0]!='B' || result->header.magic[1] != 'M') {
		ESP_LOGW(__FUNCTION__, "File is not BMP");
		free(result);
		fclose(fp);
		return 0;
	}
	ret = fread(&result->header.filesz, 4, 1 , fp);
	assert(ret == 1);
	ESP_LOGD(__FUNCTION__,"result->header.filesz=%"PRIu32, result->header.filesz);
	ret = fread(&result->header.creator1, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.creator2, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.offset, 4, 1, fp);
	assert(ret == 1);

	// read dib header
	ret = fread(&result->dib.header_sz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.width, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.height, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nplanes, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.depth, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.compress_type, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.bmp_bytesz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.hres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.vres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.ncolors, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nimpcolors, 4, 1, fp);
	assert(ret == 1);

	if((result->dib.depth == 24) && (result->dib.compress_type == 0)) {
		// BMP rows are padded (if needed) to 4-byte boundary
		uint32_t rowSize = (result->dib.width * 3 + 3) & ~3;
		int32_t w = result->dib.width;
		int32_t h = result->dib.height;
		ESP_LOGD(__FUNCTION__,"w=%d h=%d", (int)w, (int)h);
		int32_t _x;
		int32_t _w;
		int32_t _cols;
		int32_t _cole;
		if (width >= w) {
			_x = (width - w) / 2;
			_w = w;
			_cols = 0;
			_cole = w - 1;
		} else {
			_x = 0;
			_w = width;
			_cols = (w - width) / 2;
			_cole = _cols + width - 1;
		}
		ESP_LOGD(__FUNCTION__,"_x=%d _w=%d _cols=%d _cole=%d",(int)_x, (int)_w, (int)_cols, (int)_cole);

		int32_t _y;
		int32_t _rows;
		int32_t _rowe;
		if (height >= h) {
			_y = (height - h) / 2;
			_rows = 0;
			_rowe = h -1;
		} else {
			_y = 0;
			_rows = (h - height) / 2;
			_rowe = _rows + height - 1;
		}
		ESP_LOGD(__FUNCTION__,"_y=%d _rows=%d _rowe=%d", (int)_y, (int)_rows, (int)_rowe);

#define BUFFPIXEL 20
		uint8_t sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * w);

		for (int32_t row=0; row<h; row++) { // For each scanline...
			if (row < _rows || row > _rowe) continue;
			// Seek to start of scan line.	It might seem labor-
			// intensive to be doing this on every line, but this
			// method covers a lot of gritty details like cropping
			// and scanline padding.  Also, the seek only takes
			// place if the file position actually needs to change
			// (avoids a lot of cluster math in SD library).
			// Bitmap is stored bottom-to-top order (normal BMP)
			int32_t pos = result->header.offset + (h - 1 - row) * rowSize;
			fseek(fp, pos, SEEK_SET);
			int32_t buffidx = sizeof(sdbuffer); // Force buffer reload

			int32_t index = 0;
			for (int32_t col=0; col<w; col++) { // For each pixel...
				if (buffidx >= sizeof(sdbuffer)) { // Indeed
					fread(sdbuffer, sizeof(sdbuffer), 1, fp);
					buffidx = 0; // Set index to beginning
				}
				if (col < _cols || col > _cole) continue;
				// Convert pixel from BMP to TFT format, push to display
				uint8_t b = sdbuffer[buffidx++];
				uint8_t g = sdbuffer[buffidx++];
				uint8_t r = sdbuffer[buffidx++];
				colors[index++] = rgb565(r, g, b);
			} // end for col
			ESP_LOGD(__FUNCTION__,"lcdDrawMultiPixels _x=%d _y=%d row=%d",(int)_x, (int)_y, (int)row);
			//lcdDrawMultiPixels(dev, _x, row+_y, _w, colors);
			lcdDrawMultiPixels(dev, _x, _y, _w, colors);
			_y++;
		} // end for row
		free(colors);
	} // end if
	lcdWriteFrame(dev);
	free(result);
	fclose(fp);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t QRTest(TFT_t *dev, char * file, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);

	// open requested file
	esp_err_t ret;
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	// read bmp header
	bmpfile_t *result = (bmpfile_t*)malloc(sizeof(bmpfile_t));
	ret = fread(result->header.magic, 1, 2, fp);
	assert(ret == 2);
	if (result->header.magic[0]!='B' || result->header.magic[1] != 'M') {
		ESP_LOGW(__FUNCTION__, "File is not BMP");
		free(result);
		fclose(fp);
		return 0;
	}
	ret = fread(&result->header.filesz, 4, 1 , fp);
	assert(ret == 1);
	ESP_LOGD(__FUNCTION__,"result->header.filesz=%"PRIu32, result->header.filesz);
	ret = fread(&result->header.creator1, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.creator2, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->header.offset, 4, 1, fp);
	assert(ret == 1);

	// read dib header
	ret = fread(&result->dib.header_sz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.width, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.height, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nplanes, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.depth, 2, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.compress_type, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.bmp_bytesz, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.hres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.vres, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.ncolors, 4, 1, fp);
	assert(ret == 1);
	ret = fread(&result->dib.nimpcolors, 4, 1, fp);
	assert(ret == 1);

	ESP_LOGD(__FUNCTION__, "dib.depth=%hd dib.compress_type=%"PRIu32, result->dib.depth, result->dib.compress_type);
	//if((result->dib.depth == 24) && (result->dib.compress_type == 0)) {
	if((result->dib.depth == 1) && (result->dib.compress_type == 0)) {
		ESP_LOGD(__FUNCTION__, "dib.bmp_bytesz=%"PRIu32, result->dib.bmp_bytesz);
		// BMP rows are padded (if needed) to 4-byte boundary
		//uint32_t rowSize = (result->dib.width * 3 + 3) & ~3;
		int32_t w = result->dib.width;
		int32_t h = result->dib.height;
		uint32_t rowSize = result->dib.bmp_bytesz / result->dib.height;
		ESP_LOGD(__FUNCTION__,"dib.width=%"PRIu32" dib.height=%"PRIu32" rowSize=%"PRIu32, result->dib.width, result->dib.height, rowSize);
		int32_t _x;
		int32_t _w;
		int32_t _cols;
		int32_t _cole;
		if (width >= w) {
			_x = (width - w) / 2;
			_w = w;
			_cols = 0;
			_cole = w - 1;
		} else {
			_x = 0;
			_w = width;
			_cols = (w - width) / 2;
			_cole = _cols + width - 1;
		}
		ESP_LOGD(__FUNCTION__,"_x=%d _w=%d _cols=%d _cole=%d",(int)_x, (int)_w, (int)_cols, (int)_cole);

		int32_t _y;
		int32_t _rows;
		int32_t _rowe;
		if (height >= h) {
			_y = (height - h) / 2;
			_rows = 0;
			_rowe = h -1;
		} else {
			_y = 0;
			_rows = (h - height) / 2;
			_rowe = _rows + height - 1;
		}
		ESP_LOGD(__FUNCTION__,"_y=%d _rows=%d _rowe=%d", (int)_y, (int)_rows, (int)_rowe);

		uint8_t *sdbuffer = (uint8_t*)malloc(rowSize); // pixel buffer
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _w); // tft buffer

		int32_t debug = 0; // number of logging output
		for (int32_t row=0; row<h; row++) { // For each scanline...
			if (row < _rows || row > _rowe) continue;
			// Seek to start of scan line.	It might seem labor-
			// intensive to be doing this on every line, but this
			// method covers a lot of gritty details like cropping
			// and scanline padding.  Also, the seek only takes
			// place if the file position actually needs to change
			// (avoids a lot of cluster math in SD library).
			// Bitmap is stored bottom-to-top order (normal BMP)
			int32_t pos = result->header.offset + (h - 1 - row) * rowSize;
			ESP_LOGD(__FUNCTION__,"pos=%d 0x%x", (int)pos, (int)pos);
			fseek(fp, pos, SEEK_SET);
			fread(sdbuffer, rowSize, 1, fp);
			int32_t buffidx = 0;
			if (debug > 0) {
				ESP_LOGI(__FUNCTION__, "sdbuffer");
				ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, sdbuffer, rowSize, ESP_LOG_INFO);
			}

			//int32_t buffidx = sizeof(sdbuffer); // Force buffer reload

			int32_t index = 0;
			uint8_t mask = 0x80;
			for (int32_t col=0; col<w; col++) { // For each pixel...
				if (col < _cols || col > _cole) continue;
				// Convert pixel from BMP to TFT format, push to display
				colors[index] = BLACK;
				if ( (sdbuffer[buffidx] & mask) != 0) colors[index] = WHITE;
				index++;
				mask = mask >> 1;
				if (mask == 0x00) {
					buffidx++;
					mask = 0x80;
				}

				//uint8_t b = sdbuffer[buffidx++];
				//uint8_t g = sdbuffer[buffidx++];
				//uint8_t r = sdbuffer[buffidx++];
				//colors[index++] = rgb565(r, g, b);
			} // end for col
			ESP_LOGD(__FUNCTION__,"lcdDrawMultiPixels _x=%d _y=%d row=%d",(int)_x, (int)_y, (int)row);
			if (debug > 0) {
				ESP_LOGI(__FUNCTION__, "colors");
				ESP_LOG_BUFFER_HEXDUMP(__FUNCTION__, colors, _w*2, ESP_LOG_INFO);
			}
			lcdDrawMultiPixels(dev, _x, _y, _w, colors);
			debug--;
			_y++;
		} // end for row
		free(sdbuffer);
		free(colors);
	} // end if
	lcdWriteFrame(dev);
	free(result);
	fclose(fp);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}



TickType_t JPEGTest(TFT_t *dev, char * file, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);


	pixel_jpeg **pixels;
	int32_t imageWidth;
	int32_t imageHeight;
	esp_err_t err = decode_jpeg(&pixels, file, width, height, &imageWidth, &imageHeight);
	ESP_LOGD(__FUNCTION__, "decode_image err=%d imageWidth=%d imageHeight=%d", (int)err, (int)imageWidth, (int)imageHeight);
	if (err == ESP_OK) {

		int32_t _width = width;
		int32_t _cols = 0;
		if (width > imageWidth) {
			_width = imageWidth;
			_cols = (width - imageWidth) / 2;
		}
		ESP_LOGD(__FUNCTION__, "_width=%hu _cols=%hu", _width, _cols);

		int32_t _height = height;
		int32_t _rows = 0;
		if (height > imageHeight) {
			_height = imageHeight;
			_rows = (height - imageHeight) / 2;
		}
		ESP_LOGD(__FUNCTION__, "_height=%hu _rows=%hu", _height, _rows);
		uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);

#if 0
		for(int32_t y = 0; y < _height; y++){
			for(int32_t x = 0;x < _width; x++){
				pixel_jpeg pixel = pixels[y][x];
				uint16_t color = rgb565(pixel.red, pixel.green, pixel.blue);
				lcdDrawPixel(dev, x+_cols, y+_rows, color);
			}
			vTaskDelay(1);
		}
#endif

		for(int32_t y = 0; y < _height; y++){
			for(int32_t x = 0;x < _width; x++){
				//pixel_jpeg pixel = pixels[y][x];
				//colors[x] = rgb565(pixel.red, pixel.green, pixel.blue);
				colors[x] = pixels[y][x];
			}
			lcdDrawMultiPixels(dev, _cols, y+_rows, _width, colors);
			vTaskDelay(1);
		}

		lcdWriteFrame(dev);
		free(colors);
		release_image(&pixels, width, height);
		ESP_LOGD(__FUNCTION__, "Finish");
	} else {
		ESP_LOGE(__FUNCTION__, "decode_jpeg fail=%d", (int)err);
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t PNGTest(TFT_t *dev, char * file, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	lcdSetFontDirection(dev, 0);
	lcdFillScreen(dev, BLACK);

	// open PNG file
	FILE* fp = fopen(file, "rb");
	if (fp == NULL) {
		ESP_LOGW(__FUNCTION__, "File not found [%s]", file);
		return 0;
	}

	char buf[1024];
	size_t remain = 0;
	int32_t len;

	pngle_t *pngle = pngle_new(width, height);

	pngle_set_init_callback(pngle, png_init);
	pngle_set_draw_callback(pngle, png_draw);
	pngle_set_done_callback(pngle, png_finish);

	double display_gamma = 2.2;
	pngle_set_display_gamma(pngle, display_gamma);


	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			ESP_LOGE(__FUNCTION__, "Buffer exceeded");
			while(1) vTaskDelay(1);
		}

		len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
		if (len <= 0) {
			//printf("EOF\n");
			break;
		}

		int32_t fed = pngle_feed(pngle, buf, remain + len);
		if (fed < 0) {
			ESP_LOGE(__FUNCTION__, "ERROR; %s", pngle_error(pngle));
			while(1) vTaskDelay(1);
		}

		remain = remain + len - fed;
		if (remain > 0) memmove(buf, buf + fed, remain);
	}

	fclose(fp);

	int32_t _width = width;
	int32_t _cols = 0;
	if (width > pngle->imageWidth) {
		_width = pngle->imageWidth;
		_cols = (width - pngle->imageWidth) / 2;
	}
	ESP_LOGD(__FUNCTION__, "_width=%hu _cols=%hu", _width, _cols);

	int32_t _height = height;
	int32_t _rows = 0;
	if (height > pngle->imageHeight) {
			_height = pngle->imageHeight;
			_rows = (height - pngle->imageHeight) / 2;
	}
	ESP_LOGD(__FUNCTION__, "_height=%hu _rows=%hu", _height, _rows);
	uint16_t *colors = (uint16_t*)malloc(sizeof(uint16_t) * _width);

#if 0
	for(int32_t y = 0; y < _height; y++){
		for(int32_t x = 0;x < _width; x++){
			pixel_png pixel = pngle->pixels[y][x];
			uint16_t color = rgb565(pixel.red, pixel.green, pixel.blue);
			lcdDrawPixel(dev, x+_cols, y+_rows, color);
		}
	}
#endif

	for(int32_t y = 0; y < _height; y++){
		for(int32_t x = 0;x < _width; x++){
			//pixel_png pixel = pngle->pixels[y][x];
			//colors[x] = rgb565(pixel.red, pixel.green, pixel.blue);
			colors[x] = pngle->pixels[y][x];
		}
		lcdDrawMultiPixels(dev, _cols, y+_rows, _width, colors);
		vTaskDelay(1);
	}
	lcdWriteFrame(dev);
	free(colors);
	pngle_destroy(pngle, width, height);

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}

TickType_t WrapArroundTest(TFT_t *dev, int32_t width, int32_t height) {
	TickType_t startTick, endTick, diffTick;
	startTick = xTaskGetTickCount();

	for (int32_t i=0;i<width;i++) {
		lcdWrapArround(dev, SCROLL_RIGHT, 0, height-1);
		if ((i % 2) == 1) {
			lcdWriteFrame(dev);
		}
	}

	for (int32_t i=0;i<width;i++) {
		lcdWrapArround(dev, SCROLL_LEFT, 0, height-1);
		if ((i % 2) == 1) {
			lcdWriteFrame(dev);
		}
	}

	for (int32_t i=0;i<height;i++) {
		lcdWrapArround(dev, SCROLL_UP, 0, width-1);
		if ((i % 2) == 1) {
			lcdWriteFrame(dev);
		}
	}

	for (int32_t i=0;i<height;i++) {
		lcdWrapArround(dev, SCROLL_DOWN, 0, width-1);
		if ((i % 2) == 1) {
			lcdWriteFrame(dev);
		}
	}

	endTick = xTaskGetTickCount();
	diffTick = endTick - startTick;
	ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
	return diffTick;
}
#endif /* IMAGE */

void LCD(void *pvParameters)
{
	// set font file
	FontxFile fx16G[2];
	FontxFile fx24G[2];
	FontxFile fx32G[2];
	FontxFile fx32L[2];
	InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot Gothic
	InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
	InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
	InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot Latin

	FontxFile fx16M[2];
	FontxFile fx24M[2];
	FontxFile fx32M[2];
	InitFontx(fx16M,"/spiffs/ILMH16XB.FNT",""); // 8x16Dot Mincyo
	InitFontx(fx24M,"/spiffs/ILMH24XB.FNT",""); // 12x24Dot Mincyo
	InitFontx(fx32M,"/spiffs/ILMH32XB.FNT",""); // 16x32Dot Mincyo

	TFT_t dev;

	lcdInit(&dev);

	while(1) {

		FillTest(&dev, LCD_W, LCD_H);
		WAIT;

		ColorBarTest(&dev, LCD_W, LCD_H);
		WAIT;

		ColorBandTest(&dev, LCD_W, LCD_H);
		WAIT;

		ArrowTest(&dev, fx16G, LCD_W, LCD_H);
		WAIT;

		LineTestHV(&dev, LCD_W, LCD_H);
		WAIT;

		LineTest(&dev, LCD_W, LCD_H);
		WAIT;

		CircleTest(&dev, LCD_W, LCD_H);
		WAIT;

		RoundRectTest(&dev, LCD_W, LCD_H);
		WAIT;

		FillRectTest(&dev, LCD_W, LCD_H);
		WAIT;

		FillTriTest(&dev, LCD_W, LCD_H);
		WAIT;

		FillCircleTest(&dev, LCD_W, LCD_H);
		WAIT;

		if (dev._use_frame_buffer == false) {
			RectangleTest(&dev, LCD_W, LCD_H);
			WAIT;

			TriangleTest(&dev, LCD_W, LCD_H);
			WAIT;
		}

		if (LCD_W >= 240) {
			DirectionTest(&dev, fx24G, LCD_W, LCD_H);
		} else {
			DirectionTest(&dev, fx16G, LCD_W, LCD_H);
		}
		WAIT;

		if (LCD_W >= 240) {
			HorizontalTest(&dev, fx24G, LCD_W, LCD_H);
		} else {
			HorizontalTest(&dev, fx16G, LCD_W, LCD_H);
		}
		WAIT;

		if (LCD_W >= 240) {
			VerticalTest(&dev, fx24G, LCD_W, LCD_H);
		} else {
			VerticalTest(&dev, fx16G, LCD_W, LCD_H);
		}
		WAIT;

		CodeTest(&dev, fx32G, LCD_W, LCD_H);
		WAIT;

		CodeTest(&dev, fx32L, LCD_W, LCD_H);
		WAIT;

#if defined(IMAGE)
		char file[32];
		strcpy(file, "/spiffs/image.bmp");
		BMPTest(&dev, file, LCD_W, LCD_H);
		WAIT;

		strcpy(file, "/spiffs/esp32.jpeg");
		JPEGTest(&dev, file, LCD_W, LCD_H);
		WAIT;

		strcpy(file, "/spiffs/esp_logo.png");
		PNGTest(&dev, file, LCD_W, LCD_H);
		WAIT;

		if (dev._use_frame_buffer == true) {
			WrapArroundTest(&dev, LCD_W, LCD_H);
			WAIT;
		}

		strcpy(file, "/spiffs/qrcode.bmp");
		QRTest(&dev, file, LCD_W, LCD_H);
		WAIT;
#endif /* IMAGE */

		// Multi Font Test
		uint16_t color;
		uint8_t ascii[40];
		int32_t margin = 10;
		lcdFillScreen(&dev, BLACK);
		color = WHITE;
		lcdSetFontDirection(&dev, 0);
		int32_t xpos = 0;
		int32_t ypos = 15;
		int32_t xd = 0;
		int32_t yd = 1;
		if(LCD_W < LCD_H) {
			lcdSetFontDirection(&dev, 1);
			xpos = (LCD_W-1)-16;
			ypos = 0;
			xd = 1;
			yd = 0;
		}
		strcpy((char *)ascii, "16Dot Gothic Font");
		lcdDrawString(&dev, fx16G, xpos, ypos, ascii, color);

		xpos = xpos - (24 * xd) - (margin * xd);
		ypos = ypos + (16 * yd) + (margin * yd);
		strcpy((char *)ascii, "24Dot Gothic Font");
		lcdDrawString(&dev, fx24G, xpos, ypos, ascii, color);

		xpos = xpos - (32 * xd) - (margin * xd);
		ypos = ypos + (24 * yd) + (margin * yd);
		if (LCD_W >= 240) {
			strcpy((char *)ascii, "32Dot Gothic Font");
			lcdDrawString(&dev, fx32G, xpos, ypos, ascii, color);
			xpos = xpos - (32 * xd) - (margin * xd);;
			ypos = ypos + (32 * yd) + (margin * yd);
		}

		xpos = xpos - (10 * xd) - (margin * xd);
		ypos = ypos + (10 * yd) + (margin * yd);
		strcpy((char *)ascii, "16Dot Mincyo Font");
		lcdDrawString(&dev, fx16M, xpos, ypos, ascii, color);

		xpos = xpos - (24 * xd) - (margin * xd);;
		ypos = ypos + (16 * yd) + (margin * yd);
		strcpy((char *)ascii, "24Dot Mincyo Font");
		lcdDrawString(&dev, fx24M, xpos, ypos, ascii, color);

		if (LCD_W >= 240) {
			xpos = xpos - (32 * xd) - (margin * xd);;
			ypos = ypos + (24 * yd) + (margin * yd);
			strcpy((char *)ascii, "32Dot Mincyo Font");
			lcdDrawString(&dev, fx32M, xpos, ypos, ascii, color);
		}
		lcdWriteFrame(&dev);
		lcdSetFontDirection(&dev, 0);
		WAIT;

	} // end while
}
