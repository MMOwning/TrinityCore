#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

class npc_first_char : public CreatureScript
{
		public: npc_first_char() : CreatureScript("npc_first_char"){ }

				bool OnGossipHello(Player *pPlayer, Creature* _creature)
				{
					pPlayer->ADD_GOSSIP_ITEM(7, "Was tut dieser NPC?", GOSSIP_SENDER_MAIN, 0);
					pPlayer->ADD_GOSSIP_ITEM(7, "Firstausstattung", GOSSIP_SENDER_MAIN, 1);
					pPlayer->ADD_GOSSIP_ITEM(7, "Gildenaufwertung 10er", GOSSIP_SENDER_MAIN, 2);
					pPlayer->ADD_GOSSIP_ITEM(7, "Gildenaufwertung 25er", GOSSIP_SENDER_MAIN, 3);
					
					if (pPlayer->IsGameMaster()){
						pPlayer->ADD_GOSSIP_ITEM(7, "Aufwertungen einsehen", GOSSIP_SENDER_MAIN, 4);
					}
					
					pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
					return true;


				}

				bool OnGossipSelect(Player * pPlayer, Creature * pCreature, uint32 /*uiSender*/, uint32 uiAction)
				{
					switch (uiAction)
					{
					case 1:
						{

							uint32 guid = pPlayer->GetGUID();
							
							QueryResult accountres = CharacterDatabase.PQuery("SELECT account FROM characters WHERE guid = %u", guid);
							uint32 accountresint = (*accountres)[0].GetUInt32();
							QueryResult charresult = CharacterDatabase.PQuery("Select count(guid) From characters where account = '%u'", accountresint);
							uint32 charresultint = (*charresult)[0].GetUInt32();

						
							QueryResult ipadr = LoginDatabase.PQuery("SELECT last_ip FROM account where id = %u", accountresint);
							std::string ipadrint = (*ipadr)[0].GetString();
							QueryResult ipadrcount = LoginDatabase.PQuery("SELECT count(last_ip) FROM account WHERE last_ip = '%s'", ipadrint);
							uint32 ipadrcountint = (*ipadrcount)[0].GetUInt32();
							
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("ipadrcountint: %u", ipadrcountint,
								pPlayer->GetName());
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("ipadrint: %s", ipadrint,
								pPlayer->GetName());
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("charresultint %u", charresultint,
								pPlayer->GetName());
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("accountresint: %u", accountresint,
								pPlayer->GetName());
						
							
							

							 if (charresultint == 1 && ipadrcountint == 1){
								
								 time_t sek;
								 time(&sek);
								 uint32 zeit = time(&sek);
								 ChatHandler(pPlayer->GetSession()).PSendSysMessage("Deine Aufwertung wurde ausgef�hrt. Viel Spa� w�nscht Exitare sowie das MMOwning-Team.",
									 pPlayer->GetName());
								pPlayer->PlayerTalkClass->SendCloseGossip();
								pPlayer->SetLevel(80);
								pPlayer->TeleportTo(0, -4781.02, 1793.83, 132.98, 3.36, 0);
								pPlayer->AddItem(20400, 4);
								pPlayer->LearnDefaultSkill(762, 4);
								pPlayer->SetMoney(50000000);
								
								pPlayer->PlayerTalkClass->SendCloseGossip();	

								CharacterDatabase.PExecute("REPLACE INTO first_Char "
									"(guid, account, time, guildid,ip) "
									"VALUES ('%u', '%u', %u, %u, '%s')",
									guid, accountresint, zeit,0 , ipadrint);

				     			return true;
							}
							 							
							 else {
								 ChatHandler(pPlayer->GetSession()).PSendSysMessage("Du hast bereits einen anderen Charakter auf diesem Realm oder versuchst mit\nzu vielen Accounts eine Erstaustattung zu erlangen.\nDiese wird daher abgelehnt.\nMfG Exitare und das MMOwning-Team.",
								 pPlayer->GetName());
								 pPlayer->PlayerTalkClass->SendCloseGossip();
								 return true;
							}
							 
							

						}break;

					case 0:
						{
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("Dieser NPC vergibt deine Erstaustattung. Klicke einfach auf 'Firstausstattung'und es beginnt.", 
							pPlayer->GetName());
							pPlayer->PlayerTalkClass->SendCloseGossip();
							return true;
						}break;
		

					case 2:
						{
							uint32 guid = pPlayer->GetGUID();
							
							bool gild = pPlayer->GetGuild();

							if (gild)
							{
								QueryResult accountres = CharacterDatabase.PQuery("SELECT account FROM characters WHERE guid = %u", guid);
								uint32 accountresint = (*accountres)[0].GetUInt32();
								QueryResult charresult = CharacterDatabase.PQuery("Select count(guid) From characters where account = '%u'", accountresint);
								uint32 charresultint = (*charresult)[0].GetUInt32();
						
								/*Gildenselect*/
								QueryResult guildid = CharacterDatabase.PQuery("SELECT guildid FROM guild_member WHERE guid= %u", guid);
								uint32 guildidint = (*accountres)[0].GetUInt32();

								/*Gildenmemberanzahl*/
								QueryResult guildmember = CharacterDatabase.PQuery("SELECT count(guid) FROM guild_member WHERE guildid = %u", guildidint);
								uint32 guildmemberint = (*guildmember)[0].GetUInt32();

								/*Gildenerstelldatum*/
								QueryResult guildcreate = CharacterDatabase.PQuery("SELECT createdate FROM guild WHERE guildid = %u", guildidint);
								uint32 guildcreateint = (*guildcreate)[0].GetUInt32();


								QueryResult ipadr = LoginDatabase.PQuery("SELECT last_ip FROM account where id = %u", accountresint);
								std::string ipadrint = (*ipadr)[0].GetString();
								/*IPAdresse auslesen*/
								QueryResult ipadrcount = LoginDatabase.PQuery("SELECT count(last_ip) FROM account WHERE last_ip = '%s'", ipadrint);
								uint32 ipadrcountint = (*ipadrcount)[0].GetUInt32();

								time_t sek;
								time(&sek);
								uint32 zeit = time(&sek);
								uint32 zeitraum = zeit - guildcreateint;

								/*ChatHandler(pPlayer->GetSession()).PSendSysMessage("Memberanzahl: %u", guildmemberint,
									pPlayer->GetName());
								ChatHandler(pPlayer->GetSession()).PSendSysMessage("Gildeid: %u", guildidint,
									pPlayer->GetName());
								ChatHandler(pPlayer->GetSession()).PSendSysMessage("Gildecreatedate: %u", guildcreateint,
									pPlayer->GetName());
								ChatHandler(pPlayer->GetSession()).PSendSysMessage("Zeit: %u", zeit,
									pPlayer->GetName());
								ChatHandler(pPlayer->GetSession()).PSendSysMessage("Zeitraum: %u", zeitraum,
									pPlayer->GetName());

								pPlayer->PlayerTalkClass->SendCloseGossip();*/

								if (guildmemberint >= 10 && guildmemberint < 25 && zeitraum <1209600 && charresultint == 1 && ipadrcountint == 1){
									pPlayer->SetLevel(80);
									pPlayer->LearnDefaultSkill(762, 300);
									pPlayer->TeleportTo(0, -4781.02, 1793.83, 132.98, 3.36, 0);
									pPlayer->AddItem(20400, 4);
									pPlayer->SetMoney(50000000);

									CharacterDatabase.PExecute("REPLACE INTO first_Char "
										"(guid, account, time, guildid,ip) "
										"VALUES ('%u', '%u', %u, %u, '%s')",
										guid, accountresint, zeit, guildidint, ipadrint);
									return true;

									
								}

								
								else{
									ChatHandler(pPlayer->GetSession()).PSendSysMessage("Deine Gilde ist nicht neu, oder hat nicht genug oder zu viele Mitglieder.",
										pPlayer->GetName());
									pPlayer->PlayerTalkClass->SendCloseGossip();
									return true;
								}

							
								return true;

							}

							else {
								ChatHandler(pPlayer->GetSession()).PSendSysMessage("Tut mir Leid du bist in keiner Gilde.",
									pPlayer->GetName());
								pPlayer->PlayerTalkClass->SendCloseGossip();
								return false;
							}
							
						}break;
					

					case 3:
					{
						uint32 guid = pPlayer->GetGUID();
						bool gild = false;
						gild = pPlayer->GetGuild();

						if (gild)
						{
							QueryResult accountres = CharacterDatabase.PQuery("SELECT account FROM characters WHERE guid = %u", guid);
							uint32 accountresint = (*accountres)[0].GetUInt32();
							QueryResult charresult = CharacterDatabase.PQuery("Select count(guid) From characters where account = '%u'", accountresint);
							uint32 charresultint = (*charresult)[0].GetUInt32();

							/*Gildenselect*/
							QueryResult guildid = CharacterDatabase.PQuery("SELECT guildid FROM guild_member WHERE guid= %u", guid);
							uint32 guildidint = (*accountres)[0].GetUInt32();

							/*Gildenmemberanzahl*/
							QueryResult guildmember = CharacterDatabase.PQuery("SELECT count(guid) FROM guild_member WHERE guildid = %u", guildidint);
							uint32 guildmemberint = (*guildmember)[0].GetUInt32();

							/*Gildenerstelldatum*/
							QueryResult guildcreate = CharacterDatabase.PQuery("SELECT createdate FROM guild WHERE guildid = %u", guildidint);
							uint32 guildcreateint = (*guildcreate)[0].GetUInt32();


							QueryResult ipadr = LoginDatabase.PQuery("SELECT last_ip FROM account where id = %u", accountresint);
							std::string ipadrint = (*ipadr)[0].GetString();
							/*IPAdresse auslesen*/
							QueryResult ipadrcount = LoginDatabase.PQuery("SELECT count(last_ip) FROM account WHERE last_ip = '%s'", ipadrint);
							uint32 ipadrcountint = (*ipadrcount)[0].GetUInt32();

							time_t sek;
							time(&sek);
							uint32 zeit = time(&sek);
							uint32 zeitraum = zeit - guildcreateint;

							

							if (guildmemberint > 25 && zeitraum <1209600 && charresultint == 1 && ipadrcountint == 1){
								pPlayer->SetLevel(80);
								pPlayer->LearnDefaultSkill(762, 300);
								pPlayer->TeleportTo(0, -4781.02, 1793.83, 132.98, 3.36, 0);
								pPlayer->AddItem(20400, 4);
								pPlayer->SetMoney(50000000);

								CharacterDatabase.PExecute("REPLACE INTO first_Char "
									"(guid, account, time, guildid,ip) "
									"VALUES ('%u', '%u', %u, %u, '%s')",
									guid, accountresint, zeit, guildidint, ipadrint);
								return true;
							}


							else{
								ChatHandler(pPlayer->GetSession()).PSendSysMessage("Deine Gilde ist nicht neu, oder hat nicht genug oder zu viele Mitglieder.",
									pPlayer->GetName());
								pPlayer->PlayerTalkClass->SendCloseGossip();
								return true;
							}


							return true;

						}

						else {
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("Tut mir Leid du bist in keiner Gilde.",
								pPlayer->GetName());
							pPlayer->PlayerTalkClass->SendCloseGossip();
							return false;
						}
					}break;


					case 4:
					{
						ChatHandler(pPlayer->GetSession()).PSendSysMessage("Diese Funktion wird noch implementiert.",
							pPlayer->GetName());

						QueryResult erg = CharacterDatabase.PQuery("SELECT GUID FROM first_char where max(id)");
						QueryResult account = CharacterDatabase.PQuery("SELECT account FROM first_char where max(id)");
						


						uint32 size = 10;
						
						
							uint32 guid = (*erg)[0].GetUInt32();
							QueryResult guidname = CharacterDatabase.PQuery("SELECT name FROM characters where guid = %u", guid);
							std::string charname = (*guidname)[0].GetString();

							uint32 accountint = (*account)[0].GetUInt32();
							QueryResult accountname = LoginDatabase.PQuery("SELECT username FROM account where id = %u", accountint);
							std::string accname = (*accountname)[0].GetString();

							ChatHandler(pPlayer->GetSession()).PSendSysMessage("Charakter: %s", charname,
								pPlayer->GetName());
							ChatHandler(pPlayer->GetSession()).PSendSysMessage("Accountname: %s", accname,
								pPlayer->GetName());
							
							pPlayer->PlayerTalkClass->SendCloseGossip();
							

	      				
						}break;

					
						return true;
					}

					


					

			}
					
};
				






void AddSC_npcfirstchar()
{
	new npc_first_char();
}