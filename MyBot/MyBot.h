#pragma once
#include <dpp/dpp.h>
#include <iostream>
#include <fstream>
#include <dpp/nlohmann/json.hpp>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <time.h>



class skin {
    public:
        std::string name;
        std::string price_rp;
        int price_zl;
};
class order {
    public:
        std::string nickname;
        std::string item_id;
        std::string item_name;
        std::string item_price;
        std::string date;
        std::string resource_login;
        std::string resource_password;
};


void create_verification_channel(dpp::cluster& bot, const dpp::message_create_t& event);

void load_skins(std::vector<skin>& skins);

void create_order_channel(dpp::cluster& bot, const dpp::message_create_t& event);

void create_customer_ticket(dpp::cluster& bot, const dpp::button_click_t& event);

void delete_all_messages(dpp::cluster& bot, const dpp::select_click_t& event);

void champion_picking_process(const dpp::select_click_t& event, dpp::cluster& bot);

std::string skin_picking_process(const std::vector<skin>& skins, const dpp::select_click_t& event, dpp::cluster& bot);

void create_order_summary(dpp::cluster& bot, const std::string& order_type, const dpp::form_submit_t& event, order o);

void claim_ticket(dpp::cluster& bot, const dpp::button_click_t& event);

sql::Connection* establish_db_connection();

void enter_nickname(dpp::cluster& bot, const dpp::select_click_t& event);

void nickname_modal(dpp::cluster& bot, const dpp::button_click_t& event);

void add_friend(const std::string& program_name, const std::string& parameters);

void send_gift(const std::string& nickname, const std::string& item_id);

void create_timeout_thread(const std::string& nickname, const std::string& item_id);
