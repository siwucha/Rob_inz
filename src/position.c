/*
 * position.c
 *
 *  Created on: 14 lut 2016
 *      Author: Makumba
 */

#include "position.h"
#include <gsl/gsl_matrix.h>
#include "console.h"
#include <stdio.h>

void kalmanFilterPosition0(double latitude, double longitude, gsl_matrix *x, gsl_matrix *P)
{
	/* Wektor pomiarów */
	gsl_matrix *z = gsl_matrix_alloc(2, 1);
	gsl_matrix_set(z, 0, 0, latitude);
	gsl_matrix_set(z, 1, 0, longitude);
	/* Macierz przejœcia ze stanu do pomiaru */
	gsl_matrix *H = gsl_matrix_alloc(2,2);
	gsl_matrix_set_identity(H);
	/* Macierz kowariancji szumu procesu */
	gsl_matrix *Q = gsl_matrix_alloc(2,2);
	gsl_matrix_set_identity(Q);
	double q = 0.00001;
	gsl_matrix_scale(Q, q);
	/* Macierz kowariancji szumu pomiaru */
	double wrong_latitude = pow(WRONG_LATI, 2);
	double wrong_longitude = pow(WRONG_LONGI,2);
	gsl_matrix *R = gsl_matrix_calloc(2,2);
	gsl_matrix_set(R, 0, 0, wrong_latitude);
	gsl_matrix_set(R, 1, 1, wrong_longitude);
	/* Macierz jednostkowa */
	gsl_matrix *I = gsl_matrix_alloc(2,2);
	gsl_matrix_set_identity(I);

	/* P = P(k-1) + Q */
	gsl_matrix_add(P, Q);
	/* K =  P/(P + R) */
	gsl_matrix *K = gsl_matrix_calloc(2,2);
	(*K) = (*P);
	gsl_matrix_add(R, P);
	gsl_matrix_div_elements(K, R);
	/* P(k) = (I-K)*P */
	gsl_matrix_sub(I, K);
	gsl_matrix_mul_elements(I, P);
	(*P) = (*I);
	/* x(k) = x + K(z-x) */
	gsl_matrix_sub(z, x);
	gsl_matrix_mul_elements(K, z);
	gsl_matrix_add(x, K); //zaktualizowana wartoœc

	gsl_matrix_free(z);
	gsl_matrix_free(H);
	gsl_matrix_free(Q);
	gsl_matrix_free(R);
	gsl_matrix_free(I);
	gsl_matrix_free(K);
}

void getPosition0(Position0 *pos, GpsDate *gps, CircBuffer *gps_buffer)
{
	gsl_matrix *x = gsl_matrix_calloc(2,1);
	gsl_matrix *P = gsl_matrix_alloc(2,2);
	gsl_matrix_set_identity(P);
	double previous_latitude0;
	double previous_longitude0;
	double latitude0;
	double longitude0;
	int i = 0;
	while (i < 5)
	{
		if((gpsUpdate(gps, gps_buffer)) && ((*gps).status == 'A'))
		{
			kalmanFilterPosition0((*gps).latitude, (*gps).longitude, x, P);
			i++;
			printf("Aktualizacja w filtrze KALMANA poraz %i", i+1);	//potrzebne do debugowania
		}
	}
	i = 0;
	while(i<5)
	{
		previous_latitude0 = gsl_matrix_get(x, 0, 0);
		previous_longitude0 = gsl_matrix_get(x, 1, 0);
		if((gpsUpdate(gps, gps_buffer)) && ((*gps).status == 'A'))
		{
			kalmanFilterPosition0((*gps).latitude, (*gps).longitude, x, P);
			latitude0 = gsl_matrix_get(x, 0, 0);
			longitude0 = gsl_matrix_get(x, 1, 0);
			/* Blok danych do wyœwietlenia przy debugowaniu */
			printf("Wartoœc previous_latitude0 = %f", previous_latitude0);
			printf("Wartoœc previous_longitude0 = %f", previous_longitude0);
			printf("Wartoœc latitude0 = %f", latitude0);
			printf("Wartoœc longitude0 = %f", longitude0);
			if((previous_latitude0 == latitude0) && (previous_longitude0 == longitude0))
			{
				i++;	//Kolejny wektor x identyczny z poprzednim wektorem x
				if(i == 5)
				{
					pos->latitude0 = latitude0;
					pos->longitude0 = longitude0;
					gsl_matrix_free(x);
					gsl_matrix_free(P);
				}
			} else {
				i = 0;
			}
		}
	}
}
