#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h> 
#include <conio.h>

#define WINDOW_SIZE 512
#define K_MAX 256
#define N 512

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

int total_samples,total_time;

void computeRk(short *data,int cur, double *R) {
    for (int k = 0; k <= K_MAX; k++) {
        R[k] = 0.0;
        for (int n = 0; n < N - k; n++) {
            R[k] += data[n + cur] * data[n + cur + k];
        }
    }
}

void drawngang(int start, int end, int y, int step){
	for (int x = start; x < end; x += step) {
        if ((x / step) % 2 == 0) {
            setcolor(YELLOW); 
        } else {
            setcolor(LIGHTGRAY); 
        }
        line(x, y, x + step, y);
    }
}

void drawdoc(int start, int end, int x, int step){
	for (int y = start; y <= end; y += step) {
        if ((y / step) % 2 == 0) {
            setcolor(YELLOW); 
        } else {
            setcolor(LIGHTGRAY);
        }
        line(x, y,x, y+step); 
    }	
}

void drawwhiteline(int x, int y, int x2, int y2, int *l){
	for (int i=y;i<=y2;i++){
		l[i-y]=getpixel(x,i);	
	}
	setcolor(WHITE);
	line(x,y,x2,y2);
}

void redraw(int x, int y, int x2, int y2, int *l){
	for (int i=y;i<=y2;i++){
		putpixel(x,i,l[i-y]);	
	}
}

void drawxn(short *data, int cur){
	setfillstyle(SOLID_FILL, BLACK);
	bar(50,200,475,350);
	drawngang(50,475,275,3);
	drawngang(50,475,237.5,3);
	drawngang(50,475,312.5,3);
	drawngang(50,475,350,3);
	drawdoc(200,350,150,3);
	drawdoc(200,350,250,3);
	drawdoc(200,350,350,3);
	drawdoc(200,350,450,3);
	setcolor(LIGHTGREEN);
	float prex,prey,curx,cury;
	int width = 425;
    int height = 150;
	int count =0;
	prex=50;prey=275;
	for (int i=0;i<N;i++) {
		curx=(float)(50)+(float)(count*width)/(float)(N);
		cury=(float)(200)+(float)((20000-data[cur+i])/(float)(20000*2)*(float)height);
		line(prex,prey,curx,cury);
	    prex=curx;prey=cury;
	    count++;
	}
}

void drawrk(short *data, int cur, double *R){
	setfillstyle(SOLID_FILL, BLACK);
	bar(525,200,950,350);
	drawngang(525,950,275,3);
	drawngang(525,950,237.5,3);
	drawngang(525,950,312.5,3);
	drawngang(525,950,350,3);
	drawdoc(200,350,625,3);
	drawdoc(200,350,725,3);
	drawdoc(200,350,825,3);
	drawdoc(200,350,925,3);
	setcolor(LIGHTGREEN);
	computeRk(data,cur,R);
	double max=DBL_MIN,min=DBL_MAX,max2=DBL_MIN;
	int start=0;
	for (int i=0;i<=K_MAX;i++){
		if (start==0 and i>0 and R[i]>R[i-1]){
			start=1;
		}
		if (R[i]>max){
			max=R[i];
		}
		if (R[i]<min){
			min=R[i];
		}
		if (R[i]>max2 and start==1){
			max2=R[i];
		}
	}
	setcolor(LIGHTGREEN);
	setbkcolor(BLACK);
	char str[20];
	char str1[100] = "Max: ";
    sprintf(str, "%e", max);
    strcat(str1,str);
    char str2[100] =". Min: ";
    strcat(str1,str2);
    sprintf(str, "%e", min);
    strcat(str1,str);
	outtextxy(527, 205, (char*)(str1));
	int prex,prey,curx,cury;
	prex=525;prey=200;
	for (int i=0;i<=K_MAX;i++){
		curx=(float)(525)+(float)(i*425/K_MAX);
		cury=(float)(275)+(float)(-R[i]*75/max);
		if (R[i]==max2){
			setcolor(RED);
			line(curx,200,curx,350);
			setcolor(LIGHTGREEN);	
			int f=(float)((16000)/i);
			if (f<=390 and f>=160){
				circle((float)(50)+(float)(cur*900/total_samples),455-(float)((f-240)*75/160),4);
			}
		}
		line(prex,prey,curx,cury);
		prex=(float)(525)+(float)(i*425/K_MAX);
		prey=(float)(275)+(float)(-R[i]*75/max);
	}	
}

