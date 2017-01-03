#include "math.h"
#include "omp.h"
#include "stdio.h"
#include "stdlib.h"

double f(double x) {

	return sin(x);

}

double calka(
		double a, double b, double dx
) {

	double result = 0.0;

#pragma omp parallel default(none) shared(result) firstprivate(a,b,dx)
	{

#pragma omp single
		{
			int num_threads = omp_get_num_threads();

			int num_tasks = num_threads + 1;

			int itask;

			double newdx = b - a / num_tasks;
			for (itask = 0; itask < num_tasks; itask++) {
				double p_task = a +(itask*newdx);
				double k_task = a + (itask*newdx + newdx);

#pragma omp task default(none) firstprivate(p_task, k_task, dx) shared(result)
				{
					int i;
					double x1, x2, c;
					x1 = p_task;
					x2 = k_task;
					int n_loc = (int) ceil((x2-x1)/dx);
					double loc_dx = (x2-x1)/n_loc;

					c = 0.0;
					for(i=0; i<n_loc; i++) {
						x2 = x1 + loc_dx;
						c += 0.5 * (f(x1) + f(x2)) * dx;
						x1 = x2;
					}
#pragma omp critical
					{
							result+=c;
					}

				} // end task definition

			} // end loop over tasks

		} // end single region

	} // end parallel region

	return (result);
}

int main() {

	printf("CALKA: %lf", calka(0.0, M_PI, 0.000001));

}