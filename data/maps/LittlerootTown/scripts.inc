.set LOCALID_TWIN, 1
.set LOCALID_MOM, 4
.set LOCALID_RIVAL, 7
.set LOCALID_BIRCH, 8

LittlerootTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LittlerootTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LittlerootTown_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LittlerootTown_OnWarp
	.byte 0

	@ State descriptions for VAR_LITTLEROOT_TOWN_STATE
	@ 1: Met Rival
	@ 2: Saving Birch
	@ 3: Received Pokedex
	@ 4: Received Running Shoes

	@ State descriptions for VAR_LITTLEROOT_INTRO_STATE
	@ 1: In truck male
	@ 2: In truck female
	@ 3: Entered house
	@ 4: Told to go set clock
	@ 5: Entered 2F to set clock
	@ 6: Set clock
	@ 7: Told to go meet rival

	@ State descriptions for VAR_LITTLEROOT_HOUSES_STATE_MAY
	@                    and VAR_LITTLEROOT_HOUSES_STATE_BRENDAN
	@ 1: On Truck (and is corresponding gender)
	@ 2: Met Rival's Mom (and is corresponding gender)
	@ 3: Entered Hall of Fame
	@ 4: Completed Lati TV / SS Ticket event
	@ 5: Exited house after above event

	@ State descriptions for VAR_LITTLEROOT_RIVAL_STATE
	@ 1: Never occurs
	@ 2: Entered Rival's bedroom
	@ 3: Met Rival
	@ 4: Received Pokedex


LittlerootTown_OnTransition:
	setflag FLAG_VISITED_LITTLEROOT_TOWN
	call Common_EventScript_SetupRivalGfxId
	call_if_eq VAR_LITTLEROOT_INTRO_STATE, 2, LittlerootTown_EventScript_MoveMomToMaysDoor
	call_if_unset FLAG_RESCUED_BIRCH, LittlerootTown_EventScript_SetTwinPos
	call_if_eq VAR_LITTLEROOT_TOWN_STATE, 3, LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos
	call_if_eq VAR_LITTLEROOT_HOUSES_STATE_MAY, 4, LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent
	call_if_eq VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 4, LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent
	call_if_eq VAR_OLDALE_RIVAL_STATE, 1, LittlerootTown_EventScript_MoveRivalFromOldale
	call_if_eq VAR_LITTLEROOT_RIVAL_STATE, 3, LittlerootTown_EventScript_SetRivalLeftForRoute103
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_HideMapNamePopup
	call_if_eq VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 2, LittlerootTown_EventScript_LeftLabAfterDexUpgrade
	end

LittlerootTown_EventScript_LeftLabAfterDexUpgrade::
	setvar VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 3
	return

LittlerootTown_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

LittlerootTown_EventScript_SetRivalLeftForRoute103::
	setflag FLAG_RIVAL_LEFT_FOR_ROUTE103
	return

LittlerootTown_EventScript_MoveRivalFromOldale::
	setvar VAR_OLDALE_RIVAL_STATE, 2
	setflag FLAG_HIDE_OLDALE_TOWN_RIVAL
	return

LittlerootTown_EventScript_SetExitedHouseAfterLatiSSTicketEvent::
	setvar VAR_LITTLEROOT_HOUSES_STATE_MAY, 5
	setvar VAR_LITTLEROOT_HOUSES_STATE_BRENDAN, 5
	return

LittlerootTown_EventScript_MoveMomToMaysDoor::
	setobjectxyperm LOCALID_MOM, 14, 8
	return

LittlerootTown_EventScript_SetTwinPos::
	goto_if_eq VAR_LITTLEROOT_TOWN_STATE, 0, LittlerootTown_EventScript_SetTwinGuardingRoutePos
	setobjectxyperm LOCALID_TWIN, 10, 1
	setobjectmovementtype LOCALID_TWIN, MOVEMENT_TYPE_FACE_UP
	return

LittlerootTown_EventScript_SetTwinGuardingRoutePos::
	setobjectxyperm LOCALID_TWIN, 7, 2
	setobjectmovementtype LOCALID_TWIN, MOVEMENT_TYPE_FACE_DOWN
	return

LittlerootTown_EventScript_SetMomStandingInFrontOfDoorPos::
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setobjectmovementtype LOCALID_MOM, MOVEMENT_TYPE_FACE_DOWN
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetMomInFrontOfDoorMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_SetMomInFrontOfDoorFemale
	return

LittlerootTown_EventScript_SetMomInFrontOfDoorMale::
	setobjectxyperm LOCALID_MOM, 5, 9
	return

LittlerootTown_EventScript_SetMomInFrontOfDoorFemale::
	setobjectxyperm LOCALID_MOM, 14, 9
	return

LittlerootTown_OnFrame:
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 1, LittlerootTown_EventScript_StepOffTruckMale
	map_script_2 VAR_LITTLEROOT_INTRO_STATE, 2, LittlerootTown_EventScript_StepOffTruckFemale
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_BeginDexUpgradeScene
	.2byte 0

LittlerootTown_EventScript_StepOffTruckMale::
	lockall
	setvar VAR_0x8004, 5
	setvar VAR_0x8005, 8
	call LittlerootTown_EventScript_GoInsideWithMom
	setflag FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK
	warpsilent MAP_LITTLEROOT_TOWN_BRENDANS_HOUSE_1F, 8, 8
	waitstate
	releaseall
	end

LittlerootTown_EventScript_StepOffTruckFemale::
	lockall
	setvar VAR_0x8004, 14
	setvar VAR_0x8005, 8
	call LittlerootTown_EventScript_GoInsideWithMom
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK
	warpsilent MAP_LITTLEROOT_TOWN_MAYS_HOUSE_1F, 2, 8
	waitstate
	releaseall
	end

LittlerootTown_EventScript_GoInsideWithMom::
	delay 15
	playse SE_LEDGE
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_Movement_PlayerStepOffTruck
	waitmovement 0
	opendoor VAR_0x8004, VAR_0x8005
	waitdooranim
	addobject LOCALID_MOM
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitHouse
	waitmovement 0
	closedoor VAR_0x8004, VAR_0x8005
	waitdooranim
	delay 10
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerAtTruck
	waitmovement 0
	msgbox LittlerootTown_Text_OurNewHomeLetsGoInside, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachDoor
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_Movement_PlayerApproachDoor
	waitmovement 0
	opendoor VAR_0x8004, VAR_0x8005
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomEnterHouse
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_Movement_PlayerEnterHouse
	waitmovement 0
	setflag FLAG_HIDE_LITTLEROOT_TOWN_MOM_OUTSIDE
	setvar VAR_LITTLEROOT_INTRO_STATE, 3
	hideobjectat OBJ_EVENT_ID_PLAYER, 0
	closedoor VAR_0x8004, VAR_0x8005
	waitdooranim
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_FAT_MAN
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	return

LittlerootTown_Movement_MomExitHouse: // Beginning where Mom leaves house and talks with you after leaving truck.
	walk_down
	step_end

LittlerootTown_Movement_MomApproachPlayerAtTruck:
	walk_down
	walk_in_place_faster_left
	step_end

LittlerootTown_Movement_MomApproachDoor:
	delay_16
	delay_8
	walk_up
	step_end

LittlerootTown_Movement_MomEnterHouse:
	walk_up
	set_invisible
	step_end

LittlerootTown_Movement_PlayerApproachDoor:
	delay_16
	delay_8
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_PlayerEnterHouse:
	walk_up
	walk_up
	step_end

LittlerootTown_Movement_PlayerStepOffTruck:
	jump_right
	delay_16
	delay_16
	delay_16
	step_end

LittlerootTown_EventScript_BeginDexUpgradeScene::
	lockall
	playse SE_PIN
	applymovement LOCALID_BIRCH, Common_Movement_ExclamationMark
	waitmovement 0
	delay 80
	msgbox LittlerootTown_Text_BirchSomethingToShowYouAtLab, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_RIVAL
	clearflag FLAG_HIDE_LITTLEROOT_TOWN_BIRCH
	delay 20
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	warp MAP_LITTLEROOT_TOWN_PROFESSOR_BIRCHS_LAB, 6, 5
	waitstate
	releaseall
	end

LittlerootTown_OnWarp:
	map_script_2 VAR_DEX_UPGRADE_JOHTO_STARTER_STATE, 1, LittlerootTown_EventScript_SetRivalBirchPosForDexUpgrade
	.2byte 0

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgrade::
	addobject LOCALID_BIRCH
	addobject LOCALID_RIVAL
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeMale
	goto LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeFemale
	end

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeMale::
	setobjectxy LOCALID_RIVAL, 6, 10
	setobjectxy LOCALID_BIRCH, 5, 10
	end

LittlerootTown_EventScript_SetRivalBirchPosForDexUpgradeFemale::
	setobjectxy LOCALID_RIVAL, 13, 10
	setobjectxy LOCALID_BIRCH, 14, 10
	end

LittlerootTown_EventScript_FatMan::
	msgbox LittlerootTown_Text_CanUsePCToStoreItems, MSGBOX_NPC
	end

