#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <telebot.h>  
#include <telebot-types.h>  


#define BOT_TOKEN "7446528958:AAH-vUzcETFNfziBSTtShVMKaK-dy4KZLSU"  // bot token

extern int get_weather_by_city(const char* city, const char* api_key, char* weather_info, size_t weather_info_len);
extern int get_weather_by_coordinates(double latitude, double longitude, char* weather_info, size_t weather_info_len);

// Function to check if the string is a valid floating-point number (latitude or longitude)
int is_number(const char* str) {
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {  
        i++;
    }
    int has_dot = 0;
    while (str[i] != '\0') {
        if (str[i] == '.') {
            if (has_dot) return 0;  
            has_dot = 1;
        } else if (!isdigit(str[i]) && str[i] != '-') {
            return 0;  
        }
        i++;
    }
    return 1;
}


void on_message_received_coordinates(telebot_handler_t bot, const char *message, double latitude, double longitude, int64_t chat_id) {
    char weather_info[1024];
    static int message_id = 0;  // Message ID for editing purposes 

    if (latitude != 0.0 && longitude != 0.0) {
        // Get the weather by coordinates
        int result = get_weather_by_coordinates(latitude, longitude, weather_info, sizeof(weather_info));
        if (result == 0) {
            // Send the weather info for coordinates
            if (message_id == 0) {
                telebot_send_message(
                    bot, chat_id, weather_info, "Markdown", false, false, 0, NULL
                );
                message_id = bot->last_sent_message_id; 
            } else {
                telebot_edit_message_caption(
                    bot, chat_id, message_id, NULL, weather_info, "Markdown", NULL
                );
            }
        } else {
            if (message_id == 0) {
                telebot_send_message(
                    bot, chat_id, 
                    "Error: Unable to fetch weather data for the provided coordinates.", 
                    "Markdown", false, false, 0, NULL
                );
                message_id = bot->last_sent_message_id;
            } else {
                telebot_edit_message_caption(
                    bot, chat_id, message_id, NULL, 
                    "Error: Unable to fetch weather data for the provided coordinates.", 
                    "Markdown", NULL
                );
            }
        }
    } else {
        if (message_id == 0) {
            telebot_send_message(
                bot, chat_id, 
                "Error: Invalid coordinates. Please provide valid latitude and longitude.",
                "Markdown", false, false, 0, NULL
            );
            message_id = bot->last_sent_message_id;
        } else {
            telebot_edit_message_caption(
                bot, chat_id, message_id, NULL, 
                "Error: Invalid coordinates. Please provide valid latitude and longitude.",
                "Markdown", NULL
            );
        }
    }
}
void on_message_received_city(telebot_handler_t bot, const char *message, const char* city, const char* api_key, int64_t chat_id) {
    char weather_info[1024];
    static int message_id = 0;  // Message ID for editing purposes 

    if(is_number(city) == false || is_number(api_key) == false) {
        if (message_id == 0) {
            telebot_send_message(
                bot, chat_id, 
                "Error: Invalid city name or api_key. Please provide valid city and api_key.",
                "Markdown", false, false, 0, NULL
            );
            message_id = bot->last_sent_message_id;
        } else {
            telebot_edit_message_caption(
                bot, chat_id, message_id, NULL, 
                "Error: Invalid city name or api_key. Please provide valid city and api_key.",
                "Markdown", NULL
            );
        }
    } 
    else 
    {
        // Get the weather by city and api_key
        int result = get_weather_by_city(city, api_key, weather_info, sizeof(weather_info));
        if (result == 0) {
            // Send the weather info for city
            if (message_id == 0) {
                telebot_send_message(
                    bot, chat_id, weather_info, "Markdown", false, false, 0, NULL
                );
                message_id = bot->last_sent_message_id; 
            } else {
                telebot_edit_message_caption(
                    bot, chat_id, message_id, NULL, weather_info, "Markdown", NULL
                );
            }
        } else {
            if (message_id == 0) {
                telebot_send_message(
                    bot, chat_id, 
                    "Error: Unable to fetch weather data for the provided city.", 
                    "Markdown", false, false, 0, NULL
                );
                message_id = bot->last_sent_message_id;
            } else {
                telebot_edit_message_caption(
                    bot, chat_id, message_id, NULL, 
                    "Error: Unable to fetch weather data for the provided city.", 
                    "Markdown", NULL
                );
            }
        }
    }
}
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are provided
    if (argc < 2) {
        printf("Usage: %s <city|latitude longitude> [api_key]\n", argv[0]);
        return 1;
    }
    
    telebot_handler_t bot; 

    // Initialize the bot
    if (telebot_create(&bot, BOT_TOKEN) != TELEBOT_ERROR_NONE) {
        printf("Error creating bot\n");
        
        return -1;
    }


    if (argc == 3 && is_number(argv[1]) && is_number(argv[2])) {
        double latitude = atof(argv[1]);
        double longitude = atof(argv[2]);
        on_message_received_coordinates(bot, "/weather", latitude, longitude, 1158561652);  
    }
    else if (argc == 3) {
        const char* city = argv[1];
        const char* api_key = argv[2];

        on_message_received_city(bot, "/weather", city, api_key, 1158561652);
    }
    else {
        printf("Usage: %s <city|latitude longitude> [api_key]\n", argv[0]);
        return 1;
    }

    
    if (telebot_destroy(bot) != TELEBOT_ERROR_NONE) {
        printf("Error destroying bot\n");
        return -1;
    }

    return 0;
}
