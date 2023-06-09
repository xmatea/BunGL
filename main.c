#include "math.h"
#include "shapes.h"
#include "matrices.h"
#include "color.h"
#include <stdlib.h>
#include <stdio.h>
#include "SDL_setup.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

struct mesh_s {
	matrix* vertices;
	matrix* indices;
	matrix* transformation;
	matrix* xyrotation;
	matrix* yzrotation;
	matrix* xzrotation;
	matrix* projection;
	matrix* translation;
	matrix* scale;
	matrix* xcords;
	matrix* ycords;
};

typedef struct mesh_s mesh;

void DrawLine(int x1, int y1, int x2, int y2, int color) {
	int dx, dy, dx1, dy1, px, py, x, y, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;

	dx1 = m_abs(dx);
	dy1 = m_abs(dy);

	px = 2*dy1 - 2*dx1;
	py = 2*dx1 - 2*dy1;

		// If line is X-dominant
		if (dy1 <= dx1) {

			//Line is drawn left to right
			if (dx >= 0) {
				x = x1; 
				y = y1; 
				xe = x2;
			} else { // Line is drawn right to left (swap ends)
				x = x2; 
				y = y2; 
				xe = x1;
			}


			//Bdisp_SetPoint_VRAM(x, y, color); // Drawing first pixel
			SDL_RenderDrawPoint(gRenderer, x, y);


			 // Rasterize the line
			for (i = 0; x < xe; i++) {
				x = x + 1;
				// Deal with octants...
				if (px < 0) {
					px = px + 2 * dy1; 
				} else {
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
						y = y + 1;
					} else {
						y = y - 1;
					}
					px = px + 2 * (dy1 - dx1);
				}

				
				// Draw pixel from line span at
				// currently rasterized position
				//Bdisp_SetPoint_VRAM(x, y, color);
				SDL_RenderDrawPoint(gRenderer, x, y);
			}

		} else { // The line is Y-axis dominant
			if (dy >= 0) {
				x = x1; 
				y = y1; 
				ye = y2;
			} else { // Line is drawn top to bottom
				x = x2;
				y = y2;
				ye = y1;
			}

		   // Bdisp_SetPoint_VRAM(x, y, color); // Drawing first pixel
			SDL_RenderDrawPoint(gRenderer, x, y);

			// Rasterize the line
			for (i = 0; y < ye; i++) {
				y = y + 1;
				// Deal with octants...
				if (py <= 0) {
					py = py + 2 * dx1;
				} else {
					if ((dx < 0 && dy<0) || (dx > 0 && dy > 0)) {
						x = x + 1;
					} else {
						x = x - 1;
					}
					py = py + 2 * (dx1 - dy1);
				}
				//Bdisp_SetPoint_VRAM(x, y, color);
				SDL_RenderDrawPoint(gRenderer, x, y);
			}
		}
	}

void DrawStraightLine(int x1, int y1, int x2, int y2, int color) {
	if ((x1 > SCREEN_WIDTH) || (x2 > SCREEN_WIDTH) || (y1 > SCREEN_HEIGHT) || (y2 > SCREEN_HEIGHT)) {
		return;
	}

	if (x1 > x2) {
		int temp = x2;
		x2 = x1;
		x1 = temp;
	} 
	int x = x1;
	int y = y1;
	for (int i = x1; i < x2; i++) {
		x = x + 1;
		SDL_RenderDrawPoint(gRenderer, x, y);
	}
}

void SortVertices(int *ax, int *ay, int *bx, int *by, int *cx, int *cy) {
	int x_temp, y_temp;

	if (*ay >= *cy) {
		x_temp = *ax;
		y_temp = *ay;
		*ay = *cy;
		*ax = *cx;
		*cy = y_temp;
		*cx = x_temp;
	}

	if (*ay >= *by) {
		x_temp = *ax;
		y_temp = *ay;
		*ay = *by;
		*ax = *bx;
		*by = y_temp;
		*bx = x_temp;
	}

	if (*by >= *cy) {
		x_temp = *bx;
		y_temp = *by;
		*by = *cy;
		*bx = *cx;
		*cy = y_temp;
		*cx = x_temp;
	}
}


void FillBottomFlatTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color) {
	float invslope1 = (float)(bx - ax) / (float) (by - ay);
	float invslope2 = (float)(cx - ax) / (float) (cy - ay);

	float curx1 = ax;
	float curx2 = ax;
	

	for (int scanlineY = ay; scanlineY <= by; scanlineY++) {
		DrawStraightLine((int)curx1, scanlineY, (int)curx2, scanlineY, color);
		curx1 += invslope1;
		curx2 += invslope2;
	  }
}

void FillTopFlatTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color) {
	
	float invslope1 =  (float)(cx - ax) / (float)(cy - ay);
	float invslope2 = (float)(cx - bx) / (float)(cy - by);

	float curx1 = cx;
	float curx2 = cx;

	for (int scanlineY = cy; scanlineY > ay; scanlineY--) {
		DrawStraightLine((int)curx1, scanlineY, (int)curx2, scanlineY, color);
		curx1 -= invslope1;
		curx2 -= invslope2;
	}
}

void FillTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color) {
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x66, 0xFF, 0x00 );
	int *axptr, *ayptr, *bxptr, *byptr, *cxptr, *cyptr;
	axptr = &ax;
	ayptr = &ay;
	bxptr = &bx;
	byptr = &by;
	cxptr = &cx;
	cyptr = &cy;

	//printf("before sort A = (%d, %d), B = (%d, %d), C = (%d,  %d)\n", ax, ay, bx, by, cx, cy);
	SortVertices(axptr, ayptr, bxptr, byptr, cxptr, cyptr);
	//printf("after sort A = (%d, %d), B = (%d, %d), C = (%d,  %d)\n", ax, ay, bx, by, cx, cy);
	
	if (by == cy) {
		FillBottomFlatTriangle(ax, ay, bx, by, cx, cy, color);
	} else if (ay == by) {
		FillTopFlatTriangle(ax, ay, bx, by, cx, cy, color);
	} else {
		int px = (int)(ax + ((float)(by - ay) / (float)(cy - ay)) * (cx - ax));
		int py = by;
		DrawLine(bx, by, px, py, color);
		FillBottomFlatTriangle(ax, ay, bx, by, px, py, color);
		FillTopFlatTriangle(bx, by, px, py, cx, cy, color);
  }
}
 
void DrawTriangle(int ax, int ay, int bx, int by, int cx, int cy, int color) {
	DrawLine(ax, ay, bx, by, color);
	DrawLine(ax, ay, cx, cy, color);
	DrawLine(bx, by, cx, cy, color);
	FillTriangle(ax, ay, bx, by, cx, cy, color);
}

//////////////////////// TRANSFORMATIONS //////////////////////////////////////////////////
int SetTranslation(mesh* m, int x_transl, int y_transl, int z_transl) {
	m->translation->data[3][0] = x_transl;
	m->translation->data[3][1] = y_transl;
	m->translation->data[3][2] = z_transl;
	return 0;
}

int SetScale(mesh *m, int x_scale, int y_scale, int z_scale) {
	m->scale->data[0][0] = x_scale;
	m->scale->data[1][1] = y_scale;
	m->scale->data[2][2] = z_scale;
	return 0;
}

int SetRotation(mesh *m, double deg, int plane) {
	double rad = (deg*M_PI)/180;

	switch (plane) {
		case 0:
			m->xyrotation->data[0][0] = m_cos(rad);
			m->xyrotation->data[0][1] = -m_sin(rad);
			m->xyrotation->data[1][0] = m_sin(rad);
			m->xyrotation->data[1][1] = m_cos(rad);
			break;

		case 1:
			m->xzrotation->data[0][0] = m_cos(rad);
			m->xzrotation->data[0][2] = m_sin(rad);
			m->xzrotation->data[2][0] = -m_sin(rad);
			m->xzrotation->data[2][2] = m_cos(rad);
			break;

		case 2:
			m->yzrotation->data[1][1] = m_cos(rad);
			m->yzrotation->data[1][2] = -m_sin(rad);
			m->yzrotation->data[2][1] = m_sin(rad);
			m->yzrotation->data[2][2] = m_cos(rad);
			break;
	}

	return 0;
}

int ApplyTransformation(mesh *m) {
	matrix_dot_r(m->transformation, m->xyrotation);
	matrix_dot_r(m->transformation, m->xzrotation);
	matrix_dot_r(m->transformation, m->yzrotation);
	matrix_dot_r(m->transformation, m->scale);
	matrix_dot_r(m->transformation, m->translation);
	//matrix_dot_r(m->transformation, m->vertices);
	return 0;
}

///////////////////////////////////// DRAWING ////////////////////////////////////////////////////////

