#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

typedef struct{double p; int s;} ps;

double p[9] = {[2] = 1./16 - 1./256, [3] = 2./16, [4] = 3./16 - 1./256, [5] = 4./16, [6] = 3./16 - 1./256, [7] = 2./16, [8] = 1./16 - 1./256};
double P[40*40] = {0};

int cmpps(ps *restrict a, ps *restrict b){
	if(a->p > b->p){
		return -1;
	}else if(a->p < b->p){
		return 1;
	}
	return 0;
}

int main(void){
	for(int b = 0; b < 40; ++b){
		P[10*40 + b] = 1./64;
	}
	for(int a = 0; a < 40; ++a){
		for(int d = 2; d <= 8; ++d){
			int b = (a + d)%40;
			switch(b){
				case 2:
				case 17:
				case 30:
				P[b*40 + a] += p[d]*14./16;
				P[0*40 + a] += p[d]*1./16;
				P[10*40 + a] += p[d]*1./16;
				break;
				case 7:
				case 22:
				case 36:
				P[b*40 + a] += p[d]*6./16;
				P[0*40 + a] += p[d]*1./16;
				P[10*40 + a] += p[d]*1./16;
				P[11*40 + a] += p[d]*1./16;
				P[24*40 + a] += p[d]*1./16;
				P[39*40 + a] += p[d]*1./16;
				P[5*40 + a] += p[d]*1./16;
				P[((a - 5)/10*10 + 15)%40*40 + a] += p[d]*2./16;
				P[(b == 23 ? 28 : 12)*40 + a] += p[d]*1./16;
				if(b == 36){
					P[(b - 3)*40 + a] += p[d]*1./16;
				}else{
					P[(b - 3)*40 + a] += p[d]*1./16*14./16;
					P[0*40 + a] += p[d]*1./16*1./16;
					P[10*40 + a] += p[d]*1./16*1./16;
				}
				break;
				case 33:
				b = 10;
				default:
				P[b*40 + a] += p[d];
			}
		}
	}
	gsl_matrix_view m = gsl_matrix_view_array(P, 40, 40);
	gsl_vector_complex *eval = gsl_vector_complex_alloc(40);
	gsl_matrix_complex *evec = gsl_matrix_complex_alloc(40, 40);
	gsl_eigen_nonsymmv_workspace *w = gsl_eigen_nonsymmv_alloc(40);
	gsl_eigen_nonsymmv(&m.matrix, eval, evec, w);
	gsl_eigen_nonsymmv_free(w);
	ps a[40];
	for(int i = 0; i < 40; ++i){
		gsl_complex eval_i = gsl_vector_complex_get(eval, i);
		if(eval_i.dat[0] != 1. && eval_i.dat[1] != 0.){
			continue;
		}
		gsl_vector_complex_view evec_i = gsl_matrix_complex_column(evec, i);
		for(int j = 0; j < 40; ++j){
			a[j] = (ps){gsl_vector_complex_get(&evec_i.vector, j).dat[0], j};
		}
		break;
	}
	gsl_vector_complex_free(eval);
	gsl_matrix_complex_free(evec);
	qsort(a, 40, sizeof(ps), (__compar_fn_t)cmpps);
	printf("%i, %i, %i", a[0].s, a[1].s, a[2].s);
}

