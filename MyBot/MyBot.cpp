#include <dpp/dpp.h>
#include <iostream>
#include <fstream>
#include <dpp/nlohmann/json.hpp>
#include "MyBot.h"
#include <vector>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>


const std::string    BOT_TOKEN    = "MTAyOTQwNzU1MTkyNzg3NzY0Mg.GopC_c.HKqKsARW3y52DRdQDb1m2img9XNAjXgL-cxMGc";



int main()
{
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
 
	bot.on_ready([&bot](const dpp::ready_t& event) {
		std::cout << "Logged in as " << bot.me.username << std::endl;
		});
	
	

	order o = order();
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
        else if (event.custom_id == "claim_ticket") {
            claim_ticket(bot, event);
        }
        else if (event.custom_id == "enter_nickname") {
			nickname_modal(bot, event);
        }
    });
    bot.on_select_click([&bot, &skins, &o](const dpp::select_click_t& event) {
        if (event.custom_id == "order_type") {
            if (event.values[0] == "skin") {
                delete_all_messages(bot, event);
                champion_picking_process(event, bot);
            }
        }
        else if (event.custom_id == "champion_name_0" or event.custom_id == "champion_name_1" or event.custom_id == "champion_name_2" or event.custom_id == "champion_name_3" or event.custom_id == "champion_name_4" or event.custom_id == "champion_name_5" or event.custom_id == "champion_name_6") {
			delete_all_messages(bot, event);
            o.item_id = skin_picking_process(skins, event, bot);
		}
        else if (event.custom_id == "skin_select") {
            auto options = event.command.msg.components[0].components[0].options;
            int index = 0;
            for (auto& option : options) {
                if (option.value == event.values[0]) {
                    break;
                }
                index++;
            }
            o.item_name = event.values[0];
            o.item_price = event.command.msg.components[0].components[0].options[index].description.substr(6, std::string::npos);
            delete_all_messages(bot, event);
            enter_nickname(bot, event);
        }
        event.reply();
    });
	bot.on_form_submit([&o, &bot](const dpp::form_submit_t& event) {
		if (event.custom_id == "nickname_modal") {
            o.nickname = std::get<std::string>(event.components[0].components[0].value);
            event.reply();
            create_order_summary(bot, "Skin", event, o);
		}
		});
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("setup", "Setup the bot", bot.me.id));
        }
        });
    bot.start(false);

    return 0;
}


