# Blueprint Order Fix

A Satisfactory mod (SML plugin) that fixes a small but annoying bug: **the order of your blueprints inside a blueprint sub-category is forgotten every time you load your save.**

You can arrange blueprints in the blueprint menu, and the game saves that order in your save file. But when the save is loaded, the game builds its blueprint lists from the blueprint files on disk and ignores the saved order, so your arrangement is gone. This mod puts the blueprints back in the order stored in your save, every time a world loads.

## Features

- Restores your custom blueprint order after loading a save.
- Nothing to configure, no new items, no new UI. Reorder blueprints as you always do, and the order now sticks.
- Safe to remove. Your save and your blueprint files are not modified. Without the mod you are back to the vanilla behavior.
- Blueprints that are not part of the saved order, such as new ones, are placed at the end of their sub-category.

## Limitations

- Single player and the host of a multiplayer session are the only cases tested. Joining a game as a client, or playing on a dedicated server, is not handled yet.
- Only the order of blueprints inside a sub-category is restored. Categories and sub-categories keep the order the game gives them.
- Orders you set *before* installing the mod are restored too, as long as the game had saved them. Save once with the mod installed if the order does not come back.

## Installation

Requires [SML (Satisfactory Mod Loader)](https://ficsit.app/mod/SML) 3.12.0 or newer. The Satisfactory Mod Manager installs it for you. Without the Mod Manager, SML has to be installed by hand too.

### Manual install

1. Download the mod zip from the releases page and extract it.
2. The extracted files are inside a folder called `Windows`. That folder is not what the game wants, so open it.
3. Go to the `Mods` folder of your Satisfactory installation, which is the `FactoryGame\Mods` folder inside the install folder. With Steam it is usually:

   `C:\Program Files (x86)\Steam\steamapps\common\Satisfactory\FactoryGame\Mods`

   If you installed the game elsewhere, use that location. For the Epic Games Store, it's the `FactoryGame\Mods` folder in its install folder.
4. Create a new folder in `Mods` named exactly `BlueprintOrderFix`.
5. Copy the **contents** of the `Windows` folder (not the `Windows` folder itself) into `BlueprintOrderFix`. The result should look like this:

   ```
   FactoryGame\Mods\BlueprintOrderFix\
   ├── BlueprintOrderFix.uplugin
   ├── Binaries\Win64\
   └── Content\Paks\Windows\
   ```

6. Start the game. It should load without errors, and the mod appears in the in-game Mods menu.

To uninstall, delete the `BlueprintOrderFix` folder.

## Usage

1. In the blueprint menu, reorder your blueprints inside a sub-category.
2. Save your game.
3. Load it again. The blueprints stay in the order you chose.

When a world loads, the mod writes one line to the game log (`%LOCALAPPDATA%\FactoryGame\Saved\Logs\FactoryGame.log`), for example `LogBlueprintOrderFix: Applied saved blueprint order to 36 sub-categories`. If it warns that the saved records were not found, the game was probably updated in a way the mod does not support yet.

## Compatibility

Built for the current Satisfactory release. A game update that changes the blueprint system may require a new version of the mod. Please report problems with the game version you use.

## How it works

The save file stores, for each blueprint sub-category, the ordered list of blueprint names. After the game has built its blueprint lists, the mod sorts each sub-category's blueprints to follow that saved list.

## Building

This is a C++ SML mod that lives in the `Mods` folder of the Satisfactory modding project. Build it with Alpakit or with the `PackagePlugin` UAT command, as for any other SML mod.

## License

MIT, see [LICENSE](LICENSE).
