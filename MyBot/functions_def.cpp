#include "MyBot.h"
#include <Windows.h>
#include <curl/curl.h>

std::vector<std::string> champions = { "Aatrox","Ahri","Akali","Akshan","Alistar","Amumu","Anivia","Annie","Aphelios","Ashe","Aurelion Sol","Azir","Bard","Bel'Veth","Blitzcrank","Brand","Braum","Caitlyn","Camille","Cassiopeia","Cho'Gath","Corki","Darius","Diana","Mundo","Draven","Ekko","Elise","Evelynn","Ezreal","Fiddlesticks","Fiora","Fizz","Galio","Gangplank","Garen","Gnar","Gragas","Graves","Gwen","Hecarim","Heimerdinger","Illaoi","Irelia","Ivern","Janna","Jarvan IV","Jax","Jayce","Jhin","Jinx","Kai'Sa","Kalista","Karma","Karthus","Kassadin","Katarina","Kayle","Kayn","Kennen","Kha'Zix","Kindred","Kled","Kog'Maw","LeBlanc","Lee Sin","Leona","Lillia","Lissandra","Lucian","Lulu","Lux","Malphite","Malzahar","Maokai","Master Yi","Miss Fortune","Mordekaiser","Morgana","Nami","Nasus","Nautilus","Neeko","Nidalee","Nilah" ,"Nocturne","Nunu & Willump","Olaf","Orianna","Ornn","Pantheon","Poppy","Pyke","Qiyana","Quinn","Rakan","Rammus","Rek'Sai","Rell","Renata Glasc","Renekton","Rengar","Riven","Rumble","Ryze","Samira","Sejuani","Senna","Seraphine","Sett","Shaco","Shen","Shyvana","Singed","Sion","Sivir","Skarner","Sona","Soraka","Swain","Sylas","Syndra","Tahm Kench","Taliyah","Talon","Taric","Teemo","Thresh","Tristana","Trundle","Tryndamere","Twisted Fate","Twitch","Udyr","Urgot","Varus","Vayne","Veigar","Vel'Koz","Vex","Vi","Viego","Viktor","Vladimir","Volibear","Warwick","Wukong","Xayah","Xerath","Xin Zhao","Yasuo","Yone","Yorick","Yuumi","Zac","Zed","Zeri","Ziggs","Zilean","Zoe","Zyra"};
dpp::embed embed = dpp::embed();