void drawblueline(int x,int *l){
	for (int i=20;i<=170;i++){
		l[i-20]=getpixel(x,i);	
	}
	setcolor(BLUE);
	line(x,20,x,170);
}

void drawredline(int x,int *l){
	for (int i=20;i<=170;i++){
		l[i-20]=getpixel(x,i);	
	}
	setcolor(RED);
	line(x,20,x,170);
}

void zoom(int start,int end, short *data){
	setfillstyle(SOLID_FILL, BLACK);
	bar(50,200,475,350);
	drawngang(50,475,275,3);
	drawngang(50,475,237.5,3);
	drawngang(50,475,312.5,3);
	drawngang(50,475,350,3);
	drawdoc(200,350,150,3);
	drawdoc(200,350,250,3);
	drawdoc(200,350,350,3);
	drawdoc(200,350,450,3);
	setcolor(LIGHTGREEN);
	int samples=(int)((end-start)*total_samples/900);
	int first_samples=(int)((start-50)*total_samples/900);
	float prex,prey,curx,cury;
	prey=(float)(275)-(float)(data[first_samples]*75/20000);
	prex=(float)(50);
	for (int i=first_samples+1;i<=first_samples+samples-1;i++){
		cury=(float)(275)-(float)(data[i]*75/20000);
		curx=(float)(50)+(float)(425*(i-first_samples)/(samples));
		line(prex,prey,curx,cury);
		prex=curx;prey=cury;
	}
	
}


