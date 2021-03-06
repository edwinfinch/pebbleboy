//Edwin is the bomb.com or might not be depending on if you believe so or not

#include <pebble.h>
 
Window* window;
TextLayer *text_layer, *date_text_layer;
GBitmap *gameboy_bitmap;
BitmapLayer *gameboy_layer;

char timeBuffer[] = "00:00";
char dateBuffer[] = "February 31";
int animationNumber = 0;
bool booted = 0;

void on_animation_stopped(Animation *anim, bool finished, void *context){
    //Free the memoery used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay){
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
     
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
     
    //Set stopped handler to free memory
    AnimationHandlers handlers = {
        //The reference to the stopped handler is the only one in the array
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
	animationNumber++;
	
	if(animationNumber == 2 && booted == false){
		animationNumber = 0;
		booted = true;
	}
     
    //Start animation
    animation_schedule((Animation*) anim);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed){
		Layer *window_layer = window_get_root_layer(window);
        //Format the buffers using tick_time as the time source
        strftime(timeBuffer, sizeof("00:00"), "%H:%M", tick_time);
	    strftime(dateBuffer, sizeof(dateBuffer), "%B %e", tick_time);
        
		if(clock_is_24h_style()){
      		strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", tick_time);
   		 }
   		 else{
     		 strftime(timeBuffer,sizeof(timeBuffer),"%I:%M",tick_time);
   		 }
	
        //Change the TextLayer text to show the new time!
        text_layer_set_text(text_layer, timeBuffer);
	    text_layer_set_text(date_text_layer, dateBuffer);
	
	int seconds = tick_time->tm_sec;
   
	if(seconds == 58 && booted == true)
	{
		//To re-enable date animations take the comment slashes out from under the next 6 (indented) comments
		GRect start = GRect(4, 38, 132, 168);
        GRect finish = GRect(4, 78, 132, 168);
		  GRect start1 = GRect(35, 68, 150, 140);
          GRect finish1 = GRect(35, 8, 150, 140);
        animate_layer(text_layer_get_layer(text_layer), &start, &finish, 2000, 0);
		  //animate_layer(text_layer_get_layer(date_text_layer), &start1, &finish1, 2000, 0);
	}
	
    else if(seconds == 0 && booted == true)
        {
          GRect start = GRect(4, 78, 132, 168);
          GRect finish = GRect(4, 38, 132, 168); 
		    GRect start1 = GRect(35, 8, 150, 140);
            GRect finish1 = GRect(35, 68, 150, 140);
          animate_layer(text_layer_get_layer(text_layer), &start, &finish, 4000, 0);
		    //animate_layer(text_layer_get_layer(date_text_layer), &start1, &finish1, 4000, 0);
        }
		
}
 
void window_load(Window *window){
		Layer *window_layer = window_get_root_layer(window);
	    ResHandle font_handle = resource_get_handle(RESOURCE_ID_FONT_GAMEGIRL_17);
	    ResHandle font_handle2 = resource_get_handle(RESOURCE_ID_FONT_POKEMONGB_7);
		//Load bitmap into GBitmap structure
		gameboy_bitmap = gbitmap_create_with_resource(RESOURCE_ID_GAMEBOY_OUTLINE);
		gameboy_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
		bitmap_layer_set_bitmap(gameboy_layer, gameboy_bitmap);
		layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(gameboy_layer));
	
	    //Time layer
        text_layer = text_layer_create(GRect(6, 38, 132, 168));
        text_layer_set_background_color(text_layer, GColorClear);
        text_layer_set_text_color(text_layer, GColorBlack);
        text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
        text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
	
		//Date layer
  		date_text_layer = text_layer_create(GRect(35,68,168,144));
  		text_layer_set_font(date_text_layer,fonts_load_custom_font(font_handle2));
 		text_layer_set_background_color(date_text_layer, GColorClear);
  		text_layer_set_text_color(date_text_layer, GColorBlack);
 		layer_add_child(window_layer, text_layer_get_layer(date_text_layer));
        
        layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
        
        //Get a time structure so that the face doesn't start blank
        struct tm *t;
        time_t temp;        
        temp = time(NULL);        
        t = localtime(&temp);        
        
        //Manually call the tick handler when the window is loading
        tick_handler(t, SECOND_UNIT);
}
 
void window_unload(Window *window){
        text_layer_destroy(text_layer);
		text_layer_destroy(date_text_layer);
}
 
void init(){
		Layer *window_layer = window_get_root_layer(window);
        window = window_create();
        window_set_window_handlers(window, (WindowHandlers) {
                .load = window_load,
                .unload = window_unload,
        });
        
        tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
        
        window_stack_push(window, true);
	    GRect start = GRect(4, 5, 140, 168);
        GRect finish = GRect(4, 38, 132, 168);
	    GRect start1 = GRect(35, 5, 150, 140);
        GRect finish1 = GRect(35, 68, 150, 140);
        animate_layer(text_layer_get_layer(text_layer), &start, &finish, 4000, 0);
        animate_layer(text_layer_get_layer(date_text_layer), &start1, &finish1, 4000, 0);
}
 
void deinit(){
        //De-initialize elements here to save memory!
        tick_timer_service_unsubscribe();
        window_destroy(window);
		//Destroy GBitmaps
		gbitmap_destroy(gameboy_bitmap);
 
		//Destroy BitmapLayers
		bitmap_layer_destroy(gameboy_layer);
}
 
int main(void){
	init();
	app_event_loop();
	deinit();
}