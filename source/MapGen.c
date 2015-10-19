#include "MapGen.h"

int w = 0;
int h = 0;

float nextFloat(){
    return (float)rand()/RAND_MAX;
}

double sample(double * values, int x, int y) {
	return values[(x & (w - 1)) + (y & (h - 1)) * w];
}

double * Noise(int width, int height, int featureSize) {
        w = width;
        h = height;
		double * values = malloc(sizeof(double) * w * h);
        int x, y;
		for(x = 0; x < w; x+=featureSize){
            for(y = 0; y < w; y+=featureSize){
				values[(x & (w - 1)) + (y & (h - 1)) * w] = nextFloat() * 2 - 1;
			}
		}

		int stepSize = featureSize;
		double scale = 1.0 / w;
		double scaleMod = 1;
		do {
			int halfStep = stepSize / 2;
			for(x = 0; x < w; x+=stepSize){
                for(y = 0; y < w; y+=stepSize){
					double a = sample(values,x, y);
					double b = sample(values,x + stepSize, y);
					double c = sample(values,x, y + stepSize);
					double d = sample(values,x + stepSize, y + stepSize);

					double e = (a + b + c + d) / 4.0 + (nextFloat() * 2 - 1) * stepSize * scale;
					values[((x + halfStep) & (w - 1)) + ((y + halfStep) & (h - 1)) * w] = e;
                }
            }
			for(x = 0; x < w; x+=stepSize){
                for(y = 0; y < w; y+=stepSize){
					double a = sample(values,x, y);
					double b = sample(values,x + stepSize, y);
					double c = sample(values,x, y + stepSize);
					double d = sample(values,x + halfStep, y + halfStep);
					double e = sample(values,x + halfStep, y - halfStep);
					double f = sample(values,x - halfStep, y + halfStep);
					double H = (a + b + d + e) / 4.0 + (nextFloat() * 2 - 1) * stepSize * scale * 0.5;
					double g = (a + c + d + f) / 4.0 + (nextFloat() * 2 - 1) * stepSize * scale * 0.5;
					values[((x + halfStep) & (w - 1)) + (y & (h - 1)) * w] = H;
		            values[(x & (w - 1)) + ((y + halfStep) & (h - 1)) * w] = g;
                }
            }
			
			stepSize /= 2;
			scale *= (scaleMod + 0.8);
			scaleMod *= 0.3;
		} while (stepSize > 1);
		return values;
	}
	
void newSeed(){
    srand(time(NULL));
}
	
void createAndValidateTopMap(int w, int h, u8 * map, u8 * data) {
	do {
		createTopMap(w, h, map, data);

		int count[256]={[0 ... 255] = 0};
        int i;
        for (i = 0; i < w * h; ++i) count[map[i] & 0xff]++;
		if (count[TILE_ROCK & 0xff] < 100) continue;
		if (count[TILE_SAND & 0xff] < 100) continue;
		if (count[TILE_GRASS & 0xff] < 100) continue;
		if (count[TILE_TREE & 0xff] < 100) continue;
		if (count[TILE_STAIRS_DOWN & 0xff] < 2) continue;
            
		return;
	} while (true);
}

void createAndValidateUndergroundMap(int w, int h,int depthLevel, u8 * map, u8 * data) {
	do {
		createUndergroundMap(w, h, depthLevel, map, data);
        
		int count[256]={[0 ... 255] = 0};
        int i = 0;
        for (i = 0; i < w * h; ++i)count[map[i] & 0xff]++;
		if (count[TILE_ROCK & 0xff] < 100) continue;
		if (count[TILE_DIRT & 0xff] < 100) continue;
		switch(depthLevel){
            case 1: if (count[TILE_IRONORE & 0xff] < 20) continue; break;
            case 2: if (count[TILE_GOLDORE & 0xff] < 20) continue; break;
            case 3: if (count[TILE_GEMORE & 0xff] < 20) continue; break;
        }
		if (depthLevel < 3) if (count[TILE_STAIRS_DOWN & 0xff] < 2) continue;
            
		return;
	} while (true);
}
void createAndValidateSkyMap(int w, int h, u8 * map, u8 * data) {
	do {
		createSkyMap(w, h, map, data);
        
		int count[256]={[0 ... 255] = 0};
        int i = 0;
        for (i = 0; i < w * h; ++i)count[map[i] & 0xff]++;
		if (count[TILE_CLOUD & 0xff] < 1600) continue;
		if (count[TILE_STAIRS_DOWN & 0xff] < 2) continue;
            
		return;
	} while (true);
}
	
	
	
