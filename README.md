# Implementation of Case-Based Reasoning for Pokemon Battles
# (Description Below In Progress ...)
Simple Implementation of Case-Based Reasoning (CBR) for pokemon battle rematches of the AI in the game. Not advanced but a foundation based on my current understanding of the pillars of CBR (Retain, Retrieve, Reuse, and Revise) and the code in this Decomp. Future work from either me or whoever wishes to work on it with better experience with the code or understanding to make it more advanced is encouraged since this isn't a final form of the project. Some Future work can be to make the storage more dynamic since it only can hold a constant value amount of information for both the one gym leader and the users team based on SaveBlock3 in `global.h`.

## Important
Recordings of the CBR project is located in `Case-Based Reasoning Project Videos`.
Inside that folder is 3 parts of an entire video which are:
- Part 1 Contains the default battle initially with the NPC before any CBR is active.
- Part 2 Rematch of NPC with the CBR implementation based on AI losing from prior match
- Part 3 Rematch of NPC with CBR implementation based on AI wining from prior match

# Project Description
## Overview of Case-Based Reasoning
Case-Based Reasoning is a form of Artificial Intelligence that focuses on an experience-based approache. In storing information from the current task at hand or situation (For example a pokemon match against the NPC) it can then utilize such information for future events that it encounters that are of similarity. In doing so it can act differently depending on the information stored from the prior match and then from this new action taken see if it improved or not afterwards. All of this it to make the agent have an improved decision-making capability. 
In order for the implementation of Case-Based Reasoning to occur there are four foundational steps that are needed. These four steps are Retain, Retrieve, Reuse, and Revise (The four Rs) and each of them play a role of varying size depending on the situation at hand that we wish to create it for. 
The purpose of each are:
1. Retain: Acquire information of the situation during execution or fed old data that is already stored so that it can be used for future occurences. 
2. Retrieve: Gather the best similar data from the storage that is being used to retain such information. 
3. Reuse: Apply that retrieved case to the current task at hand.
4. Revise: Check the effectiveness of applying said case and to update and store the new gained experience.

For my situation, which is for Pokemon Emerald Battles, the four steps are followed but are modified to fit into that usage for the AI of the game. I modified LittleRoot Town (So its scripts and JSON files) to implement all the Gym Leaders into the map and did all of my testing and implementation using Roxanne. For this I gave her a full team of 6 that scaled with my teams levels, and some healing items. This also only works for post game matches effectively so future work needed to make it have ability to check users and ai's party evolutions to modify its decision-making. With an overview of what CBR is and what was set up I will now below talk about the four R's and what I did to implement and modify for it to improve the ai logic for pokemon battles. The files for this implementation are talked about below with a description of what was added or modified.

### Retain
In order to store specific data from a battle I first looked through the files for ones that involved ai battle logic. For this I used the built in DebugPrintfLevel printing statement for the decomp to display output to the log of the mGBA software used to play the game. Through this I found storing information through the `RecordKnownMove` function from `battle_ai_util.c` to work where it stores both user and npc's current pokemon, move, and typing of both pokemon and move used during each turn. If said pokemon uses the same move again during the match it won't activate and store that information again. It only will store new information it hasn't seen before. 

Now to store this information I used one of the three built in structs of the decomp for storing current player progress in the game. The newest one that was added into the decomp that I used is `SaveBlock3` which can be found in `global.h`. Here I have defined macros that are then used for the arrays stored in the saveblock which are called in the `RecordKnownMove` through the use of the external pointer for saveblock3 you can see in `global.h`. With this struct able to store information from the match I can then move onto the rest of the steps in CBR which will use `SaveBlock3`.

### Retrieve
After information is stored from the first match I then went into `trainers.h` and looked at all that is assigned for each specific trainer. Following a pokeemerald wiki on how to make my own ai flag I created mine named, `AI_FLAG_ADAPTIVE_AI` where I will attach this flag at the end of `TRAINER_ROXANNE_2` (in `trainers.h`). I also as mentioned earlier updated the other Roxanne party calles after `TRAINER_ROXANNE_1` for the rematch in `trainer_parties.h`, and with the ai flag set up and the parties matching as they did for the default match. In the `battle_ai.h` all the ai flags with their specific purposes are listed there including the adaptive flag I created. At first I was going to make all the logic on my own but due to the update of `AI_FLAG_SMART_MON_CHOICES` I looked through it and saw it has most of what I wished to use for my CBR logic. I therefore went through and found all instances of the smart-mon-choices flag being called and replaced them with my adaptive flag. With my flag now in the smart-mon-choices place I moved on and added in my own for loops and conditionals to retrieve my stored information inside the `battle_ai_switch_item.c`, particularly the `HasBadOdds` function. 

In the `HasBadOdds` I retrieve stored data from saveblock3 and store the pokemon of the user and the npc that are currently out on the field into their own respective u16 variables. This does update after each turn so if you switch out it will display on the debug print statement what the current pokemon is. In this function I also am able to retrieve the moves correctly from their respective pokemon, including the outcome of the battles where the outcome of if the ai won or lost is stored from within the `Cmd_checkteamslost` function from `battle_script_commands.c`. The outcome part plays mostly into the revising part of CBR which I'll get into later down below in the revise section.
Once I was able to show that it does indeed retrieve and display correctly the pokemon that were of the user or npc I then moved onto how to reuse such information.

### Reuse
`battle_ai_switch_item.c`, `battle_main.c`
For the reusing part of the CBR I have the ai work by checking to see if it has encountered the users pokemon on the field currently with what it knows from `SaveBlock3`. To do this I first have in the `GetMostSuitableMonToSwitchInto` from `battle_ai_switch_items.c` a conditional where it will check if the ai has certain flags set, and for this to see if the `TRAINER_ROXANNE_2` from `trainers.h` has the ai flag `AI_FLAG_ADAPTIVE_AI`. if this flag is set it will jump into `GetBestMonIntegrated` which is also in `battle_ai_switch_items.c` and use all of its logic instead. 

### Revise
`battle_script_commands.c`, `battle_ai_switch_item.c`
