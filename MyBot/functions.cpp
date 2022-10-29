void create_verification_channel(dpp::cluster& bot, const dpp::message_create_t& event)
{
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::green)
        .set_description("Click the button to get access to whole server!");
    bot.message_create(dpp::message(event.msg.channel_id, embed).add_component(dpp::component().add_component(
        dpp::component()
        .set_label("Get verified!")
        .set_type(dpp::cot_button)
        .set_emoji(u8"✅")
        .set_style(dpp::cos_success)
        .set_id("verification"))));
}

void load_skins(std::vector<skin>& skins)
{
    for (auto& element : data) {
        skin s;
        s.name = element["skin"];
        s.price_rp = element["price_rp"];
        s.price_zl = element["price_pln"];
        skins.push_back(s);
    }
}

void create_order_channel(dpp::cluster& bot, const dpp::message_create_t& event)
{
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::green)
        .set_description("Click button below to place your order!");
    bot.message_create(dpp::message(event.msg.channel_id, embed).add_component(dpp::component().add_component(
        dpp::component()
        .set_label("Order")
        .set_type(dpp::cot_button)
        .set_emoji(u8"📩")
        .set_style(dpp::cos_secondary)
        .set_id("order"))));
}

void create_customer_ticket(dpp::cluster& bot, const dpp::button_click_t& event)
{
    bot.channel_create(dpp::channel().set_guild_id(event.command.guild_id).set_name("ticket-" + event.command.usr.username), [&bot](const dpp::confirmation_callback_t& event) {
        if (!event.is_error()) {
            auto channel = std::get<dpp::channel>(event.value);
            dpp::embed embed = dpp::embed()
                .set_color(dpp::colors::green)
                .set_description("Choose order type!");
            bot.message_create(dpp::message(channel.id, embed).add_component(dpp::component().add_component(
                dpp::component()
                .set_type(dpp::cot_selectmenu)
                .add_select_option(dpp::select_option("Skin", "skin", "Order a skin"))
                .add_select_option(dpp::select_option("Hextech loot", "hextech", "Order hextech loot"))
                .add_select_option(dpp::select_option("Boosting", "boosting", "Order boosting"))
                .add_select_option(dpp::select_option("Other", "other", "Order something else/receive support"))
                .set_id("order_type"))));
        }
        });
    event.reply();
}

void delete_all_messages(dpp::cluster& bot, const dpp::select_click_t& event)
{
    bot.messages_get(event.command.channel_id, 0, 0, 0, 5, [&bot](const dpp::confirmation_callback_t& event) {
        if (!event.is_error()) {
            auto messages = std::get<dpp::message_map>(event.value);
            for (auto& message : messages) {
                bot.message_delete(message.second.id, message.second.channel_id);
            }
        }
        });
}

