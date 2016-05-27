cmake_minimum_required(VERSION 2.8)
project(linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(core)
include_directories(core/ControlLibrary)
include_directories(core/tinyxml)
include_directories(OpenAL)
include_directories("./")
include_directories("/usr/local/include/OGRE")
include_directories("/usr/local/include/OGRE/Overlay")
include_directories("/usr/local/include/OGRE/Plugins/ParticleFX")
include_directories("/usr/local/include/OGRE/Plugins/CgProgramManager")
include_directories("/usr/local/include/OGRE/RenderSystems/GL")

set(SOURCE_FILES
    BillingProcessor.h
    BillingProcessor.cpp
    SoundSystem.h
    SoundSystem.cpp
    RenderProcessor.h
    RenderProcessor.cpp
    OgreApplication.h
    OgreApplication.cpp
    Main.cpp
    LinuxDefs.h
    Input.h
    Input.cpp
    FileSystem.h
    FileSystem.cpp
    OpenAL/OpenAL.h
    OpenAL/OpenAL.cpp
    core/PrecompileHeaders.h
    core/PrecompileHeaders.cpp
    core/Basic.h
    core/Basic.cpp
    core/tinyxml/tinyxml2.h
    core/tinyxml/tinyxml2.cpp
    core/Input/IInputListener.h
    core/Render/SceneSector.h
    core/Render/SceneSector.cpp
    core/Render/ParticleSystem.h
    core/Render/ParticleSystem.cpp
    core/Render/Light.h
    core/Render/Light.cpp
    core/Render/Camera.h
    core/Render/Camera.cpp
    core/Render/Drawables/RectDrawable.h
    core/Render/Drawables/RectDrawable.cpp
    core/Render/Drawables/ParticleDrawable.h
    core/Render/Drawables/ParticleDrawable.cpp
    core/Render/Drawables/ModelDrawable.h
    core/Render/Drawables/ModelDrawable.cpp
    core/Render/Drawables/ModelAnimation.h
    core/Render/Drawables/ModelAnimation.cpp
    core/Render/Drawables/LineDrawable.h
    core/Render/Drawables/LineDrawable.cpp
    core/Render/Drawables/IDrawable.h
    core/Render/Drawables/CircleDrawable.h
    core/Render/Drawables/CircleDrawable.cpp
    core/Game/SkinManager.h
    core/Game/SkinManager.cpp
    core/Game/Scores.h
    core/Game/Scores.cpp
    core/Game/Gun.h
    core/Game/Gun.cpp
    core/Game/GameState.h
    core/Game/GameDefs.h
    core/Game/Game.h
    core/Game/Game.cpp
    core/Game/Config.h
    core/Game/Config.cpp
    core/Game/StateProcessorFactory.h
    core/Game/StateProcessorFactory.cpp
    core/Game/StateProcessors/IStateProcessor.h
    core/Game/StateProcessors/Race/Space/Sparks.h
    core/Game/StateProcessors/Race/Space/Sparks.cpp
    core/Game/StateProcessors/Race/Space/SpaceObject.h
    core/Game/StateProcessors/Race/Space/SpaceObject.cpp
    core/Game/StateProcessors/Race/Space/SpaceDust.h
    core/Game/StateProcessors/Race/Space/SpaceDust.cpp
    core/Game/StateProcessors/Race/Space/SpaceDefs.h
    core/Game/StateProcessors/Race/Space/Space.h
    core/Game/StateProcessors/Race/Space/Space.cpp
    core/Game/StateProcessors/Race/Space/Missile.h
    core/Game/StateProcessors/Race/Space/Missile.cpp
    core/Game/StateProcessors/Race/Space/Mine.h
    core/Game/StateProcessors/Race/Space/Mine.cpp
    core/Game/StateProcessors/Race/Space/Fence.h
    core/Game/StateProcessors/Race/Space/Fence.cpp
    core/Game/StateProcessors/Race/Space/Explosion.h
    core/Game/StateProcessors/Race/Space/Explosion.cpp
    core/Game/StateProcessors/Race/Space/EnergyOrb.h
    core/Game/StateProcessors/Race/Space/EnergyOrb.cpp
    core/Game/StateProcessors/Race/Space/Cruiser.h
    core/Game/StateProcessors/Race/Space/Cruiser.cpp
    core/Game/StateProcessors/Race/Space/BossInterface.h
    core/Game/StateProcessors/Race/Space/Boss.h
    core/Game/StateProcessors/Race/Space/Boss.cpp
    core/Game/StateProcessors/Race/Space/BlasterBurst.h
    core/Game/StateProcessors/Race/Space/BlasterBurst.cpp
    core/Game/StateProcessors/Race/Space/BigBoss.h
    core/Game/StateProcessors/Race/Space/BigBoss.cpp
    core/Game/StateProcessors/Race/Space/Barrier.h
    core/Game/StateProcessors/Race/Space/Barrier.cpp
    core/Game/StateProcessors/Race/Space/AsteroidProcessor.h
    core/Game/StateProcessors/Race/Space/AsteroidProcessor.cpp
    core/Game/StateProcessors/Race/Space/Asteroid.h
    core/Game/StateProcessors/Race/Space/Asteroid.cpp
    core/Game/StateProcessors/Race/RaceStateProcessor.h
    core/Game/StateProcessors/Race/RaceStateProcessor.cpp
    core/Game/StateProcessors/Race/PrefabManager.h
    core/Game/StateProcessors/Race/PrefabManager.cpp
    core/Game/StateProcessors/Race/Prefab.h
    core/Game/StateProcessors/Race/Prefab.cpp
    core/Game/StateProcessors/Race/LevelStructure.h
    core/Game/StateProcessors/Race/LevelFileManager.h
    core/Game/StateProcessors/Race/LevelFileManager.cpp
    core/Game/StateProcessors/Menu/StoryboardStateProcessor.h
    core/Game/StateProcessors/Menu/StoryboardStateProcessor.cpp
    core/Game/StateProcessors/Menu/StoreStateProcessor.h
    core/Game/StateProcessors/Menu/StoreStateProcessor.cpp
    core/Game/StateProcessors/Menu/SelectShipStateProcessor.h
    core/Game/StateProcessors/Menu/SelectShipStateProcessor.cpp
    core/Game/StateProcessors/Menu/ScoreStateProcessor.h
    core/Game/StateProcessors/Menu/ScoreStateProcessor.cpp
    core/Game/StateProcessors/Menu/PauseStateProcessor.h
    core/Game/StateProcessors/Menu/PauseStateProcessor.cpp
    core/Game/StateProcessors/Menu/MessageStateProcessor.h
    core/Game/StateProcessors/Menu/MessageStateProcessor.cpp
    core/Game/StateProcessors/Menu/MenuStateProcessor.h
    core/Game/StateProcessors/Menu/MenuStateProcessor.cpp
    core/Game/StateProcessors/Menu/CreditsStateProcessor.h
    core/Game/StateProcessors/Menu/CreditsStateProcessor.cpp
    core/Game/StateProcessors/Menu/AchievementsStateProcessor.h
    core/Game/StateProcessors/Menu/AchievementsStateProcessor.cpp
    core/Game/StateProcessors/Level/LevelManager.h
    core/Game/StateProcessors/Level/LevelManager.cpp
    core/ControlLibrary/SwipeBarControl.h
    core/ControlLibrary/SwipeBarControl.cpp
    core/ControlLibrary/SlidePanelControl.h
    core/ControlLibrary/SlidePanelControl.cpp
    core/ControlLibrary/RadioButtonGroup.h
    core/ControlLibrary/RadioButtonGroup.cpp
    core/ControlLibrary/RadioButtonControl.h
    core/ControlLibrary/RadioButtonControl.cpp
    core/ControlLibrary/PanelControl.h
    core/ControlLibrary/PanelControl.cpp
    core/ControlLibrary/LevelButtonControl.h
    core/ControlLibrary/LevelButtonControl.cpp
    core/ControlLibrary/LabelControl.h
    core/ControlLibrary/LabelControl.cpp
    core/ControlLibrary/ImageControl.h
    core/ControlLibrary/ImageControl.cpp
    core/ControlLibrary/ControlFactory.h
    core/ControlLibrary/ControlFactory.cpp
    core/ControlLibrary/ControlDocument.h
    core/ControlLibrary/ControlDocument.cpp
    core/ControlLibrary/Control.h
    core/ControlLibrary/Control.cpp
    core/ControlLibrary/ContainerControl.h
    core/ControlLibrary/ContainerControl.cpp
    core/ControlLibrary/ButtonControl.h
    core/ControlLibrary/ButtonControl.cpp
        core/Game/StateProcessors/Menu/SaveMeStateProcessor.cpp core/Game/StateProcessors/Menu/SaveMeStateProcessor.h core/Game/StateProcessors/Menu/ChallengesStateProcessor.cpp core/Game/StateProcessors/Menu/ChallengesStateProcessor.h core/Game/StateProcessors/Menu/HighscoresStateProcessor.cpp core/Game/StateProcessors/Menu/HighscoresStateProcessor.h)

add_executable(spacerush ${SOURCE_FILES})

file(GLOB OGRE_LIBS "/usr/local/lib/libOgre*.so")
file(GLOB OGRE_LIBS_2 "/usr/local/lib/OGRE/*.so")
target_link_libraries (spacerush  ${OGRE_LIBS_2} ${OGRE_LIBS})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED thread system)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(spacerush ${Boost_LIBRARIES})

target_link_libraries(spacerush Cg OIS pthread GL GLU freeimage openal audio zzip Xft dl)

# add target that will increment build number
add_custom_target( increment ALL
        "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_COMMAND}" -P "increment.cmake"
        )

add_dependencies( spacerush increment )