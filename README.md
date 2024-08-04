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

For my situation, which is for Pokemon Emerald Battles, the four steps are followed but are modified to fit into that usage for the AI of the game. I modified LittleRoot Town (So its scripts and JSON files) to implement all the Gym Leaders into the map and did all of my testing and implementation using Roxanne. For this I gave her a full team of 6 that scaled with my teams levels, and some healing items. This also only works for post game matches effectively so future work needed to make it have ability to check users and ai's party evolutions to modify its decision-making.

### Retain

### Retrieve

### Reuse

### Revise
