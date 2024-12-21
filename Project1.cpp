#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>

typedef struct header {
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    unsigned int subchunk2_size;
} Header;

int main() {
    FILE *file = fopen("xebesvexchef.wav", "rb");
    Header wavHeader;
    fread(&wavHeader, sizeof(Header), 1, file);

    int num_samples = wavHeader.subchunk2_size / (wavHeader.bits_per_sample / 8);

	int i, gd = DETECT, gm;
	initwindow(800, 150, "Xebesvexchef");
	setcolor(GREEN);
	
	int width = getmaxx();
    int height = getmaxy();
    line(0, height / 2, width, height / 2);
    line(0, 0, 0, height);
	short int sample;
	float prex,prey,curx,cury;
	int count =0;
	prex=0;prey=0;
	while (fread(&sample, sizeof(short int), 1, file) == 1) {
		curx=(float)(count*width)/(float)(85000);
		cury=(float)((29851-sample)/(float)(29851*2)*(float)height);
		line(prex,prey,curx,cury);
	    prex=curx;prey=cury;
	    count++;
	}
	getch();
    closegraph();	
    return 0;
}


