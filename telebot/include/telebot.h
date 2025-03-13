/*
 * telebot
 *
 * Copyright (c) 2015 Elmurod Talipov.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TELEBOT_H__
#define __TELEBOT_H__

#include "telebot-common.h"
#include "telebot-types.h"
#include "telebot-methods.h"

typedef struct telebot_handler_s {
    char *token;                    // Bot's API token
    int64_t bot_user_id;            // Bot user ID
    char *bot_username;             // Bot's username
    int64_t last_sent_message_id;   // ID of the last sent message
    void (*message_handler)(struct telebot_handler_s *bot, const char *message, int64_t chat_id);  // Function pointer for message handling
    void (*command_handler)(struct telebot_handler_s *bot, const char *command, int64_t chat_id); // Function pointer for command handling
    struct telebot_settings_s *settings; // Bot's settings
    int error_code;                 // Stores error code
} telebot_handler_s;

#endif /* __TELEBOT_H__ */

