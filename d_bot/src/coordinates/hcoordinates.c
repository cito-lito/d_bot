#include <zephyr.h>
#include <device.h>
#include <sys/printk.h>
#include <misc/printk.h>
#include <stdio.h>
#include <zsl/zsl.h>
//#include <math.h>
#define M_PI   3.14159265f

/*radians to degrees*/
#define DEG(x) ((x * (M_PI / 180)))


double_t absolut_pos[3] = { 5, 3, 1 };

void translation(s32_t dest_y, float_t angle)
{
	float_t tmp[3][3] = { { (cos(DEG(angle))), (-sin(DEG(angle))), 5 },
			      { (sin(DEG(angle))), (cos(DEG(angle))), 3 },
			      { 0, 0, 1 } };
	s32_t dest[3] = { 0, dest_y, 1 };

	for (u8_t i = 0; i < 3; i++) {
		double_t a = 0;
		for (u8_t j = 0; j < 3; j++) {
			a += tmp[i][j] * dest[j];
			printf("pos %lf\n", a);
		}
		absolut_pos[i] = a;
	}
}

void main(void)
{
	double a= (sin(DEG(30)));
	printk("starting\n");
	printf("rad: %lf\n", a);

	translation(3, 30);
	for (u8_t i = 0; i < 3; i++) {
		printf(" %lf\n", absolut_pos[i]);
	
	}
	printk("end\n");
}