LittlerootTown_EventScript_Boy::
	msgbox LittlerootTown_Text_BirchSpendsDaysInLab, MSGBOX_NPC
	end

LittlerootTown_EventScript_Twin::
	lock
	faceplayer
	goto_if_set FLAG_ADVENTURE_STARTED, LittlerootTown_EventScript_GoodLuck
	goto_if_set FLAG_RESCUED_BIRCH, LittlerootTown_EventScript_YouSavedBirch
	goto_if_ne VAR_LITTLEROOT_TOWN_STATE, 0, LittlerootTown_EventScript_GoSaveBirch
	msgbox LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_GoSaveBirch::
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_CanYouGoSeeWhatsHappening, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setvar VAR_LITTLEROOT_TOWN_STATE, 2
	release
	end

LittlerootTown_EventScript_YouSavedBirch::
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_YouSavedBirch, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_GoodLuck::
	msgbox LittlerootTown_Text_GoodLuckCatchingPokemon, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_NeedPokemonTriggerLeft::
	lockall
	applymovement LOCALID_TWIN, LittlerootTown_Movement_TwinApproachPlayerLeft
	waitmovement 0
	call LittlerootTown_EventScript_DangerousWithoutPokemon
	applymovement LOCALID_TWIN, LittlerootTown_Movement_TwinReturnLeft
	waitmovement 0
	releaseall
	end

LittlerootTown_EventScript_DangerousWithoutPokemon::
	msgbox LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TWIN, LittlerootTown_Movement_TwinPushPlayerFromRoute
	applymovement OBJ_EVENT_ID_PLAYER, LittlerootTown_Movement_PushPlayerBackFromRoute
	waitmovement 0
	msgbox LittlerootTown_Text_DangerousIfYouDontHavePokemon, MSGBOX_DEFAULT
	closemessage
	return

LittlerootTown_Movement_TwinApproachPlayerLeft:
	face_right
	delay_8
	disable_jump_landing_ground_effect
	jump_in_place_right
	delay_8
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_left
	face_down
	step_end

LittlerootTown_Movement_TwinPushPlayerFromRoute:
	walk_down
	step_end

LittlerootTown_Movement_TwinReturnLeft:
	walk_right
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_in_place_faster_down
	step_end

LittlerootTown_Movement_PushPlayerBackFromRoute:
	lock_facing_direction
	walk_down
	unlock_facing_direction
	step_end

LittlerootTown_EventScript_NeedPokemonTriggerRight::
	lockall
	applymovement LOCALID_TWIN, LittlerootTown_Movement_TwinApproachPlayerRight
	waitmovement 0
	call LittlerootTown_EventScript_DangerousWithoutPokemon
	applymovement LOCALID_TWIN, LittlerootTown_Movement_TwinReturnRight
	waitmovement 0
	releaseall
	end

LittlerootTown_Movement_TwinApproachPlayerRight:
	face_right
	delay_8
	disable_jump_landing_ground_effect
	jump_in_place_right
	delay_8
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_right
	face_down
	step_end

LittlerootTown_Movement_TwinReturnRight:
	walk_left
	walk_down
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

LittlerootTown_EventScript_GoSaveBirchTrigger::
	lockall
	applymovement LOCALID_TWIN, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	special GetPlayerBigGuyGirlString
	msgbox LittlerootTown_Text_CanYouGoSeeWhatsHappening, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	setvar VAR_LITTLEROOT_TOWN_STATE, 2
	releaseall
	end

LittlerootTown_EventScript_TownSign::
	msgbox LittlerootTown_Text_TownSign, MSGBOX_SIGN
	end

LittlerootTown_EventScript_BirchsLabSign::
	msgbox LittlerootTown_Text_ProfBirchsLab, MSGBOX_SIGN
	end

LittlerootTown_EventScript_BrendansHouseSign::
	lockall
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_PlayersHouseSignMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_BirchsHouseSignFemale
	releaseall
	end

LittlerootTown_EventScript_PlayersHouseSignMale::
	msgbox LittlerootTown_Text_PlayersHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_BirchsHouseSignFemale::
	msgbox LittlerootTown_Text_ProfBirchsHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_MaysHouseSign::
	lockall
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_BirchsHouseSignMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_PlayersHouseSignFemale
	releaseall
	end

LittlerootTown_EventScript_BirchsHouseSignMale::
	msgbox LittlerootTown_Text_ProfBirchsHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_PlayersHouseSignFemale::
	msgbox LittlerootTown_Text_PlayersHouse, MSGBOX_DEFAULT
	return

LittlerootTown_EventScript_GiveRunningShoesTrigger0::
	lockall
	setvar VAR_0x8008, 0
	setobjectxy LOCALID_MOM, 10, 9
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger1::
	lockall
	setvar VAR_0x8008, 1
	setobjectxy LOCALID_MOM, 11, 9
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger2::
	lockall
	setvar VAR_0x8008, 2
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger3::
	lockall
	setvar VAR_0x8008, 3
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger4::
	lockall
	setvar VAR_0x8008, 4
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger5::
	lockall
	setvar VAR_0x8008, 5
	goto LittlerootTown_EventScript_GiveRunningShoesTrigger
	end

LittlerootTown_EventScript_GiveRunningShoesTrigger::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_MomNoticePlayerMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_MomNoticePlayerFemale
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetHomeDoorCoordsMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_SetHomeDoorCoordsFemale
	msgbox LittlerootTown_Text_WaitPlayer, MSGBOX_DEFAULT
	closemessage
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_MomApproachPlayerMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_MomApproachPlayerFemale
	call LittlerootTown_EventScript_GiveRunningShoes
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_MomReturnHomeMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_MomReturnHomeFemale
	goto LittlerootTown_EventScript_SetReceivedRunningShoes
	end

LittlerootTown_EventScript_SetHomeDoorCoordsMale::
	setvar VAR_0x8009, 5
	setvar VAR_0x800A, 8
	return

LittlerootTown_EventScript_SetHomeDoorCoordsFemale::
	setvar VAR_0x8009, 14
	setvar VAR_0x800A, 8
	return

LittlerootTown_EventScript_MomNoticePlayerMale::
	applymovement LOCALID_MOM, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

LittlerootTown_EventScript_MomNoticePlayerFemale::
	applymovement LOCALID_MOM, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomApproachPlayerMale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomApproachPlayerMale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomApproachPlayerMale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomApproachPlayerMale5
	return

LittlerootTown_EventScript_MomApproachPlayerFemale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomApproachPlayer0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomApproachPlayer1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomApproachPlayerFemale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomApproachPlayerFemale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomApproachPlayerFemale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomApproachPlayerFemale5
	return

LittlerootTown_EventScript_MomApproachPlayer0::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayer0
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayer1::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayer1
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale2::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerMale2
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale3::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerMale3
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale4::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerMale4
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerMale5::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerMale5
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale2::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerFemale2
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale3::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerFemale3
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale4::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerFemale4
	waitmovement 0
	return

LittlerootTown_EventScript_MomApproachPlayerFemale5::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomApproachPlayerFemale5
	waitmovement 0
	return

LittlerootTown_EventScript_MomReturnHomeMale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomReturnHome0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomReturnHome1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomReturnHomeMale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomReturnHomeMale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomReturnHomeMale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomReturnHomeMale5
	return

LittlerootTown_EventScript_MomReturnHomeFemale::
	call_if_eq VAR_0x8008, 0, LittlerootTown_EventScript_MomReturnHome0
	call_if_eq VAR_0x8008, 1, LittlerootTown_EventScript_MomReturnHome1
	call_if_eq VAR_0x8008, 2, LittlerootTown_EventScript_MomReturnHomeFemale2
	call_if_eq VAR_0x8008, 3, LittlerootTown_EventScript_MomReturnHomeFemale3
	call_if_eq VAR_0x8008, 4, LittlerootTown_EventScript_MomReturnHomeFemale4
	call_if_eq VAR_0x8008, 5, LittlerootTown_EventScript_MomReturnHomeFemale5
	return

LittlerootTown_EventScript_MomReturnHome0::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHome0
	waitmovement 0
	return

LittlerootTown_EventScript_MomReturnHome1::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHome1
	waitmovement 0
	return

LittlerootTown_EventScript_MomReturnHomeMale2::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeMale2
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeMale3::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeMale3
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeMale4::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeMale4
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeMale5::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeMale5
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale2::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeFemale2
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale3::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeFemale3
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale4::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeFemale4
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_EventScript_MomReturnHomeFemale5::
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomReturnHomeFemale5
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	return

LittlerootTown_Movement_MomApproachPlayer0:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_Movement_MomApproachPlayer1:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

LittlerootTown_Movement_MomApproachPlayerMale2:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerMale3:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerMale4:
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerMale5:
	walk_right
	walk_right
	walk_right
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale2:
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale3:
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale4:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomApproachPlayerFemale5:
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

LittlerootTown_Movement_MomReturnHome0:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

LittlerootTown_Movement_MomReturnHome1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

LittlerootTown_Movement_MomReturnHomeMale2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeMale3:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeMale4:
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeMale5:
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale2:
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale3:
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale4:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomReturnHomeFemale5:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

