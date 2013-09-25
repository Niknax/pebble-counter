/* Todo
 * add action bar icons
 * (adaptive fontsize)
 * snprintf correct number ofdigits
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168
#define STATUS_BAR_HEIGHT 16

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define MY_UUID { 0x0D, 0xB0, 0x40, 0x0D, 0x77, 0x8E, 0x40, 0xBC, 0x8F, 0x6B, 0x17, 0x4E, 0xB9, 0xF7, 0x90, 0xEB }
PBL_APP_INFO(MY_UUID,
             "Counter", "Ps0ke",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define MAX_DIGITS 4
#define COUNTER_START 0
#define HOLD_TO_REPEAT_MS 100
// undefine to use hold-to-repeat
//#define HOLD_JUMP 10

Window window;
ActionBarLayer action_bar;
TextLayer counter_layer;
int counter;

HeapBitmap button_image_plus;
HeapBitmap button_image_reset;
HeapBitmap button_image_minus;


int digits(int num){
	int digits = 0;
//	if(num < 0)
//		digits++; // signed

	do{
		num /= 10;
		digits++;
	} while(num != 0);

	return digits;
}

void update_text_layer_with_int(const int num, TextLayer* text_layer){
	char* buffer = "\0";
	snprintf(buffer, (MAX_DIGITS+2 /* signed & \0 */)*sizeof(char), "%d", num);
	text_layer_set_text(text_layer, buffer);
	layer_mark_dirty(&(text_layer->layer));
}


void up_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(digits(counter + 1) <= MAX_DIGITS){
		counter++;
		update_text_layer_with_int(counter, &counter_layer);
	}
}
#ifdef HOLD_JUMP
void up_long_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(digits(counter + HOLD_JUMP) <= MAX_DIGITS)
		counter += HOLD_JUMP;
	else
		while(digits(counter + 1) <= MAX_DIGITS)
			counter++;
	update_text_layer_with_int(counter, &counter_layer);
}
#endif

void select_click_handler(ClickRecognizerRef recognizer, Window *window){
	counter = COUNTER_START;
	text_layer_set_text(&counter_layer, STR(COUNTER_START));
	layer_mark_dirty(&counter_layer.layer);
}

void down_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(digits(counter - 1) <= MAX_DIGITS){
		counter--;
		update_text_layer_with_int(counter, &counter_layer);
	}
}
#ifdef HOLD_JUMP
void down_long_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(digits(counter - HOLD_JUMP) <= MAX_DIGITS)
		counter -= HOLD_JUMP;
	else
		while(digits(counter - 1) <= MAX_DIGITS)
			counter--;
	update_text_layer_with_int(counter, &counter_layer);
}
#endif


void click_config_provider(ClickConfig **config, void *context) {
	config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_click_handler;
	config[BUTTON_ID_UP]->click.repeat_interval_ms = HOLD_TO_REPEAT_MS;
#ifdef HOLD_JUMP
	config[BUTTON_ID_UP]->long_click.handler = (ClickHandler) up_long_click_handler;
#endif

	config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_click_handler;

	config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_click_handler;
	config[BUTTON_ID_DOWN]->click.repeat_interval_ms = HOLD_TO_REPEAT_MS;
#ifdef HOLD_JUMP
	config[BUTTON_ID_DOWN]->long_click.handler = (ClickHandler) down_long_click_handler;
#endif
}


void handle_main_appear(Window *window){
	// Add the actionbar after window is visible, otherwise the size doesn't fit because of the menu bar
	action_bar_layer_add_to_window(&action_bar, window);
}

void handle_main_disappear(Window *window){
	action_bar_layer_remove_from_window(&action_bar);
}


void handle_init(AppContextRef ctx) {
	counter = COUNTER_START;

	// Setupt the window
	window_init(&window, "Counter Window");
	// Associate the action bar with the window:
	window_set_window_handlers(&window, (WindowHandlers){
		.appear = (WindowHandler) handle_main_appear,
		.disappear = (WindowHandler) handle_main_disappear
	});
	window_stack_push(&window, true /* Animated */);


	// Initialize the resources
	resource_init_current_app(&APP_RESOURCES);
	heap_bitmap_init(&button_image_plus, RESOURCE_ID_IMAGE_PLUS);
	heap_bitmap_init(&button_image_reset, RESOURCE_ID_IMAGE_RESET);
	heap_bitmap_init(&button_image_minus, RESOURCE_ID_IMAGE_MINUS);


	// Initialize the action bar:
	action_bar_layer_init(&action_bar);
	action_bar_layer_set_click_config_provider(&action_bar, click_config_provider);

	action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &button_image_plus.bmp);
	action_bar_layer_set_icon(&action_bar, BUTTON_ID_SELECT, &button_image_reset.bmp);
	action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &button_image_minus.bmp);


	// Setup the text layer
	text_layer_init(&counter_layer, GRect(
	/* offset x */  1, // 3px spacing
	/* offset y */  (SCREEN_HEIGHT - STATUS_BAR_HEIGHT)/2 - 50/2,
	/* width */     SCREEN_WIDTH - ACTION_BAR_WIDTH - 1*2, // 3px spacing
	/* height */    50
	));
	text_layer_set_text_alignment(&counter_layer, GTextAlignmentCenter);
	text_layer_set_font(&counter_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	//text_layer_set_background_color(&counter_layer, GColorBlack);
	//text_layer_set_text_color(&counter_layer, GColorWhite);
	text_layer_set_text(&counter_layer, STR(COUNTER_START));

	layer_add_child(&window.layer, &counter_layer.layer);
}

void handle_deinit(AppContextRef ctx){
	heap_bitmap_deinit(&button_image_plus);
	heap_bitmap_deinit(&button_image_reset);
	heap_bitmap_deinit(&button_image_minus);

	window_deinit(&window);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit
  };
  app_event_loop(params, &handlers);
}
