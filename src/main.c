#include <darnit/darnit.h>
#include "ui/ui.h"

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600

int main(int argc, char  **argv) {
	d_init_custom("birdie26", DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, "birdie26", NULL);
	ui_init(4);
	
	for(;;) {
		d_render_begin();
		d_render_blend_enable();
		d_render_end();
		d_loop();
	}

	d_quit();
	return 0;
}