int main() {
    FILE *file = fopen("xebesvexchef.wav", "rb");
    Header wavHeader;
    fread(&wavHeader, sizeof(Header), 1, file);
    int num_samples = wavHeader.subchunk2_size / (wavHeader.bits_per_sample / 8);
	int i, gd = DETECT, gm;
	//--------------------1-----------------------------
	initwindow(1000, 580, "Xebesvexchef");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor(BLACK);	
	setbkcolor(LIGHTGRAY);
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(0, 0, 1000, 20);
	bar(0, 20, 50, 170);
	bar(950, 20, 1000, 170);
	bar(0, 170, 1000, 200);
	outtextxy(35, 92, (char*)("0"));
	outtextxy(5, 20, (char*)("30000"));
	outtextxy(1, 160, (char*)("-30000"));
	drawngang(50,950,57.5,3);
	drawngang(50,950,95,3);
	drawngang(50,950,132.5,3);
	drawngang(50,950,170,3);
	drawdoc(20,170,150,3);
	drawdoc(20,170,250,3);
	drawdoc(20,170,350,3);
	drawdoc(20,170,450,3);
	drawdoc(20,170,550,3);
	drawdoc(20,170,650,3);
	drawdoc(20,170,750,3);
	drawdoc(20,170,850,3);
	
	setcolor(LIGHTGREEN);
	int width = 900;
    int height = 150;
	short int sample;
	short data[85000];
	float prex,prey,curx,cury;
	int count =50;
	prex=50;prey=95;
	while (fread(&sample, sizeof(short int), 1, file) == 1) {
		data[count-50]=sample;
		curx=(float)(50)+(float)(count*width)/(float)(85000);
		cury=(float)(20)+(float)((30000-sample)/(float)(30000*2)*(float)height);
		line(prex,prey,curx,cury);
	    prex=curx;prey=cury;
	    count++;
	}
	
	total_samples=count-50;
	total_time=(int)((total_samples)*1000/(wavHeader.sample_rate));
	char str[20];
    sprintf(str, "%d", total_time);
	setcolor(BLACK);
	outtextxy(955, 90, (char*)(str));
	outtextxy(960, 100, (char*)("ms"));
	for (int i=1;i<=8;i++){
		int time=(int)(total_time*i/9);
		sprintf(str, "%d", time);
		outtextxy(40+100*i, 175, (char*)(str));
	}
	
	
	//--------------------2-----------------------------
	
	setfillstyle(SOLID_FILL, LIGHTGRAY);
	bar(0, 200, 50, 350);
	bar(950, 200, 1000, 350);
	bar(0, 350, 1000, 380);
	bar(475,200,525,350);
	bar(0, 380, 50, 530);
	bar(950, 380, 1000, 530);
	bar(0, 530, 1000, 580);
	drawngang(50,950,417.5,3);
	drawngang(50,950,455,3);
	drawngang(50,950,492.5,3);
	drawngang(50,950,530,3);
	drawdoc(380,530,150,3);
	drawdoc(380,530,250,3);
	drawdoc(380,530,350,3);
	drawdoc(380,530,450,3);
	drawdoc(380,530,550,3);
	drawdoc(380,530,650,3);
	drawdoc(380,530,750,3);
	drawdoc(380,530,850,3);
	setcolor(BLACK);
	for (int i=1;i<=8;i++){
		int time=(int)(total_time*i/9);
		sprintf(str, "%d", time);
		outtextxy(40+100*i, 535, (char*)(str));
	}
	outtextxy(45, 535, (char*)("0"));
	outtextxy(20, 530, (char*)("80"));
	outtextxy(20, 492.5, (char*)("160"));
	outtextxy(20, 455, (char*)("240"));
	outtextxy(20, 417.5, (char*)("320"));
	outtextxy(20, 365, (char*)("400 Hz"));
	sprintf(str, "%d", total_time);
	outtextxy(930, 535, (char*)(str));
	outtextxy(970, 535, (char*)("ms"));


	int l1[155],l2[155];
	double R[K_MAX + 1];
	for (int x=0;x<total_samples-512;x+=256){
		drawwhiteline(50+(int)((float)(x*900/total_samples)),20,50+(int)((float)(x*900/total_samples)),170,l1);
		drawwhiteline(50+(int)((float)((x+512)*900/total_samples)),20,50+(int)((float)((x+512)*900/total_samples)),170,l2);
		drawxn(data,x);
		drawrk(data,x,R);
		delay(200);
		redraw(50+(int)((float)(x*900/total_samples)),20,50+(int)((float)(x*900/total_samples)),170,l1);
		redraw(50+(int)((float)((x+512)*900/total_samples)),20,50+(int)((float)((x+512)*900/total_samples)),170,l2);
	}
	
	drawblueline(150, l1);
	drawredline(100,l2);
	int red=100,blue=150;
	char comm;
	while (true){
		comm=getch();
		if (comm=='p'){
			if (red+1<blue){
				redraw(red,20,red,170,l2);
				red+=1;
				drawredline(red,l2);
			}	
		}
		else if (comm=='t'){
			if (red>50){
				redraw(red,20,red,170,l2);
				red-=1;
				drawredline(red,l2);
			}	
		}
		else if (comm=='P'){
			if (blue<950){
				redraw(blue,20,blue,170,l1);
				blue+=1;
				drawblueline(blue,l1);
			}	
		} 
		else if (comm=='T'){
			if (blue>red+1){
				redraw(blue,20,blue,170,l1);
				blue-=1;
				drawblueline(blue,l1);
			}	
		}
		else if (comm=='i'){
			zoom(red,blue,data);
		}
	}
    closegraph();	
}