LittlerootTown_Movement_MomExitThroughDoor:
	walk_up
	step_end

LittlerootTown_EventScript_Mom::
	lock
	faceplayer
	checkplayergender
	call_if_eq VAR_RESULT, MALE, LittlerootTown_EventScript_SetHomeDoorCoordsMale
	call_if_eq VAR_RESULT, FEMALE, LittlerootTown_EventScript_SetHomeDoorCoordsFemale
	call LittlerootTown_EventScript_GiveRunningShoes
	applymovement LOCALID_MOM, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor VAR_0x8009, VAR_0x800A
	waitdooranim
	applymovement LOCALID_MOM, LittlerootTown_Movement_MomExitThroughDoor
	waitmovement 0
	hideobjectat LOCALID_MOM, MAP_LITTLEROOT_TOWN
	closedoor VAR_0x8009, VAR_0x800A
	waitdooranim
	goto LittlerootTown_EventScript_SetReceivedRunningShoes
	end

LittlerootTown_EventScript_SetReceivedRunningShoes::
	removeobject LOCALID_MOM
	setflag FLAG_SYS_B_DASH
	setvar VAR_LITTLEROOT_TOWN_STATE, 4
	release
	end

LittlerootTown_EventScript_GiveRunningShoes::
	msgbox LittlerootTown_Text_WearTheseRunningShoes, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message LittlerootTown_Text_SwitchShoesWithRunningShoes
	waitfanfare
	setflag FLAG_RECEIVED_RUNNING_SHOES
	msgbox LittlerootTown_Text_ExplainRunningShoes, MSGBOX_DEFAULT
	msgbox LittlerootTown_Text_ComeHomeIfAnythingHappens, MSGBOX_DEFAULT
	closemessage
	delay 30
	return

LittlerootTown_EventScript_Steven::
	msgbox LittlerootTown_Text_TalkToSteven, MSGBOX_NPC
	end

setvar VAR_0x800A, 0

LittlerootTown_EventScript_Roxanne::
	setvar VAR_0x8005, 0
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_Brawly::
	setvar VAR_0x8005, 1
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_Wattson::
	setvar VAR_0x8005, 2
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_Flannery::
	setvar VAR_0x8005, 3
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_Norman::
	setvar VAR_0x8005, 4
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_Winona::
	setvar VAR_0x8005, 5
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_TateAndLiza::
	setvar VAR_0x8005, 6
	call LittlerootTown_EventScript_NoBadge
	end

LittlerootTown_EventScript_Juan::
	setvar VAR_0x8005, 7
	call LittlerootTown_EventScript_NoBadge
	end

Littleroottown_EventScript_Sidney::
	setvar VAR_0x8005, 11
	lock
	msgbox LittlerootTown_Text_EliteFourMemberSidney, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_SIDNEY, LittlerootTown_Text_Sidney_Defeat
	setflag FLAG_DEFEATED_SIDNEY
	release
	end

LittlerootTown_EventScript_Phoebe::
	setvar VAR_0x8005, 12
	lock
	msgbox LittlerootTown_Text_EliteFourMemberPhoebe, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_PHOEBE, LittlerootTown_Text_Phoebe_Defeat
	release
	end

LittlerootTown_EventScript_Glacia::
	setvar VAR_0x8005, 13
	lock
	msgbox LittlerootTown_Text_EliteFourMemberGlacia, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GLACIA, LittlerootTown_Text_Glacia_Defeat
	release
	end

LittlerootTown_EventScript_Drake::
	setvar VAR_0x8005, 14
	lock
	msgbox LittlerootTown_Text_EliteFourMemberDrake, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_DRAKE, LittlerootTown_Text_Drake_Defeat
	release
	end

LittlerootTown_EventScript_Wallace::
	setvar VAR_0x8005, 15
	lock
	msgbox LittlerootTown_Text_EliteFourChampionWallace, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_WALLACE, LittlerootTown_Text_Champion_Defeat
	release
	end
	

// LittlerootTown_EventScript_BadgeCheck::
// 	lock
// 	faceplayer
// 	switch VAR_0x8004
// 	case 0 LittlerootTown_EventScript_NoBadge
// 	case 1 LittlerootTown_EventScript_FirstBadge
// 	case 2 LittlerootTown_EventScript_SecondBadge
// 	case 3 LittlerootTown_EventScript_ThirdBadge
// 	case 4 LittlerootTown_EventScript_FourthBadge
// 	case 5 LittlerootTown_EventScript_FifthBadge
// 	case 6 LittlerootTown_EventScript_SixthBadge
// 	case 7 LittlerootTown_EventScript_SeventhBadge
// 	case 8 LittlerootTown_EventScript_PostBadgeBattles
//     end

LittlerootTown_EventScript_NoBadge::
	call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_FirstBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneNoBadge
	case 1 LittlerootTown_EventScript_BrawlyNoBadge
	case 2 LittlerootTown_EventScript_WattsonNoBadge
	case 3 LittlerootTown_EventScript_FlanneryNoBadge
	case 4 LittlerootTown_EventScript_NormanNoBadge
	case 5 LittlerootTown_EventScript_WinonaNoBadge
	case 6 LittlerootTown_EventScript_TateAndLizaNoBadge
	case 7 LittlerootTown_EventScript_JuanNoBadge
	end

LittlerootTown_EventScript_FirstBadge::
	call_if_set FLAG_BADGE02_GET, LittlerootTown_EventScript_SecondBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneFirstBadge
	case 1 LittlerootTown_EventScript_BrawlyFirstBadge
	case 2 LittlerootTown_EventScript_WattsonFirstBadge
	case 3 LittlerootTown_EventScript_FlanneryFirstBadge
	case 4 LittlerootTown_EventScript_NormanFirstBadge
	case 5 LittlerootTown_EventScript_WinonaFirstBadge
	case 6 LittlerootTown_EventScript_TateAndLizaFirstBadge
	case 7 LittlerootTown_EventScript_JuanFirstBadge
	end

LittlerootTown_EventScript_SecondBadge::
	call_if_set FLAG_BADGE03_GET, LittlerootTown_EventScript_ThirdBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneSecondBadge
	case 1 LittlerootTown_EventScript_BrawlySecondBadge
	case 2 LittlerootTown_EventScript_WattsonSecondBadge
	case 3 LittlerootTown_EventScript_FlannerySecondBadge
	case 4 LittlerootTown_EventScript_NormanSecondBadge
	case 5 LittlerootTown_EventScript_WinonaSecondBadge
	case 6 LittlerootTown_EventScript_TateAndLizaSecondBadge
	case 7 LittlerootTown_EventScript_JuanSecondBadge
	end

LittlerootTown_EventScript_ThirdBadge::
	call_if_set FLAG_BADGE04_GET, LittlerootTown_EventScript_FourthBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneThirdBadge
	case 1 LittlerootTown_EventScript_BrawlyThirdBadge
	case 2 LittlerootTown_EventScript_WattsonThirdBadge
	case 3 LittlerootTown_EventScript_FlanneryThirdBadge
	case 4 LittlerootTown_EventScript_NormanThirdBadge
	case 5 LittlerootTown_EventScript_WinonaThirdBadge
	case 6 LittlerootTown_EventScript_TateAndLizaThirdBadge
	case 7 LittlerootTown_EventScript_JuanThirdBadge
	end

LittlerootTown_EventScript_FourthBadge::
	call_if_set FLAG_BADGE05_GET, LittlerootTown_EventScript_FifthBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneFourthBadge
	case 1 LittlerootTown_EventScript_BrawlyFourthBadge
	case 2 LittlerootTown_EventScript_WattsonFourthBadge
	case 3 LittlerootTown_EventScript_FlanneryFourthBadge
	case 4 LittlerootTown_EventScript_NormanFourthBadge
	case 5 LittlerootTown_EventScript_WinonaFourthBadge
	case 6 LittlerootTown_EventScript_TateAndLizaFourthBadge
	case 7 LittlerootTown_EventScript_JuanFourthBadge
	end

LittlerootTown_EventScript_FifthBadge::
	call_if_set FLAG_BADGE06_GET, LittlerootTown_EventScript_SixthBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneFifthBadge
	case 1 LittlerootTown_EventScript_BrawlyFifthBadge
	case 2 LittlerootTown_EventScript_WattsonFifthBadge
	case 3 LittlerootTown_EventScript_FlanneryFifthBadge
	case 4 LittlerootTown_EventScript_NormanFifthBadge
	case 5 LittlerootTown_EventScript_WinonaFifthBadge
	case 6 LittlerootTown_EventScript_TateAndLizaFifthBadge
	case 7 LittlerootTown_EventScript_JuanFifthBadge
	end

LittlerootTown_EventScript_SixthBadge::
	call_if_set FLAG_BADGE07_GET, LittlerootTown_EventScript_SeventhBadge
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneSixthBadge
	case 1 LittlerootTown_EventScript_BrawlySixthBadge
	case 2 LittlerootTown_EventScript_WattsonSixthBadge
	case 3 LittlerootTown_EventScript_FlannerySixthBadge
	case 4 LittlerootTown_EventScript_NormanSixthBadge
	case 5 LittlerootTown_EventScript_WinonaSixthBadge
	case 6 LittlerootTown_EventScript_TateAndLizaSixthBadge
	case 7 LittlerootTown_EventScript_JuanSixthBadge
	end

