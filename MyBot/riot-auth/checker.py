import requests
import riot_auth_pack
import asyncio
import sys
import base64
import json

if sys.platform == "win32":
    asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

creds = sys.stdin
line = creds.readline()
user_pass = line.split(':')
print(user_pass[0], user_pass[1])
CREDS = user_pass[0], user_pass[1]

auth = riot_auth_pack.RiotAuth()
asyncio.run(auth.authorize(*CREDS))

session = requests.Session()
session.headers.update({"Authorization": f"Bearer {auth.access_token}"})

r = session.get("https://auth.riotgames.com/userinfo")
url = "https://euc1-green.pp.sgp.pvp.net/login-queue/v2/login/products/lol/regions/EUN1"
payload = {
   "clientName": "lcu",
   "entitlements": auth.entitlements_token,
   "userinfo": r.text
}

r = session.post(url, json=payload)
# hello 
split = r.json()["token"].split(".")
decoded = base64.b64decode(split[1]+'==').decode("utf-8")

jwt_json = json.loads(decoded)
puuid = jwt_json["sub"]
acc_id = jwt_json["lol"]["uid"]
url = "https://euc1-green.pp.sgp.pvp.net/session-external/v1/session/create"
payload = {
   "claims" : {
      "cname" : "lcu"
   },
   "product" : "lol",
   "puuid" : puuid,
   "region" : "eun1"
}
session.headers.update({"Authorization": f"Bearer {r.json()['token']}"})
r = session.post(url, json=payload)


# url = f"https://eune-blue.lol.sgp.pvp.net/lolinventoryservice-ledge/v1/inventories/simple?puuid={puuid}&location=lolriot.euc1.eun1&accountId={acc_id}&inventoryTypes=CHAMPION&inventoryTypes=CHAMPION_SKIN"
# session.headers.update({"Authorization": f"Bearer {r.json()}"})

# r = session.get(url)
# print(r.text)
# url = "https://eun.store.leagueoflegends.com/storefront/v2/wallet?language=en_GB"
# session.headers.update({"Authorization": f"Bearer {auth.access_token}"})
# r = session.get(url)
print(r.text)