void createTopMap(int w, int h, u8 * map, u8 * data) {
		double* mnoise1 = Noise(w, h, 16);
		double* mnoise2 = Noise(w, h, 16);
		double* mnoise3 = Noise(w, h, 16);
		double* noise1 = Noise(w, h, 32);
		double* noise2 = Noise(w, h, 32);
        
		int x,y,i,j,k,xx,yy;
		
		for(x = 0; x < w; ++x){
            for(y = 0; y < w; ++y){
                int i = x + y * w;

				double val = fabs(noise1[i] - noise2[i]) * 3 - 2;
				double mval = fabs(mnoise1[i] - mnoise2[i]);
				mval = fabs(mval - mnoise3[i]) * 3 - 2;

				double xd = x / (w - 1.0) * 2 - 1;
				double yd = y / (h - 1.0) * 2 - 1;
				if (xd < 0) xd = -xd;
				if (yd < 0) yd = -yd;
				double dist = xd >= yd ? xd : yd;
				dist = dist * dist * dist * dist;
				dist = dist * dist * dist * dist;
				val = val + 1 - dist * 20; //-2 before to -21.0 after
                
				if (val < -0.5) {
					map[i] = TILE_WATER;
				} else if (val > 0.5 && mval < -1) {
					map[i] = TILE_ROCK;
				} else {
					map[i] = TILE_GRASS;
				}
            }
        }
        
        for (i = 0; i < w * h / 2800; ++i) {
		int xs = rand()%w;
		int ys = rand()%h;
		  for (k = 0; k < 10; ++k) {
			x = xs + (rand()%21) - 10;
			y = ys + (rand()%21) - 10;
			for (j = 0; j < 100; ++j) {
				int xo = x + (rand()%5) - (rand()%5);
				int yo = y + (rand()%5) - (rand()%5);
				    for (yy = yo - 1;yy <= yo + 1; ++yy){
					   for(xx = xo - 1; xx <= xo + 1; ++xx){
						  if (xx >= 0 && yy >= 0 && xx < w && yy < h) if (map[xx + yy * w] == TILE_GRASS) map[xx + yy * w] = TILE_SAND;
                        }
				    } 
                }
			}
		}
        
        for (i = 0; i < w * h / 400; ++i) {
			x = rand()%w;
			y = rand()%h;
			for (j = 0; j < 200; ++j) {
				xx = x + (rand()%15) - (rand()%15);
				yy = y + (rand()%15) - (rand()%15);
				if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
					if (map[xx + yy * w] == TILE_GRASS) {
						map[xx + yy * w] = TILE_TREE;
					}
				}
			}
		}
		
        for (i = 0; i < w * h / 800; ++i) {
			x = rand()%w;
			y = rand()%h;
			for (j = 0; j < 30;++j) {
				xx = x + (rand()%5) - (rand()%5);
				yy = y + (rand()%5) - (rand()%5);
				if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
					if (map[xx + yy * w] == TILE_GRASS) {
						map[xx + yy * w] = TILE_FLOWER;
						data[xx + yy * w] = rand()%4; // determines mirroring.
					}
				}
				
			}
		}
		
		for (i = 0; i < w * h / 100; ++i) {
			xx = rand()%w;
			yy = rand()%h;
			if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				if (map[xx + yy * w] == TILE_SAND) {
					map[xx + yy * w] = TILE_CACTUS;
				}
			}
		}
		
        int sCount, attempts = 0;
		for (sCount = 0; sCount < 4;) {
			xx = rand()%w;
			yy = rand()%h;
			if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				if (map[xx + yy * w] == TILE_ROCK) 
                {
					map[xx + yy * w] = TILE_STAIRS_DOWN;
					map[xx + (yy+1) * w] = TILE_ROCK;
					map[(xx+1) + yy * w] = TILE_ROCK;
					map[xx + (yy-1) * w] = TILE_ROCK;
					map[(xx-1) + yy * w] = TILE_ROCK;
					map[(xx-1) + (yy-1) * w] = TILE_ROCK;
					map[(xx+1) + (yy+1) * w] = TILE_ROCK;
					map[(xx+1) + (yy-1) * w] = TILE_ROCK;
					map[(xx-1) + (yy+1) * w] = TILE_ROCK;
                    ++sCount;
				}
			}
			if(attempts < (w*h/100)) ++attempts; else break;
		}
        free(mnoise1);
        free(mnoise2);
        free(mnoise3);
        free(noise1);
        free(noise2);
        return;
}

