cmake_minimum_required(VERSION 3.4)
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
    core/ControlLibrary/ButtonControl.cpp
    core/ControlLibrary/ButtonControl.h
    core/ControlLibrary/ContainerControl.cpp
    core/ControlLibrary/ContainerControl.h
    core/ControlLibrary/Control.cpp
    core/ControlLibrary/Control.h
    core/ControlLibrary/ControlDocument.cpp
    core/ControlLibrary/ControlDocument.h
    core/ControlLibrary/ControlFactory.cpp
    core/ControlLibrary/ControlFactory.h
    core/ControlLibrary/ImageControl.cpp
    core/ControlLibrary/ImageControl.h
    core/ControlLibrary/LabelControl.cpp
    core/ControlLibrary/LabelControl.h
    core/ControlLibrary/LevelButtonControl.cpp
    core/ControlLibrary/LevelButtonControl.h
    core/ControlLibrary/PanelControl.cpp
    core/ControlLibrary/PanelControl.h
    core/ControlLibrary/RadioButtonControl.cpp
    core/ControlLibrary/RadioButtonControl.h
    core/ControlLibrary/RadioButtonGroup.cpp
    core/ControlLibrary/RadioButtonGroup.h
    core/ControlLibrary/SwipeBarControl.cpp
    core/ControlLibrary/SwipeBarControl.h
    core/tinyxml/tinyxml2.cpp
    core/tinyxml/tinyxml2.h
    core/Game/StateProcessors/Menu/AchievementsStateProcessor.cpp
    core/Game/StateProcessors/Menu/AchievementsStateProcessor.h
    core/Game/StateProcessors/Menu/ArcadeDifficultyStateProcessor.cpp
    core/Game/StateProcessors/Menu/ArcadeDifficultyStateProcessor.h
    core/Game/StateProcessors/Race/Space/Asteroid.cpp
    core/Game/StateProcessors/Race/Space/Asteroid.h
    core/Game/StateProcessors/Race/Space/Barrier.cpp
    core/Game/StateProcessors/Race/Space/Barrier.h
    core/Basic.cpp
    core/Basic.h
    core/Game/StateProcessors/Race/Space/BlasterBurst.cpp
    core/Game/StateProcessors/Race/Space/BlasterBurst.h
    core/Render/Camera.cpp
    core/Render/Camera.h
    core/Render/Drawables/CircleDrawable.cpp
    core/Render/Drawables/CircleDrawable.h
    core/Game/Config.cpp
    core/Game/Config.h
    core/Game/StateProcessors/Menu/CreditsStateProcessor.cpp
    core/Game/StateProcessors/Menu/CreditsStateProcessor.h
        core/Game/StateProcessors/Race/Space/Mine.cpp
        core/Game/StateProcessors/Race/Space/Mine.h
    core/Game/StateProcessors/Race/Space/EnergyOrb.cpp
    core/Game/StateProcessors/Race/Space/EnergyOrb.h
    core/Game/StateProcessors/Race/Space/Explosion.cpp
    core/Game/StateProcessors/Race/Space/Explosion.h
    core/Game/StateProcessors/Race/Space/Fence.cpp
    core/Game/StateProcessors/Race/Space/Fence.h
    core/Game/Game.cpp
    core/Game/Game.h
    core/Game/GameDefs.h
    core/Game/GameState.h
    core/Game/Gun.cpp
    core/Game/Gun.h
    core/Game/StateProcessors/Menu/HighScoresStateProcessor.cpp
    core/Game/StateProcessors/Menu/HighScoresStateProcessor.h
    core/Render/Drawables/IDrawable.h
    core/Input/IInputListener.h
    core/Game/StateProcessors/IStateProcessor.h
    core/Game/StateProcessors/Level/LevelManager.cpp
    core/Game/StateProcessors/Level/LevelManager.h
    core/Game/StateProcessors/Menu/LevelSelectionStateProcessor.cpp
    core/Game/StateProcessors/Menu/LevelSelectionStateProcessor.h
    core/Game/StateProcessors/Race/LevelStructure.h
    core/Render/Light.cpp
    core/Render/Light.h
    core/Render/Drawables/LineDrawable.cpp
    core/Render/Drawables/LineDrawable.h
    core/Game/StateProcessors/Menu/MenuStateProcessor.cpp
    core/Game/StateProcessors/Menu/MenuStateProcessor.h
    core/Game/StateProcessors/Menu/MessageStateProcessor.cpp
    core/Game/StateProcessors/Menu/MessageStateProcessor.h
    core/Render/Drawables/ModelAnimation.cpp
    core/Render/Drawables/ModelAnimation.h
    core/Render/Drawables/ModelDrawable.cpp
    core/Render/Drawables/ModelDrawable.h
    core/Render/Drawables/ParticleDrawable.cpp
    core/Render/Drawables/ParticleDrawable.h
    core/Render/ParticleSystem.cpp
    core/Render/ParticleSystem.h
    core/Game/StateProcessors/Menu/PauseStateProcessor.cpp
    core/Game/StateProcessors/Menu/PauseStateProcessor.h
    core/PrecompileHeaders.cpp
    core/PrecompileHeaders.h
    core/Game/StateProcessors/Race/RaceStateProcessor.cpp
    core/Game/StateProcessors/Race/RaceStateProcessor.h
    core/Render/Drawables/RectDrawable.cpp
    core/Render/Drawables/RectDrawable.h
    core/Render/SceneSector.cpp
    core/Render/SceneSector.h
    core/Game/Scores.cpp
    core/Game/Scores.h
    core/Game/StateProcessors/Menu/ScoreStateProcessor.cpp
    core/Game/StateProcessors/Menu/ScoreStateProcessor.h
    core/Game/SkinManager.cpp
    core/Game/SkinManager.h
    core/Game/StateProcessors/Menu/SliderProcessor.cpp
    core/Game/StateProcessors/Menu/SliderProcessor.h
    core/Game/StateProcessors/Race/Space/Space.cpp
    core/Game/StateProcessors/Race/Space/Space.h
    core/Game/StateProcessors/Race/Space/SpaceDefs.h
    core/Game/StateProcessors/Race/Space/SpaceDust.cpp
    core/Game/StateProcessors/Race/Space/SpaceDust.h
    core/Game/StateProcessors/Race/Space/SpaceObject.cpp
    core/Game/StateProcessors/Race/Space/SpaceObject.h
    core/Game/StateProcessors/Race/Space/Sparks.cpp
    core/Game/StateProcessors/Race/Space/Sparks.h
    core/Game/StateProcessorFactory.cpp
    core/Game/StateProcessorFactory.h
    core/Game/StateProcessors/Menu/StoreStateProcessor.cpp
    core/Game/StateProcessors/Menu/StoreStateProcessor.h
    core/Game/StateProcessors/Menu/WorldSelectionStateProcessor.cpp
    core/Game/StateProcessors/Menu/WorldSelectionStateProcessor.h
    OpenAL/OpenAL.cpp
    OpenAL/OpenAL.h
    BillingProcessor.cpp
    BillingProcessor.h
    FileSystem.cpp
    FileSystem.h
    Input.cpp
    Input.h
    Main.cpp
    OgreApplication.cpp
    OgreApplication.h
    RenderProcessor.cpp
    RenderProcessor.h
    SoundSystem.cpp
    SoundSystem.h LinuxDefs.h core/Game/StateProcessors/Race/Space/Cruiser.cpp
    core/Game/StateProcessors/Race/Space/Cruiser.h
    core/Game/StateProcessors/Race/Space/Missile.cpp
    core/Game/StateProcessors/Race/Space/Missile.h
    core/Game/StateProcessors/Race/Prefab.cpp
    core/Game/StateProcessors/Race/Prefab.h
    core/Game/StateProcessors/Race/PrefabManager.cpp
    core/Game/StateProcessors/Race/PrefabManager.h
    core/Game/StateProcessors/Race/Space/Boss.cpp
    core/Game/StateProcessors/Race/Space/Boss.h
    core/Game/StateProcessors/Menu/StoryboardStateProcessor.cpp
    core/Game/StateProcessors/Menu/StoryboardStateProcessor.h
    core/ControlLibrary/SlidePanelControl.cpp
    core/ControlLibrary/SlidePanelControl.h)

add_executable(spacerush ${SOURCE_FILES})

file(GLOB OGRE_LIBS "/usr/local/lib/libOgre*.so")
file(GLOB OGRE_LIBS_2 "/usr/local/lib/OGRE/*.so")
target_link_libraries (spacerush  ${OGRE_LIBS_2} ${OGRE_LIBS})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED thread system)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(spacerush ${Boost_LIBRARIES})

target_link_libraries(spacerush Cg OIS pthread GL GLU freeimage openal audio zzip Xft dl)