void champion_picking_process(const dpp::select_click_t& event, dpp::cluster& bot)
{
    std::vector<std::string> champions = { "Aatrox","Ahri","Akali","Akshan","Alistar","Amumu","Anivia","Annie","Aphelios","Ashe","Aurelion Sol","Azir","Bard","Bel'Veth","Blitzcrank","Brand","Braum","Caitlyn","Camille","Cassiopeia","Cho'Gath","Corki","Darius","Diana","Dr. Mundo","Draven","Ekko","Elise","Evelynn","Ezreal","Fiddlesticks","Fiora","Fizz","Galio","Gangplank","Garen","Gnar","Gragas","Graves","Gwen","Hecarim","Heimerdinger","Illaoi","Irelia","Ivern","Janna","Jarvan IV","Jax","Jayce","Jhin","Jinx","Kai'Sa","Kalista","Karma","Karthus","Kassadin","Katarina","Kayle","Kayn","Kennen","Kha'Zix","Kindred","Kled","Kog'Maw","LeBlanc","Lee Sin","Leona","Lillia","Lissandra","Lucian","Lulu","Lux","Malphite","Malzahar","Maokai","Master Yi","Miss Fortune","Mordekaiser","Morgana","Nami","Nasus","Nautilus","Neeko","Nidalee","Nilah" ,"Nocturne","Nunu & Willump","Olaf","Orianna","Ornn","Pantheon","Poppy","Pyke","Qiyana","Quinn","Rakan","Rammus","Rek'Sai","Rell","Renata Glasc","Renekton","Rengar","Riven","Rumble","Ryze","Samira","Sejuani","Senna","Seraphine","Sett","Shaco","Shen","Shyvana","Singed","Sion","Sivir","Skarner","Sona","Soraka","Swain","Sylas","Syndra","Tahm Kench","Taliyah","Talon","Taric","Teemo","Thresh","Tristana","Trundle","Tryndamere","Twisted Fate","Twitch","Udyr","Urgot","Varus","Vayne","Veigar","Vel'Koz","Vex","Vi","Viego","Viktor","Vladimir","Volibear","Warwick","Wukong","Xayah","Xerath","Xin Zhao","Yasuo","Yone","Yorick","Yuumi","Zac","Zed","Zeri","Ziggs","Zilean","Zoe","Zyra", };
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::green)
        .set_description("Choose champion!");
    dpp::component select_menu = dpp::component();
    select_menu.type = dpp::cot_selectmenu;
    select_menu.custom_id = "champion_name_0";
    dpp::message m = dpp::message(event.command.channel_id, embed);
    for (int i = 1; i <= 125; i++) {
        if (i % 25 == 0) {
            select_menu.options.push_back(dpp::select_option(champions[i - 1], champions[i - 1], "Choose " + champions[i - 1]));
            select_menu.custom_id = "champion_name_" + std::to_string(i / 25);
            m.add_component(dpp::component().add_component(select_menu));
            select_menu.options.clear();
        }
        else {
            select_menu.options.push_back(dpp::select_option(champions[i - 1], champions[i - 1], "Choose " + champions[i - 1]));
        }
    }
    bot.message_create(m);
    embed.description = "Choose champion part 2!";
    m = dpp::message(event.command.channel_id, "");
    select_menu.options.clear();
    for (int i = 126; i <= 150; i++) {
        if (i % 25 == 0) {
            select_menu.options.push_back(dpp::select_option(champions[i - 1], champions[i - 1], "Choose " + champions[i - 1]));
            select_menu.custom_id = "champion_name_6";
            m.add_component(dpp::component().add_component(select_menu));
            select_menu.options.clear();
        }
        else {
            select_menu.options.push_back(dpp::select_option(champions[i - 1], champions[i - 1], "Choose " + champions[i - 1]));
        }
    }
    select_menu.options.clear();
    for (int i = 151; i <= champions.size(); i++) {
        select_menu.options.push_back(dpp::select_option(champions[i - 1], champions[i - 1], "Choose " + champions[i - 1]));
    }
    select_menu.custom_id = "champion_name_7";
    m.add_component(dpp::component().add_component(select_menu));
    bot.message_create(m);
}

void skin_picking_process(const std::vector<skin>& skins, const dpp::select_click_t& event, dpp::cluster& bot)
{
    std::vector<dpp::select_option> select_menu_skins;
    for (auto& s : skins) {
        if (s.name.find(event.values[0]) != std::string::npos) {
            dpp::select_option option;
            option.label = s.name;
            option.value = s.name;
            option.description = "Price: " + s.price_rp + " RP - " + std::to_string(s.price_zl) + " ZL";
            select_menu_skins.push_back(option);
        }
    }
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::green)
        .set_description("Available skins for this champion:");
    dpp::component select_menu = dpp::component();
    select_menu.type = dpp::cot_selectmenu;
    select_menu.custom_id = "skin_select";
    select_menu.options = select_menu_skins;
    dpp::message m = dpp::message(event.command.channel_id, embed);
    m.add_component(dpp::component().add_component(select_menu));
    bot.message_create(m);
}