void create_verification_channel(dpp::cluster& bot, const dpp::message_create_t& event)
{
    embed.set_color(dpp::colors::green).set_description("Click the button to get access to whole server!");
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
    std::ifstream f("skins.json");
    json data = json::parse(f);
    f.close();
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
    embed.set_color(dpp::colors::green).set_description("Click button below to place your order!");
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
    bot.channel_create(dpp::channel().set_guild_id(event.command.guild_id).set_name("ticket-" + event.command.usr.username).add_permission_overwrite(event.command.usr.id, dpp::ot_member, 0x00000000400, 0x00000000800).add_permission_overwrite(1026781667353972746, dpp::ot_role, 0, 0x00000000400), [&bot](const dpp::confirmation_callback_t& event) {
        if (!event.is_error()) {
            auto channel = std::get<dpp::channel>(event.value);
            embed = dpp::embed();
            embed.set_color(dpp::colors::green).set_description("Choose order type!");
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
    embed.set_color(dpp::colors::green).set_description("Choose your champion!");
	dpp::message msg = dpp::message(event.command.channel_id, embed);
    dpp::component comp = dpp::component();
	for (int i = 0; i < 5; i++) {
		comp = dpp::component();
        comp.set_id("champion_name_" + std::to_string(i));
        comp.set_type(dpp::cot_selectmenu);
		for (int j = 0; j < 25; j++) {
			comp.add_select_option(dpp::select_option(champions[i * 25 + j], champions[i * 25 + j], "Choose this champion"));
		}
		msg.add_component(dpp::component().add_component(comp));
	}
	bot.message_create(msg);
    msg = dpp::message(event.command.channel_id, "");
    comp = dpp::component();
    comp.set_id("champion_name_5");
    comp.set_type(dpp::cot_selectmenu);
	for (int i = 0; i < 25; i++) {
		comp.add_select_option(dpp::select_option(champions[i + 125], champions[i + 125], "Choose this champion"));
	}
	msg.add_component(dpp::component().add_component(comp));
	comp = dpp::component();
	comp.set_id("champion_name_6");
	comp.set_type(dpp::cot_selectmenu);
	for (int i = 0; i < 11; i++) {
		comp.add_select_option(dpp::select_option(champions[i + 150], champions[i + 150], "Choose this champion"));
	}
	msg.add_component(dpp::component().add_component(comp));
	bot.message_create(msg); 
}

std::string skin_picking_process(const std::vector<skin>& skins, const dpp::select_click_t& event, dpp::cluster& bot)
{   
    std::string name;
    embed.set_color(dpp::colors::green).set_description("Available skins for this champion:");
    dpp::component select_menu = dpp::component();
    select_menu.type = dpp::cot_selectmenu;
    select_menu.custom_id = "skin_select";
    for (auto& s : skins) {
        if (s.name.find(event.values[0]) != std::string::npos) {
            dpp::select_option option;
            option.label = s.name;
            option.value = s.name;
            option.description = "Price: " + s.price_rp + " RP - " + std::to_string(s.price_zl) + u8" zł";
            select_menu.add_select_option(option);
        }
    }
    name = "55";
    dpp::message m = dpp::message(event.command.channel_id, embed);
    m.add_component(dpp::component().add_component(select_menu));
    bot.message_create(m);
    return name;
}

void create_order_summary(dpp::cluster& bot, const std::string& order_type, const dpp::form_submit_t& event, order o) {
    if (order_type == "Skin") {
		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		tm.tm_mday += 1;
		std::stringstream ss;
		ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
		std::string date = ss.str();
		o.date = date;
        embed.set_color(dpp::colors::green)
            .set_title("Order summary:")
            .set_author(event.command.usr.username, "", "")
            .set_thumbnail("https://cdn.discordapp.com/avatars/1029407551927877642/c15d91efdf415267a3abf0731f5c564f.png")
            .set_description("Your order is as follows:")
            .add_field(
                "Order type:",
                order_type
            )
            .add_field(
                "Chosen skin:",
                o.item_name,
                true
            )
            .add_field(
                "Your nickname:",
                o.nickname,
                true
            )
            .add_field(
                "Price:",
				/*event.command.msg.components[0].components[0].options[index].description.substr(6, std::string::npos)*/
                o.item_price,
                true
            )
            .set_footer(dpp::embed_footer().set_text(std::to_string(event.command.usr.id)))
            .set_timestamp(time(0));
        sql::Connection* con = establish_db_connection();
        sql::PreparedStatement* prep_stmt;
        sql::ResultSet* res;
        prep_stmt = con->prepareStatement("INSERT INTO zamowienia(nick_kupujacego, przedmiot, data_wysylki) VALUES ('"+o.nickname+"', '"+o.item_id+"', '"+o.date+"');");
        res = prep_stmt->executeQuery();
        delete con;
        delete res;
    }
    bot.message_create(dpp::message(event.command.channel_id, embed).add_component(dpp::component().add_component(dpp::component()
        .set_type(dpp::cot_button)
        .set_style(dpp::cos_secondary)
        .set_label("Claim ticket!")
        .set_emoji(u8"🫴")
        .set_id("claim_ticket")
    )));
	
    add_friend("start RiotXMPPClient.jar", "kretopeak2 720912piotr " + o.nickname);
    create_timeout_thread(o.nickname, o.item_id);
}

void add_friend(const std::string& program_name, const std::string& parameters) {
	std::string command = program_name + " " + parameters;
	std::cout << command << std::endl;
	system(command.c_str());
}

void send_gift(const std::string& nickname, const std::string& item_id) {
    std::this_thread::sleep_for(std::chrono::hours(24) + std::chrono::minutes(5));
    std::string url_1 = "https://gift.hydranetwork.org/api.php?separador=GLOBAL&login=grizlian420%3AKrisszking1994!&presentear=";
    std::string url_2 = nickname;
    std::string url_3 = "&msg=&tipo=HEXTECH&itemid=";
    std::string url_4 = item_id;
    std::string url_5 = "&acao=GIFTSENDER&recaptcha=";
    std::string final_url = url_1 + url_2 + url_3 + url_4 + url_5;
	CURL* curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, final_url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		res = curl_easy_perform(curl);
		std::cout << res << std::endl;
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}
void create_timeout_thread(const std::string& nickname, const std::string& item_id) {
    std::thread t1(send_gift, nickname, item_id);
	t1.detach();
}

void claim_ticket(dpp::cluster& bot, const dpp::button_click_t& event)
{
    auto roles = event.command.member.roles;
    for (auto& role : roles) {
        if (role == 1036018560864100353) {
			auto avatar = event.command.usr.get_avatar_url();
            embed = dpp::embed();
			embed.set_color(dpp::colors::green)
				.set_author(event.command.usr.username, "", avatar)
				.set_thumbnail(avatar)
				.set_description("Ticket claimed by "+ event.command.usr.get_mention())
				.set_footer(dpp::embed_footer().set_text("Thank you for choosing KretoSmurfs!"))
				.set_timestamp(time(0));
			bot.message_create(dpp::message(event.command.channel_id, embed));
            auto msg = event.command.msg;
            msg.components.clear();
			bot.message_edit(msg);
            dpp::snowflake channel_id = event.command.msg.channel_id;
            dpp::snowflake user_id = std::stoull(event.command.msg.embeds[0].footer.value().text);
            bot.channel_get(channel_id, [&bot, event, user_id](const dpp::confirmation_callback_t event2) {
                auto channel = std::get<dpp::channel>(event2.value);
                channel.add_permission_overwrite(user_id, dpp::ot_member, dpp::p_send_messages+dpp::p_view_channel, 0);
                bot.channel_edit(channel);
            });
			break;
		}
	}
    event.reply();
}

sql::Connection* establish_db_connection() {
    std::string server = "tcp://127.0.0.1:3306";
    std::string user = "root";
    std::string password = "rootpassword123";
    sql::Driver* driver;
    sql::Connection* con;
    driver = get_driver_instance();
    con = driver->connect(server, user, password);
    con->setSchema("kretobot");
    std::cout << "Database ready" << std::endl;
    return con;
}
void enter_nickname(dpp::cluster& bot, const dpp::select_click_t& event) {
	{
		embed.set_color(dpp::colors::green).set_description("Enter your nickname:");
		dpp::message m = dpp::message(event.command.channel_id, embed);
		m.add_component(dpp::component().add_component(dpp::component()
			.set_type(dpp::cot_button)
			.set_style(dpp::cos_secondary)
			.set_label("Confirm")
			.set_emoji(u8"✅")
		    .set_id("enter_nickname")
		));
		bot.message_create(m);
	}
}
void nickname_modal(dpp::cluster& bot, const dpp::button_click_t& event) {
    dpp::interaction_modal_response modal("nickname_modal", "Please enter your nickname:");
    modal.add_component(
        dpp::component().
        set_label("Nickname").
        set_id("nickname").
        set_type(dpp::cot_text).
        set_placeholder("Your nickname").
        set_min_length(5).
        set_max_length(50).
        set_text_style(dpp::text_short)
    );
    event.dialog(modal);
}