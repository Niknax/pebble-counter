/* Todo
 * snprintf correct number ofdigits
 * disable buttons on MAX/MIN
 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168
#define STATUS_BAR_HEIGHT 16

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

//* MagicL the Gathering Version
#define MY_UUID { 0x06, 0xBB, 0x91, 0xA7, 0x54, 0x4A, 0x4F, 0x5D, 0xAB, 0xEF, 0xF2, 0xEF, 0x28, 0x94, 0xF5, 0x41 }
PBL_APP_INFO(MY_UUID,
             "MtG Life Points", "Ps0ke",
             1, 0, /* App Version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

#define COUNTER_START 20
#define COUNTER_MAX 9999
#define COUNTER_MIN 0
#define HOLD_TO_REPEAT_MS 300

Window window;
ActionBarLayer action_bar;
TextLayer counter_layer;
int counter;

HeapBitmap button_image_plus;
HeapBitmap button_image_reset;
HeapBitmap button_image_minus;


void update_text_layer_with_int(const int num, TextLayer* text_layer){
	char* buffer = "\0";
	snprintf(buffer, (6 /* signed & \0 */)*sizeof(char), "%d", num);
	text_layer_set_text(text_layer, buffer);
	layer_mark_dirty(&(text_layer->layer));
}


void up_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(counter + 1 <= COUNTER_MAX){
		if(counter == COUNTER_MIN)
			action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &button_image_minus.bmp);

		counter++;
		update_text_layer_with_int(counter, &counter_layer);

		if(counter == COUNTER_MAX)
			action_bar_layer_clear_icon(&action_bar, BUTTON_ID_UP);
	}
}
#ifdef HOLD_JUMP
void up_long_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(counter == COUNTER_MIN)
		action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &button_image_minus.bmp);

	if(counter + HOLD_JUMP < COUNTER_MAX)
		counter += HOLD_JUMP;
	else
		counter = COUNTER_MAX;
	update_text_layer_with_int(counter, &counter_layer);

	if(counter == COUNTER_MAX)
		action_bar_layer_clear_icon(&action_bar, BUTTON_ID_UP);
}
#endif

void select_click_handler(ClickRecognizerRef recognizer, Window *window){
	counter = COUNTER_START;
	text_layer_set_text(&counter_layer, STR(COUNTER_START));
	layer_mark_dirty(&counter_layer.layer);

	if(counter < COUNTER_MAX)
		action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &button_image_plus.bmp);
	else
		action_bar_layer_clear_icon(&action_bar, BUTTON_ID_UP);

	if(counter > COUNTER_MIN)
		action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &button_image_minus.bmp);
	else
		action_bar_layer_clear_icon(&action_bar, BUTTON_ID_DOWN);
}

void down_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(counter - 1 >= COUNTER_MIN){
		if(counter == COUNTER_MAX)
			action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &button_image_plus.bmp);

		counter--;
		update_text_layer_with_int(counter, &counter_layer);

		if(counter == COUNTER_MIN)
			action_bar_layer_clear_icon(&action_bar, BUTTON_ID_DOWN);
	}
}
#ifdef HOLD_JUMP
void down_long_click_handler(ClickRecognizerRef recognizer, Window *window){
	if(counter == COUNTER_MAX)
		action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &button_image_plus.bmp);

	if(counter - HOLD_JUMP > COUNTER_MIN)
		counter -= HOLD_JUMP;
	else
		counter = COUNTER_MIN;
	update_text_layer_with_int(counter, &counter_layer);

	if(counter == COUNTER_MIN)
		action_bar_layer_clear_icon(&action_bar, BUTTON_ID_DOWN);
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

	action_bar_layer_set_icon(&action_bar, BUTTON_ID_SELECT, &button_image_reset.bmp);
	if(counter < COUNTER_MAX)
		action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &button_image_plus.bmp);
	if(counter > COUNTER_MIN)
		action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &button_image_minus.bmp);


	// Setup the text layer
	text_layer_init(&counter_layer, GRect(
	/* offset x */  1, // 1px spacing
	/* offset y */  (SCREEN_HEIGHT - STATUS_BAR_HEIGHT)/2 - 50/2,
	/* width */     SCREEN_WIDTH - ACTION_BAR_WIDTH - 1*2, // 1px spacing
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