LittlerootTown_EventScript_SeventhBadge::
	call_if_set FLAG_BADGE08_GET, LittlerootTown_EventScript_PostBadgeBattles
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxanneSeventhBadge
	case 1 LittlerootTown_EventScript_BrawlySeventhBadge
	case 2 LittlerootTown_EventScript_WattsonSeventhBadge
	case 3 LittlerootTown_EventScript_FlannerySeventhBadge
	case 4 LittlerootTown_EventScript_NormanSeventhBadge
	case 5 LittlerootTown_EventScript_WinonaSeventhBadge
	case 6 LittlerootTown_EventScript_TateAndLizaSeventhBadge
	case 7 LittlerootTown_EventScript_JuanSeventhBadge
	end

LittlerootTown_EventScript_PostBadgeBattles::
	switch VAR_0x8005
	case 0 LittlerootTown_EventScript_RoxannePostBadgeBattles
	case 1 LittlerootTown_EventScript_BrawlyPostBadgeBattles
	case 2 LittlerootTown_EventScript_WattsonPostBadgeBattles
	case 3 LittlerootTown_EventScript_FlanneryPostBadgeBattles
	case 4 LittlerootTown_EventScript_NormanPostBadgeBattles
	case 5 LittlerootTown_EventScript_WinonaPostBadgeBattles
	case 6 LittlerootTown_EventScript_TateAndLizaPostBadgeBattles
	case 7 LittlerootTown_EventScript_JuanPostBadgeBattles
	end
// LittlerootTown_EventScript_RoxanneBadgeCheck::
// 	lock
// 	faceplayer
// 	switch VAR_0x8004
// 	case 0 LittlerootTown_EventScript_RoxanneNoBadge
// 	case 1 LittlerootTown_EventScript_RoxanneFirstBadge
// 	case 2 LittlerootTown_EventScript_RoxanneSecondBadge
// 	case 3 LittlerootTown_EventScript_RoxanneThirdBadge
// 	case 4 LittlerootTown_EventScript_RoxanneFourthBadge
// 	case 5 LittlerootTown_EventScript_RoxanneFifthBadge
// 	case 6 LittlerootTown_EventScript_RoxanneSixthBadge
// 	case 7 LittlerootTown_EventScript_RoxanneSeventhBadge
// 	case 8 LittlerootTown_EventScript_PostBadgeBattles
//     end

LittlerootTown_EventScript_RoxanneNoBadge::
	lock
	faceplayer
	call_if_set FLAG_DEFEATED_ROXANNE, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_RoxanneFirstBadge
	trainerbattle_single TRAINER_ROXANNE_1, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedFirst, NO_MUSIC
	// setvar VAR_0x8005, 1
	// setvar VAR_0x800A, 1
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneFirstBadge::
	//call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE02_GET, LittlerootTown_EventScript_RoxanneSecondBadge
	msgbox LittlerootTown_Text_OneBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_2, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedSecond, NO_MUSIC
	// setvar VAR_0x8005, 1
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneSecondBadge::
	call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE03_GET, LittlerootTown_EventScript_RoxanneThirdBadge
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_3, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedThird, NO_MUSIC
	// setvar VAR_0x8004, 3
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneThirdBadge::
	call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE04_GET, LittlerootTown_EventScript_RoxanneFourthBadge
	msgbox LittlerootTown_Text_ThreeBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_4, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedFourth, NO_MUSIC
	// setvar VAR_0x8004, 4
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneFourthBadge::
	call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE05_GET, LittlerootTown_EventScript_RoxanneFifthBadge
	msgbox LittlerootTown_Text_FourBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_5, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedFifth, NO_MUSIC
	// setvar VAR_0x8004, 5
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneFifthBadge::
	call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE06_GET, LittlerootTown_EventScript_RoxanneSixthBadge
	msgbox LittlerootTown_Text_FiveBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_6, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedSixth, NO_MUSIC
	// setvar VAR_0x8004, 6
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneSixthBadge::
	call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE07_GET, LittlerootTown_EventScript_RoxanneSeventhBadge
	msgbox LittlerootTown_Text_SixBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_7, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedSeventh, NO_MUSIC
	// setvar VAR_0x8004, 7
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
	msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneSeventhBadge::
	call_if_set FLAG_DEFEATED_ROXANNE,  LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE08_GET, LittlerootTown_EventScript_PostBadgeBattles
    msgbox LittlerootTown_Text_SevenBadge, MSGBOX_DEFAULT
    trainerbattle_single TRAINER_ROXANNE_8, LittlerootTown_Text_RoxanneIntro, LittlerootTown_Text_RoxanneDefeat, LittlerootTown_EventScript_RoxanneDefeatedEighth, NO_MUSIC
    // setvar VAR_0x8004, 8
	specialvar VAR_RESULT, ShouldTryRematchBattle
    goto_if_eq VAR_RESULT, TRUE, LittlerootTown_EventScript_PostBadgeBattles
    goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, LittlerootTown_EventScript_GiveRockTomb
    msgbox LittlerootTown_Text_RoxannePostBattle, MSGBOX_DEFAULT
    release
    end

LittlerootTown_EventScript_RoxannePostBadgeBattles::
	msgbox LittlerootTown_Text_EightBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_ROXANNE_8_POST, LittlerootTown_Text_RoxannePreRematch, LittlerootTown_Text_RoxanneRematchDefeat, LittlerootTown_EventScript_RoxanneDefeatedPost, NO_MUSIC
	msgbox LittlerootTown_Text_RoxannePostRematch, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RoxanneDefeatedFirst::
	message LittlerootTown_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE01_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	// call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedSecond::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE02_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedThird::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE03_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedFourth::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE04_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedFifth::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE05_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedSixth::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE06_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedSeventh::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE07_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedEighth::
	message LittlerootTown_Text_ReceivedKnuckleBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_ROXANNE
	setflag FLAG_BADGE08_GET
	// setvar VAR_RUSTBORO_CITY_STATE, 1
	// addvar VAR_PETALBURG_GYM_STATE, 1
	// setvar VAR_0x8008, 1
	// call Common_EventScript_SetGymTrainers
	//call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto LittlerootTown_EventScript_GiveRockTomb
	end

LittlerootTown_EventScript_RoxanneDefeatedPost::
	msgbox LittlerootTown_Text_RoxanneRematchDefeat
	end

LittlerootTown_EventScript_GiveRockTomb::
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROCK_TOMB
	msgbox LittlerootTown_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

// Brawly now
// LittlerootTown_EventScript_BrawlyBadgeCheck::
// 	lock
// 	faceplayer
// 	switch VAR_0x8004
// 	case 0 LittlerootTown_EventScript_BrawlyNoBadge
// 	case 1 LittlerootTown_EventScript_BrawlyFirstBadge
//     end

// LittlerootTown_EventScript_Brawly::
// 	call LittlerootTown_EventScript_BrawlyBadgeCheck
// 	end

LittlerootTown_EventScript_BrawlyNoBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_BRAWLY_1, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedFirst, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlyFirstBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_2, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedSecond, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlySecondBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_3, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedThird, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlyThirdBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_4, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedFourth, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlyFourthBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_5, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedFifth, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlyFifthBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_6, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedSixth, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlySixthBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_7, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedSeventh, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlySeventhBadge::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_8, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedEighth, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlyPostBadgeBattles::
	call_if_set FLAG_DEFEATED_BRAWLY, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_TwoBadge, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_BRAWLY_8_POST, LittlerootTown_Text_BrawlyIntro, LittlerootTown_Text_BrawlyDefeat, LittlerootTown_EventScript_BrawlyDefeatedPost, NO_MUSIC
	setvar VAR_0x8004 2
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, DewfordTown_Gym_EventScript_GiveBulkUp2
	msgbox DewfordTown_Gym_Text_BrawlyPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedFirst::
	message LittlerootTown_Text_ReceivedStoneBadge1
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE01_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedSecond::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE02_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedThird::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE03_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedFourth::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE04_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedFifth::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE05_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedSixth::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE06_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedSeventh::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE07_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedEighth::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_KnuckleBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BRAWLY
	setflag FLAG_BADGE08_GET // This is to give the badge for Trainer
	// addvar VAR_PETALBURG_GYM_STATE, 1
	call LittlerootTown_EventScript_GiveBulkUp
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredBrawly, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_BRAWLY_MATCH_CALL
	setvar VAR_ROXANNE_CALL_STEP_COUNTER, 0
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_BrawlyDefeatedPost::
	msgbox LittlerootTown_Text_BrawlyDefeat
	end

LittlerootTown_EventScript_GiveBulkUp::
	giveitem ITEM_TM_BULK_UP
	goto_if_eq VAR_RESULT, 0, Common_EventScript_BagIsFull
	msgbox LittlerootTown_Text_ExplainBulkUp, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_BULK_UP
	return

