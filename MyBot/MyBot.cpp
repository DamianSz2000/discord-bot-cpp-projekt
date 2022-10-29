#include <dpp/dpp.h>
#include <iostream>
#include <fstream>
#include <dpp/nlohmann/json.hpp>
#include "MyBot.h"
#include <vector>

const std::string    BOT_TOKEN    = "MTAyOTQwNzU1MTkyNzg3NzY0Mg.GopC_c.HKqKsARW3y52DRdQDb1m2img9XNAjXgL-cxMGc";



int main()
{
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
	
	std::vector<skin> skins;
	
    load_skins(skins);
		
    bot.on_log(dpp::utility::cout_logger());
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (event.msg.content == "!setup_verification_channel") {
            create_verification_channel(bot, event);
        }
        else if (event.msg.content == "!setup_order_channel") {
            create_order_channel(bot, event);
        }
    });
    bot.on_button_click([&bot](const dpp::button_click_t& event) {
        if (event.custom_id == "verification") {
            bot.guild_member_add_role(event.command.guild_id, event.command.usr.id, 1033511189138898964);
            event.reply();
        }
        else if (event.custom_id == "order") {
            create_customer_ticket(bot, event);
        }
    });
    bot.on_select_click([&bot, &skins](const dpp::select_click_t& event) {
        if (event.custom_id == "order_type") {
            if (event.values[0] == "skin") {
                delete_all_messages(bot, event);
                champion_picking_process(event, bot);
            }
        }
        else if (event.custom_id == "champion_name_0" or event.custom_id == "champion_name_1" or event.custom_id == "champion_name_2" or event.custom_id == "champion_name_3" or event.custom_id == "champion_name_4" or event.custom_id == "champion_name_5" or event.custom_id == "champion_name_6" or event.custom_id == "champion_name_7") {
			delete_all_messages(bot, event);
            skin_picking_process(skins, event, bot);
		}
        event.reply();
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("setup", "Setup the bot", bot.me.id));
        }
    });
    bot.start(false);

    return 0;
}