void CalculateProjection(mesh *m) {
	double n = 0.001;
	double f = 1000;
	double ar = (double) 600/400;
	double fov = (2.0/3.0)*M_PI;
	
	m->projection->data[0][0] = 1/(ar*m_tan(fov/2));
	m->projection->data[1][1] = 1/(ar*m_tan(fov/2));
	m->projection->data[2][2] = (-1*f)/(f-n);
	m->projection->data[2][3] = (-1*f*n)/(f-n);
	m->projection->data[3][2] = -1;

	matrix_dot_r(m->transformation, m->projection);
	//matrix_print(m->transformation);

	for (int i = 0; i < m->transformation->num_rows; i++) {
		matrix_row_mult_r(m->transformation, (i), (1/(m->transformation->data[i][3])));
	} 
	//matrix_print(m->transformation);
	//SetRotation(m, 90, 2);
	SetTranslation(m, 700, 500, 0);
	//matrix_dot_r(m->transformation, m->xyrotation);
	//matrix_dot_r(m->transformation, m->translation);

	matrix_col_get_r(m->transformation, m->xcords, 0);
	matrix_col_get_r(m->transformation, m->ycords, 1);
	
	m->transformation = matrix_copy(m->vertices);

}


// draw finally
void DrawMesh(mesh *m, int color, int applyTransform) {
	CalculateProjection(m);
	if (applyTransform) {

		ApplyTransformation(m);
	}
	
	
	for (int i=0; i < m->indices->num_rows; i++) {
		int A_ix = m->indices->data[i][0];
		int B_ix = m->indices->data[i][1];
		int C_ix = m->indices->data[i][2];

		DrawTriangle(
			(int) *m->xcords->data[A_ix], \
			(int) *m->ycords->data[A_ix], \
			(int) *m->xcords->data[B_ix], \
			(int) *m->ycords->data[B_ix], \
			(int) *m->xcords->data[C_ix], \
			(int) *m->ycords->data[C_ix], \
			color);
	}
}

int main( int argc, char* args[] ) {
	int key;

	////////////////////////////// ALLOCATING CUBE PROPERTY MATRICES ////////////////////////////////////
	mesh *cube = calloc(1, sizeof(*cube));
	cube->vertices = matrix_from(14, 4, 14*4, CubePointArr);
	cube->indices = matrix_from(12, 3, 12*3, CubeIndexArr);
	cube->xyrotation = matrix_identity(4);
	cube->yzrotation = matrix_identity(4);
	cube->xzrotation = matrix_identity(4);
	cube->translation = matrix_identity(4);
	cube->scale = matrix_identity(4);
	cube->projection = matrix_identity(4);
	cube->xcords = matrix_new(cube->vertices->num_rows, 1);
	cube->ycords = matrix_copy(cube->xcords);
	cube->transformation = matrix_copy(cube->vertices);

	SetTranslation(cube, 250, 100, 0);
	SetScale(cube, 200, 30, 200);
	ApplyTransformation(cube);


	mesh *triangle = calloc(1, sizeof(*triangle));
	triangle->vertices = matrix_from(3, 4, 3*4, FlatIsoscelesTrianglePointArr);
	triangle->indices = matrix_from(1, 3, 1*3, FlatIsoscelesTriangleIndexArr);
	triangle->xyrotation = matrix_identity(4);
	triangle->yzrotation = matrix_identity(4);
	triangle->xzrotation = matrix_identity(4);
	triangle->translation = matrix_identity(4);
	triangle->projection = matrix_identity(4);
	triangle->scale = matrix_identity(4);
	triangle->xcords = matrix_new(triangle->vertices->num_rows, 1);
	triangle->ycords = matrix_copy(triangle->xcords);
	triangle->transformation = matrix_copy(triangle->vertices);

	SetTranslation(triangle, SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2 - 100, 0);
	SetScale(triangle, 50, 50, 50);
	


	/////////////////////////////////
/*
	double[4][4] multiply_transforms(double[4][4] m1, double[4][4] m2) {
		double[4][4] r;
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 4; k++) {
					r->data[i][j] += m1->data[i][k] * m2->data[k][j];
				}
			}
		}
	}

	double identity[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	}

	double A[4][4] = {
		{9,0,4,0},
		{4,1,8,5},
		{0,3,1,0},
		{0,1,0,1}
	} 

	double C[4][4] = multiply_transforms(identity, A);
	printf("%d", C[1][2]);
	
	*/

	int i = 1;
	if(!init()) {
		printf( "Failed to initialize!\n" );
	} else {
		if(!loadMedia()) {
			printf( "Failed to load media!\n" );
		} else {   
			bool quit = false;
			SDL_Event e;

			int k = 1;
			while( !quit ) {
				while(SDL_PollEvent( &e )!= 0) {
					if( e.type == SDL_QUIT ) {
						quit = true;
					}
				}

					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );

					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
					
					//SetRotation(cube, k, 0);
					SetRotation(cube, k, 1);
					//SetRotation(triangle, k, 0);

					//DrawMesh(triangle, 0x00, 1);

					DrawMesh(cube, 0x00, 1);



					SDL_RenderPresent(gRenderer);
				k++;
			}
			
		}
	}

	close();
	i++;
	return 0;
}
