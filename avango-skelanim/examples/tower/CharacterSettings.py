import avango
import avango.script
from avango.gua.skelanim.AnimationControl import AnimationConfig


def load_unreal_animations(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle")
  
  # run:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Bwd_Rif.FBX","run_bwd")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Lt_Rif.FBX","run_lt")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Rt_Rif.FBX","run_rt")
  
  # taunts:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_BringItOn.FBX","taunt1")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_ComeHear.FBX","taunt2")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_Hoolahoop.FBX","taunt3")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_NoNo.FBX","taunt4")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_PelvicThrust.FBX","taunt5")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_SlitThroat.FBX","taunt6")
  
  # swim forward: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Swim_Fwd_Rif.FBX","swim")
  
  # crouch:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Idle_Ready_Rif.FBX","crouch")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Fwd_Rif.FBX","crouch_fwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Bwd_Rif.FBX","crouch_bwd")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Lt_Rif.FBX","crouch_lt")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Rt_Rif.FBX","crouch_rt")
  
  # jump idle:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Start.FBX","jump")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Loop.FBX","jump_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Preland.FBX","jump_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Land.FBX","jump_land")
  
  # jump forward: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_Start.FBX","jump_fwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_Loop.FBX","jump_fwd_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_PreLand.FBX","jump_fwd_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_Land.FBX","jump_fwd_land")

  # jump backward: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_Start.FBX","jump_bwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_Loop.FBX","jump_bwd_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_PreLand.FBX","jump_bwd_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_Land.FBX","jump_bwd_land")

  # jump left: 
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_Start.FBX","jump_lt")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_Loop.FBX","jump_lt_loop")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_PreLand.FBX","jump_lt_preland")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_Land.FBX","jump_lt_land")

  # jump right: 
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_Start.FBX","jump_rt")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_Loop.FBX","jump_rt_loop")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_PreLand.FBX","jump_rt_preland")
  #bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_Land.FBX","jump_rt_land")


def load_unreal_animations2(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Idle_Ready_Rif.FBX","idle")
  
  # run:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Fwd_Rif.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Bwd_Rif.FBX","run_bwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Lt_Rif.FBX","run_lt")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Run_Rt_Rif.FBX","run_rt")
  
  # taunts:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_BringItOn.FBX","taunt1")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_ComeHear.FBX","taunt2")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_Hoolahoop.FBX","taunt3")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_NoNo.FBX","taunt4")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_PelvicThrust.FBX","taunt5")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Taunt_SlitThroat.FBX","taunt6")
  
  # swim forward: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Swim_Fwd_Rif.FBX","swim")
  
  # crouch:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Idle_Ready_Rif.FBX","crouch")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Fwd_Rif.FBX","crouch_fwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Bwd_Rif.FBX","crouch_bwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Lt_Rif.FBX","crouch_lt")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Crouch_Rt_Rif.FBX","crouch_rt")
  
  # jump idle:
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Start.FBX","jump")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Loop.FBX","jump_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Preland.FBX","jump_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Idle_Rif_Land.FBX","jump_land")
  
  # jump forward: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_Start.FBX","jump_fwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_Loop.FBX","jump_fwd_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_PreLand.FBX","jump_fwd_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Fwd_Rif_Land.FBX","jump_fwd_land")

  # jump backward: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_Start.FBX","jump_bwd")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_Loop.FBX","jump_bwd_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_PreLand.FBX","jump_bwd_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Bwd_Rif_Land.FBX","jump_bwd_land")

  # jump left: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_Start.FBX","jump_lt")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_Loop.FBX","jump_lt_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_PreLand.FBX","jump_lt_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Lt_Rif_Land.FBX","jump_lt_land")

  # jump right: 
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_Start.FBX","jump_rt")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_Loop.FBX","jump_rt_loop")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_PreLand.FBX","jump_rt_preland")
  bob.load_animation("/opt/project_animation/Assets/UnrealTournament/UniversalAnimations/Jump_Rt_Rif_Land.FBX","jump_rt_land")


