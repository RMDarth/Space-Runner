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
    core/AchievementsStateProcessor.cpp
    core/AchievementsStateProcessor.h
    core/ArcadeDifficultyStateProcessor.cpp
    core/ArcadeDifficultyStateProcessor.h
    core/Asteroid.cpp
    core/Asteroid.h
    core/Barrier.cpp
    core/Barrier.h
    core/Basic.cpp
    core/Basic.h
    core/BlasterBurst.cpp
    core/BlasterBurst.h
    core/Camera.cpp
    core/Camera.h
    core/CircleDrawable.cpp
    core/CircleDrawable.h
    core/Config.cpp
    core/Config.h
    core/CreditsStateProcessor.cpp
    core/CreditsStateProcessor.h
    core/EnemyFighter.cpp
    core/EnemyFighter.h
    core/EnergyOrb.cpp
    core/EnergyOrb.h
    core/Explosion.cpp
    core/Explosion.h
    core/Fence.cpp
    core/Fence.h
    core/Game.cpp
    core/Game.h
    core/GameDefs.h
    core/GameState.h
    core/Gun.cpp
    core/Gun.h
    core/HighScoresStateProcessor.cpp
    core/HighScoresStateProcessor.h
    core/IDrawable.h
    core/IInputListener.h
    core/IStateProcessor.h
    core/LevelManager.cpp
    core/LevelManager.h
    core/LevelSelectionStateProcessor.cpp
    core/LevelSelectionStateProcessor.h
    core/LevelStructure.h
    core/Light.cpp
    core/Light.h
    core/LineDrawable.cpp
    core/LineDrawable.h
    core/MenuStateProcessor.cpp
    core/MenuStateProcessor.h
    core/MessageStateProcessor.cpp
    core/MessageStateProcessor.h
    core/ModelAnimation.cpp
    core/ModelAnimation.h
    core/ModelDrawable.cpp
    core/ModelDrawable.h
    core/ParticleDrawable.cpp
    core/ParticleDrawable.h
    core/ParticleSystem.cpp
    core/ParticleSystem.h
    core/PauseStateProcessor.cpp
    core/PauseStateProcessor.h
    core/PrecompileHeaders.cpp
    core/PrecompileHeaders.h
    core/RaceStateProcessor.cpp
    core/RaceStateProcessor.h
    core/RectDrawable.cpp
    core/RectDrawable.h
    core/SceneSector.cpp
    core/SceneSector.h
    core/Scores.cpp
    core/Scores.h
    core/ScoreStateProcessor.cpp
    core/ScoreStateProcessor.h
    core/SkinManager.cpp
    core/SkinManager.h
    core/SliderProcessor.cpp
    core/SliderProcessor.h
    core/Space.cpp
    core/Space.h
    core/SpaceDefs.h
    core/SpaceDust.cpp
    core/SpaceDust.h
    core/SpaceObject.cpp
    core/SpaceObject.h
    core/Sparks.cpp
    core/Sparks.h
    core/StateProcessorFactory.cpp
    core/StateProcessorFactory.h
    core/StoreStateProcessor.cpp
    core/StoreStateProcessor.h
    core/WorldSelectionStateProcessor.cpp
    core/WorldSelectionStateProcessor.h
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
    SoundSystem.h LinuxDefs.h)

add_executable(spacerush ${SOURCE_FILES})

file(GLOB OGRE_LIBS "/usr/local/lib/libOgre*.so")
file(GLOB OGRE_LIBS_2 "/usr/local/lib/OGRE/*.so")
target_link_libraries (spacerush  ${OGRE_LIBS_2} ${OGRE_LIBS})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED thread system)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(spacerush ${Boost_LIBRARIES})

target_link_libraries(spacerush Cg OIS pthread GL GLU freeimage openal audio zzip Xft dl)