LittlerootTown_EventScript_WattsonNoBadge::
	//call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_1, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedFirst, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonFirstBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_2, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedSecond, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonSecondBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_3, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedThird, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonThirdBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_4, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedFourth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonFourthBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_5, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedFifth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonFifthBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_6, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedSixth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonSixthBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_7, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedSeventh, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonSeventhBadge::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_8, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedEighth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonPostBadgeBattles::
	call_if_set FLAG_DEFEATED_WATTSON, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_WATTSON_8_POST, LittlerootTown_Text_WattsonIntro, LittlerootTown_Text_WattsonDefeat, LittlerootTown_EventScript_WattsonDefeatedPost, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, LittlerootTown_EventScript_GiveShockWave2
	release
	end

LittlerootTown_EventScript_WattsonDefeatedFirst::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedSecond::
	message LittlerootTown_Text_ReceivedKnuckleBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE02_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedThird::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE03_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedFourth::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedFifth::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE05_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedSixth::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE06_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedSeventh::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE07_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedEighth::
	message LittlerootTown_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WATTSON
	setflag FLAG_BADGE08_GET
	call LittlerootTown_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	release
	end

LittlerootTown_EventScript_WattsonDefeatedPost::
	msgbox LittlerootTown_Text_WattsonDefeat
	end

LittlerootTown_EventScript_GiveShockWave::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	return

LittlerootTown_EventScript_GiveShockWave2::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LittlerootTown_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	release
	end

LittlerootTown_EventScript_FlanneryNoBadge::
	//call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_1, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedFirst, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlanneryFirstBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_2, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedSecond, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlannerySecondBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_3, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedThird, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlanneryThirdBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_4, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedFourth, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlanneryFourthBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_5, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedFifth, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlanneryFifthBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_6, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedSixth, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlannerySixthBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_7, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedSeventh, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlannerySeventhBadge::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_8, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedEighth, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlanneryPostBadgeBattles::
	call_if_set FLAG_DEFEATED_FLANNERY, LittlerootTown_EventScript_AlreadyFought
	//call_if_set FLAG_BADGE01_GET, LittlerootTown_EventScript_BrawlyFirstBadge
	trainerbattle_single TRAINER_FLANNERY_8_POST, LittlerootTown_Text_FlanneryIntro, LittlerootTown_Text_FlanneryDefeat, LittlerootTown_EventScript_FlanneryDefeatedPost, NO_MUSIC
	// setvar VAR_0x8004 1
	goto_if_unset FLAG_RECEIVED_TM_BULK_UP, LittlerootTown_EventScript_GiveOverheat2
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedFirst::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedSecond::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE02_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedThird::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE03_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedFourth::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE04_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedFifth::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE05_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedSixth::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE06_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedSeventh::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE07_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedEighth::
	message LittlerootTown_Text_ReceivedHeatBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainHeatBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FLANNERY
	setflag FLAG_BADGE08_GET
	call LittlerootTown_EventScript_GiveOverheat
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredFlannery, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_FLANNERY_MATCH_CALL
	release
	end

LittlerootTown_EventScript_FlanneryDefeatedPost::
	msgbox LittlerootTown_Text_FlanneryDefeat
	end

LittlerootTown_EventScript_GiveOverheat::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LittlerootTown_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	return

LittlerootTown_EventScript_GiveOverheat2::
	giveitem ITEM_TM_OVERHEAT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LittlerootTown_Text_ExplainOverheat, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_OVERHEAT
	release
	end

LittlerootTown_EventScript_NormanNoBadge::
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_1, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanFirstBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_2, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE02_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanSecondBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_3, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE03_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanThirdBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_4, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE04_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanFourthBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_5, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE05_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanFifthBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_6, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE06_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanSixthBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_7, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE07_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanSeventhBadge::
	call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_8, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	setflag FLAG_BADGE08_GET
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_NormanPostBadgeBattles::
	//call_if_set FLAG_DEFEATED_NORMAN, LittlerootTown_EventScript_AlreadyFought
	msgbox LittlerootTown_Text_NormanIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_NORMAN_8_POST, LittlerootTown_Text_NormanDefeat
	message LittlerootTown_Text_ReceivedBalanceBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainBalanceBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_NORMAN
	call LittlerootTown_EventScript_GiveFacade
	delay 30
	msgbox LittlerootTown_Text_DadHappyAndSad, MSGBOX_DEFAULT
	closemessage

LittlerootTown_EventScript_GiveFacade::
	giveitem ITEM_TM_FACADE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	setflag FLAG_RECEIVED_TM_FACADE
	msgbox LittlerootTown_Text_ExplainFacade, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaNoBadge::
	trainerbattle_single TRAINER_WINONA_1, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedFirst, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaFirstBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_2, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedSecond, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaSecondBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_3, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedThird, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaThirdBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_4, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedFourth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaFourthBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_5, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedFifth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaFifthBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_6, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedSixth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaSixthBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_7, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedSeventh, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaSeventhBadge::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_8, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedEighth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_WinonaPostBadgeBattles::
	call_if_set FLAG_DEFEATED_WINONA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_WINONA_8_POST, LittlerootTown_Text_WinonaIntro, LittlerootTown_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeatedPost, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, LittlerootTown_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedFirst::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedSecond::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE02_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedThird::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE03_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedFourth::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE04_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedFifth::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE05_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedSixth::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE06_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedSeventh::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE07_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedEighth::
	message LittlerootTown_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WINONA
	setflag FLAG_BADGE08_GET
	call LittlerootTown_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeatedPost::
	msgbox LittlerootTown_Text_WinonaDefeat
	end

LittlerootTown_EventScript_GiveAerialAce::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LittlerootTown_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	return

LittlerootTown_EventScript_GiveAerialAce2::

	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LittlerootTown_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	release
	end

LittlerootTown_EventScript_TateAndLizaNoBadge::
	trainerbattle_double TRAINER_TATE_AND_LIZA_1, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedFirst, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaFirstBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_2, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedSecond, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaSecondBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_3, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedThird, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaThirdBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_4, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedFourth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaFourthBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_5, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedFifth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaFifthBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_6, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedSixth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaSixthBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_7, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedSeventh, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaSeventhBadge::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_8, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedEighth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaPostBadgeBattles::
	call_if_set FLAG_DEFEATED_TATE_LIZA, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_double TRAINER_TATE_AND_LIZA_8_POST, LittlerootTown_Text_TateAndLizaIntro, LittlerootTown_Text_TateAndLizaDefeat, LittlerootTown_Text_TateAndLizaNeedTwoMons, LittlerootTown_EventScript_TateAndLizaDefeatedPost, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_CALM_MIND, LittlerootTown_EventScript_GiveCalmMind2
	msgbox MossdeepCity_Gym_Text_TateAndLizaPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedFirst::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedSecond::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE02_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedThird::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE03_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedFourth::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE04_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedFifth::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE05_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedSixth::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE06_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedSeventh::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE07_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedEighth::
	message LittlerootTown_Text_ReceivedMindBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainMindBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_TATE_LIZA
	setflag FLAG_BADGE08_GET
	call LittlerootTown_EventScript_GiveCalmMind
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredTateAndLiza, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_TATE_AND_LIZA_MATCH_CALL
	release
	end

LittlerootTown_EventScript_TateAndLizaDefeatedPost::
	msgbox LittlerootTown_Text_TateAndLizaIntro
	end

LittlerootTown_EventScript_GiveCalmMind::
	giveitem ITEM_TM_CALM_MIND
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LittlerootTown_Text_ExplainCalmMind, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_CALM_MIND
	return

LittlerootTown_EventScript_GiveCalmMind2::
	giveitem ITEM_TM_CALM_MIND
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LittlerootTown_Text_ExplainCalmMind, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_CALM_MIND
	release
	end

LittlerootTown_EventScript_JuanNoBadge::
	trainerbattle_single TRAINER_JUAN_1, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedFirst, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanFirstBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_2, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedSecond, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanSecondBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_3, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedThird, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanThirdBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_4, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedFourth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanFourthBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_5, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedFifth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanFifthBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_6, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedSixth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanSixthBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_7, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedSeventh, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanSeventhBadge::
	call_if_set FLAG_DEFEATED_JUAN, LittlerootTown_EventScript_AlreadyFought
	trainerbattle_single TRAINER_JUAN_8, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedEighth, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

LittlerootTown_EventScript_JuanPostBadgeBattles::
	trainerbattle_single TRAINER_JUAN_8_POST, LittlerootTown_Text_JuanIntro, LittlerootTown_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeatedPost, NO_MUSIC
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, LittlerootTown_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedFirst::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE01_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedSecond::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE02_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedThird::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE03_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedFourth::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE04_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedFifth::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE05_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedSixth::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE06_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedSeventh::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE07_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedEighth::
	message LittlerootTown_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox LittlerootTown_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_JUAN
	setflag FLAG_BADGE08_GET
	call LittlerootTown_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox LittlerootTown_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeatedPost::
	msgbox LittlerootTown_Text_JuanDefeat
	end

LittlerootTown_EventScript_GiveWaterPulse::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox LittlerootTown_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	return

