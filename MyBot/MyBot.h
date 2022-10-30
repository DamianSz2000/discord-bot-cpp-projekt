#pragma once
#include <dpp/dpp.h>
#include <iostream>
#include <fstream>
#include <dpp/nlohmann/json.hpp>



class skin {
    public:
        std::string name;
        std::string price_rp;
        int price_zl;
};


void create_verification_channel(dpp::cluster& bot, const dpp::message_create_t& event);

void load_skins(std::vector<skin>& skins);

void create_order_channel(dpp::cluster& bot, const dpp::message_create_t& event);

void create_customer_ticket(dpp::cluster& bot, const dpp::button_click_t& event);

void delete_all_messages(dpp::cluster& bot, const dpp::select_click_t& event);

void champion_picking_process(const dpp::select_click_t& event, dpp::cluster& bot);

void skin_picking_process(const std::vector<skin>& skins, const dpp::select_click_t& event, dpp::cluster& bot);

void create_order_summary(dpp::cluster& bot, const std::string& order_type, const dpp::select_click_t& event);