void createUndergroundMap(int w, int h,int depthLevel, u8 * map, u8 * data) {
		double* mnoise1 = Noise(w, h, 16);
		double* mnoise2 = Noise(w, h, 16);
		double* mnoise3 = Noise(w, h, 16);
		
		double* nnoise1 = Noise(w, h, 16);
		double* nnoise2 = Noise(w, h, 16);
		double* nnoise3 = Noise(w, h, 16);
		
		double* wnoise1 = Noise(w, h, 16);
		double* wnoise2 = Noise(w, h, 16);
		double* wnoise3 = Noise(w, h, 16);
		
		double* noise1 = Noise(w, h, 32);
		double* noise2 = Noise(w, h, 32);
		
		int x, y;
		for(x = 0; x < w; ++x){
            for(y = 0; y < w; ++y){
                int i = x + y * w;

				double val = fabs(noise1[i] - noise2[i]) * 3 - 2;

				double mval = fabs(mnoise1[i] - mnoise2[i]);
				mval = fabs(mval - mnoise3[i]) * 3 - 2;

				double nval = fabs(nnoise1[i] - nnoise2[i]);
				nval = fabs(nval - nnoise3[i]) * 3 - 2;
				
				double wval = fabs(wnoise1[i] - wnoise2[i]);
				wval = fabs(nval - wnoise3[i]) * 3 - 2;

				double xd = x / (w - 1.0) * 2 - 1;
				double yd = y / (h - 1.0) * 2 - 1;
				if (xd < 0) xd = -xd;
				if (yd < 0) yd = -yd;
				double dist = xd >= yd ? xd : yd;
				dist = dist * dist * dist * dist;
				dist = dist * dist * dist * dist;
				val = val + 1 - dist * 20;
                
				if (val > -2 && wval < -2.0 + (depthLevel) / 2 * 3) {
					if (depthLevel > 2)
						map[i] = TILE_LAVA;
					else
						map[i] = TILE_WATER;
				} else if (val > -2 && (mval < -1.7 || nval < -1.4)) {
					map[i] = TILE_DIRT;
				} else {
					map[i] = TILE_ROCK;
				}
            }
        }
        int i,j;
        for (i = 0; i < w * h / 400; ++i) {
			int x = rand()%w;
			int y = rand()%h;
			for(j = 0; j < 30; ++j) {
				int xx = x + (rand()%5) - (rand()%5);
				int yy = y + (rand()%5) - (rand()%5);
				if (xx >= 2 && yy >= 2 && xx < w-2 && yy < h-2) {
					if (map[xx + yy * w] == TILE_ROCK) {
						map[xx + yy * w] = TILE_IRONORE+depthLevel-1;
					}
				}
			}
		}
	    if (depthLevel < 3){
        int sCount, attempts = 0;
		  for (sCount = 0; sCount < 4;) {
			 int xx = rand()%w;
			 int yy = rand()%h;
			 if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				    if (map[xx + yy * w] == TILE_ROCK){
					   map[xx + yy * w] = TILE_STAIRS_DOWN;
					   map[xx + (yy+1) * w] = TILE_ROCK;
					   map[(xx+1) + yy * w] = TILE_ROCK;
					   map[xx + (yy-1) * w] = TILE_ROCK;
					   map[(xx-1) + yy * w] = TILE_ROCK;
					   map[(xx-1) + (yy-1) * w] = TILE_ROCK;
					   map[(xx+1) + (yy+1) * w] = TILE_ROCK;
					   map[(xx+1) + (yy-1) * w] = TILE_ROCK;
					   map[(xx-1) + (yy+1) * w] = TILE_ROCK;
					   ++sCount;
				    }
			 }
			 if(attempts < (w*h/100)) ++attempts; else break;
		  }
        }
        free(mnoise1);
        free(mnoise2);
        free(mnoise3);
        free(nnoise1);
        free(nnoise2);
        free(nnoise3);
        free(wnoise1);
        free(wnoise2);
        free(wnoise3);
        free(noise1);
        free(noise2);
        
}
	void createSkyMap(int w, int h, u8 * map, u8 * data) {
		double* noise1 = Noise(w, h, 8);
		double* noise2 = Noise(w, h, 8);
		int x, y;
		for(x = 0; x < w; ++x){
            for(y = 0; y < w; ++y){
                int i = x + y * w;

				double val = fabs(noise1[i] - noise2[i]) * 3 - 2;

				double xd = x / (w - 1.0) * 2 - 1;
				double yd = y / (h - 1.0) * 2 - 1;
				if (xd < 0) xd = -xd;
				if (yd < 0) yd = -yd;
				double dist = xd >= yd ? xd : yd;
				dist = dist * dist * dist * dist;
				dist = dist * dist * dist * dist;
				val = -val * 1 - 2.2;
				val = val + 1 - dist * 20;
                
				if (val < -0.25) {
					map[i] = -1; // render nothing
				} else {
					map[i] = TILE_CLOUD;
				}
            }
        }
        int i;
        for (i = 0; i < w * h / 50; ++i) {
			int xx = rand()%w;
			int yy = rand()%h;
			if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				if (map[xx + yy * w] == TILE_CLOUD) map[xx + yy * w] = TILE_CLOUDCACTUS;
			}
		}
        int sCount, attempts = 0;
		for (sCount = 0; sCount < 2;) {
			int xx = rand()%w;
			int yy = rand()%h;
			if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				if (map[xx + yy * w] == TILE_CLOUD) 
                {
					map[xx + yy * w] = TILE_STAIRS_DOWN;
					map[xx + (yy+1) * w] = TILE_CLOUD;
					map[(xx+1) + yy * w] = TILE_CLOUD;
					map[xx + (yy-1) * w] = TILE_CLOUD;
					map[(xx-1) + yy * w] = TILE_CLOUD;
					map[(xx-1) + (yy-1) * w] = TILE_CLOUD;
					map[(xx+1) + (yy+1) * w] = TILE_CLOUD;
					map[(xx+1) + (yy-1) * w] = TILE_CLOUD;
					map[(xx-1) + (yy+1) * w] = TILE_CLOUD;
					++sCount;
				}
			}
			if(attempts < w*h) ++attempts; else break;
		}
        free(noise1);
        free(noise2);
    }
	