def apply_distance_events(distance_events, ground_following):

  distance_events.DistanceToGround.connect_from(ground_following.DistanceToGround)
  distance_events.smaller_than(0.013, "jump_fwd_loop", AnimationConfig("jump_fwd_preland", False), 0.1)
  distance_events.bigger_than(0.013, "jump_fwd_preland", AnimationConfig("jump_fwd_loop", False), 0.1)
  distance_events.smaller_than(0.01, "jump_bwd_loop", AnimationConfig("jump_bwd_preland", False), 0.1)
  distance_events.smaller_than(0.01, "jump_lt_loop", AnimationConfig("jump_lt_preland", False), 0.1)
  distance_events.smaller_than(0.01, "jump_rt_loop", AnimationConfig("jump_rt_preland", False), 0.1)
  distance_events.smaller_than(0.012, "jump_loop", AnimationConfig("jump_preland", False), 0.1)
  distance_events.smaller_than(0.008, "jump_preland", AnimationConfig("jump_land", False), 0.1)
  distance_events.smaller_than(0.008, "jump_fwd_preland", AnimationConfig("jump_fwd_land", False), 0.25)
  distance_events.smaller_than(0.008, "jump_bwd_preland", AnimationConfig("jump_bwd_land", False), 0.25)
  distance_events.smaller_than(0.008, "jump_lt_preland", AnimationConfig("jump_lt_land", False), 0.25)
  distance_events.smaller_than(0.008 , "jump_rt_preland", AnimationConfig("jump_rt_land", False), 0.25)
  distance_events.smaller_than(ground_following.MaxDistanceToGround.value, "swim", AnimationConfig("run_fwd"), 0.5)
  distance_events.bigger_than(ground_following.MaxDistanceToGround.value, "run_fwd", AnimationConfig("swim"), 0.5)