LittlerootTown_EventScript_GiveWaterPulse2::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox LittlerootTown_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	release
	end

LittlerootTown_EventScript_AlreadyFought::
	lock
	faceplayer
	msgbox LittlerootTown_Text_AlreadyFoughtGymLeader, MSGBOX_DEFAULT
	release
	end

LittlerootTown_Text_AlreadyFoughtGymLeader:
	.string "Already Fought me.$"

LittlerootTown_Text_OneBadge:
	.string "I have one Badge.$"

LittlerootTown_Text_TwoBadge:
	.string "I have two Badges.$"

LittlerootTown_Text_ThreeBadge:
	.string "I have three Badges.$"

LittlerootTown_Text_FourBadge:
	.string "I have four Badges.$"

LittlerootTown_Text_FiveBadge:
	.string "I have five Badges.$"

LittlerootTown_Text_SixBadge:
	.string "I have six Badges.$"

LittlerootTown_Text_SevenBadge:
	.string "I have seven Badges.$"

LittlerootTown_Text_EightBadge:
	.string "I have all Badges.$"

LittlerootTown_Text_OurNewHomeLetsGoInside:
	.string "MOM: {PLAYER}, we're here, honey!\p"
	.string "It must be tiring riding with our things\n"
	.string "in the moving truck.\p"
	.string "Well, this is LITTLEROOT TOWN.\p"
	.string "How do you like it?\n"
	.string "This is our new home!\p"
	.string "It has a quaint feel, but it seems to be\n"
	.string "an easy place to live, don't you think?\p"
	.string "And, you get your own room, {PLAYER}!\n"
	.string "Let's go inside.$"

LittlerootTown_Text_WaitPlayer:
	.string "MOM: Wait, {PLAYER}!$"

LittlerootTown_Text_WearTheseRunningShoes:
	.string "MOM: {PLAYER}! {PLAYER}! Did you\n"
	.string "introduce yourself to PROF. BIRCH?\p"
	.string "Oh! What an adorable POKéMON!\n"
	.string "You got it from PROF. BIRCH. How nice!\p"
	.string "You're your father's child, all right.\n"
	.string "You look good together with POKéMON!\p"
	.string "Here, honey! If you're going out on an\n"
	.string "adventure, wear these RUNNING SHOES.\p"
	.string "They'll put a zip in your step!$"

LittlerootTown_Text_SwitchShoesWithRunningShoes:
	.string "{PLAYER} switched shoes with the\n"
	.string "RUNNING SHOES.$"

LittlerootTown_Text_ExplainRunningShoes:
	.string "MOM: {PLAYER}, those shoes came with\n"
	.string "instructions.\p"
	.string "“Press the B Button while wearing these\n"
	.string "RUNNING SHOES to run extra-fast!\p"
	.string "“Slip on these RUNNING SHOES and race\n"
	.string "in the great outdoors!”$"

LittlerootTown_Text_ComeHomeIfAnythingHappens:
	.string "… … … … … … … …\n"
	.string "… … … … … … … …\p"
	.string "To think that you have your very own\n"
	.string "POKéMON now…\l"
	.string "Your father will be overjoyed.\p"
	.string "…But please be careful.\n"
	.string "If anything happens, you can come home.\p"
	.string "Go on, go get them, honey!$"

LittlerootTown_Text_CanUsePCToStoreItems:
	.string "If you use a PC, you can store items\n"
	.string "and POKéMON.\p"
	.string "The power of science is staggering!$"

LittlerootTown_Text_BirchSpendsDaysInLab:
	.string "PROF. BIRCH spends days in his LAB\n"
	.string "studying, then he'll suddenly go out in\l"
	.string "the wild to do more research…\p"
	.string "When does PROF. BIRCH spend time\n"
	.string "at home?$"

LittlerootTown_Text_IfYouGoInGrassPokemonWillJumpOut:
	.string "Um, um, um!\p"
	.string "If you go outside and go in the grass,\n"
	.string "wild POKéMON will jump out!$"

LittlerootTown_Text_DangerousIfYouDontHavePokemon:
	.string "It's dangerous if you don't have\n"
	.string "your own POKéMON.$"

LittlerootTown_Text_CanYouGoSeeWhatsHappening:
	.string "Um, hi!\p"
	.string "There are scary POKéMON outside!\n"
	.string "I can hear their cries!\p"
	.string "I want to go see what's going on,\n"
	.string "but I don't have any POKéMON…\p"
	.string "Can you go see what's happening\n"
	.string "for me?$"

LittlerootTown_Text_YouSavedBirch:
	.string "You saved PROF. BIRCH!\n"
	.string "I'm so glad!$"

LittlerootTown_Text_GoodLuckCatchingPokemon:
	.string "Are you going to catch POKéMON?\n"
	.string "Good luck!$"

LittlerootTown_Text_TownSign:
	.string "LITTLEROOT TOWN\n"
	.string "“A town that can't be shaded any hue.”$"

LittlerootTown_Text_ProfBirchsLab:
	.string "PROF. BIRCH'S POKéMON LAB$"

LittlerootTown_Text_PlayersHouse:
	.string "{PLAYER}'s HOUSE$"

LittlerootTown_Text_ProfBirchsHouse:
	.string "PROF. BIRCH'S HOUSE$"

LittlerootTown_Text_BirchSomethingToShowYouAtLab:
	.string "PROF. BIRCH: Well, well, {PLAYER}{KUN}!\n"
	.string "That was good work out there!\p"
	.string "I knew there was something special\n"
	.string "about you when I first saw you,\l"
	.string "but I never expected this.\p"
	.string "Oh, yes. Do you still have the POKéDEX\n"
	.string "I gave you?\p"
	.string "I have something to show you.\n"
	.string "Let's go to my LAB.$"

LittlerootTown_Text_TalkToSteven:
	.string "Hello, {PLAYER}! "
	.string "I'm Steven the pokemon\n"
	.string "Champion in Ruby and Sapphire.\p"
	.string "Nice to meet you! "
	.string "Though, this is quite\nstrange "
	.string "being here.\p"
	.string "I'm guessing you placed me here.\p"
	.string "Anyways I'm happy to help\n"
	.string "with your project!.\p"
	.string "Just let me know!$"

LittlerootTown_Text_RoxanneIntro:
	.string "Hello, I am ROXANNE, the RUSTBORO\n"
	.string "POKéMON GYM LEADER.\p"
	.string "I became a GYM LEADER so that I might\n"
	.string "apply what I learned at the POKéMON\l"
	.string "TRAINER'S SCHOOL in battle.\p"
	.string "Would you kindly demonstrate how you\n"
	.string "battle, and with which POKéMON?$"

LittlerootTown_Text_RoxanneDefeat:
	.string "So…\n"
	.string "I lost…\p"
	.string "It seems that I still have much more\n"
	.string "to learn…\p"
	.string "I understand.\p"
	.string "The POKéMON LEAGUE's rules state\n"
	.string "that TRAINERS are to be given this\l"
	.string "if they defeat a GYM LEADER.\p"
	.string "Please accept the official POKéMON\n"
	.string "LEAGUE BADGE.$"

LittlerootTown_Text_ReceivedStoneBadge:
	.string "{PLAYER} received the STONE BADGE\n"
	.string "from ROXANNE.$"

LittlerootTown_Text_ReceivedStoneBadge1:
	.string "{PLAYER} received the STONE BADGE\n"
	.string "from BRAWLY.$"

LittlerootTown_Text_StoneBadgeInfoTakeThis:
	.string "The STONE BADGE heightens the ATTACK\n"
	.string "power of your POKéMON.\p"
	.string "It also enables them to use the HM move\n"
	.string "CUT outside of battle.\p"
	.string "Please take this with you, too.$"

LittlerootTown_Text_RoxannePostBattle:
	.string "Since you are so strong, you should\n"
	.string "challenge other GYM LEADERS.\p"
	.string "By battling many TRAINERS, you should\n"
	.string "learn many things.$"

LittlerootTown_Text_ExplainRockTomb:
	.string "That TECHNICAL MACHINE, TM39,\n"
	.string "contains ROCK TOMB.\p"
	.string "It not only inflicts damage by dropping\n"
	.string "rocks, it also lowers SPEED.\p"
	.string "If you use a TM, it instantly teaches\n"
	.string "the move to a POKéMON.\p"
	.string "Remember, a TM can be used only once,\n"
	.string "so think before you use it.$"

LittlerootTown_Text_RoxannePreRematch:
	.string "ROXANNE: I'm so glad to see you again.\n"
	.string "I'm ROXANNE, the GYM LEADER here.\p"
	.string "I'm sure we've both experienced many\n"
	.string "battles since we last met.\p"
	.string "I would like to see how much better\n"
	.string "we've become. Let us battle!$"

LittlerootTown_Text_RoxanneRematchDefeat:
	.string "Grr…\n"
	.string "Again, I have lost…$"

