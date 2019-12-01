#include <avr/io.h>
#include "cube.h"

int main(){
	cube_init();
	while (1)
	{
		//teq_hartutjun();
		//vert_hartutyun();
		//cube_verev_vazox();
		//cube_vazox();
		tetrix();
	}
	return 1;
}