def apply_character_control_settings1(character_control, device_sensor = None):

  # optional / additional xbox controller settings:
  if device_sensor != None:
    character_control.XBOX_X.connect_from(device_sensor.Value0)
    character_control.XBOX_Y.connect_from(device_sensor.Value1)
    character_control.XBOX_BTN_A.connect_from(device_sensor.Button2)
    character_control.XBOX_BTN_X.connect_from(device_sensor.Button0)
    character_control.xbox_override_key(character_control.XBOX_Y,83,0.1)
    character_control.xbox_override_key(character_control.XBOX_Y,87,-0.1,-1.0)
    character_control.xbox_override_key(character_control.XBOX_X,68,0.1)
    character_control.xbox_override_key(character_control.XBOX_X,65,-0.1,-1.0)
    character_control.xbox_override_key(character_control.XBOX_BTN_A,32)
    character_control.xbox_override_key(character_control.XBOX_BTN_X,67)
    character_control.xbox_animation_speed(character_control.XBOX_Y,"run_fwd")
    character_control.xbox_animation_speed(character_control.XBOX_Y,"run_bwd")
    #character_control.xbox_animation_speed(character_control.XBOX_X,"run_lt")
    #character_control.xbox_animation_speed(character_control.XBOX_X,"run_rt")
    character_control.xbox_animation_speed(character_control.XBOX_Y,"crouch_fwd")
    character_control.xbox_animation_speed(character_control.XBOX_Y,"crouch_bwd")
    #character_control.xbox_animation_speed(character_control.XBOX_X,"crouch_lt")
    #character_control.xbox_animation_speed(character_control.XBOX_X,"crouch_rt")

  # A
  character_control.bind_rotation(65, "idle", avango.gua.Vec4(2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "crouch", avango.gua.Vec4(2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "run_fwd", avango.gua.Vec4(4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "run_bwd", avango.gua.Vec4(4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "crouch_fwd", avango.gua.Vec4(2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "crouch_bwd", avango.gua.Vec4(2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "swim", avango.gua.Vec4(2.0, 0.0, 1.0,0.0))
  #character_control.on_key_down(65, "idle", AnimationConfig("run_lt"))
  #character_control.on_key_up(65, "run_lt", AnimationConfig("idle"))
  #character_control.on_key_down(65, "crouch", AnimationConfig("crouch_lt"))
  #character_control.on_key_up(65, "crouch_lt", AnimationConfig("crouch"))

  # D
  character_control.bind_rotation(68, "idle", avango.gua.Vec4(-2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "crouch", avango.gua.Vec4(-2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "run_fwd", avango.gua.Vec4(-4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "run_bwd", avango.gua.Vec4(-4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "crouch_fwd", avango.gua.Vec4(-2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "crouch_bwd", avango.gua.Vec4(-2.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "swim", avango.gua.Vec4(-2.0, 0.0, 1.0,0.0))
  #character_control.on_key_down(68, "idle", AnimationConfig("run_rt"))
  #character_control.on_key_up(68, "run_rt", AnimationConfig("idle"))
  #character_control.on_key_down(68, "crouch", AnimationConfig("crouch_rt"))
  #character_control.on_key_up(68, "crouch_rt", AnimationConfig("crouch"))


  # W
  character_control.on_key_down(87, "idle", AnimationConfig("run_fwd"))
  character_control.on_key_up(87, "run_fwd", AnimationConfig("idle"))
  character_control.on_key_down(87, "crouch", AnimationConfig("crouch_fwd"))
  character_control.on_key_up(87, "crouch_fwd", AnimationConfig("crouch"))
  #character_control.bind_transformation(87, "run_rt", avango.gua.make_trans_mat(0.0,0.0,0.025))
  #character_control.bind_transformation(87, "run_lt", avango.gua.make_trans_mat(0.0,0.0,0.025))
  #character_control.bind_transformation(87, "crouch_rt", avango.gua.make_trans_mat(0.0,0.0,0.0175))
  #character_control.bind_transformation(87, "crouch_lt", avango.gua.make_trans_mat(0.0,0.0,0.0175))

  # S
  character_control.on_key_down(83, "idle", AnimationConfig("run_bwd"))
  character_control.on_key_up(83, "run_bwd", AnimationConfig("idle"))
  character_control.on_key_down(83, "crouch", AnimationConfig("crouch_bwd"))
  character_control.on_key_up(83, "crouch_bwd", AnimationConfig("crouch"))
  #character_control.bind_transformation(83, "run_rt", avango.gua.make_trans_mat(0.0,0.0,-0.025))
  #character_control.bind_transformation(83, "run_lt", avango.gua.make_trans_mat(0.0,0.0,-0.025))
  #character_control.bind_transformation(83, "crouch_rt", avango.gua.make_trans_mat(0.0,0.0,-0.0175))
  #character_control.bind_transformation(83, "crouch_lt", avango.gua.make_trans_mat(0.0,0.0,-0.0175))

  # C
  character_control.on_key_down(67, "idle", AnimationConfig("crouch"),0.2)
  character_control.on_key_up(67, "crouch", AnimationConfig("idle"),0.4)
  character_control.on_key_down(67, "run_fwd", AnimationConfig("crouch_fwd"))
  character_control.on_key_up(67, "crouch_fwd", AnimationConfig("run_fwd"))
  character_control.on_key_down(67, "run_bwd", AnimationConfig("crouch_bwd"))
  character_control.on_key_up(67, "crouch_bwd", AnimationConfig("run_bwd"))
  #character_control.on_key_down(67, "run_lt", AnimationConfig("crouch_lt"))
  #character_control.on_key_up(67, "crouch_lt", AnimationConfig("run_lt"))
  #character_control.on_key_down(67, "run_rt", AnimationConfig("crouch_rt"))
  #character_control.on_key_up(67, "crouch_rt", AnimationConfig("run_rt"))

  # F
  character_control.on_key_down(70, "idle", AnimationConfig("taunt1"))
  character_control.on_key_up(70, "taunt1", AnimationConfig("idle"))

  # G
  character_control.on_key_down(71, "idle", AnimationConfig("taunt2"))
  character_control.on_key_up(71, "taunt2", AnimationConfig("idle"))

  # H
  character_control.on_key_down(72, "idle", AnimationConfig("taunt3"))
  character_control.on_key_up(72, "taunt3", AnimationConfig("idle"))

  # J
  character_control.on_key_down(74, "idle", AnimationConfig("taunt4"))
  character_control.on_key_up(74, "taunt4", AnimationConfig("idle"))

  # K
  character_control.on_key_down(75, "idle", AnimationConfig("taunt5"))
  character_control.on_key_up(75, "taunt5", AnimationConfig("idle"))

  # L
  character_control.on_key_down(76, "idle", AnimationConfig("taunt6"))
  character_control.on_key_up(76, "taunt6", AnimationConfig("idle"))


  # SPACE BAR
  character_control.on_key_down(32, "idle", AnimationConfig("jump", False),0.1)
  character_control.on_key_down(32, "run_fwd", AnimationConfig("jump_fwd", False, 1.5),0.25)
  character_control.on_key_down(32, "run_bwd", AnimationConfig("jump_bwd", False),0.25)
  #character_control.on_key_down(32, "run_lt", AnimationConfig("jump_lt", False),0.25)
  #character_control.on_key_down(32, "run_rt", AnimationConfig("jump_rt", False),0.25)

  # animation dependend translations:
  character_control.bind_translation("run_fwd",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("run_bwd",avango.gua.Vec3(0.0,0.0,-0.05))
  #character_control.bind_translation("run_lt",avango.gua.Vec3(0.05,0.0,0.0))
  #character_control.bind_translation("run_rt",avango.gua.Vec3(-0.05,0.0,0.0))
  character_control.bind_translation("swim",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("crouch_fwd",avango.gua.Vec3(0.0,0.0,0.035))
  character_control.bind_translation("crouch_bwd",avango.gua.Vec3(0.0,0.0,-0.035))
  #character_control.bind_translation("crouch_lt",avango.gua.Vec3(0.035,0.0,0.0))
  #character_control.bind_translation("crouch_rt",avango.gua.Vec3(-0.035,0.0,0.0))
  character_control.bind_translation("jump",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_loop",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_preland",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_fwd",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_loop",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_preland",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_land",avango.gua.Vec3(0.0, 0.0, 0.04))
  character_control.bind_translation("jump_bwd",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_loop",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_preland",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_land",avango.gua.Vec3(0.0, 0.0, -0.04))
  #character_control.bind_translation("jump_lt",avango.gua.Vec3(0.04, 0.08, 0.0))
  #character_control.bind_translation("jump_lt_loop",avango.gua.Vec3(0.04, 0.08, 0.0))
  #character_control.bind_translation("jump_lt_preland",avango.gua.Vec3(0.04, 0.08, 0.0))
  #character_control.bind_translation("jump_lt_land",avango.gua.Vec3(0.04, 0.0, 0.0))
  #character_control.bind_translation("jump_rt",avango.gua.Vec3(-0.04, 0.08, 0.0))
  #character_control.bind_translation("jump_rt_loop",avango.gua.Vec3(-0.04, 0.08, 0.0))
  #character_control.bind_translation("jump_rt_preland",avango.gua.Vec3(-0.04, 0.08, 0.0))
  #character_control.bind_translation("jump_rt_land",avango.gua.Vec3(-0.04, 0.0, 0.0))

  # animations only played once and followed by another one
  character_control.on_animation_end("jump", AnimationConfig("jump_loop"),0.1)
  character_control.on_animation_end("jump_fwd", AnimationConfig("jump_fwd_loop"),0.1)
  character_control.on_animation_end("jump_bwd", AnimationConfig("jump_bwd_loop"),0.1)
  #character_control.on_animation_end("jump_lt", AnimationConfig("jump_lt_loop"),0.1)
  #character_control.on_animation_end("jump_rt", AnimationConfig("jump_rt_loop"),0.1)
  character_control.on_animation_end("jump_land", AnimationConfig("idle"),0.25)
  character_control.on_animation_end("jump_fwd_land", AnimationConfig("idle"),0.25)
  character_control.on_animation_end("jump_bwd_land", AnimationConfig("idle"),0.25)
  #character_control.on_animation_end("jump_lt_land", AnimationConfig("idle"),0.25)
  #character_control.on_animation_end("jump_rt_land", AnimationConfig("idle"),0.25)


def apply_character_control_settings2(character_control, device_sensor = None):

  # optional / additional xbox controller settings:
  if device_sensor != None:
    character_control.XBOX_X.connect_from(device_sensor.Value0)
    character_control.XBOX_Y.connect_from(device_sensor.Value1)
    character_control.XBOX_BTN_A.connect_from(device_sensor.Button2)
    character_control.XBOX_BTN_X.connect_from(device_sensor.Button0)
    character_control.xbox_override_key(character_control.XBOX_Y,183,0.1)
    character_control.xbox_override_key(character_control.XBOX_Y,187,-0.1,-1.0)
    character_control.xbox_override_key(character_control.XBOX_X,168,0.1)
    character_control.xbox_override_key(character_control.XBOX_X,165,-0.1,-1.0)
    character_control.xbox_override_key(character_control.XBOX_BTN_A,132)
    character_control.xbox_override_key(character_control.XBOX_BTN_X,167)
    character_control.xbox_animation_speed(character_control.XBOX_Y,"run_fwd")
    character_control.xbox_animation_speed(character_control.XBOX_Y,"run_bwd")
    character_control.xbox_animation_speed(character_control.XBOX_X,"run_lt")
    character_control.xbox_animation_speed(character_control.XBOX_X,"run_rt")
    character_control.xbox_animation_speed(character_control.XBOX_Y,"crouch_fwd")
    character_control.xbox_animation_speed(character_control.XBOX_Y,"crouch_bwd")
    character_control.xbox_animation_speed(character_control.XBOX_X,"crouch_lt")
    character_control.xbox_animation_speed(character_control.XBOX_X,"crouch_rt")

  # A
  character_control.bind_transformation(165, "run_fwd", avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(165, "run_bwd", avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(165, "crouch_fwd", avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(165, "crouch_bwd", avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(165, "swim", avango.gua.make_rot_mat(2.0, 0.0, 1.0,0.0))
  character_control.on_key_down(165, "idle", AnimationConfig("run_lt"))
  character_control.on_key_up(165, "run_lt", AnimationConfig("idle"))
  character_control.on_key_down(165, "crouch", AnimationConfig("crouch_lt"))
  character_control.on_key_up(165, "crouch_lt", AnimationConfig("crouch"))

  # D
  character_control.bind_transformation(168, "run_fwd", avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(168, "run_bwd", avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(168, "crouch_fwd", avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(168, "crouch_bwd", avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0))
  character_control.bind_transformation(168, "swim", avango.gua.make_rot_mat(-2.0, 0.0, 1.0,0.0))
  character_control.on_key_down(168, "idle", AnimationConfig("run_rt"))
  character_control.on_key_up(168, "run_rt", AnimationConfig("idle"))
  character_control.on_key_down(168, "crouch", AnimationConfig("crouch_rt"))
  character_control.on_key_up(168, "crouch_rt", AnimationConfig("crouch"))


  # W
  character_control.on_key_down(187, "idle", AnimationConfig("run_fwd"))
  character_control.on_key_up(187, "run_fwd", AnimationConfig("idle"))
  character_control.on_key_down(187, "crouch", AnimationConfig("crouch_fwd"))
  character_control.on_key_up(187, "crouch_fwd", AnimationConfig("crouch"))
  character_control.bind_transformation(187, "run_rt", avango.gua.make_trans_mat(0.0,0.0,0.025))
  character_control.bind_transformation(187, "run_lt", avango.gua.make_trans_mat(0.0,0.0,0.025))
  character_control.bind_transformation(187, "crouch_rt", avango.gua.make_trans_mat(0.0,0.0,0.0175))
  character_control.bind_transformation(187, "crouch_lt", avango.gua.make_trans_mat(0.0,0.0,0.0175))

  # S
  character_control.on_key_down(183, "idle", AnimationConfig("run_bwd"))
  character_control.on_key_up(183, "run_bwd", AnimationConfig("idle"))
  character_control.on_key_down(183, "crouch", AnimationConfig("crouch_bwd"))
  character_control.on_key_up(183, "crouch_bwd", AnimationConfig("crouch"))
  character_control.bind_transformation(183, "run_rt", avango.gua.make_trans_mat(0.0,0.0,-0.025))
  character_control.bind_transformation(183, "run_lt", avango.gua.make_trans_mat(0.0,0.0,-0.025))
  character_control.bind_transformation(183, "crouch_rt", avango.gua.make_trans_mat(0.0,0.0,-0.0175))
  character_control.bind_transformation(183, "crouch_lt", avango.gua.make_trans_mat(0.0,0.0,-0.0175))

  # C
  character_control.on_key_down(167, "idle", AnimationConfig("crouch"),0.2)
  character_control.on_key_up(167, "crouch", AnimationConfig("idle"),0.4)
  character_control.on_key_down(167, "run_fwd", AnimationConfig("crouch_fwd"))
  character_control.on_key_up(167, "crouch_fwd", AnimationConfig("run_fwd"))
  character_control.on_key_down(167, "run_bwd", AnimationConfig("crouch_bwd"))
  character_control.on_key_up(167, "crouch_bwd", AnimationConfig("run_bwd"))
  character_control.on_key_down(167, "run_lt", AnimationConfig("crouch_lt"))
  character_control.on_key_up(167, "crouch_lt", AnimationConfig("run_lt"))
  character_control.on_key_down(167, "run_rt", AnimationConfig("crouch_rt"))
  character_control.on_key_up(167, "crouch_rt", AnimationConfig("run_rt"))

  # F
  character_control.on_key_down(170, "idle", AnimationConfig("taunt1"))
  character_control.on_key_up(170, "taunt1", AnimationConfig("idle"))

  # G
  character_control.on_key_down(171, "idle", AnimationConfig("taunt2"))
  character_control.on_key_up(171, "taunt2", AnimationConfig("idle"))

  # H
  character_control.on_key_down(172, "idle", AnimationConfig("taunt3"))
  character_control.on_key_up(172, "taunt3", AnimationConfig("idle"))

  # J
  character_control.on_key_down(174, "idle", AnimationConfig("taunt4"))
  character_control.on_key_up(174, "taunt4", AnimationConfig("idle"))

  # K
  character_control.on_key_down(175, "idle", AnimationConfig("taunt5"))
  character_control.on_key_up(175, "taunt5", AnimationConfig("idle"))

  # L
  character_control.on_key_down(176, "idle", AnimationConfig("taunt6"))
  character_control.on_key_up(176, "taunt6", AnimationConfig("idle"))


  # SPACE BAR
  character_control.on_key_down(132, "idle", AnimationConfig("jump", False),0.1)
  character_control.on_key_down(132, "run_fwd", AnimationConfig("jump_fwd", False, 1.5),0.25)
  character_control.on_key_down(132, "run_bwd", AnimationConfig("jump_bwd", False),0.25)
  character_control.on_key_down(132, "run_lt", AnimationConfig("jump_lt", False),0.25)
  character_control.on_key_down(132, "run_rt", AnimationConfig("jump_rt", False),0.25)

  # animation dependend translations:
  character_control.bind_translation("run_fwd",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("run_bwd",avango.gua.Vec3(0.0,0.0,-0.05))
  character_control.bind_translation("run_lt",avango.gua.Vec3(0.05,0.0,0.0))
  character_control.bind_translation("run_rt",avango.gua.Vec3(-0.05,0.0,0.0))
  character_control.bind_translation("swim",avango.gua.Vec3(0.0,0.0,0.05))
  character_control.bind_translation("crouch_fwd",avango.gua.Vec3(0.0,0.0,0.035))
  character_control.bind_translation("crouch_bwd",avango.gua.Vec3(0.0,0.0,-0.035))
  character_control.bind_translation("crouch_lt",avango.gua.Vec3(0.035,0.0,0.0))
  character_control.bind_translation("crouch_rt",avango.gua.Vec3(-0.035,0.0,0.0))
  character_control.bind_translation("jump",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_loop",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_preland",avango.gua.Vec3(0.0, 0.08, 0.0))
  character_control.bind_translation("jump_fwd",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_loop",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_preland",avango.gua.Vec3(0.0, 0.08, 0.04))
  character_control.bind_translation("jump_fwd_land",avango.gua.Vec3(0.0, 0.0, 0.04))
  character_control.bind_translation("jump_bwd",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_loop",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_preland",avango.gua.Vec3(0.0, 0.08, -0.04))
  character_control.bind_translation("jump_bwd_land",avango.gua.Vec3(0.0, 0.0, -0.04))
  character_control.bind_translation("jump_lt",avango.gua.Vec3(0.04, 0.08, 0.0))
  character_control.bind_translation("jump_lt_loop",avango.gua.Vec3(0.04, 0.08, 0.0))
  character_control.bind_translation("jump_lt_preland",avango.gua.Vec3(0.04, 0.08, 0.0))
  character_control.bind_translation("jump_lt_land",avango.gua.Vec3(0.04, 0.0, 0.0))
  character_control.bind_translation("jump_rt",avango.gua.Vec3(-0.04, 0.08, 0.0))
  character_control.bind_translation("jump_rt_loop",avango.gua.Vec3(-0.04, 0.08, 0.0))
  character_control.bind_translation("jump_rt_preland",avango.gua.Vec3(-0.04, 0.08, 0.0))
  character_control.bind_translation("jump_rt_land",avango.gua.Vec3(-0.04, 0.0, 0.0))

  # animations only played once and followed by another one
  character_control.on_animation_end("jump", AnimationConfig("jump_loop"),0.1)
  character_control.on_animation_end("jump_fwd", AnimationConfig("jump_fwd_loop"),0.1)
  character_control.on_animation_end("jump_bwd", AnimationConfig("jump_bwd_loop"),0.1)
  character_control.on_animation_end("jump_lt", AnimationConfig("jump_lt_loop"),0.1)
  character_control.on_animation_end("jump_rt", AnimationConfig("jump_rt_loop"),0.1)
  character_control.on_animation_end("jump_land", AnimationConfig("idle"),0.25)
  character_control.on_animation_end("jump_fwd_land", AnimationConfig("idle"),0.25)
  character_control.on_animation_end("jump_bwd_land", AnimationConfig("idle"),0.25)
  character_control.on_animation_end("jump_lt_land", AnimationConfig("idle"),0.25)
  character_control.on_animation_end("jump_rt_land", AnimationConfig("idle"),0.25)


def load_mixamo_animations_adam(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_Adam_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_adam_run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_adam_run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_Adam_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Adam/Mixamo_Adam_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_alpha(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Alpha/mixamo_runningJump_blender.FBX","jump_fwd")

def load_mixamo_animations_ganfault(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Ganfault/Mixamo_Ganfault_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Ganfault/Mixamo_Ganfault_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Ganfault/Mixamo_Ganfault_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Ganfault/Mixamo_Ganfault_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Ganfault/Mixamo_Ganfault_run_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_kachujin(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Kachujin/Kachujin_G_Rosales_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_maria(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maria/Maria_J_J_Ong_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maria/Maria_J_J_Ong_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maria/Maria_J_J_Ong_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maria/Maria_J_J_Ong_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maria/Maria_J_J_Ong_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_maw(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Maw/Maw_J_Laygo_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_swat(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Swat_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Swat_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Swat_Jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/SWAT/Maximo_Swat_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_vampire(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vampire/Maximo_Vampire_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vampire/Maximo_Vampire_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vampire/Maximo_Vampire_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vampire/Maximo_Vampire_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vampire/Maximo_Vampire_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_vanguard(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Vanguard/Vanguard_by_T__Choonyung_Running_Jump_blender.FBX","jump_fwd")

def load_mixamo_animations_zoe(bob):
  
  # idle:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_idle_blender.FBX","idle")
  # run:
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_Run_blender.FBX","run_fwd")
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_Run_blender.FBX","run_fwd_fall")
  #jump
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_jump_blender.FBX","jump")
  #jump_fwd
  bob.load_animation("/opt/project_animation/Assets/Mixamo/Zoe/Mixamo_Zoe_Running_Jump_blender.FBX","jump_fwd")


def apply_character_control_settings_mixamo(character_control, device_sensor = None):

  # optional / additional xbox controller settings:
  if device_sensor != None:
    character_control.XBOX_X.connect_from(device_sensor.Value0)
    character_control.XBOX_Y.connect_from(device_sensor.Value1)
    character_control.XBOX_BTN_A.connect_from(device_sensor.Button2)
    character_control.XBOX_BTN_X.connect_from(device_sensor.Button0)
    character_control.xbox_override_key(character_control.XBOX_Y,83,0.1)
    character_control.xbox_override_key(character_control.XBOX_Y,87,-0.1,-1.0)
    character_control.xbox_override_key(character_control.XBOX_X,68,0.1)
    character_control.xbox_override_key(character_control.XBOX_X,65,-0.1,-1.0)
    character_control.xbox_override_key(character_control.XBOX_BTN_A,32)
    character_control.xbox_override_key(character_control.XBOX_BTN_X,67)
    character_control.xbox_animation_speed(character_control.XBOX_Y,"run_fwd")

  # A
  # character_control.bind_transformation(65, "run_fwd", avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))
  # character_control.bind_transformation(65, "idle", avango.gua.make_rot_mat(4.0, 0.0, 1.0,0.0))#
  character_control.bind_rotation(65, "run_fwd", avango.gua.Vec4(4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(65, "idle", avango.gua.Vec4(4.0, 0.0, 1.0,0.0))

  # D
  # character_control.bind_transformation(68, "run_fwd", avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))
  # character_control.bind_transformation(68, "idle", avango.gua.make_rot_mat(-4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "run_fwd", avango.gua.Vec4(-4.0, 0.0, 1.0,0.0))
  character_control.bind_rotation(68, "idle", avango.gua.Vec4(-4.0, 0.0, 1.0,0.0))

  # W
  character_control.on_key_down(87, "idle", AnimationConfig("run_fwd"))
  character_control.on_key_up(87, "run_fwd", AnimationConfig("idle"))

  # SPACE BAR
  character_control.on_key_down(32, "idle", AnimationConfig("jump", False),0.1)
  character_control.on_key_down(32, "run_fwd", AnimationConfig("jump_fwd", False, 1.0, 0.6),0.1)

  # animation dependend translations:
  character_control.bind_translation("run_fwd",avango.gua.Vec3(0.0,0.0,0.00125))
  character_control.bind_translation("jump_fwd",avango.gua.Vec3(0.0, 0.0015, 0.001))
  character_control.bind_translation("run_fwd_fall",avango.gua.Vec3(0.0, 0.0, 0.001))

  character_control.on_animation_end("jump",AnimationConfig("idle"))
  character_control.on_animation_end("jump_fwd",AnimationConfig("run_fwd_fall", True, 1.0, 0.5))
  character_control.on_animation_end("run_fwd_fall",AnimationConfig("idle"))


def camera_control_xbox_connect(camera_control, device_sensor):
  # optional / additional xbox controller settings:
  camera_control.XBOX_X.connect_from(device_sensor.Value2)
  camera_control.XBOX_Y.connect_from(device_sensor.Value3)
  camera_control.XBOX_LZ.connect_from(device_sensor.Value4)
  camera_control.XBOX_RZ.connect_from(device_sensor.Value5)

def camera_control_xbox_disconnect(camera_control, device_sensor):
  # optional / additional xbox controller settings:
  camera_control.XBOX_X.disconnect()
  camera_control.XBOX_Y.disconnect()
  camera_control.XBOX_LZ.disconnect()
  camera_control.XBOX_RZ.disconnect()