LittlerootTown_Text_RoxanneRematchNeedTwoMons:
	.string "ROXANNE: I'm so glad to see you again.\n"
	.string "I'm ROXANNE, the GYM LEADER here.\p"
	.string "I'm sure we've both experienced many\n"
	.string "battles since we last met.\p"
	.string "I would like to see how much better\n"
	.string "we've become…\p"
	.string "Oh, no…\n"
	.string "You only have one POKéMON with you?\p"
	.string "Please return with at least one more\n"
	.string "POKéMON.$"

LittlerootTown_Text_RoxannePostRematch:
	.string "ROXANNE: I still have much to learn\n"
	.string "when it comes to battling POKéMON.\p"
	.string "That awareness makes me love battling\n"
	.string "all that much more!$"

LittlerootTown_Text_BrawlyIntro:
	.string "I'm BRAWLY!\n"
	.string "DEWFORD's GYM LEADER!\p"
	.string "I've been churned in the rough waves\n"
	.string "of these parts, and I've grown tough\l"
	.string "in the pitch-black cave!\p"
	.string "So you wanted to challenge me?\n"
	.string "Let me see what you're made of!$"

LittlerootTown_Text_BrawlyDefeat:
	.string "Whoah, wow! You made a much bigger\n"
	.string "splash than I expected!\p"
	.string "You swamped me!\p"
	.string "Okay, you've got me.\n"
	.string "Take this GYM BADGE!$"

LittlerootTown_Text_ReceivedKnuckleBadge:
	.string "{PLAYER} received the KNUCKLE BADGE\n"
	.string "from BRAWLY.$"

LittlerootTown_Text_ReceivedKnuckleBadge1:
	.string "{PLAYER} received the KNUCKLE BADGE\n"
	.string "from ROXANNE.$"

LittlerootTown_Text_KnuckleBadgeInfoTakeThis:
	.string "The KNUCKLE BADGE makes all POKéMON\n"
	.string "up to Level 30, even those you get in\l"
	.string "trades, obey without question.\p"
	.string "And, you'll be able to use the HM move\n"
	.string "FLASH outside of battle.\p"
	.string "If you'd like, use this TECHNICAL\n"
	.string "MACHINE, too.$"

LittlerootTown_Text_ExplainBulkUp:
	.string "That TM08 contains BULK UP.\p"
	.string "It's a move that raises both ATTACK\n"
	.string "and DEFENSE stats.\p"
	.string "… … … … … …$"

LittlerootTown_Text_RegisteredBrawly:
	.string "Registered GYM LEADER BRAWLY\n"
	.string "in the POKéNAV.$"

LittlerootTown_Text_WattsonIntro:
	.string "I've given up on my plans to convert\n"
	.string "the city, I have.\p"
	.string "And so, I put my time into making\n"
	.string "door traps in my GYM.\p"
	.string "Oh? Now, what are you doing here?\p"
	.string "What's that? You say you've gotten\n"
	.string "past all my rigged doors?\p"
	.string "Wahahahah!\n"
	.string "Now, that is amusing!\p"
	.string "Then, I, WATTSON, the LEADER of\n"
	.string "MAUVILLE GYM, shall electrify you!$"

LittlerootTown_Text_WattsonDefeat:
	.string "Wahahahah!\n"
	.string "Fine, I lost!\p"
	.string "You ended up giving me a thrill!\n"
	.string "Take this BADGE!$"

LittlerootTown_Text_ReceivedDynamoBadge:
	.string "{PLAYER} received the DYNAMO BADGE\n"
	.string "from WATTSON.$"

LittlerootTown_Text_ExplainDynamoBadgeTakeThis:
	.string "With the DYNAMO BADGE, POKéMON can\n"
	.string "use ROCK SMASH out of battle.\p"
	.string "And, it will make your POKéMON a little\n"
	.string "bit faster, too.\p"
	.string "Hmm…\n"
	.string "You should take this, too!$"

LittlerootTown_Text_ExplainShockWave:
	.string "That TM34 there contains SHOCK WAVE.\p"
	.string "It's a trustworthy move that never\n"
	.string "misses! You can count on it!\p"
	.string "… … … … … …$"

LittlerootTown_Text_RegisteredWattson:
	.string "Registered GYM LEADER WATTSON\n"
	.string "in the POKéNAV.$"

LittlerootTown_Text_FlanneryIntro:
	.string "Welcome… No, wait.\p"
	.string "Puny TRAINER, how good to see you've\n"
	.string "made it here!\p"
	.string "I have been entrusted with the…\n"
	.string "No, wait.\p"
	.string "I am FLANNERY, and I'm the GYM\n"
	.string "LEADER here!\p"
	.string "Uh…\n"
	.string "Dare not underestimate me, though\l"
	.string "I have been LEADER only a short time!\p"
	.string "With skills inherited from my grand-\n"
	.string "father, I shall, uh…demonstrate the\l"
	.string "hot moves we have honed on this land!$"

LittlerootTown_Text_FlanneryDefeat:
	.string "Oh…\n"
	.string "I guess I was trying too hard…\p"
	.string "I… I've only recently become\n"
	.string "a GYM LEADER.\p"
	.string "I tried too hard to be someone\n"
	.string "I'm not.\p"
	.string "I have to do things my natural way. If\n"
	.string "I don't, my POKéMON will be confused.\p"
	.string "Thanks for teaching me that.\n"
	.string "For that, you deserve this.$"

LittlerootTown_Text_ReceivedHeatBadge:
	.string "{PLAYER} received the HEAT BADGE\n"
	.string "from FLANNERY.$"

LittlerootTown_Text_ExplainHeatBadgeTakeThis:
	.string "If you have a HEAT BADGE, all POKéMON\n"
	.string "up to Level 50, even those you get in\l"
	.string "trades from other people, will obey\l"
	.string "you completely.\p"
	.string "And, it lets POKéMON use the HM move\n"
	.string "STRENGTH outside of battle.\p"
	.string "This is a token of my appreciation.\n"
	.string "Don't be shy about taking it!$"

LittlerootTown_Text_ExplainOverheat:
	.string "That TM50 contains OVERHEAT.\p"
	.string "That move inflicts serious damage on\n"
	.string "the opponent.\p"
	.string "But it also sharply cuts the SP. ATK\n"
	.string "of the POKéMON using it. It might not\l"
	.string "be suitable for longer battles.$"

LittlerootTown_Text_RegisteredFlannery:
	.string "Registered GYM LEADER FLANNERY\n"
	.string "in the POKéNAV.$"

LittlerootTown_Text_NormanIntro:
	.string "DAD: Hm…\n"
	.string "So, you did get four GYM BADGES.\p"
	.string "Fine, as I promised, we will have\n"
	.string "a POKéMON battle.\p"
	.string "{PLAYER}…\p"
	.string "I'm so happy that I can have\n"
	.string "a real battle with my own child.\p"
	.string "But, a battle is a battle!\p"
	.string "I will do everything in my power as\n"
	.string "a GYM LEADER to win.\p"
	.string "{PLAYER}, you'd better give it your\n"
	.string "best shot, too!$"

LittlerootTown_Text_NormanDefeat:
	.string "…\p"
	.string "I… I can't…\p"
	.string "I can't believe it. {PLAYER}…\p"
	.string "I lost to {PLAYER}?\p"
	.string "But, rules are rules!\n"
	.string "Here, take this.$"

LittlerootTown_Text_ReceivedBalanceBadge:
	.string "{PLAYER} received the BALANCE BADGE\n"
	.string "from DAD!$"

LittlerootTown_Text_ExplainBalanceBadgeTakeThis:
	.string "DAD: With that BADGE, the DEFENSE\n"
	.string "of all your POKéMON will increase.\p"
	.string "POKéMON that know the HM move SURF\n"
	.string "will be able to travel over water.\p"
	.string "This is my gift to you. {PLAYER}, I'm\n"
	.string "sure you can use it correctly.$"

LittlerootTown_Text_DadHappyAndSad:
	.string "DAD: As the GYM LEADER, I can't\n"
	.string "express how upset I am…\p"
	.string "But as a father, it makes me both\n"
	.string "happy and a little sad. It's odd…$"

LittlerootTown_Text_ExplainFacade:
	.string "DAD: TM42 contains FACADE.\p"
	.string "It doubles the power of moves if\n"
	.string "the POKéMON is poisoned, paralyzed,\l"
	.string "or burned.\p"
	.string "It might be able to turn a bad\n"
	.string "situation into an advantage.$"

LittlerootTown_Text_WinonaIntro:
	.string "I am WINONA. I am the LEADER of\n"
	.string "the FORTREE POKéMON GYM.\p"
	.string "I have become one with BIRD POKéMON\n"
	.string "and have soared the skies…\p"
	.string "However grueling the battle, we have\n"
	.string "triumphed with grace…\p"
	.string "Witness the elegant choreography\n"
	.string "of BIRD POKéMON and I!$"

LittlerootTown_Text_WinonaDefeat:
	.string "Never before have I seen a TRAINER\n"
	.string "command POKéMON with more grace\l"
	.string "than I…\p"
	.string "In recognition of your prowess,\n"
	.string "I present to you this GYM BADGE.$"

LittlerootTown_Text_ReceivedFeatherBadge:
	.string "{PLAYER} received the FEATHER BADGE\n"
	.string "from WINONA.$"

