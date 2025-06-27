--[[
###########################
# Author: VegaS			  #
# Date start: 2017.02.07  #
# Hour start: 01:41       #
# Quest: Tournament PvP   #
###########################
]]

quest tournament_pvp begin
	state start begin
		function translate()
			local __var = {
				["title_name"]	=	"Tournament PvP",
				["title_name_select"]	=	"Tournament PvP",
				["yes"]	=	"Yes",
				["no"]	=	"No",
				["error_channel"]	=	"You need to be on channel 1 for can do this.",

				["letter_a"]	=	"Do you want to enter the map as spectator and view the fights or just to register for the next fight?",
				["letter_b"]	=	"Do you want to leave the map?",

				["info_online_title"]	=	"Players that registered for the tournament:",			
				["info_online_low"]		=	"Low category: | |cFFffa02a|h%d|h|r |",
				["info_online_medium"]	=	"Medium category: | |cFFffa02a|h%d|h|r |",
				["info_online_hard"]	=	"Hard category: | |cFFffa02a|h%d|h|r |",

				["select_general"]	=	"General informations about the tournament",
				["select_timer"]	=	"Informations about the time",
				["select_register"]	=	"Register",
				["delete_register"]	=	"Delete register",
				["select_close"]	=	"Close",

				["info_timer_title"]	=	"Here you can see the hour when the PVP tournament starts and ends for each category.",
				["info_current_timer"]		=	"Current timer server: | |cFFf0ff00|h%s|h|r",	
				["info_timer_low"]		=	"Low: \t\t\t|cFF14ff48|h%s|h|r - |cFFff4c4c|h%s|h|r",
				["info_timer_medium"]	=	"Medium:\t\t|cFF14ff48|h%s|h|r - |cFFff4c4c|h%s|h|r",
				["info_timer_hard"]		=	"Hard: \t\t\t|cFF14ff48|h%s|h|r - |cFFff4c4c|h%s|h|r",

				["register_title"]		=	"Welcome to the register zone.[ENTER]Select the category you prefer to register.",
				["register_low_select"]	=	"Low category (%d-%d)",
				["register_medium_select"]	=	"Medium category (%d-%d)",
				["register_hard_select"]	=	"Hard category (%d-%d)",
				["register_blocked_key"]	=	"You are already registered for the tournament, you can't do it twice!",
				["register_question"]	=	"Are you sure you want to register on this category?",
				["register_need_level"]	=	"You can't register here, your level is not able with this category.",
				["register_wrong_map"]	=	"You need to be in tournament map!",
				["register_succes"]	=	"You have registered successfully %s.",

				["information_line_1"]	= "We’ve identified areas where PvP rewards could be more satisfying. Along the way, we made a hot tournament pvp.",
				["information_line_2"]	= "Players can register all the time, even if the tournament started or ended, but for the next round.",
				["information_line_3"]	= "The tournament selects 20 players randomly from each category and devides them in two teams, 10 players each.",
				["information_line_4"]	= "It dosen't matter in what empire you are, all player protection types are changed and now when you are in the tournament you can attack only the enemy team.",
				["information_line_5"]	= "You can't attack your teammates, just the enemy team. All PVP options are disabled while you are in the tournament.",
				["information_line_6"]	= "Each team has their own color (Red and Blue), also the color of names ar matching the color of the teams.",
				["information_line_7"]	= "When the tournament starts you will have a graphic interface in the top of the screen were you can see the details for each team, such as: The noumber of players left standing, noumber of players killed, noumber of lifes left.",
				["information_line_8"]	= "After the tournament starst you can't use: Duel, Party, Ring marriage, Poly, Mount, Horse",
				["information_line_9"]	= "When you will be killed by the enemy team, your lifes will start to drop, each player has 3 lifes, after exhausting them you will be eliminated from the tournament.",
				["information_line_10"]	= "If you disconnect while you are in the tournament, you will be eliminated.",
				["information_line_11"]	= "The team that has the most members left standing will win the tournament.",
				["information_line_12"]	= "Each member of the team will get a case witch has consistent prizes.",
				["information_line_13"]	= "To find when all categoryes start you will have to go to the time information zone.",
				
				["delete_register_ask"]	= "You want to delete your registration from tournament?",	
				["delete_register_denied"]	= "You are not register in any category for can do that.",
				["delete_register_succes"]	= "Your registration was removed from tournament with successfully.",				
				
				["category_low"]	=	"Low",
				["category_medium"]	=	"Medium",
				["category_hard"]	=	"Hard"
			}
			return __var
		end
		
		function title()
			say_title(tournament_pvp.translate()["title_name"])
		end

		function get_category(key)
			local __var = {
				[1] = tournament_pvp.translate()["category_low"],
				[2] = tournament_pvp.translate()["category_medium"],
				[3] = tournament_pvp.translate()["category_hard"]}
			return __var[key]
		end
		
		function need_level()
			local __var = {
				[1] = {  30,   55   },
				[2] = {  55,   80   },
				[3] = {  80,   105  }}
			return __var
		end
		
		when login begin
			send_letter(tournament_pvp.translate()["title_name"])
		end

		when button or info begin
			tournament_pvp.title()
			
			if not pc.get_channel_id() == 1 then
				say(translate["error_channel"])
				return
			end
			
			local isRunning = tournament.is_running()
			local isTournamentMap = tournament.is_map()
			local translate = tournament_pvp.translate()
			
			if isTournamentMap then
				say(translate["letter_b"])

			elseif not isTournamentMap then
				say(translate["letter_a"])
			end

			local selected = select(translate["yes"], translate["no"])
			if selected == 1 then
				if isTournamentMap then
					warp_to_village()
					return
				end

				if not isTournamentMap then
					tournament.observer()
					return
				end
			else
				send_letter(tournament_pvp.translate()["title_name"])
			end
		end
	
		when 20092.chat."Tournament PvP" with pc.get_channel_id() == 1 begin
			local
				TOURNAMENT_INFO_START = 0
				TOURNAMENT_INFO_FINISH = 1
				TOURNAMENT_CATEGORY_LOW = 1
				TOURNAMENT_CATEGORY_MEDIUM = 2
				TOURNAMENT_CATEGORY_HARD = 3
				TOURNAMENT_MAX_CATEGORY = 3
				TOURNAMENT_INFO_GENERAL = 1
				TOURNAMENT_INFO_TIMER = 2
				TOURNAMENT_REGISTER = 3
				TOURNAMENT_DELETE_REGISTER = 4
				TOURNAMENT_CLOSE_WINDOW = 5
				translate = tournament_pvp.translate()

			if tournament.is_map() then
				tournament_pvp.title()

				say(translate["info_online_title"])
				say(string.format(translate["info_online_low"], tournament.participants(TOURNAMENT_CATEGORY_LOW)))
				say(string.format(translate["info_online_medium"], tournament.participants(TOURNAMENT_CATEGORY_MEDIUM)))
				say(string.format(translate["info_online_hard"], tournament.participants(TOURNAMENT_CATEGORY_HARD)))

				local selected = select(translate["select_general"], translate["select_timer"], translate["select_register"], translate["delete_register"], translate["select_close"])

				if selected == TOURNAMENT_CLOSE_WINDOW then
					return
				end
				
				if selected == TOURNAMENT_INFO_TIMER then
					tournament_pvp.title()
					say(translate["info_timer_title"])
					say("")
					say(string.format(translate["info_current_timer"], tournament.info_current_timer()))
					say("")
					say(string.format(translate["info_timer_low"], tournament.info_timer(TOURNAMENT_INFO_START, TOURNAMENT_CATEGORY_LOW), tournament.info_timer(TOURNAMENT_INFO_FINISH, TOURNAMENT_CATEGORY_LOW)))
					say(string.format(translate["info_timer_medium"], tournament.info_timer(TOURNAMENT_INFO_START, TOURNAMENT_CATEGORY_MEDIUM), tournament.info_timer(TOURNAMENT_INFO_FINISH, TOURNAMENT_CATEGORY_MEDIUM)))
					say(string.format(translate["info_timer_hard"], tournament.info_timer(TOURNAMENT_INFO_START, TOURNAMENT_CATEGORY_HARD), tournament.info_timer(TOURNAMENT_INFO_FINISH, TOURNAMENT_CATEGORY_HARD)))
				end
				
				if selected == TOURNAMENT_INFO_GENERAL then
					tournament_pvp.title()
						say(translate["information_line_1"])
						say(translate["information_line_2"])
						say(translate["information_line_3"])
						say(translate["information_line_4"])
						say(translate["information_line_5"])
					wait()
						tournament_pvp.title()
						say(translate["information_line_6"])
						say(translate["information_line_7"])
						say(translate["information_line_8"])
					wait()
						tournament_pvp.title()
						say(translate["information_line_9"])
						say(translate["information_line_10"])
						say(translate["information_line_11"])
						say(translate["information_line_12"])
						say(translate["information_line_13"])
						say("")
				end
				
				if selected == TOURNAMENT_DELETE_REGISTER then
					tournament_pvp.title()
					say(translate["delete_register_ask"])
					local sel = select(translate["yes"], translate["no"])
					local isRegistered = tournament.get_is_register()
					
					if sel == 2 then
						return
					end
					if sel == 1 then
						if isRegistered then
							tournament_pvp.title()
							say(translate["delete_register_succes"])
							tournament.delete_register()
						else
							say(translate["delete_register_denied"])
						end
					end
				end
				
				if selected == TOURNAMENT_REGISTER then
					tournament_pvp.title()
					say(translate["title_welcome"])

					local
						settings = tournament_pvp.need_level()
						isRegistered = tournament.get_is_register()
						level = pc.get_level()

						catLow = string.format(translate["register_low_select"], settings[1][1], settings[1][2])
						catMedium = string.format(translate["register_medium_select"], settings[2][1], settings[2][2])
						catHard = string.format(translate["register_hard_select"], settings[3][1], settings[3][2])

					local selected = select(catLow, catMedium, catHard)
						
					for key = 1, TOURNAMENT_MAX_CATEGORY do
						if selected == key then
						
							if isRegistered then
								tournament_pvp.title()
								say(translate["register_blocked_key"])
								return
							end
						
							if level >= settings[key][1] and level <= settings[key][2] then
								tournament_pvp.title()
								say(translate["register_question"])
								local selected = select(translate["yes"], translate["no"])

								if selected == 2 then
									return
								else
									tournament.register(key)
									tournament_pvp.title()
									say(string.format(translate["register_succes"], tournament_pvp.get_category(key)))
								end
							else
								tournament_pvp.title()
								say(translate["register_need_level"])
							end
						end
					end
				end
			else
				tournament_pvp.title()
				say(translate["register_wrong_map"])
			end
		end
	end
end