LittlerootTown_Text_ExplainFeatherBadgeTakeThis:
	.string "With the FEATHER BADGE, all POKéMON up\n"
	.string "to LV 70, even those received through\l"
	.string "trades, will obey your every command.\p"
	.string "You will also be able to use\n"
	.string "the HM move FLY outside of battle.\p"
	.string "And this…\n"
	.string "This is a gift from me.$"

LittlerootTown_Text_ExplainAerialAce:
	.string "TM40 contains AERIAL ACE.\p"
	.string "Its speed…\n"
	.string "No POKéMON should be able to avoid it.\p"
	.string "… … … … … …$"

LittlerootTown_Text_RegisteredWinona:
	.string "Registered GYM LEADER WINONA\n"
	.string "in the POKéNAV.$"

LittlerootTown_Text_TateAndLizaIntro:
	.string "TATE: Hehehe… Were you surprised?\p"
	.string "LIZA: Fufufu… Were you surprised?\p"
	.string "TATE: That there are two GYM LEADERS?\n"
	.string "LIZA: That there are two GYM LEADERS?\p"
	.string "TATE: We're twins!\n"
	.string "LIZA: We're twins!\p"
	.string "TATE: We don't need to talk because…\n"
	.string "LIZA: We can each determine what…\p"
	.string "TATE: The other is thinking…\n"
	.string "LIZA: All in our minds!\p"
	.string "TATE: This combination of ours…\n"
	.string "LIZA: Can you beat it?$"

LittlerootTown_Text_TateAndLizaDefeat:
	.string "TATE: What?! Our combination…\n"
	.string "LIZA: Was shattered!\p"
	.string "TATE: It can't be helped. You've won…\n"
	.string "LIZA: So, in recognition, take this.$"

LittlerootTown_Text_ReceivedMindBadge:
	.string "{PLAYER} received the MIND BADGE\n"
	.string "from TATE and LIZA.$"

LittlerootTown_Text_ExplainMindBadgeTakeThis:
	.string "TATE: The MIND BADGE enhances the\n"
	.string "SP. ATK and SP. DEF of POKéMON.\p"
	.string "LIZA: It also lets you use the HM move\n"
	.string "DIVE outside of battle.\p"
	.string "TATE: You should also take this, too.$"

LittlerootTown_Text_ExplainCalmMind:
	.string "TATE: That TM04 contains…\n"
	.string "LIZA: CALM MIND!\p"
	.string "TATE: It raises SP. ATK and…\n"
	.string "LIZA: It raises SP. DEF!\p"
	.string "TATE: It's a move that's perfect…\n"
	.string "LIZA: For PSYCHIC POKéMON!\p"
	.string "… … … … … …$"

LittlerootTown_Text_RegisteredTateAndLiza:
	.string "Registered GYM LEADERS TATE & LIZA\n"
	.string "in the POKéNAV.$"

LittlerootTown_Text_TateAndLizaNeedTwoMons:
	.string "TATE: Hehehe… Were you surprised?\p"
	.string "LIZA: That there are two GYM LEADERS?\p"
	.string "TATE: Oops, you have only one…\n"
	.string "LIZA: POKéMON that can battle.\p"
	.string "TATE: We can't battle that way!\p"
	.string "LIZA: If you want to challenge us,\n"
	.string "bring some more POKéMON.$"

LittlerootTown_Text_JuanIntro:
	.string "Let me ask you.\n"
	.string "Did you know?\l"
	.string "Ah, I should not be so coy.\p"
	.string "It was I who taught WALLACE everything\n"
	.string "there is to know about POKéMON.\p"
	.string "Once, I had given up my position as\n"
	.string "the GYM LEADER.\p"
	.string "In my place, I had entrusted WALLACE\n"
	.string "with the GYM.\p"
	.string "However, a compelling reason arose for\n"
	.string "me to make a comeback.\p"
	.string "Ah, but enough chatter.\n"
	.string "Let us begin our match, shall we?\p"
	.string "Please, you shall bear witness to\n"
	.string "our artistry.\p"
	.string "A grand illusion of water sculpted\n"
	.string "by POKéMON and myself!$"

@ NOTE: This defeat text actually causes a buffer overflow. It's about 50 bytes too long for
@ the gDisplayedStringBattle buffer that it's put into, and it stomps all over the gBattleTextBuffs
@ after, as well as the otherwise unused array after that. One wonders if that's the reason for
@ the existence of that unused array of ints.
LittlerootTown_Text_JuanDefeat:
	.string "Ahahaha, excellent!\n"
	.string "Very well, you are the winner.\p"
	.string "From you, I sense the brilliant shine\n"
	.string "of skill that will overcome all.\p"
	.string "However, compared with me or even\n"
	.string "WALLACE, you are lacking in elegance.\p"
	.string "Perhaps I should make you a loan\n"
	.string "of my outfit?\p"
	.string "… … … … … …\n"
	.string "… … … … … …\p"
	.string "Hahaha, I merely jest!\p"
	.string "Rather than my clothes, I shall reward\n"
	.string "you with this, the RAIN BADGE!$"

LittlerootTown_Text_ReceivedRainBadge:
	.string "{PLAYER} received the RAIN BADGE\n"
	.string "from JUAN.$"

LittlerootTown_Text_ExplainRainBadgeTakeThis:
	.string "Having the RAIN BADGE shall assure you\n"
	.string "the full obedience of all your POKéMON\l"
	.string "to your every command.\p"
	.string "The RAIN BADGE also allows the use\n"
	.string "of the HM move WATERFALL to scale\l"
	.string "walls of cascading water.\p"
	.string "And, so that you never forget the\n"
	.string "battle we shared, take this…$"

LittlerootTown_Text_ExplainWaterPulse:
	.string "The TECHNICAL MACHINE I handed you\n"
	.string "contains WATER PULSE.\p"
	.string "In use, it will occasionally confuse\n"
	.string "the target with ultrasonic waves.\p"
	.string "… … … … … …$"

LittlerootTown_Text_RegisteredJuan:
	.string "Registered GYM LEADER JUAN\n"
	.string "in the POKéNAV.$"

LittlerootTown_Text_EliteFourMemberSidney:
	.string "Hello {PLAYER}, I'm Sidney\n"
	.string "an Elite Four member\p"
	.string "I'm a dark type specialist\n"
	.string "That enjoys instilling fear \p"
	.string "into the hearts of all my foes.\n"
	.string "Pleasure to meet you.$"

LittlerootTown_Text_EliteFourMemberPhoebe:
	.string "Hello {PLAYER}, I'm Phoebe\n"
	.string "Second member of the Elite Four.\p"
	.string "My Specialty is of Ghost Types.\n"
	.string "Though many are scared of the afterlife \p"
	.string "I'm not. I use this to my advantage in battle\n"
	.string "to paralyze and devour their fears in a instant.\p"
	.string "If we battle sometime, I hope you prove to be\n"
	.string "a worthy opponent.$"

LittlerootTown_Text_EliteFourMemberGlacia:
	.string "Hm, so you're {PLAYER}. \n"
	.string "The one I've been hearing about.\p"
	.string "Let's see if you have the burning\n"
	.string "spirit needed to withstand my \p"
	.string "power, devoid of warmth.$"

LittlerootTown_Text_EliteFourMemberDrake:
	.string "So {PLAYER}, you were able to\n"
	.string "defeat Glacia. Impressive...\p"
	.string "It's been a while since a foe\n"
	.string "has had the honor to challenge me.\p"
	.string "lets give a battle worthy of such\n"
	.string "vicious strength and power like\p"
	.string "that of the ancient dragons.$"

LittlerootTown_Text_EliteFourChampionWallace:
	.string "Greetings {PLAYER}, I'm Wallace\n"
	.string "Champion of the Hoenn Region\p"
	.string "and final member of the Elite Four.\p"
	.string "You have surpassed all the challenges\n"
	.string "of the Gym Leaders. And now...\p"
	.string "you've bested the great 4 that \n"
	.string "stand before my might. \p"
	.string "Now don't keep me waiting. Let's\n"
	.string "have a battle to be remembered.$"

LittlerootTown_Text_Drake_Defeat:
	.string "Superb, it should be said.$"

LittlerootTown_Text_Glacia_Defeat:
	.string "You and your POKéMON…\n"
	.string "How hot your spirits burn!\p"
	.string "The all-consuming heat overwhelms.\p"
	.string "It's no surprise that my icy skills\n"
	.string "failed to harm you.$"

LittlerootTown_Text_Phoebe_Defeat:
	.string "Oh, darn.\n"
	.string "I've gone and lost.$"

LittlerootTown_Text_Sidney_Defeat:
	.string "Well, how do you like that? I lost!\n"
	.string "Eh, it was fun, so it doesn't matter.$"

LittlerootTown_Text_Champion_Defeat:
	.string "I, the CHAMPION, fall in defeat…\p"
	.string "That was wonderful work.\n"
	.string "You were elegant, infuriatingly so.\l"
	.string "And yet it was utterly glorious!\p"
	.string "Kudos to you, {PLAYER}{KUN}!\p"
	.string "You are a truly noble POKéMON\n"
	.string "TRAINER!$"